#ifndef MethodsInScheduler_H
#define MethodsInScheduler_H

#include "MethodEquation.h"
#include <vector>
#include <map>

class MethodContainer;
class AbstractModell;


/*
* Represents a list of methods and thier information which these method
* will be used by a scheduler to control the differential equation model.
*/
class MethodsInScheduler
{

private :
	/*
    * Represents the differential equation system which will be controlled by scheduler.
    */
	AbstractModell * model;
	/*
	* Represents map of methodContainer which are initializied by scheduler.
	*/
	std::map<int, MethodContainer*> methods;
	/*
	* Represents the time execution which the method takes it to run an equation.
	* The key represents the execution time.
	*/
	std::multimap<double, MethodEquation> time_method_equation;
	/*
	* Represents the fast methods refer to specific model.
	*/
	std::vector<MethodContainer*> fastMethods;
	/*
	* Represents the slow methods refer to specific model.
	*/
	std::vector<MethodContainer*> slowMethods;
	/*
	* Represents the balanced methods refer to specific model.
	*/
	std::vector<MethodContainer*> balancedMethods;
	/*
	* Represents the average of fast methods.
	*/
	double average_fast_methods;
	/*
	* Represents the average of slow methods.
	*/
	double average_slow_methods;
	/*
	* Represents the average of balanced methods.
	*/
	double average_balanced_methods;
	/*
	* To calculate the balanced methods which they are neither in fast methods nor in slow methods.
	* The balanced methods refers to a specific model.
	*/
	void calculate_Balanced_Method();
	/*
	* To calculate the slow methods which they are located in the last third part of time_Method_Equation.
	* Slow methods refer to a specific model.
	*/
	void calculate_Slow_Method();
	/*
	* To calculate the fast methods which they are located in the first third part of time_Method_Equation.
	* Fast methods refer to a specific model.
	*/
	void calculate_Fast_Method();
	/*
	* To calculate the speed of a specific method with a specific equation in the model.
	* @param pMethodContainer is a MethodContainer.
	*		 rankequation     is the rank of equation in the model.
	*/
	void calculate_methodSpeed_equation(MethodContainer& pMethodContainer, EquationContainer& pequation, int numEquations, int count);

	/*
	* To calcuate the approximation time, then isert it in time_Method_Equation.
	* @param pMethodContainer is a method.
	*        pequation        is an equation in the model.
	*		 pexecutedTime    is the time that a method takes it to execute one step from the equation.
	*		 count            is the number of takt.
	*/
	void insert_approximate_method_equation(MethodContainer& pMethodContainer, EquationContainer& pequation, double pexecutedTime, int count);
	/*
	* To get time_Method_Equation.
	*/
	std::multimap<double, MethodEquation>& getTime_Method_Equation();

	/*
	* To calculate the acceptable range time.
	*/
	void calculate_acceptable_range_time();
	/*
	* To calculate the average of each type of method speed.
	*/
	void calculate_average_method_speed_type(int numEquations);


public:
	/*
	* Represents the acceptable range time that says if two methods from same speed can execute
	*  the equation successively.
	*/
     double acceptable_range_time;
	/*
	* Represents the constructor of MethodsContainer.
	* @param list defines the map of initializied methods by scheduler.
	*        model represents the differential equations system which will be executed by scheduler.
	*        pnumEquations represents the number of equations in the model.
	*/
	MethodsInScheduler(AbstractModell& pmodel);
	

	/*
	* To insert a method in the map of methods which will be used by scheduler.
	*/
	void insert_method(MethodContainer& pmethod, int& id);
	/*
	* To calculate the speed of all method with all equations in the model which they
	* are initializied by scheduler.
	*/
	void assess_method_speed(std::vector<EquationContainer*>& equations);

	/*
	* To get the acceptable range time
	*/
    double get_acceptable_range_time();

	/*
	* To get time method equation.
	*/
	std::multimap<double, MethodEquation>& get_time_method_equation();

	/*
	* To get the balanced methods.
	*/
	std::vector<MethodContainer*>& get_balancedMethod();
	/*
	* To get the fast methods.
	*/
	std::vector<MethodContainer*>& get_fastMethod();
	/*
	* To get the slow methods.
	*/
	std::vector<MethodContainer*>& get_slowMethod();

	/*
	* To get the avarge of fast methods.
	*/
	double get_average_fast_methods();

	/*
	* To get the avearge of slow methods.
	*/
	double get_average_slow_methods();

	/*
	* To get the avearge of balanced methods.
	*/
	double get_average_balanced_methods();

	/*
	* To get the methods in MethodsInScheduler.
	*/
	std::map<int, MethodContainer*> get_methods();

	/*
	* To calculate which method runs the first steps by scheduler.
	* @param step_size is the size step of h.
	*        error     is the accepted error which is given by user.
	*/
	MethodContainer& calculate_method_for_first_steps(double step_size, double error);
};

#endif