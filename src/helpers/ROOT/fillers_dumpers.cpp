#ifndef helpers__fillers_dumpers_cpp
#define helpers__fillers_dumpers_cpp

// Note that as of day of writing (2011-07-08) 2D "SPEC" histograms require
// offset if BinContent is set manually.

#include "helpers/fillers_dumpers.h"
#include <algorithm>
#include <iostream>

TH1I *FillingHistI(TH1I *hist, vector<int> internalVct)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        hist->Fill(internalVct[i]);
    }
    return hist;
}

TH1I *FillingHistI(TH1I *hist, vector<unsigned int> internalVct)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        hist->Fill(internalVct[i]);
    }
    return hist;
}

TH1F *FillingHistF(TH1F *hist, float *arr, unsigned long **arrsz,
                   unsigned int nm1, unsigned int nm2)
{
    unsigned long cnt = (unsigned long)arrsz[nm1][nm2];
    for (unsigned long i = 0; i < cnt; i++) {
        hist->Fill(arr[i]);
    }
    return hist;
}

TH1F *FillingHistF(TH1F *hist, vector<double> internalVct)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        hist->Fill(internalVct[i]);
    }
    return hist;
}

TH1D *FillingHistD(TH1D *hist, vector<int> internalVct)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        hist->Fill((double)internalVct[i]);
    }
    return hist;
}

TH1D *FillingHistD(TH1D *hist, vector<int> internalVct, double weight)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        hist->Fill((double)internalVct[i], weight);
    }
    return hist;
}

TH1D *FillingHistD(TH1D *hist, vector<unsigned int> internalVct)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        hist->Fill((double)internalVct[i]);
    }
    return hist;
}

TH1D *FillingHistD(TH1D *hist, vector<unsigned int> internalVct, double weight)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        hist->Fill((double)internalVct[i], weight);
    }
    return hist;
}

TH1D *FillingHistD(TH1D *hist, vector<double> internalVct)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        hist->Fill(internalVct[i]);
    }
    return hist;
}

TH1D *FillingHistD(TH1D *hist, vector<double> internalVct, double weight)
{
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        hist->Fill(internalVct[i], weight);
    }
    return hist;
}

