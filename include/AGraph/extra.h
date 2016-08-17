#ifndef AGraph__extra_h
#define AGraph__extra_h

#include <string>

using namespace std;

////Functions for Library Auto Plotting

string ExtractSentence(const string &input);

void ExtractOptionAndValue(const string &input, string &option, string &value);

// First 2: Data XZ, 3-4: Range XZ, 5-6: output coords.
// void AutoLegend(vector<double>, vector<double>, double *, double *, double *,
//                 double *);

#endif
