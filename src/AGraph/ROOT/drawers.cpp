#ifndef AGraph__drawers_cpp
#define AGraph__drawers_cpp

#include "AGraph/AGraph.h"
#include <sstream>
#include <algorithm>
#include "helpers/type_conversion.h"
#include "helpers/fillers_dumpers.h"
#include "helpers/array_manipulations.h"
#include "helpers/ROOT_IO.h" // SaveC
#include "ROOT_styles/styles.h"

int AGraph::InitializeAGraph()
{
    const auto n_x = config["Bins"]["n_x"].as<unsigned int>();
    const auto n_y = config["Bins"]["n_y"].as<unsigned int>();

    const auto x_min = config["Range"]["x_min"].as<double>();
    const auto x_max = config["Range"]["x_max"].as<double>();
    const auto y_min = config["Range"]["y_min"].as<double>();
    const auto y_max = config["Range"]["y_max"].as<double>();
    const auto z_min = config["Range"]["z_min"].as<double>();
    const auto z_max = config["Range"]["z_max"].as<double>();
    if (config["Bins"]["custom_binning"].as<bool>()) {
        auto vx = config["Bins"]["custom_x"].as<vector<double>>();
        if ((n_x + 1) != vx.size())
            return 3;
        sort(vx.begin(), vx.end());
        Custom_BinsX = new double[n_x + 1];
        for (unsigned int i = 0; i < vx.size(); ++i)
            Custom_BinsX[i] = vx[i];

        auto vy = config["Bins"]["custom_y"].as<vector<double>>();
        if ((n_y + 1) != vy.size())
            return 3;
        sort(vy.begin(), vy.end());
        Custom_BinsY = new double[n_y + 1];
        for (unsigned int i = 0; i < vy.size(); ++i)
            Custom_BinsY[i] = vy[i];
    }
    // 	if( Initialized==false )
    // 	{
    if (ContainerD == nullptr && ContainerDY == nullptr &&
        ContainerDZ == nullptr)
        return 1;
    if (Graph_Type[0] == 'd') {
        if (ContainerD == nullptr)
            return 1;
        if (ContainerD->size() == 0)
            return 1;

        SetStyle(config["Formatting"]["style"].as<string>());

        if (config["Draw_options"]["histogram"].as<string>() == "")
            RootDrawOptions = "E1 HIST";
        else
            RootDrawOptions = config["Draw_options"]["histogram"].as<string>();

        ROOTC.hist_1D =
            new TH1D(config["Generic"]["data_name"].as<string>().c_str(), "",
                     n_x, x_min, x_max);
        ROOTC.hist_1D->SetLineColor(
            config["Curves_and_fill"]["line_marker_color"].as<int>());
        ROOTC.hist_1D->SetMarkerColor(
            config["Curves_and_fill"]["line_marker_color"].as<int>());
        if (config["Curves_and_fill"]["fill"]["color"].as<int>() != -1)
            ROOTC.hist_1D->SetFillColor(
                config["Curves_and_fill"]["fill"]["color"].as<int>());
        if (config["Curves_and_fill"]["marker"]["style"].as<int>() != -1)
            ROOTC.hist_1D->SetMarkerStyle(
                config["Curves_and_fill"]["marker"]["style"].as<int>());
        if (config["Curves_and_fill"]["line"]["style"].as<int>() != -1)
            ROOTC.hist_1D->SetLineStyle(
                config["Curves_and_fill"]["line"]["style"].as<int>());
        if (config["Curves_and_fill"]["fill"]["style"].as<int>() != -1)
            ROOTC.hist_1D->SetFillStyle(
                config["Curves_and_fill"]["fill"]["style"].as<int>());
        if (config["Curves_and_fill"]["marker"]["size"].as<double>() != -1)
            ROOTC.hist_1D->SetMarkerSize(
                config["Curves_and_fill"]["marker"]["size"].as<double>());
        if (config["Curves_and_fill"]["line"]["width"].as<double>() != -1)
            ROOTC.hist_1D->SetLineWidth(
                config["Curves_and_fill"]["line"]["width"].as<double>());

        ////For errors
        ROOTC.hist_1D->Sumw2();
        // 			hist_1D->SetName( "hist" );

        ////Follows the filling procedure for the histograms
        if (ContainerDZ == nullptr) {
            if (ContainerDw == nullptr)
                ROOTC.hist_1D = FillingHistD(
                    ROOTC.hist_1D,
                    *ContainerD); // UC for LoadDataSimple(vector<double>)
            else
                ROOTC.hist_1D =
                    FillingHistD(ROOTC.hist_1D, *ContainerD, *ContainerDw);
        } else {
            if (ContainerDw == nullptr)
                ROOTC.hist_1D = FillingHistD(ROOTC.hist_1D, *ContainerD,
                                             *ContainerDZ, n_x, x_min, x_max);
            else
                ROOTC.hist_1D =
                    FillingHistD(ROOTC.hist_1D, *ContainerD, *ContainerDZ,
                                 *ContainerDw, n_x, x_min, x_max);
        }

        if (config["Smooth"]["use"].as<bool>())
            ROOTC.hist_1D->Smooth(config["Smooth"]["times"].as<int>());

        if (config["Floor"]["add"].as<bool>()) {
            auto &h = ROOTC.hist_1D;
            const auto floor = config["Floor"]["value"].as<double>();
            for (long i1 = 1; i1 <= n_x; ++i1) {
                const long bin = h->GetBin(i1);
                const double value = h->GetBinContent(bin);
                if (value == 0) {
                    h->SetBinContent(bin, floor);
                    h->SetBinError(bin, floor);
                } else
                    h->SetBinContent(bin, (floor + value));
            }
        }

        ////Normalization and rescaling
        double scale1 = config["Auto"]["rescale"].as<double>();
        if (config["Auto"]["normalize"].as<bool>()) {
            scale1 *= 1 / ROOTC.hist_1D->Integral();
            ROOTC.hist_1D->Scale(scale1);
        } else {
            if (scale1 != 1)
                ROOTC.hist_1D->Scale(scale1);
        }

        ////Setting Z range with possible autoscaling, choosing vector container
        /// for Z instead of root container
        if (config["Auto"]["scale_z_min"].as<bool>()) {
            if (ContainerDZ != nullptr)
                if (ContainerDZ->size() != 0)
                    ROOTC.hist_1D->SetMinimum(MinimumValue(*ContainerDZ) *
                                              scale1);
        } else
            ROOTC.hist_1D->SetMinimum(z_min);

        if (config["Auto"]["scale_z_max"].as<bool>()) {
            if (ContainerDZ != nullptr)
                if (ContainerDZ->size() != 0)
                    ROOTC.hist_1D->SetMaximum(MaximumValue(*ContainerDZ) *
                                              scale1);
        } else
            ROOTC.hist_1D->SetMaximum(z_max);

        const auto &title = config["Generic"]["plot_name"].as<string>();
        ROOTC.hist_1D->SetTitle(title.c_str());

        if (Initialized == false) {
            //.75,.80,.95,.95
            if (config["Legend"]["add"].as<bool>())
                ROOTC.legend = new TLegend(
                    config["Legend"]["placement"]["BL_x"].as<double>(),
                    config["Legend"]["placement"]["BL_y"].as<double>(),
                    config["Legend"]["placement"]["TR_x"].as<double>(),
                    config["Legend"]["placement"]["TR_y"].as<double>());

            ////Initializing Canvas
            ROOTC.canvas = new TCanvas("canvas", "Histogram(s)");

            if (config["Axes"]["log_x"].as<bool>())
                ROOTC.canvas->SetLogx(1);
            else
                ROOTC.canvas->SetLogx(0);

            // note flipped z with y for consistency
            if (config["Axes"]["log_z"].as<bool>())
                ROOTC.canvas->SetLogy(1);
            else
                ROOTC.canvas->SetLogy(0);

            // ROOTC.canvas->SetFrameFillColor(kWhite);
            // ROOTC.canvas->SetFillColor(kWhite);
            // ROOTC.style->SetOptStat(kFALSE);
        }
        if (config["Legend"]["add"].as<bool>()) {
            const auto &var_name =
                config["Generic"]["variable_name"].as<string>();
            const auto &mark = config["Legend"]["marker"].as<string>();
            ROOTC.legend->AddEntry(ROOTC.hist_1D, var_name.c_str(),
                                   mark.c_str());
        }

        Initialized = true;
        return 0;
    }

    if (Graph_Type[0] == 'D') {
        if (ContainerD == nullptr || ContainerDY == nullptr)
            return 1;
        if (ContainerD->size() == 0 || ContainerDY->size() == 0)
            return 1;

        SetStyle(config["Formatting"]["style"].as<string>());

        if (config["Draw_options"]["histogram_2D"].as<string>() == "") {
            RootDrawOptions = "colorz"; // lego2z
            /* SPEC mode: handicaped but good looking
            RootDrawOptions ="SPEC";
            RootDrawOptions +=" dm(1,10)"; //Display Modes
            RootDrawOptions +=" pa(2,1,1)"; //Pen Attributes
            pa(color,style,width)
            RootDrawOptions +=" ci(1,4,8)"; //Colors increments "ci(r,g,b)
            RootDrawOptions +=" cm(1,15,4,4,1)"; //Marker on each node
            cm(enable,color,width,height,style)
            RootDrawOptions +=" cg(1,2) ca(0)"; //Channel grid cg(enable,color).
            Color algorithm ca(alg)
            RootDrawOptions += " a(" + config["Other"]["angleA_3D"].as<string>()
                                + ","
                                + config["Other"]["angleB_3D"].as<string>()
                                + ","
                                + config["Other"]["angleZ_3D"].as<string>()
                                + ")";
            //Angles 15,90,270
            RootDrawOptions +=" zs(0)"; //scale: zs(0) LIN, zs(1) LOG, zs(2)
            SQRT
            */
        } else {
            RootDrawOptions =
                config["Draw_options"]["histogram_2D"].as<string>();
        }
        ROOTC.style->SetPalette(1);

        const auto &dn = config["Generic"]["data_name"].as<string>();
        if (!config["Bins"]["custom_binning"].as<bool>())
            ROOTC.hist_2D =
                new TH2D(dn.c_str(), "", n_x, x_min, x_max, n_y, y_min, y_max);
        else
            ROOTC.hist_2D =
                new TH2D(dn.c_str(), "", n_x, Custom_BinsX, n_y, Custom_BinsY);
        ROOTC.hist_2D->SetLineColor(
            config["Curves_and_fill"]["line_marker_color"].as<int>());
        ROOTC.hist_2D->SetMarkerColor(
            config["Curves_and_fill"]["line_marker_color"].as<int>());
        if (config["Curves_and_fill"]["marker"]["style"].as<int>() != -1)
            ROOTC.hist_2D->SetMarkerStyle(
                config["Curves_and_fill"]["marker"]["style"].as<int>());
        if (config["Curves_and_fill"]["line"]["style"].as<int>() != -1)
            ROOTC.hist_2D->SetLineStyle(
                config["Curves_and_fill"]["line"]["style"].as<int>());
        if (config["Curves_and_fill"]["marker"]["size"].as<double>() != -1)
            ROOTC.hist_2D->SetMarkerSize(
                config["Curves_and_fill"]["marker"]["size"].as<double>());
        if (config["Curves_and_fill"]["line"]["width"].as<double>() != -1)
            ROOTC.hist_2D->SetLineWidth(
                config["Curves_and_fill"]["line"]["width"].as<double>());

        ////For errors
        ROOTC.hist_2D->Sumw2();

        ////Follows the filling procedure for the histograms
        if (ContainerDZ == nullptr) {
            if (ContainerDw == nullptr)
                ROOTC.hist_2D =
                    FillingHist2D(ROOTC.hist_2D, *ContainerD, *ContainerDY);
            else
                ROOTC.hist_2D = FillingHist2D(ROOTC.hist_2D, *ContainerD,
                                              *ContainerDY, *ContainerDw);
        } else {
            if (ContainerDw == nullptr)
                ROOTC.hist_2D = FillingHist2DWOff(
                    ROOTC.hist_2D, *ContainerD, Offset2DHistX, *ContainerDY,
                    Offset2DHistY, *ContainerDZ, Offset2DHistZ, n_x, x_min,
                    x_max, n_y, y_min, y_max);
            else
                ROOTC.hist_2D = FillingHist2DWOff(
                    ROOTC.hist_2D, *ContainerD, Offset2DHistX, *ContainerDY,
                    Offset2DHistY, *ContainerDZ, Offset2DHistZ, *ContainerDw,
                    n_x, x_min, x_max, n_y, y_min, y_max);
        }

        if (config["Smooth"]["use"].as<bool>()) {
            for (auto i = 0; i < config["Smooth"]["times"].as<int>(); ++i)
                ROOTC.hist_2D->Smooth(
                    1, config["Smooth"]["algorithm_2D"].as<string>().c_str());
        }

        if (config["Floor"]["add"].as<bool>()) {
            auto &h = ROOTC.hist_2D;
            const auto floor = config["Floor"]["value"].as<double>();
            for (long i1 = 1; i1 <= n_x; ++i1) {
                for (long i2 = 1; i2 <= n_y; ++i2) {
                    const long bin = h->GetBin(i1, i2);
                    const double value = h->GetBinContent(bin);
                    if (value == 0) {
                        h->SetBinContent(bin, floor);
                        h->SetBinError(bin, floor);
                    } else
                        h->SetBinContent(bin, (floor + value));
                }
            }
        }

        ////Normalization and rescaling
        double scale1 = config["Auto"]["rescale"].as<double>();
        if (config["Auto"]["normalize"].as<bool>()) {
            scale1 *= 1 / (ROOTC.hist_2D->Integral());
            ROOTC.hist_2D->Scale(scale1);
        } else {
            if (scale1 != 1)
                ROOTC.hist_2D->Scale(scale1);
        }

        ////Setting Z range with possible autoscaling, choosing vector container
        /// for Z instead of root container
        if (config["Auto"]["scale_z_min"].as<bool>()) {
            if (ContainerDZ != nullptr)
                if (ContainerDZ->size() != 0)
                    ROOTC.hist_2D->SetMinimum(MinimumValue(*ContainerDZ) *
                                              scale1);
        } else
            ROOTC.hist_2D->SetMinimum(z_min);

        if (config["Auto"]["scale_z_max"].as<bool>()) {
            if (ContainerDZ != nullptr)
                if (ContainerDZ->size() != 0)
                    ROOTC.hist_2D->SetMaximum(MaximumValue(*ContainerDZ) *
                                              scale1);
        } else
            ROOTC.hist_2D->SetMaximum(z_max);

        ROOTC.hist_2D->SetTitle(
            str2char(config["Generic"]["plot_name"].as<string>()));

        if (Initialized == false) {
            //.75,.80,.95,.95
            if (config["Legend"]["add"].as<bool>())
                ROOTC.legend = new TLegend(
                    config["Legend"]["placement"]["BL_x"].as<double>(),
                    config["Legend"]["placement"]["BL_y"].as<double>(),
                    config["Legend"]["placement"]["TR_x"].as<double>(),
                    config["Legend"]["placement"]["TR_y"].as<double>());

            ////Initializing Canvas
            ROOTC.canvas = new TCanvas("canvas", "Histogram(s)");

            if (config["Axes"]["log_x"].as<bool>())
                ROOTC.canvas->SetLogx(1);
            else
                ROOTC.canvas->SetLogx(0);

            if (config["Axes"]["log_y"].as<bool>())
                ROOTC.canvas->SetLogy(1);
            else
                ROOTC.canvas->SetLogy(0);

            if (config["Axes"]["log_z"].as<bool>())
                ROOTC.canvas->SetLogz(1);
            else
                ROOTC.canvas->SetLogz(0);

            // ROOTC.canvas->SetFrameFillColor(kWhite);
            // ROOTC.canvas->SetFillColor(kWhite);
            // ROOTC.style->SetOptStat(kFALSE);
        }
        if (config["Legend"]["add"].as<bool>()) {
            const auto &var_name =
                config["Generic"]["variable_name"].as<string>();
            const auto &mark = config["Legend"]["marker"].as<string>();
            ROOTC.legend->AddEntry(ROOTC.hist_2D, var_name.c_str(),
                                   mark.c_str());
        }

        Initialized = true;
        return 0;
    }

    if (Graph_Type[0] == 'p') {
        if (ContainerD == nullptr)
            return 1;
        if (ContainerD->size() == 0)
            return 1;

        SetStyle(config["Formatting"]["style"].as<string>());

        if (config["Draw_options"]["graph"].as<string>() == "") {
            RootDrawOptions = "PL";
        } else {
            RootDrawOptions = config["Draw_options"]["graph"].as<string>();
        }

        ROOTC.graph = new TGraph(ContainerD->size(), vct2arrD(*ContainerD),
                                 vct2arrD(*ContainerDZ));
        ROOTC.graph->SetLineColor(
            config["Curves_and_fill"]["line_marker_color"].as<int>());
        ROOTC.graph->SetMarkerColor(
            config["Curves_and_fill"]["line_marker_color"].as<int>());
        if (config["Curves_and_fill"]["marker"]["style"].as<int>() != -1)
            ROOTC.graph->SetMarkerStyle(
                config["Curves_and_fill"]["marker"]["style"].as<int>());
        if (config["Curves_and_fill"]["line"]["style"].as<int>() != -1)
            ROOTC.graph->SetLineStyle(
                config["Curves_and_fill"]["line"]["style"].as<int>());
        if (config["Curves_and_fill"]["marker"]["size"].as<double>() != -1)
            ROOTC.graph->SetMarkerSize(
                config["Curves_and_fill"]["marker"]["size"].as<double>());
        if (config["Curves_and_fill"]["line"]["width"].as<double>() != -1)
            ROOTC.graph->SetLineWidth(
                config["Curves_and_fill"]["line"]["width"].as<double>());

        ////Setting Z range with possible autoscaling, choosing vector container
        /// for Z instead of root container
        if (config["Auto"]["scale_z_min"].as<bool>()) {
            if (ContainerDZ != nullptr)
                if (ContainerDZ->size() != 0)
                    ROOTC.graph->SetMinimum(MinimumValue(*ContainerDZ));
        } else
            ROOTC.graph->SetMinimum(z_min);

        if (config["Auto"]["scale_z_max"].as<bool>()) {
            if (ContainerDZ != nullptr)
                if (ContainerDZ->size() != 0)
                    ROOTC.graph->SetMaximum(MaximumValue(*ContainerDZ));
        } else
            ROOTC.graph->SetMaximum(z_max);

        ROOTC.graph->SetTitle(
            str2char(config["Generic"]["plot_name"].as<string>()));

        if (Initialized == false) {
            //.75,.80,.95,.95
            if (config["Legend"]["add"].as<bool>())
                ROOTC.legend = new TLegend(
                    config["Legend"]["placement"]["BL_x"].as<double>(),
                    config["Legend"]["placement"]["BL_y"].as<double>(),
                    config["Legend"]["placement"]["TR_x"].as<double>(),
                    config["Legend"]["placement"]["TR_y"].as<double>());

            ////Initializing Canvas
            ROOTC.canvas = new TCanvas("canvas", "Histogram(s)");

            if (config["Axes"]["log_x"].as<bool>())
                ROOTC.canvas->SetLogx(1);
            else
                ROOTC.canvas->SetLogx(0);

            // note flipped z with y for consistency
            if (config["Axes"]["log_z"].as<bool>())
                ROOTC.canvas->SetLogy(1);
            else
                ROOTC.canvas->SetLogy(0);

            // ROOTC.canvas->SetFrameFillColor(kWhite);
            // ROOTC.canvas->SetFillColor(kWhite);
            // style->SetOptStat(kFALSE);
        }
        if (config["Legend"]["add"].as<bool>()) {
            const auto &var_name =
                config["Generic"]["variable_name"].as<string>();
            const auto &mark = config["Legend"]["marker"].as<string>();
            ROOTC.legend->AddEntry(ROOTC.graph, var_name.c_str(), mark.c_str());
        }

        Initialized = true;
        return 0;
    }

    // 	}
    return 2;
}

