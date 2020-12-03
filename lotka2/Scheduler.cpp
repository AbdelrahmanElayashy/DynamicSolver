
#include "Scheduler.h"
#include "MethodContainer.h"
#include "EquationContainer.h"
#include "ExplicitFourStep.h"
#include "ExplicitThreeStep.h"
#include "ExplicitTwoStep.h"
#include "ExplicitEuler.h"
#include "ApproxHeun.h"
#include "ApproxRK3.h"
#include "ApproxRK4.h"
#include "RK2.h"
#include "RK3.h"
#include "RK4.h"
#include <iostream>
#include "MyLibrary.h"


namespace N {
	MyLibrary lib;
};

Scheduler::Scheduler(AbstractModell& pmodel, Eigen::VectorXd initial_value, int pthreads, long long numStep, double pstep, double paccepted_error, std::string file_name) :
	model(&pmodel), number_of_threads(pthreads), number_of_steps(numStep), step(pstep),
	thread_pool(new ThreadsPool(pthreads)), scheduler_methods(pmodel), scheduler_equations(),
	scheduler_queue(new Priority_SafeQueue<EquationContainer>()), accepted_error(paccepted_error), myfile(file_name), used_methods("space_method.txt") {

	this->initialize_system();
	this->yi_plus_1.resize(pthreads);
	this->yi.push_back(initial_value);
	this->yi.push_back(initial_value);
	this->yi.push_back(initial_value);
	this->yi.push_back(initial_value);
	this->dt = 0.0;
	//myfile.open("result.txt");
	this->use_approx = new bool[pthreads];
	for (int i = 0; i < pthreads; i++) {
		memset(&this->use_approx[i], 0, sizeof(bool));
		this->use_approx[i] = false;
	}
	this->current_step = 4;
	///////////////////
	all_result.resize(pthreads);
	for (auto& element : this->all_result) {
		element.resize(number_of_steps);
	}
	this->last_number_step_each_equation.resize(pthreads);
	this->initialize_values(initial_value);
};

Scheduler::~Scheduler() {
	this->myfile.close();
}

void Scheduler::initialize_values(Eigen::VectorXd& initial_val) {

	for (int id = 0; id < this->number_of_threads; ++id) {
		this->all_result[id](0) = initial_val(id);
	}

}

void Scheduler::initialize_system() {

	this->initialize_methods();
	this->initialize_equations();
	this->scheduler_methods.assess_method_speed(this->scheduler_equations.get_equations());
	this->scheduler_equations.set_time_method_equation(this->scheduler_methods.get_time_method_equation());
	this->scheduler_equations.assess_Equation_Speed();
	this->acceptable_range_time = scheduler_methods.get_acceptable_range_time();
	this->initialize_queue();

}


void Scheduler::initialize_methods() {

	new MethodContainer(new ExplicitEuler(), scheduler_methods, 0);
	new MethodContainer(new RK2(), scheduler_methods, 1);
	new MethodContainer(new RK3(), scheduler_methods, 2);
	new MethodContainer(new RK4(), scheduler_methods, 3);
	new MethodContainer(new ExplicitTwoStep(), scheduler_methods, 4);
	new MethodContainer(new ExplicitThreeStep(), scheduler_methods, 5);
	new MethodContainer(new ExplicitFourStep(), scheduler_methods, 6);
	//	new MethodContainer(new ApproxHeun(), scheduler_methods, 7);
	//	new MethodContainer(new ApproxRK3(), scheduler_methods, 8);
	//	new MethodContainer(new ApproxRK4(), scheduler_methods, 9);

		//this->approx_method = this->scheduler_methods.get_methods().find(7)->second;

}

void Scheduler::initialize_equations() {

	// number of threads equal to the number of equations.
	for (int i = 0; i < this->number_of_threads; i++)
		new EquationContainer(this->scheduler_equations, i, standard_method);

}

void Scheduler::initialize_queue() {

	auto vec = this->scheduler_equations.get_equations();
	auto it = vec.begin();
	while (it != vec.end()) {
		this->scheduler_queue->enqueue(**it);
		it++;
	}
}

