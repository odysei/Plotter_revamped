#ifndef AGraph__drawers_cpp
#define AGraph__drawers_cpp

#include "AGraph/AGraph.h"
#include <algorithm>
#include "helpers/type_conversion.h"
#include "helpers/fillers_dumpers.h"
#include "helpers/ROOT_IO.h" // SaveC
#include "ROOT_styles/styles.h"

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

        int error = Init();
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
                        ROOTC.hist_1DM[count]->Draw(RootDrawOptions.c_str());
                    if (Graph_Type[0] == 'D')
                        ROOTC.hist_2DM[count]->Draw(RootDrawOptions.c_str());
                    if (Graph_Type[0] == 'p')
                        ROOTC.graphM[count]->Draw(
                            str2char("A" + RootDrawOptions));

                    Draw_axes(ROOTC); // Setting up axes
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
            cout << "Drawing failed. Data input error.\n";
            Deinit();
            return 1;

            break;
        case 2:
            cout << "Drawing failed. Failed to determine type of graph.\n";
            Deinit();
            return 1;

            break;
        case 3:
            cout << "Drawing failed. The provided number of custom bins "
                    "mismatch.\n";
            Deinit();
            return 1;

            break;
        default:
            cout << "Drawing failed. Unknown error (error code: " << error
                 << ").\n";
            Deinit();
            return 1;
        }
    }

    if (!Print_Instead_Of_Draw) {
        /// case 0 must be valid
        if (config["Legend"]["add"].as<bool>())
            Draw_legend(ROOTC.legend); // Legends

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

    Deinit();

    return 0;
}

void AGraph::Draw_axes(ROOT_containers &r)
{
    r.axis_X = new TAxis();
    r.axis_Y = new TAxis();
    r.axis_Z = new TAxis();

    if (Graph_Type[0] == 'd') {
        r.axis_X = r.hist_1D->GetXaxis();
        r.axis_Y = r.hist_1D->GetYaxis();
        r.axis_Z = r.hist_1D->GetZaxis();
    }

    if (Graph_Type[0] == 'D') {
        r.axis_X = r.hist_2D->GetXaxis();
        r.axis_Y = r.hist_2D->GetYaxis();
        r.axis_Z = r.hist_2D->GetZaxis();
    }

    if (Graph_Type[0] == 'p') {
        r.axis_X = r.graph->GetXaxis();
        r.axis_Y = r.graph->GetYaxis();
        // axis_Z = graph->GetZaxis(); //Does not have a member. Why?
    }

    ////Labels on axis (thus TAxis)
    const auto &label_x = config["Axes"]["label_x"].as<string>();
    const auto &label_y = config["Axes"]["label_y"].as<string>();
    const auto &label_z = config["Axes"]["label_z"].as<string>();
    r.axis_X->SetTitle(label_x.c_str());
    r.axis_Y->SetTitle(label_y.c_str());
    if (Graph_Type[0] != 'p' && Graph_Type[0] != 'P')
        r.axis_Z->SetTitle(label_z.c_str());

    ////Additional settings
    // r.axis_X->SetAxisColor(1);
    // r.axis_X->SetLabelColor(1);
    // r.axis_X->SetLabelFont(62);
    // r.axis_X->SetLabelOffset(0.005);
    // r.axis_X->SetLabelSize(0.04);
    // r.axis_X->SetNdivisions(5, kFALSE);
    // r.axis_X->SetNoExponent(kTRUE);
    // r.axis_X->SetTickLength(0.03);
    // r.axis_X->SetTitleOffset(1.5);
    // r.axis_X->SetTitleSize(0.02);
    // r.axis_X->CenterLabels();
    // r.axis_X->CenterTitle();

    ////Axes' ranges. Added for adjusting TGraph X range
    const auto x_min = config["Range"]["x_min"].as<double>();
    const auto x_max = config["Range"]["x_max"].as<double>();
    r.axis_X->SetLimits(x_min, x_max);
    // r.axis_Y->SetLimits(z_min, z_max); //Does not work??
    // if(Graph_Type[0] != 'p' && Graph_Type[0] != 'P')
    //         r.axis_Z->SetLimits(y_min, y_min); // Does not work??

    r.axis_X->SetTitleOffset(
        config["Formatting"]["title_offset"]["x"].as<double>());
    r.axis_X->CenterTitle();
    r.axis_Y->SetTitleOffset(
        config["Formatting"]["title_offset"]["y"].as<double>());
    r.axis_Y->CenterTitle();
    if (Graph_Type[0] != 'p' && Graph_Type[0] != 'P') {
        r.axis_Z->SetTitleOffset(
            config["Formatting"]["title_offset"]["z"].as<double>());
        r.axis_Z->CenterTitle();
    }

    r.axis_X->Draw(); //"ALP"
    r.axis_Y->Draw();
    if (Graph_Type[0] != 'p' && Graph_Type[0] != 'P')
        r.axis_Z->Draw();
}

void AGraph::Draw_legend(TLegend *l)
{
    l->SetTextFont(22);
    l->SetTextSize(0.04);
    l->SetFillColor(kWhite);

    l->Draw();
}

void AGraph::Set_style(const string &value)
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
