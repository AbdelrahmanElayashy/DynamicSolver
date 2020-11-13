#include "EinspurModell.h"


EinspurModell::EinspurModell() {

	this->initialize_model();
}



long double EinspurModell::func(const Eigen::VectorXd& y, const int& rank) {
	long double tmp;
	long double v, psi, alpha, omega, delta;
	long double F_sf, F_sr, F_lr, F_lf, F_Ax, F_Ay;

	v = y(2);
	psi = y(3);
	alpha = y(4);
	omega = y(5);
	delta = y(6);

	F_sf = this->F_sf(v, omega, alpha, delta);
	F_sr = this->F_sr(v, omega, alpha);
	F_lr = this->F_lr(v);
	F_lf = this->F_lf(v);
	F_Ax = this->F_Ax(v);
	F_Ay = this->F_Ay(v);

	switch (rank) {
	case 0:
		tmp = v * cos(psi - alpha);
		return tmp;
	case 1:
		tmp = v * sin(psi - alpha);
		return tmp;
	case 2:
		tmp = 1 / (this->m) * ((F_lr - F_Ax) * cos(alpha) + F_lr * cos(delta + alpha) -
			(F_sr - F_Ay) * sin(alpha) - F_sf * sin(delta + alpha));
		return tmp;
	case 3:
		tmp = omega;
		return tmp;
	case 4:
		tmp = omega - 1 / (this->m * v) * ((F_lr - F_Ax) * sin(alpha) + F_lr * sin(delta + alpha) +
			(F_sr - F_Ay) * cos(alpha) + F_sf * cos(delta + alpha));
		return tmp;
	case 5:
		tmp = 1 / this->Izz * (F_sf * this->lf * cos(delta) + F_lf * this->lf * sin(delta) - F_sr * this->lr - F_Ay * this->esp);
		return tmp;
	case 6:
		tmp = this->is * this->w_delta;
		return tmp;
	default:
		exit(-1);
	}

}


void EinspurModell::initialize_model() {

	this->m = 1239;
	this->g = 9.81;
	this->lf = 1.19026;
	this->lr = 1.37484;
	this->esp = 0.5;
	this->R = 0.302;
	this->r_delta = 0.35225;
	this->Izz = 1752;
	this->cw = 0.3;
	this->row = 1.249512;
	this->A = 1.4378946874;
	this->it = 3.91;
	this->is = 0.0625;
	this->Bf = 10.96;
	this->Br = 12.67;
	this->Cf = 1.3;
	this->Cr = 1.3;
	this->Df = 4560.4;
	this->Dr = 3947.81;
	this->Ef = -0.5;
	this->Er = -0.5;
	this->fR0 = 0.009;
	this->fR1 = 0.002;
	this->fR4 = 0.0003;
	//////
	this->F_B = 1;
	this->fee = 1;
	this->mu = 1;
	this->w_delta = -this->r_delta / this->is;
}