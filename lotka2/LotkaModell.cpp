#include "LotkaModell.h"
#include "AbstractModell.h"
#include <iostream>


using namespace Eigen;
typedef Eigen::Matrix<double, Dynamic, Dynamic, RowMajor> RowMatrixXi;

/*
* The constructor of LotkaModell.
* @param matrix is a Matrix of interaction factors.
        vectorb is a vector of interaction factors.
*/
    LotkaModell::LotkaModell(Eigen::MatrixXd& matr, Eigen::VectorXd& vectorb) :
        interactionsFactor(matr), b(vectorb)
    {
     
        
    };
    
    /*
    * Represents the functions in lotka-volterra-model
    * @param y    represents values at a specific time point.
    *        rank represents the function's rank in the lotka-volterra-model.
    */
    long double LotkaModell::func(const Eigen::VectorXd& y, const int& rank) {
        
        long double tmp;
        tmp = y(rank) * (this->b(rank) + y.dot(this->interactionsFactor.row(rank)));

        return tmp;
    }

