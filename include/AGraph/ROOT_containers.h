#ifndef AGraph__ROOT_containers_h
#define AGraph__ROOT_containers_h

/*
 * ROOT includes
 *
 */
// #include <TROOT.h>
// #include <TFile.h>
// #include <TTree.h>
#include <TCanvas.h>
// #include <TH1I.h>
#include "TF1.h"
#include <TH1F.h>
#include <TH2F.h>
// #include <TString.h>
//
// #include "TLine.h"
// #include "TBox.h"
#include "TGraph.h"
// #include "TGraphErrors.h"
#include "TStyle.h"
// #include "TPad.h"
// #include "TLatex.h"
#include "TLegend.h"
// #include "TLegendEntry.h"
// #include "THStack.h"

struct ROOT_containers {
    // ROOT graph containers
    TH1D *hist_1D, **hist_1DM;
    TH2D *hist_2D, **hist_2DM;
    TGraph *graph, **graphM; // Towards AGraph unification. v. beta 1.2.x

    // ROOT "beauty"
    TAxis *axis_X, *axis_Y, *axis_Z;
    TCanvas *canvas;
    TLegend *legend;
    // Used for setting the style inside the plotting modules
    TStyle *style;
};

#endif
