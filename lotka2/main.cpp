#include "ExplicitEuler.h"
#include "ApproxRK4.h"
#include "LotkaModell.h"
#include <iostream>
#include <fstream>
#include "eigen-3.3.7/Eigen/Dense"
#include "MyLibrary.h"
#include "Scheduler.h"
#include "RK2.h"
#include "RK3.h"
#include "RK4.h"
#include "ApproxHeun.h"
#include "ApproxRK3.h"
#include "ExplicitFourStep.h"
#include "ExplicitThreeStep.h"
#include "ExplicitTwoStep.h"
#include "EinspurModell.h"

void solution_method(int solve_way, AbstractModell& mod, Eigen::VectorXd& yi, Eigen::VectorXd& yi_plus_1, int numberOfEquations, double step, long long numberOfLoops
						,long long numberofloopsRk4, double step_seq_rk4);
void call_python_script(long long loops, int numEquations, int solve_w, double, double);

int global_size = 0;

int main(int argc, char* argv[]) {


	/*int model = 0;
	int choose_step = 42;*/

	int model = std::atoi(argv[1]);
	int choose_step = std::atoi(argv[2]);

	int solve_way = 1;
	double step = 0;
	int numberOfEquations = model;
	long long numberOfLoops = 0; 
	global_size = choose_step;

	//unused.
	double step_seq_rk4 = 0.0;
	double numberOfLoops_seq_rk4 = 0;
	int choose_step_seq_rk4 = 0;
	//if (choose_step_seq_rk4 == 9) {
	//	step_seq_rk4 = 0.000000001;
	//	numberOfLoops_seq_rk4 = 300000000000;
	//}
	//else if (choose_step_seq_rk4 == 4) {
	//	numberOfLoops_seq_rk4 = 3000000;
	//	step_seq_rk4 = 0.0001;
	//}
	//else if (choose_step_seq_rk4 == 42) {
	//	//test
	//	numberOfLoops_seq_rk4 = 30;
	//	step_seq_rk4 = 0.01;
	//}
	//else {
	//	std::cout << "not valid input for the step size for seq. rk4 ,please choose one of 9 or 4" << std::endl;
	//	exit(-1);
	//}

	if (choose_step == 9) {
		numberOfLoops = 300000000000;
		step = 0.000000001;
	}
	else if (choose_step == 8) {
		numberOfLoops = 30000000000;
		step = 0.00000001;
	}
	else if (choose_step == 7) {
		numberOfLoops = 3000000000;
		step = 0.0000001;
	}
	else if (choose_step == 6) {
		numberOfLoops = 300000000;
		step = 0.000001;
	}
	else if (choose_step == 5) {
		numberOfLoops = 30000000;
		step = 0.00001;
	}
	else if (choose_step == 4) {
		numberOfLoops = 3000000;
		step = 0.0001;
	}
	else if (choose_step = 42) {
		numberOfLoops = 30000;
		step = 0.01;
	}
	else {
		std::cout << "not valid input for the step size for scheduler ,please choose one of them 9,8,7,6,5,4" << std::endl;
		exit(-1);
	}
	
		 
	if (model == 0) {
		EinspurModell lm;
		numberOfEquations = 7;
		Eigen::VectorXd yi(7);
		Eigen::VectorXd yi_plus_1(7);
		yi << 0, 0, 10, 0, 0, 0, 0;
		solve_way = 0;
		solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, numberOfLoops_seq_rk4, step_seq_rk4);
	}
	else if (model == 3) {
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		numberOfEquations = 3;
		Eigen::MatrixXd a(3, 3);
		a.row(0) << -1.0, -0.1, -0.1;
		a.row(1) << -0.1, -1.0, -0.1;
		a.row(2) << -0.1, -0.1, -1.0;

		Eigen::VectorXd b(3);
		b << 1, 1, 1;

		Eigen::VectorXd yi(model);
		Eigen::VectorXd yi_plus_1(model);
		yi << 0.2, 0.2, 0.5;
		LotkaModell lm(a, b);
		solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops ,numberOfLoops_seq_rk4, step_seq_rk4);

	}
	else if (model == 4) {
		numberOfEquations = 4;
		Eigen::MatrixXd a(4, 4);
		a.row(0) << -0.014, -0.017, -0.016, -0.009;
		a.row(1) << -0.009, -0.014, -0.017, -0.014;
		a.row(2) << -0.016, -0.009, -0.014, -0.017;
		a.row(3) << -0.017, -0.016, -0.009, -0.014;

		Eigen::VectorXd b(4);
		b << 1.0, 1.0, 1.0, 1.0;


		Eigen::VectorXd yi(model);
		Eigen::VectorXd yi_plus_1(model);
		yi << 0.6, 0.4, 0.5, 0.5;
		
		LotkaModell lm(a, b);

		solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, numberOfLoops_seq_rk4, step_seq_rk4);
	}
	else if (model == 5) {
		numberOfEquations = 5;
		Eigen::MatrixXd a(5, 5);
		a.row(0) << -0.006, -0.007, -0.0016, -0.009, -0.006;
		a.row(1) << -0.006, -0.006, -0.007, -0.0016, -0.009;
		a.row(2) << -0.009, -0.006, -0.006, -0.007, -0.0016;
		a.row(3) << -0.0016, -0.009, -0.006, -0.006, -0.007;
		a.row(4) << -0.007, -0.0016, -0.009, -0.006, -0.006;

		Eigen::VectorXd b(5);
		b << 1.0, 1.0, 1.0, 1.0, 1.0;


		Eigen::VectorXd yi(model);
		Eigen::VectorXd yi_plus_1(model);
		yi << 0.6, 0.4, 0.5, 0.5, 0.3;
	
		LotkaModell lm(a, b);
		solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, numberOfLoops_seq_rk4, step_seq_rk4);

	}
	else if (model == 6) {
		numberOfEquations = 6;
		Eigen::MatrixXd a(6, 6);
		a.row(0) << -0.014, -0.007, -0.016, -0.009, -0.006, -0.012;
		a.row(1) << -0.012, -0.014, -0.007, -0.016, -0.009, -0.006;
		a.row(2) << -0.006, -0.012, -0.014, -0.007, -0.016, -0.009;
		a.row(3) << -0.009, -0.006, -0.012, -0.014, -0.007, -0.016;
		a.row(4) << -0.016, -0.009, -0.006, -0.012, -0.014, -0.007;
		a.row(5) << -0.007, -0.016, -0.009, -0.006, -0.012, -0.014;

		Eigen::VectorXd b(6);
		b << 1.0, 1.0, 1.0, 1.0, 1.0, 1.0;


		Eigen::VectorXd yi(model);
		Eigen::VectorXd yi_plus_1(model);
		yi << 0.6, 0.4, 0.5, 0.5, 0.3, 0.5;
		LotkaModell lm(a, b);
		solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, numberOfLoops_seq_rk4, step_seq_rk4);
	}
	else if (model == 7) {
		numberOfEquations = 7;
		Eigen::MatrixXd a(7, 7);
		a.row(0) << -0.0033, -0.0017, -0.0016, -0.0032, -0.0016, -0.0036, -0.0018;
		a.row(1) << -0.0018, -0.0033, -0.0017, -0.0016, -0.0032, -0.0016, -0.0036;
		a.row(2) << -0.0036, -0.0018, -0.0033, -0.0017, -0.0016, -0.0032, -0.0016;
		a.row(3) << -0.0016, -0.0036, -0.0018, -0.0033, -0.0017, -0.0016, -0.0032;
		a.row(4) << -0.0032, -0.0016, -0.0036, -0.0018, -0.0033, -0.0017, -0.0016;
		a.row(5) << -0.0016, -0.0032, -0.0016, -0.0036, -0.0018, -0.0033, -0.0017;
		a.row(6) << -0.0017, -0.0016, -0.0032, -0.0016, -0.0036, -0.0018, -0.0033;

		Eigen::VectorXd b(7);
		b << 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0;


		Eigen::VectorXd yi(model);
		Eigen::VectorXd yi_plus_1(model);
		yi << 0.6, 0.4, 0.5, 0.5, 0.3, 0.5, 0.3;


		
		LotkaModell lm(a, b);
		solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, numberOfLoops_seq_rk4, step_seq_rk4);
	}
	else if (model == 8) {
		numberOfEquations = 8;
		Eigen::MatrixXd a(8, 8);
		a.row(0) << -0.0014, -0.0017, -0.0016, -0.0010, -0.0012, -0.0003, -0.0007, -0.0002;
		a.row(1) << -0.0002, -0.0014, -0.0017, -0.0016, -0.0010, -0.0012, -0.0003, -0.0007;
		a.row(2) << -0.0007, -0.0002, -0.0014, -0.0017, -0.0016, -0.0010, -0.0012, -0.0003;
		a.row(3) << -0.0003, -0.0007, -0.0002, -0.0014, -0.0017, -0.0016, -0.0010, -0.0012;
		a.row(4) << -0.0012, -0.0003, -0.0007, -0.0002, -0.0014, -0.0017, -0.0016, -0.0010;
		a.row(5) << -0.0010, -0.0012, -0.0003, -0.0007, -0.0002, -0.0014, -0.0017, -0.0016;
		a.row(6) << -0.0016, -0.0010, -0.0012, -0.0003, -0.0007, -0.0002, -0.0014, -0.0017;
		a.row(7) << -0.0017, -0.0016, -0.0010, -0.0012, -0.0003, -0.0007, -0.0002, -0.0014;

		Eigen::VectorXd b(8);
		b << 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0;


		Eigen::VectorXd yi(model);
		Eigen::VectorXd yi_plus_1(model);
		yi << 0.6, 0.4, 0.5, 0.5, 0.3, 0.5, 0.3, 0.4;

		
		LotkaModell lm(a, b);
		solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, numberOfLoops_seq_rk4, step_seq_rk4);
	}
	else if (model == 9) {
		numberOfEquations = 9;
		Eigen::MatrixXd a(9, 9);
		a.row(0) << -0.0014, -0.0017, -0.0016, -0.0010, -0.0012, -0.0003, -0.0007, -0.0002, -0.0004;
		a.row(1) << -0.0004, -0.0014, -0.0017, -0.0016, -0.0010, -0.0012, -0.0003, -0.0007, -0.0002;
		a.row(2) << -0.0002, -0.0004, -0.0014, -0.0017, -0.0016, -0.0010, -0.0012, -0.0003, -0.0007;
		a.row(3) << -0.0007, -0.0002, -0.0004, -0.0014, -0.0017, -0.0016, -0.0010, -0.0012, -0.0003;
		a.row(4) << -0.0003, -0.0007, -0.0002, -0.0004, -0.0014, -0.0017, -0.0016, -0.0010, -0.0012;
		a.row(5) << -0.0012, -0.0003, -0.0007, -0.0002, -0.0004, -0.0014, -0.0017, -0.0016, -0.0010;
		a.row(6) << -0.0010, -0.0012, -0.0003, -0.0007, -0.0002, -0.0004, -0.0014, -0.0017, -0.0016;
		a.row(7) << -0.0016, -0.0010, -0.0012, -0.0003, -0.0007, -0.0002, -0.0004, -0.0014, -0.0017;
		a.row(8) << -0.0017, -0.0016, -0.0010, -0.0012, -0.0003, -0.0007, -0.0002, -0.0004, -0.0014;

		Eigen::VectorXd b(9);
		b << 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0;


		Eigen::VectorXd yi(model);
		Eigen::VectorXd yi_plus_1(model);
		yi << 0.6, 0.4, 0.5, 0.5, 0.3, 0.5, 0.3, 0.4, 0.5;

		LotkaModell lm(a, b);
		solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, numberOfLoops_seq_rk4, step_seq_rk4);
	}
	else if (model == 10) {
		numberOfEquations = 10;
		Eigen::MatrixXd a(10, 10);
		a.row(0) << -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009;
		a.row(1) << -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014;
		a.row(2) << -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002;
		a.row(3) << -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007;
		a.row(4) << -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017;
		a.row(5) << -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012;
		a.row(6) << -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010;
		a.row(7) << -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008;
		a.row(8) << -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015;
		a.row(9) << -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016;

		Eigen::VectorXd b(10);
		b << 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0;


		Eigen::VectorXd yi(model);
		Eigen::VectorXd yi_plus_1(model);
		yi << 0.6, 0.4, 0.5, 0.5, 0.3, 0.5, 0.3, 0.4, 0.5, 0.4;

		
		LotkaModell lm(a, b);
		solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, numberOfLoops_seq_rk4, step_seq_rk4);
	}
	else if (model == 20) {
		numberOfEquations = 20;
		Eigen::MatrixXd a(20, 20);
		a.row(0) << -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009;
		a.row(1) << -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014;
		a.row(2) << -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002;
		a.row(3) << -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007;
		a.row(4) << -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017;
		a.row(5) << -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012;
		a.row(6) << -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010;
		a.row(7) << -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008;
		a.row(8) << -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015;
		a.row(9) << -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016;
		a.row(10) << -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009;
		a.row(11) << -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014;
		a.row(12) << -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002;
		a.row(13) << -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007;
		a.row(14) << -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017;
		a.row(15) << -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012;
		a.row(16) << -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010;
		a.row(17) << -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008;
		a.row(18) << -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015;
		a.row(19) << -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016, -0.0015, -0.0008, -0.0010, -0.0012, -0.0017, -0.0007, -0.0002, -0.0014, -0.0009, -0.0016;

		Eigen::VectorXd b(20);
		b << 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0;


		Eigen::VectorXd yi(model);
		Eigen::VectorXd yi_plus_1(model);
		yi << 0.6, 0.4, 0.5, 0.5, 0.3, 0.5, 0.3, 0.4, 0.5, 0.4, 0.6, 0.4, 0.5, 0.5, 0.3, 0.5, 0.3, 0.4, 0.5, 0.4;

		LotkaModell lm(a, b);
		solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, numberOfLoops_seq_rk4, step_seq_rk4);
	}
	else {
		std::cout << "Not Valid model choose : 0 for single track model, n for lotk-volterra with size n" << std::endl;
		exit(-1);
	}
	
	
}

