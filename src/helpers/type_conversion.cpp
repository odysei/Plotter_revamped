#ifndef type_conversion_cpp
#define type_conversion_cpp

#include <cstring>
#include <sstream>

#include "helpers/type_conversion.h"

char *str2char(const string &name)
{
    char *nam;
    nam = new char[name.size() + 1];
    strcpy(nam, name.c_str());
    return nam;
}

unsigned int str2uint(const string &input)
{
    stringstream ss;
    unsigned int output;
    ss << input;
    ss >> output;
    return output;
}

int str2int(const string &input)
{
    stringstream ss;
    int output;
    ss << input;
    ss >> output;
    return output;
}

template<class type>
inline float *convert_(const vector<type> &v)
{
    const auto sz = v.size();
    float *a = new float[sz];
    for (unsigned long i = 0; i < sz; ++i)
        a[i] = static_cast<float>(v[i]);
    return a;
}

float *vct2arrF(const vector<double> &v)
{
    return convert_(v);
}

float *vct2arrF(const vector<float> &v)
{
    return convert_(v);
}

double *vct2arrD(const vector<double> &v)
{
    const auto sz = v.size();
    double *a = new double[sz];
    for (unsigned long i = 0; i < sz; ++i)
        a[i] = static_cast<double>(v[i]);
    return a;
}

double str2double(const string &input)
{
    stringstream ss;
    double output;
    ss << input;
    ss >> output;
    return output;
}

vector<float> arr2vctF(const float *arr, const unsigned long &sz)
{
    vector<float> v;
    v.reserve(sz);
    for (unsigned long i = 0; i < sz; ++i)
        v.push_back(arr[i]);

    return v;
}

#endif
