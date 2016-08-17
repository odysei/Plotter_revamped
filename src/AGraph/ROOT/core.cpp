#ifndef AGraph__core_cpp
#define AGraph__core_cpp

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

#include "AGraph/AGraph.h"
#include <istream>
#include <string>
#include <vector>
#include "helpers/fillers_dumpers.h"
#include "helpers/type_conversion.h"

/*
 * ROOT includes
 *
 */
#include <TFile.h>

using namespace std;

AGraph::AGraph() { InitContructor(""); }

AGraph::AGraph(const string &filename)
{
    InitContructor(filename);
    const auto &df = config["Data_file"].as<string>();
    if (df.size() > 0)
        LoadDataFromFile(df);
}

void AGraph::InitContructor(const string &filename)
{
    // Declaring "Empty" values: rescues from possible crashes and provides
    // comparable nonset values
    Debug = false;

    //     BinsX = 1;
    //     BinsY = 1;
    //     RangeX[0] = 0;
    //     RangeY[0] = 0;
    //     RangeZ[0] = 0;
    //     RangeX[1] = 10;
    //     RangeY[1] = 10;
    //     RangeZ[1] = 10;
    //     Angles[0] = "0";
    //     Angles[1] = "0";
    //     Angles[2] = "0";
    //     AxesTitlesOffsets[0] = 1;
    //     AxesTitlesOffsets[1] = 1;
    //     AxesTitlesOffsets[2] = 1;
    //     LegendCoordinates[0] = 0.55;
    //     LegendCoordinates[1] = 0.6;
    //     LegendCoordinates[2] = 0.95;
    //     LegendCoordinates[3] = 0.75;
    //
    //     Names[0] = "Default plot";
    //     Names[1] = "Default file";
    //     Names[2] = "X axis";
    //     Names[3] = "Y axis";
    //     Names[4] = "Z axis";
    //     Names[5] = "Default variable";
    //     Names[6] = "Histogram";
    //
    //     Names[8] = "Histogram";
    //     Names[10] = "Data";
    //     Names[11] = "LP";
    //
    //     Options_string[1] = "k5a";
    //
    //     RootDrawOptionsMH2D = "lego2";
    //
    //     SpitRoot = "";
    //
    //     Options[0] = 1;
    //     Options[1] = 2;
    //     Options[2] = 1;
    //
    //     Data_Rescaling_Factor = 1;
    //
    //     Color = 1;
    //     Fill_Color = 1;
    //     ExtraOptions[0] = -1;
    //     ExtraOptions[1] = -1;
    //     ExtraOptions[2] = -1;
    //
    //     ExtraOptions_double[0] = -1;
    //     ExtraOptions_double[1] = 1;
    //     ExtraOptions_double[2] = -1;
    //
    //     for (unsigned int counter = 0; counter < 17; counter++)
    //         SpecOptions[counter] = false;
    //
    //     StyleOptions[0] = 600;
    //     StyleOptions[1] = 600;
    //     StyleOptions[2] = 0.05;
    //     StyleOptions[3] = 0.13;
    //     StyleOptions[4] = 0.16;
    //     StyleOptions[5] = 0.02;
    //     StyleOptions[6] = 1;

    ContainerD = nullptr;
    ContainerDZ = nullptr;
    ContainerDY = nullptr;
    ContainerDw = nullptr;

    ROOTC.hist_1D = nullptr;
    ROOTC.hist_2D = nullptr;
    ROOTC.graph = nullptr;
    ROOTC.axis_X = nullptr;
    ROOTC.axis_Y = nullptr;
    ROOTC.axis_Z = nullptr;
    ROOTC.canvas = nullptr;
    ROOTC.legend = nullptr;
    ROOTC.style = nullptr;
    Initialized = false;
    Print_Instead_Of_Draw = false;
    Print_wErrors = false;

    ROOTC.hist_1DM = nullptr;
    ROOTC.hist_2DM = nullptr;
    ROOTC.graphM = nullptr;

    Graph_Type = "E";
    if (Read_config(filename) != 0)
        return;

    if (config["Generic"]["plot_type"].as<string>() == "Histogram")
        Graph_Type = "d";
    if (config["Generic"]["plot_type"].as<string>() == "Histogram2D")
        Graph_Type = "D";
    if (config["Generic"]["plot_type"].as<string>() == "Plot")
        Graph_Type = "p";
    if (config["Generic"]["plot_type"].as<string>() == "Plot2D")
        Graph_Type = "P";
    if (config["Generic"]["use_weights"].as<bool>())
        Graph_Type += "w";
}

