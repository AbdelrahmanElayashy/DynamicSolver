#ifndef RK4_H
#define RK4_H

#include "AbstractMethod.h"

/*
* Represents the runge-kutta 4th order method.
*/
class RK4 :
	public AbstractMethod
{
	/*
	* To calculate the next point with RK4.
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

		template<typename F>
    long double do_step(const F&& function, const long double yi[],const int& rank,const double& dt,const size_t& len)
    {
        long double k1, k2, k3, k4;
        long double *yi_new = new long double[len];
        std::memcpy(yi_new, yi, len * sizeof(double));
        
            k1 = function(yi, rank);
            yi_new[rank] += 0.5 * k1 * dt;
            k2 = function(yi_new, rank);
            yi_new[rank] += 0.5 * k2 * dt;
            k3 = function(yi_new, rank);
            yi_new[rank] += 0.5 * k3 * dt;
            k4 = function(yi_new, rank);
        

            long double tmp;
             tmp = yi[rank] + (k1 + 2 * k2 + 2 * k3 + k4) * dt / 6;
             return tmp;
        
    }
};

#endif