TH1D *FillingHistD(TH1D *hist, vector<double> internalVct,
                   vector<double> weight)
{
    if (internalVct.size() != weight.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    for (unsigned long i = 0; i < internalVct.size(); i++) {
        hist->Fill(internalVct[i], weight[i]);
    }
    return hist;
}

TH1D *FillingHistD(TH1D *hist, vector<double> DataX, vector<double> DataZ,
                   int Nbins, double Xmin, double Xmax)
{
    if (DataX.size() != DataZ.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    int BinNr;
    double Present_value, step = (Xmax - Xmin) / Nbins;
    for (unsigned long i = 0; i < DataX.size(); i++) {
        BinNr = 1;
        while (DataX[i] < Xmin && i < (DataX.size() - 1))
            i++;
        while (DataX[i] > Xmax && i < (DataX.size() - 1))
            i++;
        while (DataX[i] >= (BinNr * step + Xmin) && DataX[i] < Xmax)
            BinNr++;

        if (BinNr <= Nbins) {
            Present_value = hist->GetBinContent(BinNr);
            hist->SetBinContent(BinNr, (Present_value + DataZ[i]));
        }
    }
    return hist;
}

TH1D *FillingHistD(TH1D *hist, vector<double> DataX, vector<double> DataZ,
                   vector<double> DataWeight, int Nbins, double Xmin,
                   double Xmax)
{
    if (DataX.size() != DataZ.size() || DataX.size() != DataWeight.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    int BinNr;
    double Present_value, step = (Xmax - Xmin) / Nbins;
    for (unsigned long i = 0; i < DataX.size(); i++) {
        BinNr = 1;
        while (DataX[i] < Xmin && i < (DataX.size() - 1))
            i++;
        while (DataX[i] > Xmax && i < (DataX.size() - 1))
            i++;
        while (DataX[i] >= (BinNr * step + Xmin) && DataX[i] < Xmax)
            BinNr++;

        if (BinNr <= Nbins) {
            Present_value = hist->GetBinContent(BinNr);
            hist->SetBinContent(BinNr,
                                (Present_value + DataZ[i] * DataWeight[i]));
        }
    }
    return hist;
}

void DumpHistD(vector<double> DataX, int Nbins, double Xmin, double Xmax)
{
    int BinNr;
    double step = (Xmax - Xmin) / Nbins, *aDataX = new double[Nbins];
    unsigned long *aDataZ = new unsigned long[Nbins];

    sort(DataX.begin(), DataX.end());
    BinNr = 1; // because of the sort it should only grow
    for (int i = 0; i < Nbins; i++) {
        aDataX[i] = (0.5 + i) * step + Xmin;
        aDataZ[i] = 0;
    }
    for (unsigned long i = 0; i < DataX.size(); i++) {
        while (DataX[i] < Xmin && i < (DataX.size() - 1))
            i++;
        while (DataX[i] > Xmax && i < (DataX.size() - 1))
            i++;
        while (DataX[i] >= (BinNr * step + Xmin) && DataX[i] < Xmax)
            BinNr++;

        if (BinNr <= Nbins) {
            aDataZ[BinNr - 1]++;
        }
    }

    for (int i = 0; i < Nbins; i++) {
        cout << aDataX[i] << " " << aDataZ[i] << endl;
    }
    delete aDataX;
    delete aDataZ;
}

void DumpHistD(vector<double> DataX, vector<double> DataZ, int Nbins,
               double Xmin, double Xmax)
{
    if (DataX.size() != DataZ.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    int BinNr;
    double step = (Xmax - Xmin) / Nbins, *aDataX = new double[Nbins],
           *aDataZ = new double[Nbins];

    for (int i = 0; i < Nbins; i++) {
        aDataX[i] = (0.5 + i) * step + Xmin;
        aDataZ[i] = 0;
    }
    for (unsigned long i = 0; i < DataX.size(); i++) {
        BinNr = 1;
        while (DataX[i] < Xmin && i < (DataX.size() - 1))
            i++;
        while (DataX[i] > Xmax && i < (DataX.size() - 1))
            i++;
        while (DataX[i] >= (BinNr * step + Xmin) && DataX[i] < Xmax)
            BinNr++;

        if (BinNr <= Nbins) {
            aDataZ[BinNr - 1] += DataZ[i];
        }
    }

    for (int i = 0; i < Nbins; i++) {
        cout << aDataX[i] << " " << aDataZ[i] << endl;
    }
    delete aDataX;
    delete aDataZ;
}

//////////////////// For 2D histograms ////////////////////
TH2D *FillingHist2D(TH2D *hist, vector<double> internalVctx,
                    vector<double> internalVcty)
{
    if (internalVctx.size() != internalVcty.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    for (unsigned long i = 0; i < internalVctx.size(); i++) {
        hist->Fill(internalVctx[i], internalVcty[i]);
    }
    return hist;
}

TH2D *FillingHist2D(TH2D *hist, vector<double> internalVctx,
                    vector<double> internalVcty,
                    vector<double> internalVctWeight)
{
    if (internalVctx.size() != internalVcty.size() ||
        internalVctx.size() != internalVctWeight.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    for (unsigned long i = 0; i < internalVctx.size(); i++) {
        hist->Fill(internalVctx[i], internalVcty[i], internalVctWeight[i]);
    }
    return hist;
}

TH2D *FillingHist2D(TH2D *hist, vector<double> DataX, vector<double> DataY,
                    vector<double> DataZ, int NbinsX, double Xmin, double Xmax,
                    int NbinsY, double Ymin, double Ymax)
{
    if (DataX.size() != DataY.size() || DataX.size() != DataZ.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    int BinNrX, BinNrY;
    double Present_value, stepX = (Xmax - Xmin) / (NbinsX),
                          stepY = (Ymax - Ymin) / (NbinsY);
    for (unsigned long i = 0; i < DataX.size(); i++) {
        BinNrX = 1;
        BinNrY = 1;
        while (DataX[i] < Xmin && i < (DataX.size() - 1))
            i++;
        while (DataY[i] < Ymin && i < (DataX.size() - 1))
            i++;
        while (DataX[i] > Xmax && i < (DataX.size() - 1))
            i++;
        while (DataY[i] > Ymax && i < (DataX.size() - 1))
            i++;
        while (DataX[i] >= (BinNrX * stepX + Xmin) && DataX[i] < Xmax)
            BinNrX++;
        while (DataY[i] >= (BinNrY * stepY + Ymin) && DataY[i] < Ymax)
            BinNrY++;

        if (BinNrX <= NbinsX && BinNrY <= NbinsY) {
            Present_value = hist->GetBinContent(BinNrX, BinNrY);
            hist->SetBinContent(BinNrX, BinNrY, (Present_value + DataZ[i]));
        }
    }
    return hist;
}

TH2D *FillingHist2DWOff(TH2D *hist, vector<double> DataX, double OffsetX,
                        vector<double> DataY, double OffsetY,
                        vector<double> DataZ, double OffsetZ, int NbinsX,
                        double Xmin, double Xmax, int NbinsY, double Ymin,
                        double Ymax)
{
    if (DataX.size() != DataY.size() || DataX.size() != DataZ.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    int BinNrX, BinNrY;
    double Present_value, stepX = (Xmax - Xmin) / (NbinsX),
                          stepY = (Ymax - Ymin) / (NbinsY);
    for (unsigned long i = 0; i < DataX.size(); i++) {
        BinNrX = 1;
        BinNrY = 1;
        while (DataX[i] < Xmin && i < (DataX.size() - 1))
            i++;
        while (DataY[i] < Ymin && i < (DataX.size() - 1))
            i++;
        while (DataX[i] > Xmax && i < (DataX.size() - 1))
            i++;
        while (DataY[i] > Ymax && i < (DataX.size() - 1))
            i++;
        while (DataX[i] >= (BinNrX * stepX + Xmin) && DataX[i] < Xmax)
            BinNrX++;
        while (DataY[i] >= (BinNrY * stepY + Ymin) && DataY[i] < Ymax)
            BinNrY++;

        if (BinNrX <= NbinsX && BinNrY <= NbinsY) {
            Present_value =
                hist->GetBinContent((BinNrX + OffsetX), (BinNrY + OffsetY));
            hist->SetBinContent((BinNrX + OffsetX), (BinNrY + OffsetY),
                                (Present_value + DataZ[i] + OffsetZ));
        }
    }
    return hist;
}

TH2D *FillingHist2DWOff(TH2D *hist, vector<double> DataX, double OffsetX,
                        vector<double> DataY, double OffsetY,
                        vector<double> DataZ, double OffsetZ,
                        vector<double> DataWeight, int NbinsX, double Xmin,
                        double Xmax, int NbinsY, double Ymin, double Ymax)
{
    if (DataX.size() != DataY.size() || DataX.size() != DataZ.size() ||
        DataX.size() != DataWeight.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    int BinNrX, BinNrY;
    double Present_value, stepX = (Xmax - Xmin) / (NbinsX),
                          stepY = (Ymax - Ymin) / (NbinsY);
    for (unsigned long i = 0; i < DataX.size(); i++) {
        BinNrX = 1;
        BinNrY = 1;
        while (DataX[i] < Xmin && i < (DataX.size() - 1))
            i++;
        while (DataY[i] < Ymin && i < (DataX.size() - 1))
            i++;
        while (DataX[i] > Xmax && i < (DataX.size() - 1))
            i++;
        while (DataY[i] > Ymax && i < (DataX.size() - 1))
            i++;
        while (DataX[i] >= (BinNrX * stepX + Xmin) && DataX[i] < Xmax)
            BinNrX++;
        while (DataY[i] >= (BinNrY * stepY + Ymin) && DataY[i] < Ymax)
            BinNrY++;

        if (BinNrX <= NbinsX && BinNrY <= NbinsY) {
            Present_value =
                hist->GetBinContent((BinNrX + OffsetX), (BinNrY + OffsetY));
            hist->SetBinContent(
                (BinNrX + OffsetX), (BinNrY + OffsetY),
                (Present_value + DataZ[i] * DataWeight[i] + OffsetZ));
        }
    }
    return hist;
}

// Same as previous but specialized to SPEC option with edge effects
TH2D *FillingHist2DSPEC(TH2D *hist, vector<double> DataX, vector<double> DataY,
                        vector<double> DataZ, int NbinsX, double Xmin,
                        double Xmax, int NbinsY, double Ymin, double Ymax)
{
    if (DataX.size() != DataY.size() || DataX.size() != DataZ.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    int BinNrX, BinNrY;
    double Present_value,
        stepX = (Xmax - Xmin) / (NbinsX - 1),
        stepY = (Ymax - Ymin) / (NbinsY - 1); // due to edge in SPEC
    for (unsigned long i = 0; i < DataX.size(); i++) {
        BinNrX = 1;
        BinNrY = 1;
        while (DataX[i] < Xmin && i < (DataX.size() - 1))
            i++;
        while (DataY[i] < Ymin && i < (DataX.size() - 1))
            i++;
        while (DataX[i] > Xmax && i < (DataX.size() - 1))
            i++;
        while (DataY[i] > Ymax && i < (DataX.size() - 1))
            i++;
        while (DataX[i] >= (BinNrX * stepX + Xmin) && DataX[i] < (Xmax + stepX))
            BinNrX++;
        while (DataY[i] >= (BinNrY * stepY + Ymin) && DataY[i] < (Ymax + stepY))
            BinNrY++;

        if (BinNrX <= NbinsX && BinNrY <= NbinsY) {
            Present_value = hist->GetBinContent(BinNrX, BinNrY);
            hist->SetBinContent(BinNrX, BinNrY, (Present_value + DataZ[i]));
        }
    }
    return hist;
}

TH2D *FillingHist2DSPECWOff(TH2D *hist, vector<double> DataX, double OffsetX,
                            vector<double> DataY, double OffsetY,
                            vector<double> DataZ, double OffsetZ, int NbinsX,
                            double Xmin, double Xmax, int NbinsY, double Ymin,
                            double Ymax)
{
    if (DataX.size() != DataY.size() || DataX.size() != DataZ.size()) {
        cout << "\nError in Data. Arrays mismatch!\n";
        exit(1);
    }
    int BinNrX, BinNrY;
    double Present_value, stepX = (Xmax - Xmin) / (NbinsX - 1),
                          stepY = (Ymax - Ymin) / (NbinsY - 1); // due to edge
    for (unsigned long i = 0; i < DataX.size(); i++) {
        BinNrX = 1;
        BinNrY = 1;
        while (DataX[i] < Xmin && i < (DataX.size() - 1))
            i++;
        while (DataY[i] < Ymin && i < (DataX.size() - 1))
            i++;
        while (DataX[i] > Xmax && i < (DataX.size() - 1))
            i++;
        while (DataY[i] > Ymax && i < (DataX.size() - 1))
            i++;
        while (DataX[i] >= (BinNrX * stepX + Xmin) && DataX[i] < (Xmax + stepX))
            BinNrX++;
        while (DataY[i] >= (BinNrY * stepY + Ymin) && DataY[i] < (Ymax + stepY))
            BinNrY++;

        if (BinNrX <= NbinsX && BinNrY <= NbinsY) {
            Present_value =
                hist->GetBinContent((BinNrX + OffsetX), (BinNrY + OffsetY));
            hist->SetBinContent((BinNrX + OffsetX), (BinNrY + OffsetY),
                                (Present_value + DataZ[i] + OffsetZ));
        }
    }
    return hist;
}
//////////////////// End of 2D histogram section ////////////////////

void ExtractHistDataXZInRange(TH1D *histogram, vector<double> &DataX,
                              vector<double> &DataZ)
{
    TAxis *AxisX;
    AxisX = histogram->GetXaxis();

    int NoOfBins = AxisX->GetNbins();
    double Range[2], BinValue;

    Range[0] = AxisX->GetXmin();
    Range[1] = AxisX->GetXmax();

    for (int i = 1; i <= NoOfBins; i++) {
        BinValue = Range[0] + (i - 0.5) * (Range[1] - Range[0]) / NoOfBins;
        DataX.push_back(BinValue);
        DataZ.push_back(histogram->GetBinContent(i));
    }
}

void ExtractHistDataXZErrInRange(TH1D *histogram, vector<double> &DataX,
                                 vector<double> &DataZ, vector<double> &DataE)
{
    TAxis *AxisX;
    AxisX = histogram->GetXaxis();

    int NoOfBins = AxisX->GetNbins();
    double Range[2], BinValue;

    Range[0] = AxisX->GetXmin();
    Range[1] = AxisX->GetXmax();

    for (int i = 1; i <= NoOfBins; i++) {
        BinValue = Range[0] + (i - 0.5) * (Range[1] - Range[0]) / NoOfBins;
        DataX.push_back(BinValue);
        DataZ.push_back(histogram->GetBinContent(i));
        DataE.push_back(histogram->GetBinError(i));
    }
}

void ExtractHistDataXYZInRange(TH2D *histogram, vector<double> &DataX,
                               vector<double> &DataY, vector<double> &DataZ)
{
    TAxis *AxisX, *AxisY;
    AxisX = histogram->GetXaxis();
    AxisY = histogram->GetYaxis();

    int NoOfBinsX = histogram->GetNbinsX();
    int NoOfBinsY = histogram->GetNbinsY();

    double Range[4], BinValueX, BinValueY;

    Range[0] = AxisX->GetXmin();
    Range[1] = AxisX->GetXmax();
    Range[2] = AxisY->GetXmin(); // silly ROOT notation
    Range[3] = AxisY->GetXmax();

    for (int i1 = 1; i1 <= NoOfBinsX; i1++) {
        BinValueX = Range[0] + (i1 - 0.5) * (Range[1] - Range[0]) / NoOfBinsX;
        for (int i2 = 1; i2 <= NoOfBinsY; i2++) {
            BinValueY =
                Range[2] + (i2 - 0.5) * (Range[3] - Range[2]) / NoOfBinsY;
            DataX.push_back(BinValueX);
            DataY.push_back(BinValueY);
            DataZ.push_back(histogram->GetBinContent(i1, i2));
        }
    }
}

void ExtractHistDataXYZErrInRange(TH2D *histogram, vector<double> &DataX,
                                  vector<double> &DataY, vector<double> &DataZ,
                                  vector<double> &DataE)
{
    TAxis *AxisX, *AxisY;
    AxisX = histogram->GetXaxis();
    AxisY = histogram->GetYaxis();

    int NoOfBinsX = histogram->GetNbinsX();
    int NoOfBinsY = histogram->GetNbinsY();

    double Range[4], BinValueX, BinValueY;

    Range[0] = AxisX->GetXmin();
    Range[1] = AxisX->GetXmax();
    Range[2] = AxisY->GetXmin(); // silly ROOT notation
    Range[3] = AxisY->GetXmax();

    for (int i1 = 1; i1 <= NoOfBinsX; i1++) {
        BinValueX = Range[0] + (i1 - 0.5) * (Range[1] - Range[0]) / NoOfBinsX;
        for (int i2 = 1; i2 <= NoOfBinsY; i2++) {
            BinValueY =
                Range[2] + (i2 - 0.5) * (Range[3] - Range[2]) / NoOfBinsY;
            DataX.push_back(BinValueX);
            DataY.push_back(BinValueY);
            DataZ.push_back(histogram->GetBinContent(i1, i2));
            DataE.push_back(histogram->GetBinError(i1, i2));
        }
    }
}

#endif
