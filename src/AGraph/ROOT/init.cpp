#ifndef AGraph__init_cpp
#define AGraph__init_cpp

#include "AGraph/AGraph.h"
#include <algorithm>
#include "helpers/type_conversion.h"
#include "helpers/fillers_dumpers.h"
#include "helpers/array_manipulations.h"
#include "ROOT_styles/styles.h"

inline bool AGraph::Init_containers_OK()
{
    if (ContainerD == nullptr && ContainerDY == nullptr &&
        ContainerDZ == nullptr)
        return false;
    
    if (Graph_Type[0] == 'd' || Graph_Type[0] == 'p') {
        if (ContainerD == nullptr)
            return false;
        if (ContainerD->size() == 0)
            return false;
    }
    if (Graph_Type[0] == 'D') {
        if (ContainerD == nullptr || ContainerDY == nullptr)
            return false;
        if (ContainerD->size() == 0 || ContainerDY->size() == 0)
            return false;
    }
    
    return true;
}

template <class gra>
inline void AGraph::Init_commons(gra *g)
{
    const auto line_marker_color =
        config["Curves_and_fill"]["line_marker_color"].as<int>();
    const auto marker_style =
        config["Curves_and_fill"]["marker"]["style"].as<int>();
    const auto line_style =
        config["Curves_and_fill"]["line"]["style"].as<int>();
    const auto marker_size =
        config["Curves_and_fill"]["marker"]["size"].as<double>();
    const auto line_width =
        config["Curves_and_fill"]["line"]["width"].as<double>();
    
    g->SetLineColor(line_marker_color);
    g->SetMarkerColor(line_marker_color);
    if (marker_style != -1)
        g->SetMarkerStyle(marker_style);
    if (line_style != -1)
        g->SetLineStyle(line_style);
    if (marker_size != -1)
        g->SetMarkerSize(marker_size);
    if (line_width != -1)
        g->SetLineWidth(line_width);
}

inline void AGraph::Init_hist_fill(TH1D *h)
{
    const auto n_x = config["Bins"]["n_x"].as<unsigned int>();
    const auto x_min = config["Range"]["x_min"].as<double>();
    const auto x_max = config["Range"]["x_max"].as<double>();
    // Follows the filling procedure for the histograms
    if (ContainerDZ == nullptr) {
        if (ContainerDw == nullptr)
            h = FillingHistD(h, *ContainerD); // UC LoadDataSimple(vector<>)
        else
            h = FillingHistD(h, *ContainerD, *ContainerDw);
    } else {
        if (ContainerDw == nullptr)
            h = FillingHistD(h, *ContainerD, *ContainerDZ, n_x, x_min, x_max);
        else
            h = FillingHistD(h, *ContainerD, *ContainerDZ, *ContainerDw, n_x,
                             x_min, x_max);
    }
}

inline void AGraph::Init_hist_fill(TH2D *h)
{
    const auto n_x = config["Bins"]["n_x"].as<unsigned int>();
    const auto n_y = config["Bins"]["n_y"].as<unsigned int>();
    const auto x_min = config["Range"]["x_min"].as<double>();
    const auto x_max = config["Range"]["x_max"].as<double>();
    const auto y_min = config["Range"]["y_min"].as<double>();
    const auto y_max = config["Range"]["y_max"].as<double>();
    // Follows the filling procedure for the histograms
    if (ContainerDZ == nullptr) {
        if (ContainerDw == nullptr)
            h = FillingHist2D(h, *ContainerD, *ContainerDY);
        else
            h = FillingHist2D(h, *ContainerD, *ContainerDY, *ContainerDw);
    } else {
        if (ContainerDw == nullptr)
            h = FillingHist2DWOff(h, *ContainerD, Offset2DHistX, *ContainerDY,
                Offset2DHistY, *ContainerDZ, Offset2DHistZ, n_x, x_min,
                x_max, n_y, y_min, y_max);
        else
            h = FillingHist2DWOff(h, *ContainerD, Offset2DHistX, *ContainerDY,
                Offset2DHistY, *ContainerDZ, Offset2DHistZ, *ContainerDw,
                n_x, x_min, x_max, n_y, y_min, y_max);
    }
}