void AGraph::DeInitializeAGraph()
{
    Initialized = false;

    for (unsigned int i = 0; i < Nr_of_graphs; ++i) {
        if (Graph_TypeM[i][0] == 'd' && ROOTC.hist_1DM[i] != nullptr)
            delete ROOTC.hist_1DM[i];
        if (Graph_TypeM[i][0] == 'D' && ROOTC.hist_2DM[i] != nullptr)
            delete ROOTC.hist_2DM[i];
        if (Graph_TypeM[i][0] == 'p' && ROOTC.graphM[i] != nullptr)
            delete ROOTC.graphM[i];
    }

    if (ROOTC.hist_1DM != nullptr)
        delete[] ROOTC.hist_1DM;
    if (ROOTC.hist_2DM != nullptr)
        delete[] ROOTC.hist_2DM;
    if (ROOTC.graphM != nullptr)
        delete[] ROOTC.graphM;

    Graph_Type = Graph_TypeM[0];
    delete[] Graph_TypeM;

    // deteleting *Ms takes care of these 3
    // 	delete ROOTC.hist_1D;
    // 	delete ROOTC.hist_2D;
    // 	delete ROOTC.graph;
    // delete ROOTC.axis_X; //This should not be deleted as it will cause crash
    // in most cases
    // delete ROOTC.axis_Y; //This should not be deleted as it will cause crash
    // in most cases
    // delete ROOTC.axis_Z; //This should not be deleted as it will cause crash
    // in most cases
    if (ROOTC.canvas != nullptr)
        delete ROOTC.canvas;
    if (ROOTC.legend != nullptr)
        delete ROOTC.legend;
    if (ROOTC.style != nullptr)
        delete ROOTC.style;

    ROOTC.hist_1D = nullptr;
    ROOTC.hist_1DM = nullptr;
    ROOTC.hist_2D = nullptr;
    ROOTC.hist_2DM = nullptr;
    ROOTC.graph = nullptr;
    ROOTC.graphM = nullptr;
    ROOTC.axis_X = nullptr;
    ROOTC.axis_Y = nullptr;
    ROOTC.axis_Z = nullptr;
    ROOTC.canvas = nullptr;
    ROOTC.legend = nullptr;
    ROOTC.style = nullptr;
}

