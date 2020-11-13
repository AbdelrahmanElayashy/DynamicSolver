#ifndef EquationsInScheduler_H
#define EquationsInScheduler_H

#include <map>
#include <vector>
#include "MethodEquation.h"
#include <vector>


/*
* Represents the Equations in the model which will be controlled by scheduler.
*/
class EquationsInScheduler {

private:
	/*
	* Represents the time execution which the method takes it to run an equation.
	* The key represents the execution time.
	*/
	std::multimap<double, MethodEquation>* time_method_equation;
	/*
	* Represent the equations in the meodel.
	*/
	std::vector<EquationContainer*> equations;

	/*
	* To calculate the fast equations which they are located in the last third part
	* of time_Method_Equation.
	*/
	void calculate_Fast_Equation();


	/*
	* To calculate the slow equations which they are located in the last third part
	* of time_Method_Equation.
	*/
	void calculate_Slow_Equation();
	/*
	* To calculate the balanced equations which they are not in slow or fast equations.
	*/
	void calculate_Balanced_Equation() {
		// it is initializied by the constructor in EquationContainer as default value.
	}


public:
	/*
	* Represents the constructor of the EquationsInScheduler.
	*/
	EquationsInScheduler();

	/*
	* To calculate the speed of all equation with all methods in the model which they
	* are initializied by scheduler.
	*/
	void assess_Equation_Speed();
	/*
	* To insert an equation.
	*/
	void insert_equation(EquationContainer& equation);

	/*
	* To set the time method equation.
	*/
	void set_time_method_equation(std::multimap<double, MethodEquation>& pexecutionTime);

	/*
	* To get the equations.
	*/
	std::vector<EquationContainer*>& get_equations();
};

#endif