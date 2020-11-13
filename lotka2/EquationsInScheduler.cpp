#include "EquationsInScheduler.h"
#include "MethodContainer.h"
#include "EquationContainer.h"
#include "MyLibrary.h"
	
	/*
	* Represents the constructor of the EquationsInScheduler.
	*/
	EquationsInScheduler::EquationsInScheduler() {};


	/*
	* To calculate the speed of all equation with all methods in the model which they
	* are initializied by scheduler.
	*/
	void EquationsInScheduler::assess_Equation_Speed() {


		calculate_Fast_Equation();
		calculate_Slow_Equation();
		calculate_Balanced_Equation();
	}

	/*
	* To calculate the fast equations which they are located in the last third part 
	* of time_Method_Equation.
	*/
	void EquationsInScheduler::calculate_Fast_Equation() {

		std::map<int, int> count;
		size_t size = (this->time_method_equation->size() + 1) / 3;
		auto it = this->time_method_equation->begin();
		while (it != this->time_method_equation->end() && size-- != 0) {

			if (count.count(it->second.equation->getID()) == 0) {
				count.insert(std::pair<int, int>(it->second.equation->getID(), 1));
			}
			else {
				auto itt = count.find(it->second.equation->getID());
				itt->second++;
			}
			it++;
		}

		std::multimap<int, int> flipedCount = MyLibrary::flip_map(count);
		auto iter = flipedCount.end();
		size = (this->equations.size() + 1) / 3;
		while (iter != flipedCount.begin() && size-- != 0) {
			iter--;
			for (auto& equation : equations) {
				if (equation->getID() == iter->second) {
					equation->setEquationSpeed(FAST);
				}
			}
		}
	}

	/*
	* To calculate the slow equations which they are located in the last third part 
	* of time_Method_Equation.
	*/
	void EquationsInScheduler::calculate_Slow_Equation() {

		std::map<int, int> count;
		size_t size = (this->time_method_equation->size() + 1) / 3;
		auto it = this->time_method_equation->end();
		while (it != this->time_method_equation->begin() && size-- != 0) {
			it--;
			if (count.count(it->second.equation->getID()) == 0) {
				count.insert(std::pair<int, int>(it->second.equation->getID(), 1));
			}
			else {
				auto itt = count.find(it->second.equation->getID());
				itt->second++;
			}
		}

		std::multimap<int, int> flipedCount = MyLibrary::flip_map(count);
		auto iter = flipedCount.end();
		size = (this->equations.size() + 1) / 3;
		while (iter != flipedCount.begin() && size-- != 0) {
			iter--;
			for (auto& equation : equations) {
				if (equation->getID() == iter->second) {
					equation->setEquationSpeed(SLOW);
				}
			}
		}
	}

	/*
	* To insert an equation.
	*/
	void EquationsInScheduler::insert_equation(EquationContainer& equation) {
		this->equations.push_back(&equation);
	}

	/*
	* To set the time method equation.
	*/
	void EquationsInScheduler::set_time_method_equation(std::multimap<double, MethodEquation>& pexecutionTime) {
		
		this->time_method_equation = &pexecutionTime;
	}

	/*
	* To get the equations.
	*/
	std::vector<EquationContainer*>& EquationsInScheduler::get_equations() {
		return this->equations;
	}