int AGraph::Draw() { return Draw(nullptr, 0); }

int AGraph::Draw(AGraph **GraphContainer, unsigned int Nr_of_graphsI)
{
    Nr_of_graphs = 0;
    ++Nr_of_graphsI; // to make Nr_of_graphs=0 work with preset containers

    if (Nr_of_graphsI == 1) {
        Nr_of_graphs = 1;
        Graph_TypeM = new string[Nr_of_graphsI];
        Graph_TypeM[0] = Graph_Type;

        ROOTC.hist_1DM = new TH1D *[Nr_of_graphsI];
        ROOTC.hist_2DM = new TH2D *[Nr_of_graphsI];
        ROOTC.graphM = new TGraph *[Nr_of_graphsI];

        ROOTC.hist_1DM[0] = nullptr;
        ROOTC.hist_2DM[0] = nullptr;
        ROOTC.graphM[0] = nullptr;

    } else {
        Nr_of_graphs = Nr_of_graphsI - 1;
        Graph_TypeM = new string[Nr_of_graphs];

        ROOTC.hist_1DM = new TH1D *[Nr_of_graphs];
        ROOTC.hist_2DM = new TH2D *[Nr_of_graphs];
        ROOTC.graphM = new TGraph *[Nr_of_graphs];

        for (unsigned int i = 0; i < Nr_of_graphs; ++i) {
            ROOTC.hist_1DM[i] = nullptr;
            ROOTC.hist_2DM[i] = nullptr;
            ROOTC.graphM[i] = nullptr;
        }
    }

    for (unsigned int count = 0; count < Nr_of_graphsI; count++) {
        if (GraphContainer != nullptr && Nr_of_graphsI > 1) {
            ContainerD = (*GraphContainer[count]).ContainerD;
            ContainerDY = (*GraphContainer[count]).ContainerDY;
            ContainerDZ = (*GraphContainer[count]).ContainerDZ;
            ContainerDw = (*GraphContainer[count]).ContainerDw;

            config["Curves_and_fill"]["line_marker_color"] =
                (*GraphContainer[count])
                    .config["Curves_and_fill"]["line_marker_color"]
                    .as<int>();
            config["Curves_and_fill"]["marker"]["style"] =
                (*GraphContainer[count])
                    .config["Curves_and_fill"]["marker"]["style"]
                    .as<int>();
            config["Curves_and_fill"]["line"]["style"] =
                (*GraphContainer[count])
                    .config["Curves_and_fill"]["line"]["style"]
                    .as<int>();
            config["Curves_and_fill"]["marker"]["size"] =
                (*GraphContainer[count])
                    .config["Curves_and_fill"]["marker"]["size"]
                    .as<double>();
            config["Curves_and_fill"]["line"]["width"] =
                (*GraphContainer[count])
                    .config["Curves_and_fill"]["line"]["width"]
                    .as<double>();
            config["Generic"]["variable_name"] =
                (*GraphContainer[count]).config["Generic"]["variable_name"];
            config["Generic"]["data_name"] =
                (*GraphContainer[count]).config["Generic"]["data_name"];
            config["Legend"]["marker"] =
                (*GraphContainer[count]).config["Legend"]["marker"];
            Graph_Type = (*GraphContainer[count]).Graph_Type;
            Graph_TypeM[count] = Graph_Type;

            config["Draw_options"]["histogram"] =
                (*GraphContainer[count])
                    .config["Draw_options"]["histogram"]
                    .as<string>();
            config["Draw_options"]["histogram_2D"] =
                (*GraphContainer[count])
                    .config["Draw_options"]["histogram_2D"]
                    .as<string>();
            config["Draw_options"]["graph"] =
                (*GraphContainer[count])
                    .config["Draw_options"]["graph"]
                    .as<string>();
            config["Draw_options"]["graph_2D"] =
                (*GraphContainer[count])
                    .config["Draw_options"]["graph_2D"]
                    .as<string>();
        }
        if (count == 0)
            --Nr_of_graphsI; // to use a real number of graphs

        ROOTC.hist_1D = nullptr;
        ROOTC.hist_2D = nullptr;
        ROOTC.graph = nullptr;

        int error = InitializeAGraph();
        if (Graph_Type[0] == 'd' && error == 0)
            ROOTC.hist_1DM[count] = ROOTC.hist_1D;
        if (Graph_Type[0] == 'D' && error == 0)
            ROOTC.hist_2DM[count] = ROOTC.hist_2D;
        if (Graph_Type[0] == 'p' && error == 0)
            ROOTC.graphM[count] = ROOTC.graph;

        switch (error) {
        case 0:

            if (count == 0) {
                if (Print_Instead_Of_Draw) {
                    vector<double> DataX, DataY, DataZ, DataE;

                    cout << "Printing a graph out for "
                         << config["Generic"]["variable_name"].as<string>()
                         << endl;
                    if (Graph_Type[0] == 'd') {
                        if (Print_wErrors) {
                            ExtractHistDataXZErrInRange(ROOTC.hist_1DM[count],
                                                        DataX, DataZ, DataE);
                            for (unsigned long i1 = 0; i1 < DataX.size(); i1++)
                                cout << DataX[i1] << " " << DataZ[i1] << " "
                                     << DataE[i1] << endl;
                        } else {
                            ExtractHistDataXZInRange(ROOTC.hist_1DM[count],
                                                     DataX, DataZ);
                            for (unsigned long i1 = 0; i1 < DataX.size(); i1++)
                                cout << DataX[i1] << " " << DataZ[i1] << endl;
                        }
                    }

                    if (Graph_Type[0] == 'D') {
                        if (Print_wErrors) {
                            ExtractHistDataXYZErrInRange(ROOTC.hist_2DM[count],
                                                         DataX, DataY, DataZ,
                                                         DataE);
                            for (unsigned long i1 = 0; i1 < DataX.size(); i1++)
                                cout << i1 << " " << DataZ[i1] << endl;
                            // 								cout << i1 << " " << DataX[i1] << " "
                            // <<
                            // DataY[i1] << " " << DataZ[i1] << " " << DataE[i1]
                            // << endl;
                            //  								cout <<
                            //  DataX[i1] << " " << DataY[i1] << " " <<
                            //  DataZ[i1] << " " << DataE[i1] << endl;
                            // 								cout << i1 << " " << DataZ[i1]
                            // <<
                            // endl;
                        } else {
                            ExtractHistDataXYZInRange(ROOTC.hist_2DM[count],
                                                      DataX, DataY, DataZ);
                            for (unsigned long i1 = 0; i1 < DataX.size(); i1++)
                                cout << DataX[i1] << " " << DataY[i1] << " "
                                     << DataZ[i1] << endl;
                        }
                    }
                } else {
                    if (Graph_Type[0] == 'd')
                        ROOTC.hist_1DM[count]->Draw(str2char(RootDrawOptions));
                    if (Graph_Type[0] == 'D')
                        ROOTC.hist_2DM[count]->Draw(str2char(RootDrawOptions));
                    if (Graph_Type[0] == 'p')
                        ROOTC.graphM[count]->Draw(
                            str2char("A" + RootDrawOptions));

                    DrawAxes(); // Setting up axes
                }
            } else {
                if (Print_Instead_Of_Draw) {

                } else {
                    if (Graph_Type[0] == 'd')
                        ROOTC.hist_1DM[count]->Draw(
                            str2char(RootDrawOptions + " same"));
                    if (Graph_Type[0] == 'D')
                        ROOTC.hist_2DM[count]->Draw(
                            str2char(RootDrawOptions + " same"));
                    if (Graph_Type[0] == 'p')
                        ROOTC.graphM[count]->Draw(
                            str2char(RootDrawOptions + " same"));
                }
            }

            if (!Print_Instead_Of_Draw)
                ROOTC.canvas->Update();

            break;
        case 1:

            DeInitializeAGraph();
            cout << "Drawing failed. Data input error.\n";
            return 1;

            break;
        case 2:

            DeInitializeAGraph();
            cout << "Drawing failed. Failed to determine type of graph.\n";
            return 1;

            break;
        case 3:

            DeInitializeAGraph();
            cout << "Drawing failed. The provided number of custom bins "
                    "mismatch.\n";
            return 1;

            break;
        default:

            DeInitializeAGraph();
            cout << "Drawing failed. Unknown error (error code: " << error
                 << ").\n";
            return 1;
        }
    }

    if (!Print_Instead_Of_Draw) {
        /// case 0 must be valid
        if (config["Legend"]["add"].as<bool>())
            DrawLegend(); // Legends

        ////output files
        const string suffix = config["Output"]["type"].as<string>();
        const string stub = config["Output"]["filename_prefix"].as<string>() +
                            config["Output"]["filename"].as<string>();
        const string full = stub + "." + suffix;
        SaveC(ROOTC.canvas, full.c_str());

        const auto ROOT_file = config["Output"]["add_root_file"].as<string>();
        if (ROOT_file.size() > 0)
            SpitRootContainer(stub + ".root", ROOT_file);
    }

    DeInitializeAGraph();

    return 0;
}

