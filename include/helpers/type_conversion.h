#ifndef helpers__type_conversion_h
#define helpers__type_conversion_h

#include <string>
#include <vector>

using namespace std;

char *str2char(const string &); // conversion

unsigned int str2uint(const string &);

int str2int(const string &);

float *vct2arrF(const vector<double> &);
float *vct2arrF(const vector<float> &);

double *vct2arrD(const vector<double> &);
double str2double(const string &);

vector<float> arr2vctF(const float *arr, const unsigned long &);

#endif
