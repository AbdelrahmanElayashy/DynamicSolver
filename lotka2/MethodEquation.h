#ifndef MethodEquation_H
#define MethodEquation_H

class MethodContainer;
class EquationContainer;
/*
* Represents the relation between an equation and a method.
*/
struct MethodEquation {
public:
	
	MethodContainer* method;
	EquationContainer* equation;
};


#endif