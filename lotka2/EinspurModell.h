#pragma once
#include "AbstractModell.h"
#include <cmath>

class EinspurModell :
    public AbstractModell
{


private:
    double m;
    double g;
    double lf, lr;
    double esp;
    double R;
    double r_delta;
    double Izz;
    double cw;
    double row;
    double A;
    double it;
    double is;
    double Bf, Br;
    double Cf, Cr;
    double Df, Dr;
    double Ef, Er;
    double fR0, fR1, fR4;
    /////////////
    double F_B;
    double fee;
    double mu;
    double w_delta; /// maybe! error

    void initialize_model();
    ///error maybe!
    inline long double F_sf(long double v, long double psi, long double alpha, long double delta)//(v, omega, alpha, delta)
    {
        long double alpha_f_tmp;
        alpha_f_tmp = this->alpha_f(v, psi, alpha, delta);

        return this->Df * sin(this->Cf * atan(this->Bf * alpha_f_tmp - this->Ef * (this->Bf * alpha_f_tmp - atan(this->Bf * alpha_f_tmp))));
    };

    inline long double F_sr(long double v, long double psi, long double alpha)
    {
        long double alpha_r_tmp;
        alpha_r_tmp = this->alpha_r(v, psi, alpha);

        return this->Dr * sin(this->Cr * atan(this->Br * alpha_r_tmp - this->Er * (this->Br * alpha_r_tmp - atan(this->Br * alpha_r_tmp))));
    };

    inline long double alpha_f(long double v, long double psi_dot, long double alpha, long double delta)
    {
        return delta - atan((this->lf * psi_dot - v * sin(alpha)) / (v * cos(alpha)));
    };

    inline long double alpha_r(long double v, long double psi_dot, long double alpha)
    {
        return atan((this->lr * psi_dot + v * sin(alpha)) / (v * cos(alpha)));
    };

    inline long double F_Ax(long double v)
    {
        return 0.5 * this->cw * this->row * this->A * std::pow(v, 2);
    };

    inline long double F_Ay(long double v)
    {
        return 0;
    };

    inline long double F_Rf(long double v)
    {
        return ((this->m * this->lr * this->g) * (this->fR0 + 0.036 * this->fR1 * v + this->fR4 * std::pow(0.036 * v, 4))) / (this->lr + this->lf);
    };

    inline long double F_Rr(long double v)
    {
        return ((this->m * this->lf * this->g) * (this->fR0 + 0.036 * this->fR1 * v + this->fR4 * std::pow(0.036 * v, 4))) / (this->lr + this->lf);
    };

    inline long double F_lf(long double v)
    {
        return -2 / 3 * this->F_B - this->F_Rf(v);
    };

    inline long double F_lr(long double v)
    {
        long double tmp1 = this->ig();
        long double tmp2 = this->M(v);
        long double tmp3 = this->F_Rr(v);

        return ((tmp1 * this->it * tmp2) / this->R) - 1 / 3 * F_B - tmp3;
    };

    inline long double M(long double v)
    {
        long double f1 = this->f1();
        long double wm = this->wm(v);
        return  f1 * this->f2(wm) + (1 - f1) * this->f3(wm);

    };

    inline long double wm(long double v)
    {
        return (v * this->it * this->ig()) / this->R;

    };

    inline long double f1(void)
    {
        return 1 - exp(-3 * this->fee);
    };

    inline long double f2(long double v)
    {
        return -37.8 + 1.54 * v - 0.0019 * std::pow(v, 2);
    };

    inline long double f3(long double v)
    {
        return -34.9 - 0.04775 * v;
    };

    inline long double ig(void)
    {
        return 0.028625 * std::pow(this->mu, 4) - 0.43525 * std::pow(this->mu, 3) + 2.513875 * std::pow(this->mu, 2) - 6.83225 * this->mu + 8.635;
    };

public:

    EinspurModell(void);
    virtual long double func(const Eigen::VectorXd& y, const int& rank) override;
};