void Scheduler::run() {


	this->run_first_steps();


	int index;
	while (this->current_step < this->number_of_steps - 2) {

		index = 0;
		std::string str = "";
		double update_range_time = 0;
		while (index < this->number_of_threads) {

			auto equation = this->scheduler_queue->dequeue();
			if (equation.get_last_number_step() > this->current_step) {
				equation.setEquationSpeed(FAST);
				this->get_scheduler_queue()->enqueue(equation);
				continue;
			}


			this->check_next_equation_speed(equation);
			this->assign_equation_method(equation);
			update_range_time += equation.get_last_execution_time();
			index++;
		}

		this->acceptable_range_time = (update_range_time / this->number_of_threads) * 2;
		update_range_time = 0;
		if (this->current_step % 10 == 0) {
			this->accelerate_system = false;
			this->decelerate_system = false;
		}
		//	used_methods << this->scheduler_equations.get_equations().at(0)->getID() << std::endl;
		this->current_step += 1;
		//	this->push_front(this->yi_plus_1);
	}


}

void Scheduler::run_first_steps() {

	MethodContainer& method = this->scheduler_methods.calculate_method_for_first_steps(this->step, this->accepted_error);
	Eigen::VectorXd next_step;
	next_step.resize(this->number_of_threads);
	for (int current_step1 = 0; current_step1 < 4; current_step1++) {
		for (int index = 0; index < this->number_of_threads; index++) {
			EquationContainer equation = (this->scheduler_queue->dequeue());
			auto start = MyLibrary::startTimer();
			method.executeMethod(*this->model, next_step, yi, equation, this->step);
			equation.increment_number_step();
			this->push_step_value(equation, next_step(equation.getID()));
			auto stop = MyLibrary::startTimer();
			auto duration = MyLibrary::durationTime(start, stop);
			equation.set_last_execution_time(duration);
			this->scheduler_queue->enqueue(equation);
		}
		//this->push_front(this->yi_plus_1);

	}

}

void Scheduler::assign_equation_method(EquationContainer& equation) {

	MethodContainer* method;
	int id = equation.getID();


	switch (equation.getEquationSpeed()) {
	case FAST:
		method = this->scheduler_methods.get_slowMethod()[(this->current_step % this->scheduler_methods.get_slowMethod().size())];
		//	equation.set_last_execution_time(equation.get_method_execution_time().find(method->getID())->second);

		break;
	case BALANCED:

		method = this->scheduler_methods.get_balancedMethod()[(this->current_step % this->scheduler_methods.get_balancedMethod().size())];

		//		equation.set_last_execution_time(equation.get_method_execution_time().find(method->getID())->second);

		break;
	case SLOW:

		method = this->scheduler_methods.get_fastMethod()[(this->current_step % this->scheduler_methods.get_fastMethod().size())];

		//		equation.set_last_execution_time(equation.get_method_execution_time().find(method->getID())->second);

		break;
	default:
		method = this->scheduler_methods.get_balancedMethod()[(this->current_step % this->scheduler_methods.get_balancedMethod().size())];
	}



	this->assign_task_pool(*method, equation);

}

void Scheduler::assign_task_pool(MethodContainer& method, EquationContainer& equation) {


	this->thread_pool->enqueue_work([=]() {

		auto start = MyLibrary::startTimer();
		EquationContainer _equation = equation;
		MethodContainer _method = method;
		double _step = this->step;


		auto yis = this->build_old_values(_equation.getID());
		/*yis.push_back(this->get_yi()->at(0));
		yis.push_back(this->get_yi()->at(1));
		yis.push_back(this->get_yi()->at(2));
		yis.push_back(this->get_yi()->at(3));*/
		//std::cout << "getting in thread" << std::endl;

		//Eigen::VectorXd yi_plus_new(this->get_yi_plus_1());
		Eigen::VectorXd next_step;
		next_step.resize(this->get_number_of_threads());
		_method.executeMethod(this->get_model(), next_step, yis, _equation, _step);
		//this->set_yi_plus_1(yi_plus_new);
		_equation.increment_number_step();
		this->push_step_value(_equation, next_step(_equation.getID()));
		auto stop = MyLibrary::stopTimer();
		auto duration = MyLibrary::durationTime(start, stop);
		_equation.set_last_execution_time(duration);

		this->get_scheduler_queue()->enqueue(_equation);
		});

}
// call it after increment step
void Scheduler::push_step_value(EquationContainer& equation, double step_value) {

	this->all_result[equation.getID()](equation.get_last_number_step()) = step_value;
	this->last_number_step_each_equation[equation.getID()] = equation.get_last_number_step();
}
// call it before increment step.
Eigen::VectorXd Scheduler::get_old_values(int id) {
	Eigen::VectorXd old_values;
	old_values.resize(4);
	old_values(0) = this->all_result[id](this->last_number_step_each_equation[id]);
	old_values(1) = this->all_result[id](this->last_number_step_each_equation[id] - 1);
	old_values(2) = this->all_result[id](this->last_number_step_each_equation[id] - 2);
	old_values(3) = this->all_result[id](this->last_number_step_each_equation[id] - 3);

	return old_values;
}

