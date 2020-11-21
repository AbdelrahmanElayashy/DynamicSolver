#include "ExplicitEuler.h"
#include "ApproxRK4.h"
#include "LotkaModell.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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

void solution_method(int solve_way, AbstractModell& mod, Eigen::VectorXd& yi, Eigen::VectorXd& yi_plus_1, int numberOfEquations, double step, long long numberOfLoops, int numTest);

void solution_method2(int solve_way, AbstractModell& mod, Eigen::VectorXd& yi, Eigen::VectorXd& yi_plus_1, int numberOfEquations, double step, long long numberOfLoops);

void call_python_script_time(int numEquations, int solve_w, double);
void call_python_script_error(std::string fi, long numLoops, int numEquations, double step, int sw);

std::string merge_files(AbstractModell& mod, const int numberOfEquation, const double step);

std::vector<double> executionTimes;
std::vector<double> executionTimes_seq;

 int num_tests = 10;
std::string merged_file[50];
std::vector<std::string> seq_file_name;

int main(int argc, char* argv[]) {


	int model = std::atoi(argv[1]);

	num_tests = std::atoi(argv[2]);
	int solve_way = 1; // for python script time to determine the model.
	double step = 0.001;
	int numberOfEquations = model;
	long long numberOfLoops = 300000;


	if (model == 0) {
		EinspurModell lm;
		numberOfEquations = 7;
		Eigen::VectorXd yi(7);
		Eigen::VectorXd yi_plus_1(7);
		yi << 0, 0, 10, 0, 0, 0, 0;
		solve_way = 0;
		for (int i = 0; i < 4; ++i) {
			if (i == 0) {
				double seq_step = 0.001;
				double seq_loops = 300000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				seq_step = 0.00001;
				seq_loops = 30000000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				
			}
			for (int j = 0; j < 10; ++j) {
				solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, j);
			}
			std::string search_file = merge_files(lm, numberOfEquations, step);
			std::cout << "plotting now the execution time and error" << std::endl;
			call_python_script_time(numberOfEquations, solve_way, step);
			call_python_script_error(search_file, numberOfLoops, numberOfEquations, step, solve_way);
			executionTimes.clear();
			numberOfLoops *= 10;
			step /= 10;
		}
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
		for (int i = 0; i < 4; ++i) {
			if (i == 0) {
				double seq_step = 0.001;
				double seq_loops = 300000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				seq_step = 0.00001;
				seq_loops = 30000000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
			}
			for (int j = 0; j < 10; ++j) {
				solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, j);
			}
			std::string search_file = merge_files(lm, numberOfEquations, step);
			std::cout << "plotting now the execution time and error" << std::endl;
			call_python_script_time(numberOfEquations, solve_way, step);
			call_python_script_error(search_file, numberOfLoops, numberOfEquations, step, solve_way);
			executionTimes.clear();
			numberOfLoops *= 10;
			step /= 10;
		}

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
		for (int i = 0; i < 4; ++i) {
			if (i == 0) {
				double seq_step = 0.001;
				double seq_loops = 300000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				seq_step = 0.00001;
				seq_loops = 30000000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
			}
			for (int j = 0; j < 10; ++j) {
				solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, j);
			}
			std::string search_file = merge_files(lm, numberOfEquations, step);
			std::cout << "plotting now the execution time and error" << std::endl;
			call_python_script_time(numberOfEquations, solve_way, step);
			call_python_script_error(search_file, numberOfLoops, numberOfEquations, step, solve_way);
			executionTimes.clear();
			numberOfLoops *= 10;
			step /= 10;
		}
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
		for (int i = 0; i < 4; ++i) {
			if (i == 0) {
				double seq_step = 0.001;
				double seq_loops = 300000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				seq_step = 0.00001;
				seq_loops = 30000000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
			}
			for (int j = 0; j < 10; ++j) {
				solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, j);
			}
			std::string search_file = merge_files(lm, numberOfEquations, step);
			std::cout << "plotting now the execution time and error" << std::endl;
			call_python_script_time(numberOfEquations, solve_way, step);
			call_python_script_error(search_file, numberOfLoops, numberOfEquations, step, solve_way);
			executionTimes.clear();
			numberOfLoops *= 10;
			step /= 10;
		}
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
		for (int i = 0; i < 4; ++i) {
			if (i == 0) {
				double seq_step = 0.001;
				double seq_loops = 300000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				seq_step = 0.00001;
				seq_loops = 30000000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
			}
			for (int j = 0; j < 10; ++j) {
				solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, j);
			}
			std::string search_file = merge_files(lm, numberOfEquations, step);
			std::cout << "plotting now the execution time and error" << std::endl;
			call_python_script_time(numberOfEquations, solve_way, step);
			call_python_script_error(search_file, numberOfLoops, numberOfEquations, step, solve_way);
			executionTimes.clear();
			numberOfLoops *= 10;
			step /= 10;
		}
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
		for (int i = 0; i < 4; ++i) {
			if (i == 0) {
				double seq_step = 0.001;
				double seq_loops = 300000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				seq_step = 0.00001;
				seq_loops = 30000000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
			}
			for (int j = 0; j < 10; ++j) {
				solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, j);
			}
			std::string search_file = merge_files(lm, numberOfEquations, step);
			std::cout << "plotting now the execution time and error" << std::endl;
			call_python_script_time(numberOfEquations, solve_way, step);
			call_python_script_error(search_file, numberOfLoops, numberOfEquations, step, solve_way);
			executionTimes.clear();
			numberOfLoops *= 10;
			step /= 10;
		}
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
		for (int i = 0; i < 4; ++i) {
			if (i == 0) {
				double seq_step = 0.001;
				double seq_loops = 300000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				seq_step = 0.00001;
				seq_loops = 30000000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
			}
			for (int j = 0; j < 10; ++j) {
				solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, j);
			}
			std::string search_file = merge_files(lm, numberOfEquations, step);
			std::cout << "plotting now the execution time and error" << std::endl;
			call_python_script_time(numberOfEquations, solve_way, step);
			call_python_script_error(search_file, numberOfLoops, numberOfEquations, step, solve_way);
			executionTimes.clear();
			numberOfLoops *= 10;
			step /= 10;
		}
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
		for (int i = 0; i < 4; ++i) {
			if (i == 0) {
				double seq_step = 0.001;
				double seq_loops = 300000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				seq_step = 0.00001;
				seq_loops = 30000000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
			}
			for (int j = 0; j < 10; ++j) {
				solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, j);
			}
			std::string search_file = merge_files(lm, numberOfEquations, step);
			std::cout << "plotting now the execution time and error" << std::endl;
			call_python_script_time(numberOfEquations, solve_way, step);
			call_python_script_error(search_file, numberOfLoops, numberOfEquations, step, solve_way);
			executionTimes.clear();
			numberOfLoops *= 10;
			step /= 10;
		}
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
		for (int i = 0; i < 4; ++i) {
			if (i == 0) {
				double seq_step = 0.001;
				double seq_loops = 300000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				seq_step = 0.00001;
				seq_loops = 30000000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
			}
			for (int j = 0; j < 10; ++j) {
				solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, j);
			}
			std::string search_file = merge_files(lm, numberOfEquations, step);
			std::cout << "plotting now the execution time and error" << std::endl;
			call_python_script_time(numberOfEquations, solve_way, step);
			call_python_script_error(search_file, numberOfLoops, numberOfEquations, step, solve_way);
			executionTimes.clear();
			numberOfLoops *= 10;
			step /= 10;
		}
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
		for (int i = 0; i < 4; ++i) {
			if (i == 0) {
				double seq_step = 0.001;
				double seq_loops = 300000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
				seq_step = 0.00001;
				seq_loops = 30000000;
				solution_method2(solve_way, lm, yi, yi_plus_1, numberOfEquations, seq_step, seq_loops);
			}
			for (int j = 0; j < 10; ++j) {
				solution_method(solve_way, lm, yi, yi_plus_1, numberOfEquations, step, numberOfLoops, j);
			}
			std::string search_file = merge_files(lm, numberOfEquations, step);
			std::cout << "plotting now the execution time and error" << std::endl;
			call_python_script_time(numberOfEquations, solve_way, step);
			call_python_script_error(search_file, numberOfLoops, numberOfEquations, step, solve_way);
			executionTimes.clear();
			numberOfLoops *= 10;
			step /= 10;
		}
	}
	else {
		std::cout << "Not Valid model choose : 0 for single track model, n for lotk-volterra with size n" << std::endl;
		exit(-1);
	}
	
	
}

