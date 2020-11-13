#include "RK2.h"

/*
* To calculate the next point with Rk2.
*@param model is the differential equation sytem.
*       yi_plus_1 is the next point.
*		yi     are the previous points. the current values in yi are in front of the vector and
*              the old values are in the back of the vector.
*	    rank   indicates to a sepcific function in the differential equation system.
*		h      is the step size.
*/
void RK2::step(AbstractModell& model, Eigen::VectorXd& yi_plus_1,
    const std::vector<Eigen::VectorXd>& yi, const int& rank, const double& h) {

    Eigen::VectorXd yi_new(yi[0].size());
    Eigen::VectorXd k1(yi[0].size());
    int iter = 0;

    while (yi[0].size() > iter) {
        k1(iter) = h * model.func(yi[0], iter);
        yi_new(iter) = yi[0](iter) + k1(iter) * h * 0.5;
        iter++;
    }


    Eigen::VectorXd k2(yi[0].size());
    iter = 0;
    while (yi[0].size() > iter) {
        k2(iter) = h * model.func(yi_new, iter);
        iter++;
    }

    iter = 0;
    while (yi[0].size() > iter) {
        yi_plus_1(iter) = yi[0](iter) + k2(iter) + std::pow(h, 3);
        iter++;
    }


}

