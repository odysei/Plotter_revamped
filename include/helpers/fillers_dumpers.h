#ifndef helpers__fillers_dumpers_h
#define helpers__fillers_dumpers_h

// Note that as of day of writing (2011-07-08) 2D "SPEC" histograms require
// offset if BinContent is set manually.

#include <TH1I.h>
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include <vector>

using namespace std;

TH1I *FillingHistI(TH1I *, vector<int>);
TH1I *FillingHistI(TH1I *, vector<unsigned int>);

TH1F *FillingHistF(TH1F *, float *, unsigned long **, unsigned int,
                   unsigned int);
TH1F *FillingHistF(TH1F *, vector<double>);

TH1D *FillingHistD(TH1D *, vector<int>);
TH1D *FillingHistD(TH1D *, vector<int>, double); // with weight
TH1D *FillingHistD(TH1D *, vector<unsigned int>);
TH1D *FillingHistD(TH1D *, vector<unsigned int>, double); // with weight
TH1D *FillingHistD(TH1D *, vector<double>);
TH1D *FillingHistD(TH1D *, vector<double>, double);         // with weight
TH1D *FillingHistD(TH1D *, vector<double>, vector<double>); // with weights
TH1D *FillingHistD(TH1D *, vector<double>, vector<double>, int, double,
                   double); // Pt.; Data X;Y; Nr. bins; Min; Max. Dedicated for
                            // small sets but automatically puts into the right
                            // place
TH1D *FillingHistD(TH1D *, vector<double>, vector<double>, vector<double>, int,
                   double, double); // Pt.; Data X;Y; Weights; Nr. bins; Min;
                                    // Max. Dedicated for small sets but
                                    // automatically puts into the right place

void DumpHistD(vector<double>, int, double, double);
void DumpHistD(vector<double>, vector<double>, int, double,
               double); // Data X;Y; Nr. bins; Min; Max. Dedicated for small
                        // sets but automatically puts into the right place.
                        // Same as above but dedicated for printig the contents

// For 2D histograms. WOff: with (bin) offset
TH2D *FillingHist2D(TH2D *, vector<double>, vector<double>);
TH2D *FillingHist2D(TH2D *, vector<double>, vector<double>,
                    vector<double>); // with weights
TH2D *FillingHist2D(TH2D *, vector<double>, vector<double>, vector<double>, int,
                    double, double, int, double,
                    double); // Same as 1D but *;X;Y;Z;X;x;x;Y;y;y
TH2D *FillingHist2DWOff(TH2D *, vector<double>, double, vector<double>, double,
                        vector<double>, double, int, double, double, int,
                        double,
                        double); // Same but *;X;xO;Y;yO;Z;z0;X;x;x;Y;y;y
TH2D *FillingHist2DWOff(TH2D *, vector<double>, double, vector<double>, double,
                        vector<double>, double, vector<double>, int, double,
                        double, int, double,
                        double); // Same but *;X;xO;Y;yO;Z;z0;Wght;X;x;x;Y;y;y
TH2D *FillingHist2DSPEC(TH2D *, vector<double>, vector<double>, vector<double>,
                        int, double, double, int, double, double);
TH2D *FillingHist2DSPECWOff(TH2D *, vector<double>, double, vector<double>,
                            double, vector<double>, double, int, double, double,
                            int, double, double);

void ExtractHistDataXZInRange(
    TH1D *histogram, vector<double> &DataX,
    vector<double> &DataZ); // Extract vectors of X and Z from the histogram
void ExtractHistDataXZErrInRange(TH1D *histogram, vector<double> &DataX,
                                 vector<double> &DataZ, vector<double> &DataE);
void ExtractHistDataXYZInRange(
    TH2D *histogram, vector<double> &DataX, vector<double> &DataY,
    vector<double> &DataZ); // Extract vectors of X, Y, and Z from the histogram
void ExtractHistDataXYZErrInRange(TH2D *histogram, vector<double> &DataX,
                                  vector<double> &DataY, vector<double> &DataZ,
                                  vector<double> &DataE);

#endif