double Scheduler::get_one_value(int id, long long step_value) {
	//sure that step_value is not err.
	double val = this->all_result[id](step_value);
	return val;
}

std::vector<Eigen::VectorXd> Scheduler::build_old_values(int pid) {
	std::vector<Eigen::VectorXd> old_values_equations;
	int mark_speed = 0;
	old_values_equations.resize(4);
	for (int i = 0; i < 4; ++i) {
		old_values_equations[i].resize(this->number_of_threads);
	}
	long long cur_step = this->last_number_step_each_equation[pid];
	for (int id = 0; id < this->number_of_threads; ++id) {

		for (int i = 0; i < 4; ++i) {
			long long last_step_equ = this->last_number_step_each_equation[id];
			if (cur_step == last_step_equ) {
				old_values_equations[i](id) = this->get_one_value(id, cur_step - i);
			}
			else if (cur_step > last_step_equ) {
				old_values_equations[i](id) = this->get_one_value(id, last_step_equ - i);
				mark_speed++;
			}
			else if (cur_step < last_step_equ) {
				old_values_equations[i](id) = this->get_one_value(id, cur_step - i);
				mark_speed--;
			}
		}
	}
	return old_values_equations;
}




void Scheduler::check_next_equation_speed(EquationContainer& equation) {

	double range_time = this->acceptable_range_time;
	double sum_last_next_method_time;

	switch (equation.getEquationSpeed()) {
	case FAST:
		sum_last_next_method_time = equation.get_last_execution_time() + scheduler_methods.get_average_slow_methods();

		if (sum_last_next_method_time > range_time) {
			equation.setEquationSpeed(BALANCED);
		}
		else if (sum_last_next_method_time < (range_time / 2)) {
			// "SCHEDULER NEEDS MORE SLOWER METHODS FOR THAT EQUATION" SO WE NEED TO ACCELERATE THE REST OF EQUATIONS IN THE SYSTEM!
			this->accelerate_system = true;
			this->decelerate_system = false;
		}
		return;
	case BALANCED:
		sum_last_next_method_time = equation.get_last_execution_time() + scheduler_methods.get_average_balanced_methods();

		if (this->accelerate_system) {
			equation.setEquationSpeed(SLOW);
		}
		else if (this->decelerate_system) {
			equation.setEquationSpeed(FAST);
		}
		else if (sum_last_next_method_time > range_time) {
			equation.setEquationSpeed(SLOW);

		}
		else if (sum_last_next_method_time < (range_time / 2)) {
			equation.setEquationSpeed(FAST);

		}
		return;
	case SLOW:
		sum_last_next_method_time = equation.get_last_execution_time() + scheduler_methods.get_average_fast_methods();

		if (this->accelerate_system) {
			equation.setEquationSpeed(SLOW);

		}
		else if (sum_last_next_method_time < (range_time / 2)) {
			equation.setEquationSpeed(BALANCED);

		}
		else if (sum_last_next_method_time > range_time) {
			//  "SCHEDULER NEEDS MORE FASTER METHODS FOR THAT EQUATION" SO WE NEED TO DECELERATE THE REST OF EQUATIONS IN THE SYSTEM!
			this->decelerate_system = true;
			this->accelerate_system = false;
		}
		return;
	}
}

void Scheduler::stop() {

}

AbstractModell& Scheduler::get_model() {

	return *this->model;
}

Priority_SafeQueue<EquationContainer>* Scheduler::get_scheduler_queue() {
	return this->scheduler_queue;
}

MethodsInScheduler Scheduler::get_scheduler_methods() {
	return this->get_scheduler_methods();
}

Eigen::VectorXd Scheduler::get_yi_plus_1() {
	return this->yi_plus_1;
}

