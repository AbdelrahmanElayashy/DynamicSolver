#ifndef LotkaModell_H
#define LotkaModell_H

#include "AbstractModell.h"
#include "eigen-3.3.7/Eigen/Dense"
/*
* Represents the lotka-volterra-model.
*/
class LotkaModell :
    public AbstractModell
{

private:
    /*
     * Represents the interaction factor matrix of lotka-volterra-model.
    */
    const Eigen::MatrixXd interactionsFactor;
    /*
    * Represents the interaction factor vector of lotka-volterra-model.
    */
    const Eigen::VectorXd b;

/*
* The constructor of LotkaModell.
* @param matrix is a Matrix of interaction factors.
*	   	 vectorb is a vector of interaction factors.
*/
public :
	LotkaModell(Eigen::MatrixXd& a, Eigen::VectorXd& vectorb);
   /*
    * Represents the functions in lotka-volterra-model
    * @param y    represents values at a specific time point.
    *        rank represents the function's rank in the lotka-volterra-model.
    */
	virtual long double func(const Eigen::VectorXd& y, const int& rank) override;



};

#endif