AGraph::~AGraph()
{
    // 	delete ContainerD;	//crashes for some reason
    // 	delete ContainerDZ;	//crashes for some reason
    // 	delete ContainerDY;	//crashes for some reason

    // deinitialization takes care of these:
    // 	delete ROOTC.hist_1D;
    // 	delete ROOTC.hist_2D;
    // 	delete ROOTC.graph;
    // delete ROOTC.axis_X; //This should not be deleted as it will cause crash
    // in most cases
    // delete ROOTC.axis_Y; //This should not be deleted as it will cause crash
    // in most cases
    // delete ROOTC.axis_Z; //This should not be deleted as it will cause crash
    // in most cases
    // 	delete ROOTC.canvas;
    // 	delete ROOTC.legend;
    // 	delete ROOTC.style;
}

void AGraph::DumpContainers()
{
    const auto n_x = config["Bins"]["n_x"].as<unsigned int>();
    const auto x_min = config["Range"]["x_min"].as<double>();
    const auto x_max = config["Range"]["x_min"].as<double>();
    if (ContainerD != nullptr && ContainerDZ == nullptr &&
        ContainerDY == nullptr)
        DumpHistD(*ContainerD, n_x, x_min, x_max);
    if (ContainerD != nullptr && ContainerDZ != nullptr &&
        ContainerDY == nullptr)
        DumpHistD(*ContainerD, *ContainerDZ, n_x, x_min, x_max);
    if (ContainerD != nullptr && ContainerDZ != nullptr &&
        ContainerDY != nullptr)
        cout << "Dumping not implemented yet.\n";
}

int AGraph::Print_Graph()
{
    Print_Instead_Of_Draw = true;
    int error = Draw();
    Print_Instead_Of_Draw = false;

    return error;
}

int AGraph::Print_Graph_wErrors()
{
    Print_wErrors = true;
    int error = Print_Graph();
    Print_wErrors = false;

    return error;
}

void AGraph::SpitRootContainer(const string &File, const string &Type) // public
{
    bool DeInitialize = true; // In order to incorporate into AGraph.Draw()
    if (Initialized == true)
        DeInitialize = false;
    else
        InitializeAGraph();

    if (Graph_Type[0] == 'd')
        ROOTC.hist_1D->Draw(RootDrawOptions.c_str());
    if (Graph_Type[0] == 'D')
        ROOTC.hist_2D->Draw(RootDrawOptions.c_str());
    if (Graph_Type[0] == 'p')
        ROOTC.graph->Draw(RootDrawOptions.c_str());

    // as it may be noninitialized if wrong type is set
    if (ROOTC.canvas != nullptr) {
        if (config["Legend"]["add"].as<bool>())
            DrawLegend(); // Legends
        DrawAxes();       // Setting up axes

        TFile *f = new TFile(str2char(File), "RECREATE");
        if (Type == "Canvas")
            ROOTC.canvas->Write();
        if (Type == "Histogram") {
            if (Graph_Type[0] == 'd')
                ROOTC.hist_1D->Write();
            if (Graph_Type[0] == 'D')
                ROOTC.hist_2D->Write();
            if (Graph_Type[0] == 'p')
                ROOTC.graph->Write();
        }

        f->Close("R");
        delete f;
    }

    if (DeInitialize == true)
        DeInitializeAGraph();
}

void AGraph::SpitRootContainer(const string &type)
{
    const string name = config["Output"]["filename_prefix"].as<string>() +
                        config["Output"]["filename"].as<string>() + ".root";
    SpitRootContainer(name, type);
}

#endif
