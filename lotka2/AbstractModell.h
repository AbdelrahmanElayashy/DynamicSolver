#ifndef AbstractModell_H
#define AbstractModell_H

#include "eigen-3.3.7/Eigen/Dense"

/*
* Represents the abstract differential equeations system.
*/
class AbstractModell
{
/*
 * This function is suitable for inhomogen system too.
 * Write the functions inside func as lambda and use switch(rank)
 * to get which function you want.
 */
public:
	virtual long double func(const Eigen::VectorXd& y, const int& rank) = 0;
};

#endif