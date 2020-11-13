#ifndef MethodContainer_H
#define MethodContainer_H

#include "AbstractMethod.h"
#include "EquationContainer.h"


class MethodsInScheduler;
/*
* Represents a methods and  information about this method which it
* will be used by a scheduler to control the differential equation model.
*/
class MethodContainer
{

private:
	/*
	* Represents the art of the method.
	*/
	AbstractMethod* method;

	/*
	* Is the id of the method.
	*/
	int id;

	/*
	* Is the precision of the method.
	*/
	double precision;

public:
	/*
	* Represents the constructor of MethodContainer.
	* @param pmodel defines the initializied model by scheduler.
	*        pmethod defines the initializied method by scheduler.
	*/
	MethodContainer(AbstractMethod* pmethod, MethodsInScheduler& pscheduler, int pid);
	/*
	* To calculate the next point of the equation with the defined method .
	* @param model          is the differential equation sytem.
	*		 yi_plus_1      is placeholder for the next point.
	*		 yi             are the previous points. the current values in yi are in front of the vector and
	*                       the old values are in the back of the vector.
	*	     equation       indicates to a sepcific function in the differential equation system.
	*		 h              is the step size.
	*/
	void executeMethod(AbstractModell& model, Eigen::VectorXd& yi_plus_1, const std::vector<Eigen::VectorXd>& yi,
		EquationContainer& equation, const double& h);

	/*
	* To calculate the next point of the equation with the defined method .
	* @param model          is the differential equation sytem.
	*        yi_plus_1      is placeholder for the next point.
	*		 yi             are the previous points. the current values in yi are in front of the vector and
	*                       the old values are in the back of the vector.
	*	     equationRank   indicates to a sepcific function in the differential equation system.
	*		 h              is the step size.
	*/
	void executeDummy(AbstractModell& model, Eigen::VectorXd& yi_plus_1, const std::vector<Eigen::VectorXd>& yi,
		const int& equationRank, const double& h);

	/*
	* To get the method.
	*/
	AbstractMethod& getMethod();

	/*
	* To get the id of the method.
	*/
	int getID();

	/*
	* To set precision of the method.
	*/
	void setPrecision(double& pprecision);

	/*
	* To get the precision of the method.
	*/
	double getPrecision();


};

#endif