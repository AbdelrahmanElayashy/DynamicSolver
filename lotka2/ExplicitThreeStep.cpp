#include "ExplicitThreeStep.h"

/*
* To calculate the next point with ExplicitThreeStep.
*@param modell is the differential equation sytem.
*       yi_plus_1 is the next point.
*		yi     are the previous points. the current values in yi are in front of the vector and
*              the old values are in the back of the vector.
*	    rank   indicates to a sepcific function in the differential equation system.
*		h      is the step size.
*/
	 void ExplicitThreeStep::step(AbstractModell& model, Eigen::VectorXd& yi_plus_1,
		 const std::vector<Eigen::VectorXd>& yi, const int& rank, const double& h) {

		yi_plus_1(rank) = yi[0](rank) +  h * ( (23 * model.func(yi[0], rank) / 12)
			- (16 * model.func(yi[1], rank) / 12 ) + (5 * model.func(yi[2], rank) / 12)) ;

	}
