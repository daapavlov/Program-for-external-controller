#ifndef MODEL_ENGINE_H
#define MODEL_ENGINE_H



class Model_Engine
{
public:
    Model_Engine();

    void eng(double in[],double out[]);
    void init(double p_in1[],double pout[]);


private:
    void ca(double hp,double *tatm,double *patm);
    void f08b24(double b,double gto,double gtf,double gb,double *k,double *al);
    double entalp (double t,double q);
    double tent (double i,double q);
    void fint1(const double xm[],int jx,double x,double *y,int l);
    void fint3n(const double xm[],double x1,double x2,double x3,double *y,int l);
    void fint2n(const double xm[],double x1,double x2,double *y,int l);
    double  ogra (double X,double xmi,double xma);
    void fint1iden(double xm[],int jx,double x,double *y,int l);
    double f08b36(double x,int m,double c[]);
    double entd(double t,double q);
    void fint3on(const double xm[],int jx1z,int jx2z,int jx3z,
    int kz[7][5],double qz[11][5],int l);
    void fint2on(const double xm[],int jx1z,int jx2z,int kz[5][8],double qz[5][8],int l);



};
#endif // MODEL_ENGINE_H
