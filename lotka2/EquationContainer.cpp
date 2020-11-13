#include "EquationContainer.h"


	/*
	* Represents the constructor of EquationConatianer.
	* @param pid is the id of the equation.
	*        standardMethod is the id of standard method.
	*/
    EquationContainer::EquationContainer(EquationsInScheduler& equation, int pid, int standardMethod) {
		this->id = pid;
		this->speed = BALANCED;
		this->last_method_used = standardMethod;
		this->last_number_step = 0;
		equation.insert_equation(*this);
	}

	/*
	* To get the id of the equation.
	*/
	int EquationContainer::getID() {
		return this->id;
	}

	/*
	* To set the speed of an equation.
	*/
	void EquationContainer::setEquationSpeed(EquationSpeed pspeed) {
		this->speed = pspeed;
	}

	/*
	* To get the speed of the equation.
	*/
	EquationSpeed EquationContainer::getEquationSpeed() const{
		return this->speed;
	}


	/*
	* To get the last execution time.
	*/
	double EquationContainer::get_last_execution_time() {
		return this->last_execution_time;
	}

	/*
	* To set the last execution time.
	*/
	void EquationContainer::set_last_execution_time(double executionTime) {
		this->last_execution_time = executionTime;
	}

	/*
	* To get the method execution time.
	*/
	std::map<int, double> EquationContainer::get_method_execution_time(){
		return this->method_execution_time;
	}

	/*
	* To set the method execution time.
	*/
	void EquationContainer::set_method_execution_time(std::map<int, double> executionTime) {
		this->method_execution_time = executionTime;
	}

	/*
	* To insert a method execution time.
	*/
	void EquationContainer::insert_method_execution_time(int methodId, double executionTime) {
		this->method_execution_time.insert(std::pair<int, double>(methodId, executionTime));
	}

	/*
	*
	*/
	void EquationContainer::increment_number_step() {
		this->last_number_step++;
	}

	/*
	* To push new item into front of the vector. The size of vector does not exceed 20 elements.
	*/
	void EquationContainer::push_front_in_last_values(double new_value) {

		if (this->last_values.size() == 20) {
			this->last_values.pop_back();
		}
		
		this->last_values.insert(this->last_values.begin(), new_value);
	}

	/*
	* To get the last values of the equation.
	*/
	std::vector<double> EquationContainer::get_last_values() {
		return this->last_values;
	}

	/*
	* To get the last number step.
	*/
	int EquationContainer::get_last_number_step() const {
		return this->last_number_step;
	}