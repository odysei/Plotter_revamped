#ifndef helpers__array_manipulations_cpp
#define helpers__array_manipulations_cpp

#include "helpers/array_manipulations.h"
#include <iostream>
#include <cassert>
#include <cstring>  //for memset
#include <cmath>
#include <algorithm>    //for Min/Max values in vector

int **al2di(unsigned int nm1, unsigned int nm2)
{
    int **array;
    array = new int *[nm1];
    assert(array != 0);
    for (unsigned int i = 0; i < nm1; i++) {
        array[i] = new int[nm2];
        assert(array[i] != 0);
        memset(array[i], 0, sizeof(int) * nm2);
    }

    return array;
}

unsigned int **al2dui(unsigned int nm1, unsigned int nm2)
{
    unsigned int **array;
    array = new unsigned int *[nm1];
    assert(array != 0);
    for (unsigned int i = 0; i < nm1; i++) {
        array[i] = new unsigned int[nm2];
        assert(array[i] != 0);
        memset(array[i], 0, sizeof(unsigned int) * nm2);
    }

    return array;
}

long **al2dlg(unsigned long nm1, unsigned long nm2)
{
    long **array;
    array = new long *[nm1];
    assert(array != 0);
    for (unsigned long i = 0; i < nm1; i++) {
        array[i] = new long[nm2];
        assert(array[i] != 0);
        memset(array[i], 0, sizeof(long) * nm2);
    }

    return array;
}

unsigned long **al2dulg(unsigned long nm1, unsigned long nm2)
{
    unsigned long **array;
    array = new unsigned long *[nm1];
    assert(array != 0);
    for (unsigned long i = 0; i < nm1; i++) {
        array[i] = new unsigned long[nm2];
        assert(array[i] != 0);
        memset(array[i], 0, sizeof(unsigned long) * nm2);
    }

    return array;
}

float **al2df(unsigned long nm1, unsigned long nm2)
{
    float **array;
    array = new float *[nm1];
    assert(array != 0);
    for (unsigned long i = 0; i < nm1; i++) {
        array[i] = new float[nm2];
        assert(array[i] != 0);
        memset(array[i], 0, sizeof(float) * nm2);
    }

    return array;
}

float *JoinArrF(float *array1, float *array2, unsigned long size1,
                unsigned long size2)
{
    float *array = new float[size1 + size2];
    unsigned long i;
    for (i = 0; i < size1; i++) {
        array[i] = array1[i];
    }
    for (i = 0; i < size2; i++) {
        array[i + size1] = array2[i];
    }
    return array;
}

float *CopyArrF(float *array1, unsigned long size1)
{
    float *array = new float[size1];
    for (unsigned long i = 0; i < size1; i++) {
        array[i] = array1[i];
    }
    return array;
}

float *MultpArrF(float *data, float multip, unsigned long **arrsz,
                 unsigned int nm1, unsigned int nm2)
{
    for (unsigned long i = 0; i < arrsz[nm1][nm2]; i++) {
        data[i] = data[i] * multip;
    }
    return data;
}

float *MultpArrF(float *data, float multip, unsigned long size)
{
    for (unsigned long i = 0; i < size; i++) {
        data[i] = data[i] * multip;
    }
    return data;
}

float *DivideArrF(float *data, float divid, unsigned long **arrsz,
                  unsigned int nm1, unsigned int nm2)
{
    for (unsigned long i = 0; i < arrsz[nm1][nm2]; i++) {
        data[i] = data[i] / divid;
    }
    return data;
}

float *DivideArrF(float *data, float divid, unsigned long size)
{
    for (unsigned long i = 0; i < size; i++) {
        data[i] = data[i] / divid;
    }
    return data;
}

long FindValueInVect(vector<unsigned int> input, unsigned int value)
{
    long cnt = 0;
    if (input.size() != 0)
        for (; cnt < (long)input.size() && input[cnt] != value; cnt++) {
        }
    if (cnt == (long)input.size())
        return -1; // Did not find
    else
        return cnt; // Value was found, first one
    return 0; // Input vector was empty
}