inline void AGraph::Init_hist_smooth(TH1D *h)
{
    h->Smooth(config["Smooth"]["times"].as<int>());
}

inline void AGraph::Init_hist_smooth(TH2D *h)
{
    const auto algo = config["Smooth"]["algorithm_2D"].as<string>().c_str();
    for (auto i = 0; i < config["Smooth"]["times"].as<int>(); ++i)
        h->Smooth(1, algo);
}

inline void AGraph::Init_hist_floor(TH1D *h)
{
    const auto n_x = config["Bins"]["n_x"].as<unsigned int>();
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

inline void AGraph::Init_hist_floor(TH2D *h)
{
    const auto n_x = config["Bins"]["n_x"].as<unsigned int>();
    const auto n_y = config["Bins"]["n_y"].as<unsigned int>();
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

template <class hist>
inline int AGraph::Init_hist_tail(hist *h)
{
    // For errors
    h->Sumw2();

    Init_hist_fill(h);

    if (config["Smooth"]["use"].as<bool>())
        Init_hist_smooth(h);
    if (config["Floor"]["add"].as<bool>())
        Init_hist_floor(h);

    // Normalization and rescaling
    double scale1 = config["Auto"]["rescale"].as<double>();
    if (config["Auto"]["normalize"].as<bool>()) {
        scale1 *= 1 / h->Integral();
        h->Scale(scale1);
    } else {
        if (scale1 != 1)
            h->Scale(scale1);
    }

    return Init_tail(scale1, h);
}

template <class gra>
inline int AGraph::Init_tail(const double &scale, gra *g)
{
    const auto z_min = config["Range"]["z_min"].as<double>();
    const auto z_max = config["Range"]["z_max"].as<double>();
    
    ////Setting Z range with possible autoscaling, choosing vector container
    /// for Z instead of root container
    if (config["Auto"]["scale_z_min"].as<bool>()) {
        if (ContainerDZ != nullptr)
            if (ContainerDZ->size() != 0)
                g->SetMinimum(MinimumValue(*ContainerDZ) * scale);
    } else
        g->SetMinimum(z_min);

    if (config["Auto"]["scale_z_max"].as<bool>()) {
        if (ContainerDZ != nullptr)
            if (ContainerDZ->size() != 0)
                g->SetMaximum(MaximumValue(*ContainerDZ) * scale);
    } else
        g->SetMaximum(z_max);
    
    
    g->SetTitle(config["Generic"]["plot_name"].as<string>().c_str());
        
    if (Initialized == false) {
        //.75,.80,.95,.95
        if (config["Legend"]["add"].as<bool>())
            ROOTC.legend = new TLegend(
                config["Legend"]["placement"]["BL_x"].as<double>(),
                config["Legend"]["placement"]["BL_y"].as<double>(),
                config["Legend"]["placement"]["TR_x"].as<double>(),
                config["Legend"]["placement"]["TR_y"].as<double>());
        
        /// Initializing Canvas
        ROOTC.canvas = new TCanvas("canvas", "Histogram(s)");
        
        if (config["Axes"]["log_x"].as<bool>())
            ROOTC.canvas->SetLogx(1);
        else
            ROOTC.canvas->SetLogx(0);
    
        if (Graph_Type[0] == 'd' || Graph_Type[0] == 'p') {
            // note flipped z with y for consistency
            if (config["Axes"]["log_z"].as<bool>())
                ROOTC.canvas->SetLogy(1);
            else
                ROOTC.canvas->SetLogy(0);
        }
        if (Graph_Type[0] == 'D') {
            if (config["Axes"]["log_y"].as<bool>())
                ROOTC.canvas->SetLogy(1);
            else
                ROOTC.canvas->SetLogy(0);

            if (config["Axes"]["log_z"].as<bool>())
                ROOTC.canvas->SetLogz(1);
            else
                ROOTC.canvas->SetLogz(0);
        }

        // ROOTC.canvas->SetFrameFillColor(kWhite);
        // ROOTC.canvas->SetFillColor(kWhite);
        // ROOTC.style->SetOptStat(kFALSE);
    }
    
    if (config["Legend"]["add"].as<bool>()) {
        const auto &var_name = config["Generic"]["variable_name"].as<string>();
        const auto &mark = config["Legend"]["marker"].as<string>();
        ROOTC.legend->AddEntry(g, var_name.c_str(), mark.c_str());
    }

    Initialized = true;
    return 0;
}

int AGraph::Init(TH1D *&h)
{
    const auto n_x = config["Bins"]["n_x"].as<unsigned int>();
    const auto x_min = config["Range"]["x_min"].as<double>();
    const auto x_max = config["Range"]["x_max"].as<double>();
    const auto &opt = config["Draw_options"]["histogram"].as<string>();
    const auto &dn = config["Generic"]["data_name"].as<string>();
    if (opt == "")
        RootDrawOptions = "E1 HIST";
    else
        RootDrawOptions = opt;
    
    h = new TH1D(dn.c_str(), "", n_x, x_min, x_max);
        
    Init_commons(h);
        
    // extra styling for 1D
    if (config["Curves_and_fill"]["fill"]["color"].as<int>() != -1)
        h->SetFillColor(config["Curves_and_fill"]["fill"]["color"].as<int>());
    if (config["Curves_and_fill"]["fill"]["style"].as<int>() != -1)
        h->SetFillStyle(config["Curves_and_fill"]["fill"]["style"].as<int>());

    return Init_hist_tail(h);
}

int AGraph::Init(TH2D *&h)
{
    const auto n_x = config["Bins"]["n_x"].as<unsigned int>();
    const auto n_y = config["Bins"]["n_y"].as<unsigned int>();
    const auto x_min = config["Range"]["x_min"].as<double>();
    const auto x_max = config["Range"]["x_max"].as<double>();
    const auto y_min = config["Range"]["y_min"].as<double>();
    const auto y_max = config["Range"]["y_max"].as<double>();
    const auto &opt = config["Draw_options"]["histogram_2D"].as<string>();
    const auto &dn = config["Generic"]["data_name"].as<string>();
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
    if (opt == "") {
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
    } else
        RootDrawOptions = opt;
    ROOTC.style->SetPalette(1);
    
    if (!config["Bins"]["custom_binning"].as<bool>())
        h = new TH2D(dn.c_str(), "", n_x, x_min, x_max, n_y, y_min, y_max);
    else
        h = new TH2D(dn.c_str(), "", n_x, Custom_BinsX, n_y, Custom_BinsY);
        
    Init_commons(h);

    return Init_hist_tail(h);
}

int AGraph::Init(TGraph *&g)
{
    const auto &opt = config["Draw_options"]["graph"].as<string>();
    if (opt == "")
        RootDrawOptions = "PL";
    else
        RootDrawOptions = opt;

    g = new TGraph(ContainerD->size(), vct2arrD(*ContainerD),
                   vct2arrD(*ContainerDZ));
        
    Init_commons(g);
    return Init_tail(1, g);
}

int AGraph::Init()
{
    // 	if( Initialized==false )
    // 	{
    
    if (!Init_containers_OK())
        return 1;
    Set_style(config["Formatting"]["style"].as<string>());
    
    if (Graph_Type[0] == 'd')
        return Init(ROOTC.hist_1D);

    if (Graph_Type[0] == 'D')
        return Init(ROOTC.hist_2D);

    if (Graph_Type[0] == 'p')
        return Init(ROOTC.graph);

    // 	}
    return 2;
}

void AGraph::Deinit()
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

#endif
