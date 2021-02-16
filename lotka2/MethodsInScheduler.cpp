#include "MethodsInScheduler.h"
#include "MethodContainer.h"
#include "EquationContainer.h"
#include "MyLibrary.h"
#include "ExplicitEuler.h"
#include "RK2.h"
#include "RK3.h"
#include "RK4.h"

   /*
	* Represents the constructor of MethodsContainer.
	* @param list defines the map of initializied methods by scheduler.
	*        model represents the differential equations system which will be executed by scheduler.
	*        pnumEquations represents the number of equations in the model.
	*/
	MethodsInScheduler::MethodsInScheduler(AbstractModell& pmodel) :
		model(&pmodel)
	{};


	/*
	* To insert a method in the map of methods which will be used by scheduler.
	*/
	void MethodsInScheduler::insert_method(MethodContainer& pmethod, int& id) {
		this->methods.insert(std::pair<int, MethodContainer*>(id, &pmethod));
	}
	/*
	* To calculate the speed of all method with all equations in the model which they
	* are initializied by scheduler.
	*/
	void MethodsInScheduler::assess_method_speed(std::vector<EquationContainer*>& equations) {

		int count = 0;
		
		// TO CALCULATE THE AVARGE OF METHOD SPEED AFTER 10 TAKT.
		while (count < 10) {

			// TO DELET THE DATE SET FROM THE FIRST TAKT AND SECOND TAKT, BECAUSE THEY INCLUDE OUTLIERS.
			if (count == 1 || count == 2) {
				this->time_method_equation.erase(this->time_method_equation.begin(), this->time_method_equation.end());
			}

			auto it = this->methods.begin();
			while (it != this->methods.end()) {

				for (int rankEquation = 0; rankEquation < equations.size(); rankEquation++) {

					this->calculate_methodSpeed_equation(*it->second, *equations.at(rankEquation), equations.size(), count);
				}

				it++;
			}
			count++;
		}

		this->calculate_Fast_Method();
		this->calculate_Slow_Method();
		this->calculate_Balanced_Method();
		this->calculate_acceptable_range_time();
		this->calculate_average_method_speed_type(equations.size());
		
	}

	/*
	* To calculate the balanced methods which they are neither in fast methods nor in slow methods.
	* The balanced methods refers to a specific model.
	*/
	void MethodsInScheduler::calculate_Balanced_Method() {

		auto it = this->methods.begin();
		while (it != this->methods.end()) {

			if (std::find(this->fastMethods.begin(), this->fastMethods.end(), it->second) == this->fastMethods.end() &&
				std::find(this->slowMethods.begin(), this->slowMethods.end(), it->second) == this->slowMethods.end()) {

				this->balancedMethods.push_back(it->second);
			}

			it++;
		}
	}


	/*
	* To calculate the slow methods which they are located in the last third part of time_Method_Equation.
	* Slow methods refer to a specific model.
	*/
	void MethodsInScheduler::calculate_Slow_Method() {

		std::map<int, int> count;
		size_t size = (this->time_method_equation.size() + 1) / 3;
		auto it = this->time_method_equation.end();
		while (it != this->time_method_equation.begin() && size-- > 0) {
			it--;
			if (count.count(it->second.method->getID()) == 0) {
				count.insert(std::pair<int, int>(it->second.method->getID(), 1));
			}
			else {
				auto itt = count.find(it->second.method->getID());
				itt->second++;
			}
			
		}

		
		std::multimap<int, int> flipedCount = MyLibrary::flip_map(count);
		auto iter = flipedCount.end();
		size = (this->methods.size() + 1) / 3;
		while (iter != flipedCount.begin() && size-- != 0) {
			iter--;

			MethodContainer* p = methods.find(iter->second)->second;
			if (std::find(this->fastMethods.begin(), this->fastMethods.end(), p) == this->fastMethods.end()) {
				this->slowMethods.push_back(this->methods.find(iter->second)->second);
			}
		}
	}

	/*
	* To calculate the fast methods which they are located in the first third part of time_Method_Equation.
	* Fast methods refer to a specific model.
	*/
	void MethodsInScheduler::calculate_Fast_Method() {

		std::map<int, int> count;
		size_t size = (this->time_method_equation.size() + 1) / 3;
		auto it = this->time_method_equation.begin();
		while (it != this->time_method_equation.end() && size-- != 0) {

			if (count.count(it->second.method->getID()) == 0) {
				count.insert(std::pair<int, int>(it->second.method->getID(), 1));
			}
			else {
				auto itt = count.find(it->second.method->getID());
				itt->second++;
			}
			it++;
		}

		std::multimap<int, int> flipedCount = MyLibrary::flip_map(count);
		auto iter = flipedCount.end();
		size = (this->methods.size() + 1) / 3;
		while (iter != flipedCount.begin() && size-- != 0) {
			iter--;
			this->fastMethods.push_back(this->methods.find(iter->second)->second);
		}

	}

	/*
	* To calculate the speed of a specific method with a specific equation in the model.
	* @param pMethodContainer is a MethodContainer.
	*		 equation         is an equation in the model.
	*		 count            is the number of takt.
	*/
	void MethodsInScheduler::calculate_methodSpeed_equation(MethodContainer& pMethodContainer, EquationContainer& equation, int numEquations, int count) {

		Eigen::VectorXd yi_plus_1(numEquations);
		Eigen::VectorXd yi(numEquations);
		int size = numEquations;
		while (--size >= 0) {
			yi(size) = 42;
		}
		std::vector<Eigen::VectorXd> y;
		y.assign(numEquations + 2, yi);

		auto start = MyLibrary::startTimer();
		for (int i = 0; i < 1; i++) {
			pMethodContainer.executeDummy(*this->model, yi_plus_1, y, equation.getID(), 0.01);
		}
		auto end = MyLibrary::stopTimer();
		auto duration = MyLibrary::durationTime(start, end);

		insert_approximate_method_equation(pMethodContainer, equation, duration, count);

	}
	/*
	* To calcuate the approximation time, then insert it in time_Method_Equation.
	* @param pMethodContainer is a method.
	*        pequation        is an equation in the model.
	*		 pexecutedTime    is the time that a method takes it to execute one step from the equation.
	*		 count            is the number of takt.
	*/
	void MethodsInScheduler::insert_approximate_method_equation(MethodContainer& pMethodContainer, 
		EquationContainer& pequation, double pexecutedTime, int count) {

		MethodEquation methodEquation;
		methodEquation.equation = &pequation;
		methodEquation.method = &pMethodContainer;

		if (count < 3) {
			this->time_method_equation.insert(std::pair<double, MethodEquation>(pexecutedTime, methodEquation));
		}
		else  {
			auto it = this->time_method_equation.begin();
			while (it != this->time_method_equation.end()) {

				if (it->second.equation->getID() == pequation.getID() && it->second.method->getID() == pMethodContainer.getID()) {
					double new_value = ((it->first * ((double)count - 2)) + pexecutedTime) / ((double)count - 1);
					this->time_method_equation.erase(it);
					this->time_method_equation.insert(std::pair<double, MethodEquation>(new_value, methodEquation));
					pequation.insert_method_execution_time(pMethodContainer.getID(), new_value);
					return;
				}

				it++;
			}

		}
		
	
	}

	
   /*
	* To calculate the acceptable range time.
	*/
	void MethodsInScheduler::calculate_acceptable_range_time() {

		int count = 0;
		double sum = 0;
		auto it = this->time_method_equation.begin();
		auto vecBegin = this->balancedMethods.begin();
		auto vecEnd = this->balancedMethods.end();

		while (it != this->time_method_equation.end()) {

			if (std::find(vecBegin, vecEnd, it->second.method) != vecEnd) {
				sum = sum + it->first;
				count += 1;
			}
			it++;
		}

		this->acceptable_range_time = (sum / count) * 2;
	}

	/*
	* To calculate the average of each type of method speed.
	*/
	void MethodsInScheduler::calculate_average_method_speed_type(int numEquations) {

		double sum_fast = 0, sum_slow = 0, sum_balanced = 0;

		std::for_each(this->time_method_equation.begin(), this->time_method_equation.end(), [&](const std::pair<double, MethodEquation> & it) {
					
			if (std::find(this->get_fastMethod().begin(), this->get_fastMethod().end(),
				it.second.method) != this->get_fastMethod().end()) {
				sum_fast += it.first;
			}
			else if (std::find(this->get_slowMethod().begin(), this->get_slowMethod().end(),
				it.second.method) != this->get_slowMethod().end())
				sum_slow += it.first;
			else {
				sum_balanced += it.first;
			}
			return;
			});

		this->average_fast_methods = sum_fast / (this->fastMethods.size() * numEquations);
		this->average_balanced_methods = sum_balanced / (this->balancedMethods.size() * numEquations);
		this->average_slow_methods = sum_slow / (this->slowMethods.size() * numEquations);
	}

	/*
	* To get the acceptable range time
	*/
	double MethodsInScheduler::get_acceptable_range_time() {
		return this->acceptable_range_time;
	}

	/*
	* To get the balanced methods.
	*/
	std::vector<MethodContainer*>& MethodsInScheduler::get_balancedMethod() {
		return this->balancedMethods;
	}
	/*
	* To get the fast methods.
	*/
	std::vector<MethodContainer*>& MethodsInScheduler::get_fastMethod() {
		return this->fastMethods;
	}
	/*
	* To get the slow methods.
	*/
	std::vector<MethodContainer*>& MethodsInScheduler::get_slowMethod() {
		return this->slowMethods;
	}

	/*
	* To get the time method equation.
	*/
	std::multimap<double, MethodEquation>& MethodsInScheduler::get_time_method_equation() {
		return this->time_method_equation;
	}


	/*
	* To get the avearge of slow methods.
	*/
	double MethodsInScheduler::get_average_slow_methods() {
		return this->average_slow_methods;
	}

	/*
	* To get the avearge of fast methods.
	*/
	double MethodsInScheduler::get_average_fast_methods() {
		return this->average_fast_methods;
	}

	/*
	* To get the avearge of balanced methods.
	*/
	double MethodsInScheduler::get_average_balanced_methods() {
		return this->average_balanced_methods;
	}


	/*
	* To get the methods in MethodsInScheduler.
	*/
	std::map<int, MethodContainer*> MethodsInScheduler::get_methods() {
		return this->methods;
	}

	/*
	* To calculate which method runs the first steps by scheduler.
	* @param step_size is the size step of h.
	*        error     is the accepted error which is given by user.
	*/
	MethodContainer& MethodsInScheduler::calculate_method_for_first_steps(double step_size, double error) {
		std::multimap<double, MethodContainer*> _method;

		auto it = this->methods.begin();
		while (it != this->methods.end()) {
			if (typeid(it->second->getMethod()) == typeid(ExplicitEuler)) {
				double error_estimate = std::fabs(std::pow(step_size, 2) - error);
				_method.insert(std::pair<double, MethodContainer*>(error_estimate, it->second));
			}
			else if (typeid(it->second->getMethod()) == typeid(RK2)) {
				double error_estimate = std::fabs(std::pow(step_size, 3) - error);
				_method.insert(std::pair<double, MethodContainer*>(error_estimate, it->second));
			}
			else if (typeid(it->second->getMethod()) == typeid(RK3)) {
				double error_estimate = std::fabs(std::pow(step_size, 4) - error);
				_method.insert(std::pair<double, MethodContainer*>(error_estimate, it->second));
			}
			else if (typeid(it->second->getMethod()) == typeid(RK4)) {
				double error_estimate = std::fabs(std::pow(step_size, 5) - error);
				_method.insert(std::pair<double, MethodContainer*>(error_estimate, it->second));
			}
			it++;
		}

		return *_method.begin()->second;
	}