void Scheduler::set_yi_plus_1(Eigen::VectorXd pyi) {
	this->yi_plus_1 = pyi;
}

void Scheduler::push_front(Eigen::VectorXd new_value) {

	this->dt += this->step;

	auto swap = this->yi[0];
	this->yi[0] = new_value;
	auto swap1 = this->yi[1];
	this->yi[1] = swap;
	swap = this->yi[2];
	this->yi[2] = swap1;
	this->yi[3] = swap;
	this->print_result(new_value);

}

void Scheduler::print_result(Eigen::VectorXd yi) {

	std::string str = std::to_string(this->dt) + ",";

	int count = 0;
	while (count < yi.size()) {
		str = str + std::to_string(yi(count)) + ",";
		count++;
	}

	//this->myfile << str << std::endl;
	this->scheduler_result.push_back(str);
}

void Scheduler::print_scheduler_result() {

	//for (std::vector<std::string>::const_iterator i = this->scheduler_result.begin(); i != this->scheduler_result.end(); ++i) {
	//	this->myfile << *i << std::endl;
	//}
	//this->myfile.close();
	//this->scheduler_result.resize(0);

	//this->scheduler_result.shrink_to_fit();
	long long min_steps = this->number_of_steps;

	for (int i = 0; i < this->scheduler_queue->get_size(); ++i) {
		EquationContainer equation = this->scheduler_queue->dequeue();
		if (equation.get_last_number_step() < min_steps) {
			min_steps = equation.get_last_number_step();
		}
	}

	std::string str = "";
	for (int i = 0; i < min_steps; ++i) {
    this->dt += this->step;
		str = std::to_string(this->dt) + ",";
		for (int j = 0; j < this->number_of_threads; ++j) {
			str += std::to_string(this->all_result[j](i)) + ",";
		}
		this->myfile << str << std::endl;
		str = "";
	}
	this->myfile.close();
	this->all_result.resize(0);
	this->all_result.shrink_to_fit();

}

std::vector<Eigen::VectorXd>* Scheduler::get_yi() {
	return &this->yi;
}

int Scheduler::get_number_of_threads() {
	return this->number_of_threads;
}


void Scheduler::test() {
	//sometimes there is no slow method dont forget to put the slow equation in balanced method.

	std::cout << "Number of fast methods : " << scheduler_methods.get_fastMethod().size() << std::endl;
	std::cout << "Number of slow methods : " << scheduler_methods.get_slowMethod().size() << std::endl;
	std::cout << "Number of balanced methods : " << scheduler_methods.get_balancedMethod().size() << std::endl;

	auto it1 = scheduler_methods.get_fastMethod().begin();
	auto it2 = scheduler_methods.get_slowMethod().begin();
	auto it3 = scheduler_methods.get_balancedMethod().begin();

	while (it1 != scheduler_methods.get_fastMethod().end()) {

		std::cout << "FAST method : " << (*it1)->getID() << std::endl;
		it1++;
	}

	while (it2 != scheduler_methods.get_slowMethod().end()) {

		std::cout << "SLOW method : " << (*it2)->getID() << std::endl;
		it2++;
	}

	while (it3 != scheduler_methods.get_balancedMethod().end()) {

		std::cout << "Balancd method : " << (*it3)->getID() << std::endl;
		it3++;
	}

	std::vector<EquationContainer*> equ = this->scheduler_equations.get_equations();
	auto it4 = equ.begin();
	const char* str[] = { "FAST", "BALANCED", "SLOW " };

	while (it4 != equ.end()) {
		std::cout << "Id of Equation : " << (*it4)->getID() << " Speed : " << str[(*it4)->getEquationSpeed()] << std::endl;
		it4++;
	}

	/*auto size = this->scheduler_queue->get_size();
	while (size-- >= 0) {
		std::cout << "The equation id in safe queue : " << this->scheduler_queue->dequeue().getID() << std::endl;
	}*/

	/*auto it5 = this->scheduler_methods.get_time_method_equation().begin();
	while (it5 != scheduler_methods.get_time_method_equation().end()) {
		std::cout << it5->first << std::endl;
		it5++;
	}*/


	for (int i = 0; i < this->number_of_threads; i++)
		std::cout << this->scheduler_equations.get_equations().at(i)->get_last_number_step() << std::endl;

	std::cout << this->acceptable_range_time << std::endl;

}