void solution_method(int solve_way, AbstractModell& mod, Eigen::VectorXd& yi, Eigen::VectorXd& yi_plus_1,int numberOfEquations, double step, long long numberOfLoops
	        , long long numberOfLoops_seq_rk4, double step_seq_rk4) {
		
		std::string model_name = "";
		if (typeid(mod) == typeid(LotkaModell)) {
			model_name = "LotkaModell size " + std::to_string(numberOfEquations);
		}
		else {
			model_name = "single track model size " + std::to_string(numberOfEquations);
		}

		std::vector<Eigen::VectorXd> y;
		y.push_back(yi);
		y.push_back(yi);
		y.push_back(yi);
		y.push_back(yi);

		std::cout << "Solving now the model with Scheduler" << std::endl;
		/*Scheduler*/
		double user_error = 0.001;
		Scheduler scheduler(mod, yi, numberOfEquations, numberOfLoops, step, user_error);
		//scheduler.test();
		std::cout << "///////////////////////////////////////////////////" << std::endl;
		auto start = MyLibrary::startTimer();
		scheduler.run();
		auto stop = MyLibrary::stopTimer();
		auto duration = MyLibrary::durationTime(start, stop);
		duration = ((duration / 1000) / 1000) / 60;
		std::cout << "Time taken by scheduler: " << duration << " minutes " << "to solve " << model_name << std::endl;
		std::cout << "Printing now the results of scheduler" << std::endl;
		scheduler.print_scheduler_result();

		std::cout << "Solving now the model seq. with RK4" << std::endl;
		std::vector<std::string> seq_result;

		std::ofstream myfile_Euler("resultSequRK4.txt");
		auto start1 = MyLibrary::startTimer();

		RK4 rk4;

		double t = 0;

		for (int i = 1; i < numberOfLoops; i++) {
			t = t + step;

			for (int j = 0; j < numberOfEquations; j++) {
				rk4.step(mod, yi_plus_1, y, j, step);
			}
			std::string str = "";

			myfile_Euler << t << ",";
			str = std::to_string(t) + ",";
			for (int k = 0; k < numberOfEquations; k++)
			{
				myfile_Euler << yi_plus_1(k) << ",";
				str += std::to_string(yi_plus_1(k)) + ",";
			}

			myfile_Euler << std::endl;
			seq_result.push_back(str);

			auto swap1 = y[0];
			auto swap2 = y[1];
			auto swap3 = y[2];
			auto swap4 = y[3];
			y[0] = yi_plus_1;
			y[1] = swap1;
			y[2] = swap2;
			y[3] = swap3;
		}

		auto stop1 = MyLibrary::stopTimer();

		auto duration1 = MyLibrary::durationTime(start, stop);
		duration1 = ((duration1 / 1000) / 1000) / 60;

		std::cout << "Time taken by sequ Rk4: " << duration1 << " minutes " << "to solve " << model_name << std::endl;
		std::cout << "Printing now the results of seq Rk4" << std::endl;

		for (std::vector<std::string>::const_iterator iter = seq_result.begin(); iter != seq_result.end(); ++iter) {
			myfile_Euler << *iter << std::endl;
		}
		
		myfile_Euler.close();
	
		std::cout << "Calling now python script for calculating the er. and plotting it" << std::endl;
		call_python_script(numberOfLoops, numberOfEquations, solve_way, duration1, duration);
}


void call_python_script(long long loops, int numEquations, int sw, double seqTime, double schTime) {

	std::string filename = "calc_error.py " + std::to_string(loops) + " " + std::to_string(numEquations) + " "
							+ std::to_string(sw) + " " + std::to_string(global_size) + " " + std::to_string(seqTime) + " " 
							+ std::to_string(schTime);
	std::string command = "python ";
	command += filename;
	system(command.c_str());
}