void AGraph::DrawAxes()
{
    ROOTC.axis_X = new TAxis();
    ROOTC.axis_Y = new TAxis();
    ROOTC.axis_Z = new TAxis();

    if (Graph_Type[0] == 'd') {
        ROOTC.axis_X = ROOTC.hist_1D->GetXaxis();
        ROOTC.axis_Y = ROOTC.hist_1D->GetYaxis();
        ROOTC.axis_Z = ROOTC.hist_1D->GetZaxis();
    }

    if (Graph_Type[0] == 'D') {
        ROOTC.axis_X = ROOTC.hist_2D->GetXaxis();
        ROOTC.axis_Y = ROOTC.hist_2D->GetYaxis();
        ROOTC.axis_Z = ROOTC.hist_2D->GetZaxis();
    }

    if (Graph_Type[0] == 'p') {
        ROOTC.axis_X = ROOTC.graph->GetXaxis();
        ROOTC.axis_Y = ROOTC.graph->GetYaxis();
        // axis_Z = graph->GetZaxis(); //Does not have a member. Why?
    }

    ////Labels on axis (thus TAxis)
    const auto &label_x = config["Axes"]["label_x"].as<string>();
    const auto &label_y = config["Axes"]["label_y"].as<string>();
    const auto &label_z = config["Axes"]["label_z"].as<string>();
    ROOTC.axis_X->SetTitle(label_x.c_str());
    ROOTC.axis_Y->SetTitle(label_y.c_str());
    if (Graph_Type[0] != 'p' && Graph_Type[0] != 'P')
        ROOTC.axis_Z->SetTitle(label_z.c_str());

    ////Additional settings
    // axis_X->SetAxisColor(1);
    // axis_X->SetLabelColor(1);
    // axis_X->SetLabelFont(62);
    // axis_X->SetLabelOffset(0.005);
    // axis_X->SetLabelSize(0.04);
    // axis_X->SetNdivisions( 5, kFALSE);
    // axis_X->SetNoExponent(kTRUE);
    // axis_X->SetTickLength(0.03);
    // axis_X->SetTitleOffset(1.5);
    // axis_X->SetTitleSize(0.02);
    // axis_X->CenterLabels();
    // axis_X->CenterTitle();

    ////Axes' ranges. Added for adjusting TGraph X range
    const auto x_min = config["Range"]["x_min"].as<double>();
    const auto x_max = config["Range"]["x_max"].as<double>();
    ROOTC.axis_X->SetLimits(x_min, x_max);
    // axis_Y->SetLimits( z_min, z_max ); //Does not work??
    // if( Graph_Type[0]!='p' && Graph_Type[0]!='P' ) axis_Z->SetLimits(
    // y_min, y_min ); //Does not work??

    ROOTC.axis_X->SetTitleOffset(
        config["Formatting"]["title_offset"]["x"].as<double>());
    ROOTC.axis_X->CenterTitle();
    ROOTC.axis_Y->SetTitleOffset(
        config["Formatting"]["title_offset"]["y"].as<double>());
    ROOTC.axis_Y->CenterTitle();
    if (Graph_Type[0] != 'p' && Graph_Type[0] != 'P') {
        ROOTC.axis_Z->SetTitleOffset(
            config["Formatting"]["title_offset"]["z"].as<double>());
        ROOTC.axis_Z->CenterTitle();
    }

    ROOTC.axis_X->Draw(); //"ALP"
    ROOTC.axis_Y->Draw();
    if (Graph_Type[0] != 'p' && Graph_Type[0] != 'P')
        ROOTC.axis_Z->Draw();
}