void solution_method(int solve_way, AbstractModell& mod, Eigen::VectorXd& yi, Eigen::VectorXd& yi_plus_1,int numberOfEquations, double step, long long numberOfLoops, int numTest) {
		
		std::string model_name = "";
		if (typeid(mod) == typeid(LotkaModell)) {
			model_name = "LotkaModel";
		}
		else {
			model_name = "trackModel";
		}
		std::cout << "solving now the model with scheduler: iteration test" << numTest << ", step: " << step << std::endl;
		
		double user_error = 0.001;
		std::string file_name = "Rscheduler_" + model_name + "_" + std::to_string(numberOfEquations) + "_" + std::to_string(step) + "_" +std::to_string(numTest) +".txt";
		Scheduler scheduler(mod, yi, numberOfEquations, numberOfLoops, step, user_error, file_name);
		auto start = MyLibrary::startTimer();
		scheduler.run();
		auto stop = MyLibrary::stopTimer();
		auto duration = MyLibrary::durationTime(start, stop);
		duration = ((duration / 1000) / 1000) / 60;
		executionTimes.push_back(duration);
		merged_file[numTest] = file_name;
		scheduler.print_scheduler_result();

}

std::string merge_files(AbstractModell& mod, const int numberOfEquation, const double step) {

	std::cout << "merging the files now" << std::endl;

	std::string model_name = "";
	if (typeid(mod) == typeid(LotkaModell)) {
		model_name = "LotkaModel";
	}
	else {
		model_name = "trackModel";
	}
	std::string file_name = "Rscheduler_" + model_name + "_" + std::to_string(numberOfEquation) + "_" + std::to_string(step) + ".txt";
	std::fstream* streams = new std::fstream [num_tests];
	std::ofstream out_result(file_name);

	for (int i = 0; i < num_tests; ++i) {
		streams[i].open(merged_file[i]);
	}

	double *numbers = new double [numberOfEquation + 1];
	int count = 0;
	std::string *lines = new std::string [num_tests];
	bool condition = false;
	while (true) {
		for (int k = 0; k < num_tests; ++k) {
			if (!std::getline(streams[k], lines[k]))
				condition = true;
		}
		if (condition)
			break;

		count = 0;
		std::stringstream* str_stream = new std::stringstream [num_tests];
		for (int j = 0; j < num_tests; ++j) {
			str_stream[j] << lines[j];
		}

		while (count < numberOfEquation + 1) {
			std::string *strs = new std::string [num_tests];
			for (int q = 0; q < num_tests; ++q) {
				std::getline(str_stream[q], strs[q], ',');
			}
			if (count == 0) {
				numbers[count] = std::stod(strs[0]);
				count++;
				continue;
			}
			numbers[count] = 0;
			for (int g = 0; g < num_tests; ++g) {
				numbers[count] += std::stod(strs[g]);
			}
		    numbers[count] /= num_tests;
			count++;
		}

		for (int i = 0; i < numberOfEquation + 1; ++i) {
			out_result << numbers[i] << ",";
		}
		out_result << std::endl;

	}

	for (int i = 0; i < num_tests; ++i) {
		streams[i].close();
		std::remove(merged_file[i].c_str());
	}
	return file_name;

}


