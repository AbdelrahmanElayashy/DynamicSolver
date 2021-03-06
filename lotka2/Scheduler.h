#ifndef Scheduler_H
#define Scheduler_H

#include "MethodsInScheduler.h"
#include "EquationsInScheduler.h"
#include "ThreadsPool.h"
#include "Priority_SafeQueue.h"
#include <fstream>
#include "eigen-3.3.7/Eigen/Dense"

class Scheduler
{

private:
	AbstractModell* model;
	ThreadsPool* thread_pool;
	MethodsInScheduler scheduler_methods;
	EquationsInScheduler scheduler_equations;
	Priority_SafeQueue<EquationContainer>* scheduler_queue;


	bool* use_approx;

	double acceptable_range_time;
	bool accelerate_system = false;
	bool decelerate_system = false;
	int standard_method = 0;
	int number_of_threads;
	long long number_of_steps;
	double step;
	double accepted_error;
	double dt;
	MethodContainer* approx_method;
	int current_step;
	std::vector<std::string> scheduler_result;


	Eigen::VectorXd yi_plus_1;
	std::ofstream myfile;
	std::ofstream used_methods;
	std::vector<Eigen::VectorXd> yi;
	////////////////////////////////
	std::vector<Eigen::VectorXd> all_result;
	std::vector<long long> last_number_step_each_equation;

	void initialize_queue();
	void initialize_methods();
	void initialize_equations();
	void initialize_system();
	void check_next_equation_speed(EquationContainer& equation);
	void assign_equation_method(EquationContainer& equation);
	void run_first_steps();
	void assign_task_pool(MethodContainer& method, EquationContainer& equation);
	////////////////////
	void push_step_value(EquationContainer& equation, double step_value);
	Eigen::VectorXd get_old_values(int);

	double get_one_value(int, long long);
	void initialize_values(Eigen::VectorXd&);


public:
	Scheduler(AbstractModell& pmodel, Eigen::VectorXd initial_value, int pthreads, long long numStep, double step, double paccepted_error, std::string file_name);
	~Scheduler();
	Scheduler();
	void* operator new(std::size_t);
	Scheduler& operator=(const Scheduler&);
	void test();
	void run();
	void stop();
	AbstractModell& get_model();
	Priority_SafeQueue<EquationContainer>* get_scheduler_queue();
	MethodsInScheduler get_scheduler_methods();
	void print_scheduler_result();

	Eigen::VectorXd get_yi_plus_1();

	void set_yi_plus_1(Eigen::VectorXd pyi);

	void push_front(Eigen::VectorXd new_value);

	void print_result(Eigen::VectorXd yi);

	std::vector<Eigen::VectorXd>* get_yi();
	int get_number_of_threads();
	std::vector<Eigen::VectorXd> build_old_values(int);
};
#endif