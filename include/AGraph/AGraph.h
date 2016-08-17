#ifndef AGraph__AGraph_h
#define AGraph__AGraph_h

/*

string Names[12] contains names for the plots. Internal values will be
constantly interpreted as
0: Plot name,
1: File name for printing,
2: X axis name,
3: Y axis name,
4: Z axis name,
5: Variable name
6: Graph name prefix to file
7: Output format
8: Graph type
9: Style
10: Data name, usually should be similar to Plot name or to Variable name
11: Legend line options /// to be removed ///

string Options_string[4]
0: Lengend line options /// to be placed here ///
1: Smoothing Algorithm for 2D histograms
2: Manual/custom binning for X
3: Manual/custom binning for Y

unsigned int Options[3] contains options for the plots
0: Legend, 0 - Off, 1 - ON
1: Number of columns if reading a data file
2: Smooth Number of times

int ExtraOptions[3]
0: Marker style
1: Line style
2: Fill style

double ExtraOptions_double[3]
0: Marker size
1: Floor (background) value
2: Line width

bool SpecOptions[17] contains bool options for the plots
0: Normalization of Z,
1: Autoscale of Z minimum value
2: Autoscale of Z maximum value
3: Autoscale of X minimum value - dummy variable
4: Autoscale of X maximum value - dummy variable
5: Autoscale of Y minimum value - dummy variable
6: Autoscale of Y maximum value - dummy variable
7: Logscale for X
8: Logscale for Y
9: Logscale for Z
10: Pad grid for X
11: Pad grid for Y
12: Pad grid for Z. Not  yet functional as TSyle doesn't have the right member
13: Use weighting
14: Use smoothing
15: Add a unitary floor
16: Use manual/custom binning

double StyleOptions[7] contains settings to adjust customizable styles
0: Resolution X
1: Resolution Y
2: Top margin
3: Bottom margin
4: Left margin
5: Right margin
6: Palette to use (for example, in 2D hists)


In AGraphM
AGraphM[-1] accesses the core settings used to plot that are firt built on the
AGraphM[0].


TODO:
Requires to be done: automatic Legend. The worked was ceased at void
DrawSimple(), which is a preversion of Draw(). Draw() is supposed to
automatically choose data container and plot.
Auto plot naming with respect to file name and directory is also required to be
done. Most likely offering reading version for future expansion. Reading naming
algorithm (versions) hard programmed? Or maybe put to the config file?

Universal legend maker should have interpreters.

Legend function input values: 2 vectors of data X, Z (Y in future), limits,
{correction factor read from config}, entries, font {from config?}

*/

#include <istream>
#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"

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

using namespace std;

const double Legend_Area = 0.4; // Max possible area 1. Selected number is the
                                // upper limit. However, it is forbidden to
                                // shrink more than twice.
const double Legend_Ratio = 4 / 2;  // Preferred sides ratio for the legend.
                                    //Will take into consideration.

// Due to bugs in Root SPEC mode, an additional offset in manual binning
const double Offset2DHistX = 0;
// Due to bugs in Root SPEC mode, an additional offset in manual binning
const double Offset2DHistY = 0;
// Due to bugs in Root SPEC mode, an additional offset in manual binning
const double Offset2DHistZ = 0;

class AGraph
{
  public:
    YAML::Node config;
    
    /// Variables
    string Graph_Type; // Expected types for histograms (1D): i, f, or d. E -
                       // empty
    // Expected types for histograms (2D): I, F, or D. Containers in capital
    // letters
    // Expected types for plots 2D: p.
    // Expected types for plots 3D: P.
    string *Graph_TypeM; // For multiplotting.
    bool Debug;          /// Debug flag, has to be implemented
    unsigned int Nr_of_graphs;

    vector<int> ContainerI;
    vector<float> ContainerF;
    vector<double> *ContainerD, *ContainerDZ, *ContainerDY, *ContainerDw;

    string RootDrawOptions;

    /// Functions
    int LoadDataFromFile(const string &);
    int Draw();
    int Print_Graph();
    int Print_Graph_wErrors();
    int Read_config();
    int Read_config(const string &);

    void Change_variable(const string &, const string &); // Temporary or not??
    void DumpContainers();                /// Not well maintained and updated
    // void LoadDataSimple(vector<int>); //UC
    void LoadDataSimple(const vector<double> &); // UC ??
    void LoadDataSimple(const vector<double> &, const vector<double> &);
    void LoadDataSimple(const vector<double> &, const vector<double> &,
                        const vector<double> &);
    void SpitRootContainer(const string &);
    void SpitRootContainer(const string &, const string &);

    /// Constructors, destructors
    AGraph();
    AGraph(const string &);
    ~AGraph();

  private:
    /// Variables
    bool Initialized, Print_Instead_Of_Draw, Print_wErrors;

    double *Custom_BinsX, *Custom_BinsY, buffer_double;
    vector<double> buffer_vector;

    TH1D *AGraphHistogram_d, **AGraphHistogram_dM;
    TH2D *AGraphHistogram_D, **AGraphHistogram_DM;
    TGraph *AGraphPlot, **AGraphPlotM; // Towards AGraph unification. v. beta
                                       // 1.2.x
    TAxis *AGraphAxisX, *AGraphAxisY, *AGraphAxisZ;
    TCanvas *AGraphCanvas;
    TLegend *AGraphLegend;
    TStyle *AGraphStyle; // Using for setting the style inside the plotting
                         // modules

    /// Functions
    int InitializeAGraph();
    int Draw(AGraph **, unsigned int);

    void InitContructor(const string &);
    void DeInitializeAGraph();
    void AutoLegendCoordinates();
    // Char - priority: O - Override, S - set if blank
    void Change_variable(const string &, const char); 
    void Change_variable(const string &, const double); // Priority 'O'
    
    inline bool Change_variable_generic(const char, const string &,
                                        const string &);
    inline bool Change_variable_output(const char, const string &,
                                       const string &);
    inline bool Change_variable_axes(const char, const string &,
                                     const string &);
    inline bool Change_variable_bins(const char, const string &,
                                     const string &);
    inline bool Change_variable_range(const char, const string &,
                                      const string &);
    inline bool Change_variable_auto(const char, const string &,
                                     const string &);
    inline bool Change_variable_smooth(const char, const string &,
                                       const string &);
    inline bool Change_variable_floor(const char, const string &,
                                      const string &);
    inline bool Change_variable_other(const char, const string &,
                                      const string &);
    inline bool Change_variable_draw_options(const char, const string &,
                                             const string &);
    inline bool Change_variable_curves_and_fill(const char, const string &,
                                                const string &);
    inline bool Change_variable_formatting(const char, const string &,
                                           const string &);
    inline bool Change_variable_legend(const char, const string &,
                                       const string &);
    // void Change_variable(string, string); //Priority 'O'
    void DrawAxes();
    void DrawLegend();
    void SetStyle(string);      // So far {"","TDR","Matt"}
    void Minimum(char, double); // char= X, Y, Z which corresponds to axis

    /// Extra
    friend class AGraphM;
};

#endif