vector<double> MultpVctD(vector<double> internalVct, double multip)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        internalVct[i] = internalVct[i] * multip;
    }
    return internalVct;
}

vector<double> DivideVctD(vector<double> internalVct, double divide)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        internalVct[i] = internalVct[i] / divide;
    }
    return internalVct;
}

vector<double> SumTwoVctsD(vector<double> Vector1, vector<double> Vector2)
{
    double a = 0, b = 0;
    vector<double> Vector;
    if (Vector1.size() != Vector2.size())
        cout << endl << "WARNING! Vectors sizes mismatch. Forcing" << endl;
    for (unsigned long i = 0; i < Vector1.size() || i < Vector2.size(); i++) {
        if (i < Vector1.size())
            a = Vector1[i];
        else
            a = 0;
        if (i < Vector2.size())
            b = Vector2[i];
        else
            b = 0;
        Vector.push_back(a + b);
    }
    return Vector;
}

vector<double> SubtrTwoVctsD(vector<double> Vector1,
                             vector<double> Vector2) // Vct=Vct1-Vct2
{
    double a = 0, b = 0;
    vector<double> Vector;
    if (Vector1.size() != Vector2.size())
        cout << endl << "WARNING! Vectors sizes mismatch. Forcing" << endl;
    for (unsigned long i = 0; i < Vector1.size() || i < Vector2.size(); i++) {
        if (i < Vector1.size())
            a = Vector1[i];
        else
            a = 0;
        if (i < Vector2.size())
            b = Vector2[i];
        else
            b = 0;
        Vector.push_back(a - b);
    }
    return Vector;
}

vector<double> PowVctD(vector<double> Vector, double degree) // Vct=pow(Vct1,n)
{
    for (unsigned long i = 0; i < Vector.size(); i++)
        Vector.at(i) = pow(Vector[i], degree);
    return Vector;
}

///// Additional functions /////
unsigned long MinimumValueAt(vector<double> input)
{
    std::vector<double>::iterator result =
        std::min_element(input.begin(), input.end());
    return std::distance(input.begin(), result);
}

unsigned long MaximumValueAt(vector<double> input)
{
    std::vector<double>::iterator result =
        std::max_element(input.begin(), input.end());
    return std::distance(input.begin(), result);
}

double MinimumValue(vector<double> input)
{
    return input[MinimumValueAt(input)];
}

double MaximumValue(vector<double> input)
{
    return input[MaximumValueAt(input)];
}

void FilterVctLowPass(vector<double> &InputVct, double value)
{
    vector<double> OutputVct;
    for (unsigned long cnt = 0; cnt < InputVct.size(); cnt++) {
        if (InputVct[cnt] < value)
            OutputVct.push_back(InputVct[cnt]);
    }
    InputVct = OutputVct;
}

void FilterVctHighPass(vector<double> &InputVct, double value)
{
    vector<double> OutputVct;
    for (unsigned long cnt = 0; cnt < InputVct.size(); cnt++) {
        if (InputVct[cnt] > value)
            OutputVct.push_back(InputVct[cnt]);
    }
    InputVct = OutputVct;
}

void PrintArr(float *arr, unsigned long size)
{
    for (unsigned long i = 0; i < size; i++)
        cout << arr[i] << endl;
}

void PrintArr(double *arr, unsigned long size)
{
    for (unsigned long i = 0; i < size; i++)
        cout << arr[i] << endl;
}

void PrintVct(vector<double> data)
{
    for (unsigned long i = 0; i < data.size(); i++)
        cout << data[i] << endl;
}

void PrintVct(vector<unsigned int> data)
{
    for (unsigned long i = 0; i < data.size(); i++)
        cout << data[i] << endl;
}

void JoinVcts(vector<double> &Vector1,
              vector<double> Vector2) // Add Vct2 to the end of Vct1
{
    for (unsigned long i = 0; i < Vector2.size(); i++)
        Vector1.push_back(Vector2[i]);
}

#endif