void AGraph::DrawLegend()
{
    ROOTC.legend->SetTextFont(22);
    ROOTC.legend->SetTextSize(0.04);
    ROOTC.legend->SetFillColor(kWhite);

    ROOTC.legend->Draw();
}

void AGraph::SetStyle(string value)
{
    if (value == "" || value == '\n' || value == '\0') {
        ROOTC.style = styles::Set_TDRM3();
        auto &st = ROOTC.style;
        st->SetPadGridX(config["Other"]["pad_grid_x"].as<bool>());
        st->SetPadGridY(config["Other"]["pad_grid_y"].as<bool>());
        // not implemented
        // st->SetPadGridZ(config["Other"]["pad_grid_z"].as<bool>());

        st->SetPalette(config["Formatting"]["pallete_nr"].as<double>());
    }
    if (value == "TDR") {
        // ROOTC.style=new TStyle("Style","Style");
        // styles::Set_TDRM2();
        ROOTC.style = styles::Set_TDRM3();
        auto &st = ROOTC.style;
        st->SetPadGridX(config["Other"]["pad_grid_x"].as<bool>());
        st->SetPadGridY(config["Other"]["pad_grid_y"].as<bool>());
        // not implemented
        // st->SetPadGridZ(config["Other"]["pad_grid_z"].as<bool>());

        st->SetPalette(config["Formatting"]["pallete_nr"].as<double>());
    }
    if (value == "TDRc") {
        // ROOTC.style=new TStyle("Style","Style");
        // styles::Set_TDRM2();
        ROOTC.style = styles::Set_TDRM3();
        auto &st = ROOTC.style;
        st->SetPadGridX(config["Other"]["pad_grid_x"].as<bool>());
        st->SetPadGridY(config["Other"]["pad_grid_y"].as<bool>());
        // not implemented
        // st->SetPadGridZ(config["Other"]["pad_grid_z"].as<bool>());

        st->SetCanvasDefW(config["Formatting"]["x_resolution"].as<double>());
        st->SetCanvasDefH(config["Formatting"]["y_resolution"].as<double>());
        st->SetPadTopMargin(config["Formatting"]["margin"]["top"].as<double>());
        st->SetPadBottomMargin(
            config["Formatting"]["margin"]["bottom"].as<double>());
        st->SetPadLeftMargin(
            config["Formatting"]["margin"]["left"].as<double>());
        st->SetPadRightMargin(
            config["Formatting"]["margin"]["right"].as<double>());
        st->SetPalette(config["Formatting"]["pallete_nr"].as<double>());
    }
    if (value == "Matt") {
        ROOTC.style = new TStyle("Style", "Style");
        ROOTC.style = styles::Set_TDRM3();
        auto &st = ROOTC.style;
        styles::Set_Matt(
            static_cast<int>(config["Other"]["pad_grid_x"].as<bool>()),
            static_cast<int>(config["Other"]["pad_grid_y"].as<bool>()), 0, 0);

        st->SetPalette(config["Formatting"]["pallete_nr"].as<double>());
    }

    if (value != "" && value != '\n' && value != '\0' && value == "TDR" &&
        value == "Matt") {
        cout << "\nStyle is not set. Intended plotting is supposed to fail. "
                "You are on your own kid.\n";
    }
}

#endif