void solution_method2(int solve_way, AbstractModell& mod, Eigen::VectorXd& yi, Eigen::VectorXd& yi_plus_1, int numberOfEquations, double step, long long numberOfLoops)
{
	std::cout << "solving the model with seq. rk4" << std::endl;

	std::string model_name = "";
	if (typeid(mod) == typeid(LotkaModell)) {
		model_name = "LotkaModel";
	}
	else {
		model_name = "trackModel";
	}

	std::vector<std::string> seq_result;

	std::vector<Eigen::VectorXd> y;
	y.push_back(yi);
	y.push_back(yi);
	y.push_back(yi);
	y.push_back(yi);

	std::string file_name = "Rseq_" + model_name + "_" + std::to_string(numberOfEquations) + "_" + std::to_string(step) + ".txt";
	std::ofstream myfile_Euler(file_name);
	auto start1 = MyLibrary::startTimer();

	RK4 rk4;

	double t = 0;

	for (int i = 1; i < numberOfLoops; i++) {
		t = t + step;

		for (int j = 0; j < numberOfEquations; j++) {
			rk4.step(mod, yi_plus_1, y, j, step);
		}

		std::string str = "";

	//	myfile_Euler << t << ",";
		str = std::to_string(t) + ",";
		for (int k = 0; k < numberOfEquations; k++)
		{
	//		myfile_Euler << yi_plus_1(k) << ",";
			str += std::to_string(yi_plus_1(k)) + ",";
		}

	//	myfile_Euler << std::endl;
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

	auto duration1 = MyLibrary::durationTime(start1, stop1);
	duration1 = ((duration1 / 1000) / 1000) / 60;
	executionTimes_seq.push_back(duration1);

	for (std::vector<std::string>::const_iterator iter = seq_result.begin(); iter != seq_result.end(); ++iter) {
		myfile_Euler << *iter << std::endl;
	}

	seq_file_name.push_back(file_name);
	myfile_Euler.close();



}


void call_python_script_time(int numEquations, int sw, double step_size) {


	std::string ex_time = " ";
	for (int i = 0; i < num_tests; i++) {
		ex_time += std::to_string(executionTimes.at(i)) + " ";
	}
	ex_time += std::to_string(executionTimes_seq.at(0)) + " " + std::to_string(executionTimes_seq.at(1)) + +" "
		+ std::to_string(sw) + " " + std::to_string(step_size) + " " + std::to_string(numEquations);

	std::string filename = "draw_execution_time.py " + std::to_string(num_tests) + ex_time;
	std::string command = "python ";
	command += filename;
	system(command.c_str());
}

void call_python_script_error(std::string fi, long numLoops, int numEquations, double step, int sw) {

	std::string args = fi + " " + seq_file_name.at(0) + " " + seq_file_name.at(1) + " " + std::to_string(numLoops) + " " +
		std::to_string(numEquations) + " " + std::to_string(step) + " " + std::to_string(sw);
	std::string filename = "calculate_model_error.py " + args;
	std::string command = "python ";
	command += filename;
	system(command.c_str());
}


