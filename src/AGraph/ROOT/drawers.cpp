#ifndef AGraph__drawers_cpp
#define AGraph__drawers_cpp

#include "AGraph/AGraph.h"
#include <sstream>
#include <algorithm>
#include "helpers/type_conversion.h"
#include "helpers/fillers_dumpers.h"
#include "helpers/array_manipulations.h"
#include "helpers/ROOT_IO.h"    // SaveC
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
    if (ContainerD == NULL && ContainerDY == NULL && ContainerDZ == NULL)
        return 1;
    if (Graph_Type[0] == 'd') {
        if (ContainerD == NULL)
            return 1;
        if (ContainerD->size() == 0)
            return 1;

        SetStyle(config["Formatting"]["style"].as<string>());

        if (config["Draw_options"]["histogram"].as<string>() == "")
            RootDrawOptions = "E1 HIST";
        else
            RootDrawOptions = config["Draw_options"]["histogram"].as<string>();

        AGraphHistogram_d =
            new TH1D(config["Generic"]["data_name"].as<string>().c_str(), "",
                     n_x, x_min, x_max);
        AGraphHistogram_d->SetLineColor(config["Curves_and_fill"]["line_marker_color"].as<int>());
        AGraphHistogram_d->SetMarkerColor(config["Curves_and_fill"]["line_marker_color"].as<int>());
        if (config["Curves_and_fill"]["fill"]["color"].as<int>() != -1)
            AGraphHistogram_d->SetFillColor(config["Curves_and_fill"]["fill"]["color"].as<int>());
        if (config["Curves_and_fill"]["marker"]["style"].as<int>() != -1)
            AGraphHistogram_d->SetMarkerStyle(config["Curves_and_fill"]["marker"]["style"].as<int>());
        if (config["Curves_and_fill"]["line"]["style"].as<int>() != -1)
            AGraphHistogram_d->SetLineStyle(config["Curves_and_fill"]["line"]["style"].as<int>());
        if (config["Curves_and_fill"]["fill"]["style"].as<int>() != -1)
            AGraphHistogram_d->SetFillStyle(config["Curves_and_fill"]["fill"]["style"].as<int>());
        if (config["Curves_and_fill"]["marker"]["size"].as<double>() != -1)
            AGraphHistogram_d->SetMarkerSize(config["Curves_and_fill"]["marker"]["size"].as<double>());
        if (config["Curves_and_fill"]["line"]["width"].as<double>() != -1)
            AGraphHistogram_d->SetLineWidth(config["Curves_and_fill"]["line"]["width"].as<double>());

        ////For errors
        AGraphHistogram_d->Sumw2();
        // 			AGraphHistogram_d->SetName( "hist" );

        ////Follows the filling procedure for the histograms
        if (ContainerDZ == NULL) {
            if (ContainerDw == NULL)
                AGraphHistogram_d = FillingHistD(
                    AGraphHistogram_d,
                    *ContainerD); // UC for LoadDataSimple(vector<double>)
            else
                AGraphHistogram_d =
                    FillingHistD(AGraphHistogram_d, *ContainerD, *ContainerDw);
        } else {
            if (ContainerDw == NULL)
                AGraphHistogram_d =
                    FillingHistD(AGraphHistogram_d, *ContainerD, *ContainerDZ,
                                 n_x, x_min, x_max);
            else
                AGraphHistogram_d =
                    FillingHistD(AGraphHistogram_d, *ContainerD, *ContainerDZ,
                                 *ContainerDw, n_x, x_min, x_max);
        }

        if (config["Smooth"]["use"].as<bool>())
            AGraphHistogram_d->Smooth(config["Smooth"]["times"].as<int>());

        if (config["Floor"]["add"].as<bool>()) {
            auto &h = AGraphHistogram_d;
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
            scale1 *= 1 / AGraphHistogram_d->Integral();
            AGraphHistogram_d->Scale(scale1);
        } else {
            if (scale1 != 1)
                AGraphHistogram_d->Scale(scale1);
        }

        ////Setting Z range with possible autoscaling, choosing vector container
        ///for Z instead of root container
        if (config["Auto"]["scale_z_min"].as<bool>()) {
            if (ContainerDZ != NULL)
                if (ContainerDZ->size() != 0)
                    AGraphHistogram_d->SetMinimum(MinimumValue(*ContainerDZ) *
                                                  scale1);
        } else
            AGraphHistogram_d->SetMinimum(z_min);

        if (config["Auto"]["scale_z_max"].as<bool>()) {
            if (ContainerDZ != NULL)
                if (ContainerDZ->size() != 0)
                    AGraphHistogram_d->SetMaximum(MaximumValue(*ContainerDZ) *
                                                  scale1);
        } else
            AGraphHistogram_d->SetMaximum(z_max);

        const auto &title = config["Generic"]["plot_name"].as<string>();
        AGraphHistogram_d->SetTitle(title.c_str());

        if (Initialized == false) {
            //.75,.80,.95,.95
            if (config["Legend"]["add"].as<bool>())
                AGraphLegend =new TLegend(
                    config["Legend"]["placement"]["BL_x"].as<double>(),
                    config["Legend"]["placement"]["BL_y"].as<double>(),
                    config["Legend"]["placement"]["TR_x"].as<double>(),
                    config["Legend"]["placement"]["TR_y"].as<double>());

            ////Initializing Canvas
            AGraphCanvas = new TCanvas("AGraphCanvas", "Histogram(s)");

            if (config["Axes"]["log_x"].as<bool>())
                AGraphCanvas->SetLogx(1);
            else
                AGraphCanvas->SetLogx(0);

            // note flipped z with y for consistency
            if (config["Axes"]["log_z"].as<bool>())
                AGraphCanvas->SetLogy(1);
            else
                AGraphCanvas->SetLogy(0);

            // AGraphCanvas->SetFrameFillColor(kWhite);
            // AGraphCanvas->SetFillColor(kWhite);
            // AGraphStyle->SetOptStat(kFALSE);
        }
        if (config["Legend"]["add"].as<bool>()) {
            const auto &var_name =
                config["Generic"]["variable_name"].as<string>();
            const auto &mark = config["Legend"]["marker"].as<string>();
            AGraphLegend->AddEntry(AGraphHistogram_d, var_name.c_str(),
                                   mark.c_str());
        }

        Initialized = true;
        return 0;
    }

    if (Graph_Type[0] == 'D') {
        if (ContainerD == NULL || ContainerDY == NULL)
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
            RootDrawOptions +=" zs(0)"; //scale: zs(0) LIN, zs(1) LOG, zs(2) SQRT
            */
        } else {
            RootDrawOptions = config["Draw_options"]["histogram_2D"].as<string>();
        }
        AGraphStyle->SetPalette(1);

        const auto &dn = config["Generic"]["data_name"].as<string>();
        if (!config["Bins"]["custom_binning"].as<bool>())
            AGraphHistogram_D =
                new TH2D(dn.c_str(), "", n_x, x_min, x_max, n_y, y_min, y_max);
        else
            AGraphHistogram_D =
                new TH2D(dn.c_str(), "", n_x, Custom_BinsX, n_y, Custom_BinsY);
        AGraphHistogram_D->SetLineColor(config["Curves_and_fill"]["line_marker_color"].as<int>());
        AGraphHistogram_D->SetMarkerColor(config["Curves_and_fill"]["line_marker_color"].as<int>());
        if (config["Curves_and_fill"]["marker"]["style"].as<int>() != -1)
            AGraphHistogram_D->SetMarkerStyle(config["Curves_and_fill"]["marker"]["style"].as<int>());
        if (config["Curves_and_fill"]["line"]["style"].as<int>() != -1)
            AGraphHistogram_D->SetLineStyle(config["Curves_and_fill"]["line"]["style"].as<int>());
        if (config["Curves_and_fill"]["marker"]["size"].as<double>() != -1)
            AGraphHistogram_D->SetMarkerSize(config["Curves_and_fill"]["marker"]["size"].as<double>());
        if (config["Curves_and_fill"]["line"]["width"].as<double>() != -1)
            AGraphHistogram_D->SetLineWidth(config["Curves_and_fill"]["line"]["width"].as<double>());

        ////For errors
        AGraphHistogram_D->Sumw2();

        ////Follows the filling procedure for the histograms
        if (ContainerDZ == NULL) {
            if (ContainerDw == NULL)
                AGraphHistogram_D =
                    FillingHist2D(AGraphHistogram_D, *ContainerD, *ContainerDY);
            else
                AGraphHistogram_D = FillingHist2D(
                    AGraphHistogram_D, *ContainerD, *ContainerDY, *ContainerDw);
        } else {
            if (ContainerDw == NULL)
                AGraphHistogram_D = FillingHist2DWOff(
                    AGraphHistogram_D, *ContainerD, Offset2DHistX, *ContainerDY,
                    Offset2DHistY, *ContainerDZ, Offset2DHistZ, n_x,
                    x_min, x_max, n_y, y_min, y_max);
            else
                AGraphHistogram_D = FillingHist2DWOff(
                    AGraphHistogram_D, *ContainerD, Offset2DHistX, *ContainerDY,
                    Offset2DHistY, *ContainerDZ, Offset2DHistZ, *ContainerDw,
                    n_x, x_min, x_max, n_y, y_min, y_max);
        }

        if (config["Smooth"]["use"].as<bool>()) {
            for (auto i = 0; i < config["Smooth"]["times"].as<int>(); ++i)
                AGraphHistogram_D->Smooth(1,
                                          config["Smooth"]["algorithm_2D"].as<string>().c_str());
        }

        if (config["Floor"]["add"].as<bool>()) {
            auto &h = AGraphHistogram_D;
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
            scale1 *= 1 / (AGraphHistogram_D->Integral());
            AGraphHistogram_D->Scale(scale1);
        } else {
            if (scale1 != 1)
                AGraphHistogram_D->Scale(scale1);
        }

        ////Setting Z range with possible autoscaling, choosing vector container
        ///for Z instead of root container
        if (config["Auto"]["scale_z_min"].as<bool>()) {
            if (ContainerDZ != NULL)
                if (ContainerDZ->size() != 0)
                    AGraphHistogram_D->SetMinimum(MinimumValue(*ContainerDZ) *
                                                  scale1);
        } else
            AGraphHistogram_D->SetMinimum(z_min);

        if (config["Auto"]["scale_z_max"].as<bool>()) {
            if (ContainerDZ != NULL)
                if (ContainerDZ->size() != 0)
                    AGraphHistogram_D->SetMaximum(MaximumValue(*ContainerDZ) *
                                                  scale1);
        } else
            AGraphHistogram_D->SetMaximum(z_max);

        AGraphHistogram_D->SetTitle(str2char(config["Generic"]["plot_name"].as<string>()));

        if (Initialized == false) {
            //.75,.80,.95,.95
            if (config["Legend"]["add"].as<bool>())
                AGraphLegend = new TLegend(
                    config["Legend"]["placement"]["BL_x"].as<double>(),
                    config["Legend"]["placement"]["BL_y"].as<double>(),
                    config["Legend"]["placement"]["TR_x"].as<double>(),
                    config["Legend"]["placement"]["TR_y"].as<double>());

            ////Initializing Canvas
            AGraphCanvas = new TCanvas("AGraphCanvas", "Histogram(s)");

            if (config["Axes"]["log_x"].as<bool>())
                AGraphCanvas->SetLogx(1);
            else
                AGraphCanvas->SetLogx(0);

            if (config["Axes"]["log_y"].as<bool>())
                AGraphCanvas->SetLogy(1);
            else
                AGraphCanvas->SetLogy(0);

            if (config["Axes"]["log_z"].as<bool>())
                AGraphCanvas->SetLogz(1);
            else
                AGraphCanvas->SetLogz(0);

            // AGraphCanvas->SetFrameFillColor(kWhite);
            // AGraphCanvas->SetFillColor(kWhite);
            // AGraphStyle->SetOptStat(kFALSE);
        }
        if (config["Legend"]["add"].as<bool>()) {
            const auto &var_name =
                config["Generic"]["variable_name"].as<string>();
            const auto &mark = config["Legend"]["marker"].as<string>();
            AGraphLegend->AddEntry(AGraphHistogram_D, var_name.c_str(),
                                   mark.c_str());
        }

        Initialized = true;
        return 0;
    }

    if (Graph_Type[0] == 'p') {
        if (ContainerD == NULL)
            return 1;
        if (ContainerD->size() == 0)
            return 1;

        SetStyle(config["Formatting"]["style"].as<string>());

        if (config["Draw_options"]["graph"].as<string>() == "") {
            RootDrawOptions = "PL";
        } else {
            RootDrawOptions = config["Draw_options"]["graph"].as<string>();
        }

        AGraphPlot = new TGraph(ContainerD->size(), vct2arrD(*ContainerD),
                                vct2arrD(*ContainerDZ));
        AGraphPlot->SetLineColor(config["Curves_and_fill"]["line_marker_color"].as<int>());
        AGraphPlot->SetMarkerColor(config["Curves_and_fill"]["line_marker_color"].as<int>());
        if (config["Curves_and_fill"]["marker"]["style"].as<int>() != -1)
            AGraphPlot->SetMarkerStyle(config["Curves_and_fill"]["marker"]["style"].as<int>());
        if (config["Curves_and_fill"]["line"]["style"].as<int>() != -1)
            AGraphPlot->SetLineStyle(config["Curves_and_fill"]["line"]["style"].as<int>());
        if (config["Curves_and_fill"]["marker"]["size"].as<double>() != -1)
            AGraphPlot->SetMarkerSize(config["Curves_and_fill"]["marker"]["size"].as<double>());
        if (config["Curves_and_fill"]["line"]["width"].as<double>() != -1)
            AGraphPlot->SetLineWidth(config["Curves_and_fill"]["line"]["width"].as<double>());

        ////Setting Z range with possible autoscaling, choosing vector container
        ///for Z instead of root container
        if (config["Auto"]["scale_z_min"].as<bool>()) {
            if (ContainerDZ != NULL)
                if (ContainerDZ->size() != 0)
                    AGraphPlot->SetMinimum(MinimumValue(*ContainerDZ));
        } else
            AGraphPlot->SetMinimum(z_min);

        if (config["Auto"]["scale_z_max"].as<bool>()) {
            if (ContainerDZ != NULL)
                if (ContainerDZ->size() != 0)
                    AGraphPlot->SetMaximum(MaximumValue(*ContainerDZ));
        } else
            AGraphPlot->SetMaximum(z_max);

        AGraphPlot->SetTitle(str2char(config["Generic"]["plot_name"].as<string>()));

        if (Initialized == false) {
            //.75,.80,.95,.95
            if (config["Legend"]["add"].as<bool>())
                AGraphLegend = new TLegend(
                    config["Legend"]["placement"]["BL_x"].as<double>(),
                    config["Legend"]["placement"]["BL_y"].as<double>(),
                    config["Legend"]["placement"]["TR_x"].as<double>(),
                    config["Legend"]["placement"]["TR_y"].as<double>());

            ////Initializing Canvas
            AGraphCanvas = new TCanvas("AGraphCanvas", "Histogram(s)");

            if (config["Axes"]["log_x"].as<bool>())
                AGraphCanvas->SetLogx(1);
            else
                AGraphCanvas->SetLogx(0);

            // note flipped z with y for consistency
            if (config["Axes"]["log_z"].as<bool>())
                AGraphCanvas->SetLogy(1);
            else
                AGraphCanvas->SetLogy(0);

            // AGraphCanvas->SetFrameFillColor(kWhite);
            // AGraphCanvas->SetFillColor(kWhite);
            // AGraphStyle->SetOptStat(kFALSE);
        }
        if (config["Legend"]["add"].as<bool>()) {
            const auto &var_name =
                config["Generic"]["variable_name"].as<string>();
            const auto &mark = config["Legend"]["marker"].as<string>();
            AGraphLegend->AddEntry(AGraphPlot, var_name.c_str(), mark.c_str());
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

    for (unsigned int i = 0; i < Nr_of_graphs; i++) {
        if (Graph_TypeM[i][0] == 'd' && AGraphHistogram_dM[i] != NULL)
            delete AGraphHistogram_dM[i];
        if (Graph_TypeM[i][0] == 'D' && AGraphHistogram_DM[i] != NULL)
            delete AGraphHistogram_DM[i];
        if (Graph_TypeM[i][0] == 'p' && AGraphPlotM[i] != NULL)
            delete AGraphPlotM[i];
    }

    if (AGraphHistogram_dM != NULL)
        delete[] AGraphHistogram_dM;
    if (AGraphHistogram_DM != NULL)
        delete[] AGraphHistogram_DM;
    if (AGraphPlotM != NULL)
        delete[] AGraphPlotM;

    Graph_Type = Graph_TypeM[0];
    delete[] Graph_TypeM;

    // deteleting *Ms takes care of these 3
    // 	delete AGraphHistogram_d;
    // 	delete AGraphHistogram_D;
    // 	delete AGraphPlot;
    // delete AGraphAxisX; //This should not be deleted as it will cause crash
    // in most cases
    // delete AGraphAxisY; //This should not be deleted as it will cause crash
    // in most cases
    // delete AGraphAxisZ; //This should not be deleted as it will cause crash
    // in most cases
    if (AGraphCanvas != NULL)
        delete AGraphCanvas;
    if (AGraphLegend != NULL)
        delete AGraphLegend;
    if (AGraphStyle != NULL)
        delete AGraphStyle;

    AGraphHistogram_d = NULL;
    AGraphHistogram_dM = NULL;
    AGraphHistogram_D = NULL;
    AGraphHistogram_DM = NULL;
    AGraphPlot = NULL;
    AGraphPlotM = NULL;
    AGraphAxisX = NULL;
    AGraphAxisY = NULL;
    AGraphAxisZ = NULL;
    AGraphCanvas = NULL;
    AGraphLegend = NULL;
    AGraphStyle = NULL;
}

int AGraph::Draw() { return Draw(NULL, 0); }

int AGraph::Draw(AGraph **GraphContainer, unsigned int Nr_of_graphsI)
{
    Nr_of_graphs = 0;
    Nr_of_graphsI++; // to make Nr_of_graphs=0 work with preset containers

    if (Nr_of_graphsI == 1) {
        Nr_of_graphs = 1;
        Graph_TypeM = new string[Nr_of_graphsI];
        Graph_TypeM[0] = Graph_Type;

        AGraphHistogram_dM = new TH1D *[Nr_of_graphsI];
        AGraphHistogram_DM = new TH2D *[Nr_of_graphsI];
        AGraphPlotM = new TGraph *[Nr_of_graphsI];

        AGraphHistogram_dM[0] = NULL;
        AGraphHistogram_DM[0] = NULL;
        AGraphPlotM[0] = NULL;

    } else {
        Nr_of_graphs = Nr_of_graphsI - 1;
        Graph_TypeM = new string[Nr_of_graphs];

        AGraphHistogram_dM = new TH1D *[Nr_of_graphs];
        AGraphHistogram_DM = new TH2D *[Nr_of_graphs];
        AGraphPlotM = new TGraph *[Nr_of_graphs];

        for (unsigned int i = 0; i < Nr_of_graphs; i++) {
            AGraphHistogram_dM[i] = NULL;
            AGraphHistogram_DM[i] = NULL;
            AGraphPlotM[i] = NULL;
        }
    }

    for (unsigned int count = 0; count < Nr_of_graphsI; count++) {
        if (GraphContainer != NULL && Nr_of_graphsI > 1) {
            ContainerD = (*GraphContainer[count]).ContainerD;
            ContainerDY = (*GraphContainer[count]).ContainerDY;
            ContainerDZ = (*GraphContainer[count]).ContainerDZ;
            ContainerDw = (*GraphContainer[count]).ContainerDw;

            config["Curves_and_fill"]["line_marker_color"] =
                (*GraphContainer[count]).config["Curves_and_fill"]["line_marker_color"].as<int>();
            config["Curves_and_fill"]["marker"]["style"] =
                (*GraphContainer[count]).config["Curves_and_fill"]["marker"]["style"].as<int>();
            config["Curves_and_fill"]["line"]["style"] =
                (*GraphContainer[count]).config["Curves_and_fill"]["line"]["style"].as<int>();
            config["Curves_and_fill"]["marker"]["size"] =
                (*GraphContainer[count]).config["Curves_and_fill"]["marker"]["size"].as<double>();
            config["Curves_and_fill"]["line"]["width"] =
                (*GraphContainer[count]).config["Curves_and_fill"]["line"]["width"].as<double>();
            config["Generic"]["variable_name"] =
                (*GraphContainer[count]).config["Generic"]["variable_name"];
            config["Generic"]["data_name"] =
                (*GraphContainer[count]).config["Generic"]["data_name"];
            config["Legend"]["marker"] =
                (*GraphContainer[count]).config["Legend"]["marker"];
            Graph_Type = (*GraphContainer[count]).Graph_Type;
            Graph_TypeM[count] = Graph_Type;

            config["Draw_options"]["histogram"] =
                (*GraphContainer[count]).config["Draw_options"]["histogram"].as<string>();
            config["Draw_options"]["histogram_2D"] =
                (*GraphContainer[count]).config["Draw_options"]["histogram_2D"].as<string>();
            config["Draw_options"]["graph"] =
                (*GraphContainer[count]).config["Draw_options"]["graph"].as<string>();
            config["Draw_options"]["graph_2D"] =
                (*GraphContainer[count]).config["Draw_options"]["graph_2D"].as<string>();
        }
        if (count == 0)
            --Nr_of_graphsI; // to use a real number of graphs

        AGraphHistogram_d = NULL;
        AGraphHistogram_D = NULL;
        AGraphPlot = NULL;

        int error = InitializeAGraph();
        if (Graph_Type[0] == 'd' && error == 0)
            AGraphHistogram_dM[count] = AGraphHistogram_d;
        if (Graph_Type[0] == 'D' && error == 0)
            AGraphHistogram_DM[count] = AGraphHistogram_D;
        if (Graph_Type[0] == 'p' && error == 0)
            AGraphPlotM[count] = AGraphPlot;

        switch (error) {
        case 0:

            if (count == 0) {
                if (Print_Instead_Of_Draw) {
                    vector<double> DataX, DataY, DataZ, DataE;

                    cout << "Printing a graph out for " <<
                        config["Generic"]["variable_name"].as<string>() << endl;
                    if (Graph_Type[0] == 'd') {
                        if (Print_wErrors) {
                            ExtractHistDataXZErrInRange(
                                AGraphHistogram_dM[count], DataX, DataZ, DataE);
                            for (unsigned long i1 = 0; i1 < DataX.size(); i1++)
                                cout << DataX[i1] << " " << DataZ[i1] << " "
                                     << DataE[i1] << endl;
                        } else {
                            ExtractHistDataXZInRange(AGraphHistogram_dM[count],
                                                     DataX, DataZ);
                            for (unsigned long i1 = 0; i1 < DataX.size(); i1++)
                                cout << DataX[i1] << " " << DataZ[i1] << endl;
                        }
                    }

                    if (Graph_Type[0] == 'D') {
                        if (Print_wErrors) {
                            ExtractHistDataXYZErrInRange(
                                AGraphHistogram_DM[count], DataX, DataY, DataZ,
                                DataE);
                            for (unsigned long i1 = 0; i1 < DataX.size(); i1++)
                                cout << i1 << " " << DataZ[i1] << endl;
                            // 								cout << i1 << " " << DataX[i1] << " " <<
                            // DataY[i1] << " " << DataZ[i1] << " " << DataE[i1]
                            // << endl;
                            //  								cout <<
                            //  DataX[i1] << " " << DataY[i1] << " " <<
                            //  DataZ[i1] << " " << DataE[i1] << endl;
                            // 								cout << i1 << " " << DataZ[i1] <<
                            // endl;
                        } else {
                            ExtractHistDataXYZInRange(AGraphHistogram_DM[count],
                                                      DataX, DataY, DataZ);
                            for (unsigned long i1 = 0; i1 < DataX.size(); i1++)
                                cout << DataX[i1] << " " << DataY[i1] << " "
                                     << DataZ[i1] << endl;
                        }
                    }
                } else {
                    if (Graph_Type[0] == 'd')
                        AGraphHistogram_dM[count]->Draw(
                            str2char(RootDrawOptions));
                    if (Graph_Type[0] == 'D')
                        AGraphHistogram_DM[count]->Draw(
                            str2char(RootDrawOptions));
                    if (Graph_Type[0] == 'p')
                        AGraphPlotM[count]->Draw(
                            str2char("A" + RootDrawOptions));

                    DrawAxes(); // Setting up axes
                }
            } else {
                if (Print_Instead_Of_Draw) {

                } else {
                    if (Graph_Type[0] == 'd')
                        AGraphHistogram_dM[count]->Draw(
                            str2char(RootDrawOptions + " same"));
                    if (Graph_Type[0] == 'D')
                        AGraphHistogram_DM[count]->Draw(
                            str2char(RootDrawOptions + " same"));
                    if (Graph_Type[0] == 'p')
                        AGraphPlotM[count]->Draw(
                            str2char(RootDrawOptions + " same"));
                }
            }

            if (!Print_Instead_Of_Draw)
                AGraphCanvas->Update();

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
        SaveC(AGraphCanvas, full.c_str());
        
        const auto ROOT_file = config["Output"]["add_root_file"].as<string>();
        if (ROOT_file.size() > 0)
            SpitRootContainer(stub + ".root", ROOT_file);
    }

    DeInitializeAGraph();

    return 0;
}

void AGraph::DrawAxes()
{
    AGraphAxisX = new TAxis();
    AGraphAxisY = new TAxis();
    AGraphAxisZ = new TAxis();

    if (Graph_Type[0] == 'd') {
        AGraphAxisX = AGraphHistogram_d->GetXaxis();
        AGraphAxisY = AGraphHistogram_d->GetYaxis();
        AGraphAxisZ = AGraphHistogram_d->GetZaxis();
    }

    if (Graph_Type[0] == 'D') {
        AGraphAxisX = AGraphHistogram_D->GetXaxis();
        AGraphAxisY = AGraphHistogram_D->GetYaxis();
        AGraphAxisZ = AGraphHistogram_D->GetZaxis();
    }

    if (Graph_Type[0] == 'p') {
        AGraphAxisX = AGraphPlot->GetXaxis();
        AGraphAxisY = AGraphPlot->GetYaxis();
        // AGraphAxisZ = AGraphPlot->GetZaxis(); //Does not have a member. Why?
    }

    ////Labels on axis (thus TAxis)
    const auto &label_x = config["Axes"]["label_x"].as<string>();
    const auto &label_y = config["Axes"]["label_y"].as<string>();
    const auto &label_z = config["Axes"]["label_z"].as<string>();
    AGraphAxisX->SetTitle(label_x.c_str());
    AGraphAxisY->SetTitle(label_y.c_str());
    if (Graph_Type[0] != 'p' && Graph_Type[0] != 'P')
        AGraphAxisZ->SetTitle(label_z.c_str());

    ////Additional settings
    // AGraphAxisX->SetAxisColor(1);
    // AGraphAxisX->SetLabelColor(1);
    // AGraphAxisX->SetLabelFont(62);
    // AGraphAxisX->SetLabelOffset(0.005);
    // AGraphAxisX->SetLabelSize(0.04);
    // AGraphAxisX->SetNdivisions( 5, kFALSE);
    // AGraphAxisX->SetNoExponent(kTRUE);
    // AGraphAxisX->SetTickLength(0.03);
    // AGraphAxisX->SetTitleOffset(1.5);
    // AGraphAxisX->SetTitleSize(0.02);
    // AGraphAxisX->CenterLabels();
    // AGraphAxisX->CenterTitle();

    ////Axes' ranges. Added for adjusting TGraph X range
    const auto x_min = config["Range"]["x_min"].as<double>();
    const auto x_max = config["Range"]["x_max"].as<double>();
    AGraphAxisX->SetLimits(x_min, x_max);
    // AGraphAxisY->SetLimits( z_min, z_max ); //Does not work??
    // if( Graph_Type[0]!='p' && Graph_Type[0]!='P' ) AGraphAxisZ->SetLimits(
    // y_min, y_min ); //Does not work??

    AGraphAxisX->SetTitleOffset(config["Formatting"]["title_offset"]["x"].as<double>());
    AGraphAxisX->CenterTitle();
    AGraphAxisY->SetTitleOffset(config["Formatting"]["title_offset"]["y"].as<double>());
    AGraphAxisY->CenterTitle();
    if (Graph_Type[0] != 'p' && Graph_Type[0] != 'P') {
        AGraphAxisZ->SetTitleOffset(config["Formatting"]["title_offset"]["z"].as<double>());
        AGraphAxisZ->CenterTitle();
    }

    AGraphAxisX->Draw(); //"ALP"
    AGraphAxisY->Draw();
    if (Graph_Type[0] != 'p' && Graph_Type[0] != 'P')
        AGraphAxisZ->Draw();
}

void AGraph::DrawLegend()
{
    AGraphLegend->SetTextFont(22);
    AGraphLegend->SetTextSize(0.04);
    AGraphLegend->SetFillColor(kWhite);

    AGraphLegend->Draw();
}

void AGraph::SetStyle(string value)
{
    if (value == "" || value == '\n' || value == '\0') {
        AGraphStyle = styles::Set_TDRM3();
        auto &st = AGraphStyle;
        st->SetPadGridX(config["Other"]["pad_grid_x"].as<bool>());
        st->SetPadGridY(config["Other"]["pad_grid_y"].as<bool>());
        // st->SetPadGridZ(config["Other"]["pad_grid_z"].as<bool>()); //not implemented

        st->SetPalette(config["Formatting"]["pallete_nr"].as<double>());
    }
    if (value == "TDR") {
        // AGraphStyle=new TStyle("Style","Style");
        // styles::Set_TDRM2();
        AGraphStyle = styles::Set_TDRM3();
        auto &st = AGraphStyle;
        st->SetPadGridX(config["Other"]["pad_grid_x"].as<bool>());
        st->SetPadGridY(config["Other"]["pad_grid_y"].as<bool>());
        // st->SetPadGridZ(config["Other"]["pad_grid_z"].as<bool>()); //not implemented

        st->SetPalette(config["Formatting"]["pallete_nr"].as<double>());
    }
    if (value == "TDRc") {
        // AGraphStyle=new TStyle("Style","Style");
        // styles::Set_TDRM2();
        AGraphStyle = styles::Set_TDRM3();
        auto &st = AGraphStyle;
        st->SetPadGridX(config["Other"]["pad_grid_x"].as<bool>());
        st->SetPadGridY(config["Other"]["pad_grid_y"].as<bool>());
        // st->SetPadGridZ(config["Other"]["pad_grid_z"].as<bool>()); //not implemented

        st->SetCanvasDefW(config["Formatting"]["x_resolution"].as<double>());
        st->SetCanvasDefH(config["Formatting"]["y_resolution"].as<double>());
        st->SetPadTopMargin(config["Formatting"]["margin"]["top"].as<double>());
        st->SetPadBottomMargin(config["Formatting"]["margin"]["bottom"].as<double>());
        st->SetPadLeftMargin(config["Formatting"]["margin"]["left"].as<double>());
        st->SetPadRightMargin(config["Formatting"]["margin"]["right"].as<double>());
        st->SetPalette(config["Formatting"]["pallete_nr"].as<double>());
    }
    if (value == "Matt") {
        AGraphStyle = new TStyle("Style", "Style");
        AGraphStyle = styles::Set_TDRM3();
        auto &st = AGraphStyle;
        styles::Set_Matt(static_cast<int>(config["Other"]["pad_grid_x"].as<bool>()),
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
