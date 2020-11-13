#include "ApproxRK3.h"


    /*
     * To calculate the next point with approximation of 3th order Runge-kutta-method.
     * @param model is the differential equation sytem.
     *       yi_plus_1 is the next point.
     *		 yi     are the previous points. the current values in yi are in front of the vector and
     *              the old values are in the back of the vector.
     *	     rank   indicates to a sepcific function in the differential equation system.
     *		 h      is the step size.
     */
     void ApproxRK3::step(AbstractModell& model, Eigen::VectorXd& yi_plus_1,
         const std::vector<Eigen::VectorXd>& yi, const int& rank, const double& h) {

        double k1, k2;

        k1 = model.func(yi[0], rank);
        k2 = k1 / 2;

        yi_plus_1(rank) = yi[0](rank) + (h / 2) * (k1 + k2);

    }

