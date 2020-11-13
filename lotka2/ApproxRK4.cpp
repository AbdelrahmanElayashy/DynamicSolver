#include "ApproxRK4.h"

/*
* To calculate the next point with ApproxRk4.
*@param model is the differential equation sytem.
*       yi_plus_1 is the next point.
*		yi     are the previous points. the current values in yi are in front of the vector and
*              the old values are in the back of the vector.
*	    rank   indicates to a sepcific function in the differential equation system.
*		h      is the step size.
*/
	 void ApproxRK4::step(AbstractModell& model, Eigen::VectorXd& yi_plus_1,
         const std::vector<Eigen::VectorXd>& yi, const int& rank, const double& h) {

        long double k1, k2, k3, k4, z;

        k1 = model.func(yi[0], rank);
        z = (yi[0](rank) - yi[1](rank) * h * (1 / 6)) - k1;
        k3 = z / 5;
        k2 = (k1 + k3) / 2;
        k4 = z - 2 * (k2 + k3);

        yi_plus_1(rank) = yi[0](rank) + h * (k1 + 2 * k2 + 2 * k3 + k4);

	}

