#ifndef helpers__array_manipulations_h
#define helpers__array_manipulations_h

#include <vector>

using namespace std;

// double arrays, allocate them
int **al2di(unsigned int, unsigned int);
unsigned int **al2dui(unsigned int, unsigned int);
long **al2dlg(unsigned long, unsigned long);
unsigned long **al2dulg(unsigned long, unsigned long);
float **al2df(unsigned long, unsigned long);

float *JoinArrF(float *, float *, unsigned long, unsigned long);
float *CopyArrF(float *, unsigned long);

float *MultpArrF(float *, float, unsigned long **, unsigned int, unsigned int);
float *MultpArrF(float *, float, unsigned long);
float *DivideArrF(float *, float, unsigned long **, unsigned int, unsigned int);
float *DivideArrF(float *, float, unsigned long);

long FindValueInVect(vector<unsigned int>,
                     unsigned int); //-1 if not found, thus signed long

// Math part
vector<double> MultpVctD(vector<double>, double);
vector<double> DivideVctD(vector<double>, double);
vector<double> SumTwoVctsD(vector<double>, vector<double>); // Vct=Vct1+Vct2
vector<double> SubtrTwoVctsD(vector<double>, vector<double>); // Vct=Vct1-Vct2
vector<double> PowVctD(vector<double>, double); // Vct=pow(Vct1,n)

void FilterVctLowPass(vector<double> &, double);
void FilterVctHighPass(vector<double> &, double);

// additional functions
unsigned long MinimumValueAt(vector<double>);
unsigned long MaximumValueAt(vector<double>);

double MinimumValue(vector<double>);
double MaximumValue(vector<double>);

void PrintArr(float *, unsigned long);
void PrintArr(double *, unsigned long);
void PrintVct(vector<double>);
void PrintVct(vector<unsigned int>);
void JoinVcts(vector<double> &, vector<double>); // Add Vct2 to the end of Vct1

#endif
