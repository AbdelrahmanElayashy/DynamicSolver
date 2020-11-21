

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
	scheduler_queue(new Priority_SafeQueue<EquationContainer>()), accepted_error(paccepted_error), myfile(file_name)
{
	this->initialize_system();
	this->yi_plus_1.resize(pthreads);
	this->yi.push_back(initial_value);
	this->dt = 0.0;
	//myfile.open("result.txt");
	this->use_approx = new bool[pthreads];
	for (int i = 0; i < pthreads; i++) {
		memset(&this->use_approx[i], 0, sizeof(bool));
		this->use_approx[i] = false;
	}
	this->current_step = 0;

};

Scheduler::~Scheduler() {
	this->myfile.close();
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
	//new MethodContainer(new ApproxHeun(), scheduler_methods, 7);
	//new MethodContainer(new ApproxRK3(), scheduler_methods, 8);
	//new MethodContainer(new ApproxRK4(), scheduler_methods, 9);

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
	while (this->current_step < this->number_of_steps - 3) {

		index = 0;
		while (index < this->number_of_threads) {

			auto equation = this->scheduler_queue->dequeue();

			this->check_next_equation_speed(equation);
			this->assign_equation_method(equation);

			index++;
		}
		this->current_step += 1;
		this->push_front(this->yi_plus_1);

		if (this->current_step % 10 == 0) {
			this->accelerate_system = false;
			this->decelerate_system = false;
		}
	}


}

void Scheduler::run_first_steps() {

	MethodContainer& method = this->scheduler_methods.calculate_method_for_first_steps(this->step, this->accepted_error);
	EquationContainer* equation;
	for (int current_step1 = 0; current_step1 < 4; current_step1++) {
		for (int index = 0; index < this->number_of_threads; index++) {
			equation = this->scheduler_equations.get_equations().at(index);
			equation->set_last_execution_time(equation->get_method_execution_time().find(method.getID())->second);
			method.executeMethod(*this->model, this->yi_plus_1, yi, *equation, this->step);
		}
		this->push_front(this->yi_plus_1);
		used_methods << method.getID() << std::endl;
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


		std::vector<Eigen::VectorXd> yis;
		yis.push_back(this->get_yi()->at(0));
		yis.push_back(this->get_yi()->at(1));
		yis.push_back(this->get_yi()->at(2));
		yis.push_back(this->get_yi()->at(3));

		Eigen::VectorXd yi_plus_new(this->get_yi_plus_1());

		_method.executeMethod(this->get_model(), yi_plus_new, yis, _equation, _step);
		this->set_yi_plus_1(yi_plus_new);
		_equation.increment_number_step();
		auto stop = MyLibrary::stopTimer();
		auto duration = MyLibrary::durationTime(start, stop);
		_equation.set_last_execution_time(duration);
		this->get_scheduler_queue()->enqueue(_equation);
		});

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
	std::vector<Eigen::VectorXd> new_yi;
	new_yi.push_back(new_value);
	new_yi.insert(new_yi.end(), this->yi.begin(), this->yi.end());

	this->yi = new_yi;

	this->print_result(new_value);

	if (this->yi.size() == 7) {
		this->yi.pop_back();
	}
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
	
	for (std::vector<std::string>::const_iterator i = this->scheduler_result.begin(); i != this->scheduler_result.end(); ++i) {
		this->myfile << *i << std::endl;
	}
	this->myfile.close();
	this->scheduler_result.clear();
	this->scheduler_result.shrink_to_fit();
	
}

std::vector<Eigen::VectorXd>* Scheduler::get_yi() {
	return &this->yi;
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

