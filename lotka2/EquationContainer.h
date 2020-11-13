#ifndef EquationContainer_H
#define EquationContainer_H

#include "EquationSpeed.h"
#include "EquationsInScheduler.h"

class EquationContainer
{
private:
	/*
	* Represents the id of an equation.
	* rank = id.
	*/
	int id;
	/*
	* Represents the speed of the equation.
	*/
	EquationSpeed speed;
	/*
	* Represents the last executed time that method takes to execute the equation.
	*/
	double last_execution_time;
	/*
	* Represents the id of the last method used to executed the equation by scheduler.
	*/
	int last_method_used;
	/*
	* Represents the steps are done by Scheduler for an equation.
	*/
	int last_number_step;

	/*
	* Represents the methods execution time that a method takes to execute the equation.
	*/
	std::map<int, double> method_execution_time;

	/*
	* Represent the last 20 values of the equation.
	*/
	std::vector<double> last_values;

public:
	/*
	* 
	*/
	void increment_number_step();
	/*
	* Represents the constructor of EquationContainer.
	* @param pid is the id of equation.
	*      standardMethod is the id of standardMethod.
	*/
	EquationContainer(EquationsInScheduler& equation, int pid, int standardMethod);

	/*
	* To get the id of the equation.
	*/
	int getID();
	/*
	* To set the speed of an equation.
	*/
	void setEquationSpeed(EquationSpeed pspeed);

	/*
	* To get the speed of the equation.
	*/
	EquationSpeed getEquationSpeed() const;
	
	/*
	* To get the last execution time.
	*/
	double get_last_execution_time();

	/*
	* To set the last execution time.
	*/
	void set_last_execution_time(double executionTime);

	/*
	* To get the method execution time.
	*/
	std::map<int, double> get_method_execution_time();

	/*
	* To set the method execution time.
	*/
	void set_method_execution_time(std::map<int, double> executionTime);

	/*
	* To insert a method execution time.
	*/
	void insert_method_execution_time(int methodId, double executionTime);

	/*
	* To push new item into front of the vector. The size of vector does not exceed 20 elements.
	*/
	void push_front_in_last_values(double new_value);

	/*
	* To get the last values of the equation.
	*/
	std::vector<double> get_last_values();

	/*
	* To get the last number step.
	*/
	int get_last_number_step() const;
};
#endif