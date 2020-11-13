#include "MethodContainer.h"
#include "MethodsInScheduler.h"


   /*
	* represents the constructor of methodcontainer.
	* @param pmodel defines the initializied model by scheduler.
	*        pmethod defines the initializied method by scheduler.
	*/
    MethodContainer::MethodContainer(AbstractMethod* pmethod, MethodsInScheduler& pscheduler, int pid) :
		method(pmethod) {
		this->id = pid;
		pscheduler.insert_method(*this, pid);
    };
	/*
	* to calculate the next point of the equation with the defined method . 
	* @param model          is the differential equation sytem.
	*		 yi_plus_1      is placeholder for the next point.
	*		 yi             are the previous points. the current values in yi are in front of the vector and
	*                       the old values are in the back of the vector.
	*	     equation       indicates to a sepcific function in the differential equation system.
	*		 h              is the step size.
	*/
	void MethodContainer::executeMethod(AbstractModell& model, Eigen::VectorXd& yi_plus_1, const std::vector<Eigen::VectorXd>& yi,
		EquationContainer& equation, const double& h) {

		this->method->step(model, yi_plus_1, yi, equation.getID(), h);

	}

	/*
	* to calculate the next point of the equation with the defined method .
	* @param model          is the differential equation sytem. 
	*        yi_plus_1      is placeholder for the next point.
	*		 yi             are the previous points. the current values in yi are in front of the vector and
	*                       the old values are in the back of the vector.
	*	     equationrank   indicates to a sepcific function in the differential equation system.
	*		 h              is the step size.
	*/
	void MethodContainer::executeDummy(AbstractModell& model, Eigen::VectorXd& yi_plus_1, const std::vector<Eigen::VectorXd>& yi,
		const int& equationRank, const double& h) {
		//todo
		this->method->step(model, yi_plus_1, yi, equationRank, h);

	}

	/*
	* to get the method.
	*/
	AbstractMethod& MethodContainer::getMethod() {
		
		return *this->method;
	}

	/*
	* to get the id of the method.
	*/
	int MethodContainer::getID() {
		return this->id;
	}

	/*
	* to set precision of the method.
	*/
	void MethodContainer::setPrecision(double& pprecision) {
		this->precision = pprecision;
	}

	/*
	* to get the precision of the method.
	*/
	double MethodContainer::getPrecision() {
		return this->precision;
	}

	