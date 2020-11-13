#ifndef RK3_H
#define RK3_H

#include "AbstractMethod.h"

/*
* Represents the runge-kutta 3th order method.
*/
class RK3 :
	public AbstractMethod
{
	/*
	* To calculate the next point with RK3.
	* @param modell is the differential equation sytem.
	*       yi_plus_1 is the next point.
	*		yi     are the previous points. the current values in yi are in front of the vector and
	*              the old values are in the back of the vector.
	*	    rank   indicates to a sepcific function in the differential equation system.
	*		h      is the step size.
	*/
public:
	virtual void step(AbstractModell& model, Eigen::VectorXd& yi_plus_1,
		const std::vector<Eigen::VectorXd>& yi, const int& rank, const double& h) override;
};

#endif