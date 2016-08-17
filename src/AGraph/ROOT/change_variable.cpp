#ifndef AGraph__change_variable_cpp
#define AGraph__change_variable_cpp

#include "AGraph/AGraph.h"
#include "AGraph/extra.h"
#include "helpers/type_conversion.h"

void AGraph::Change_variable(const string &variable, const string &value)
{
    Change_variable((variable + "=" + value), 'O');
}

void AGraph::Change_variable(const string &variable, const double value)
{
    Change_variable((variable + "=" + to_string(value)), 'O');
}

inline bool AGraph::Change_variable_generic(const char priority,
                                            const string &option,
                                            const string &value)
{
    if (option == "Plot Type=") {
        if (priority == 'O' && value != "")
            config["Generic"]["plot_type"] = value;
        if (priority == 'S' &&
            config["Generic"]["plot_type"].as<string>() == "Histogram")
            config["Generic"]["plot_type"] = value;
        
        return true;
    }
    if (option == "Use Weights=" && priority == 'O') {
        if (value == "Y")
            config["Generic"]["use_weights"] = true;
        else
            config["Generic"]["use_weights"] = false;
        
        return true;
    }
    if (option == "Plot Name=") {
        if (priority == 'O' && value != "")
            config["Generic"]["plot_name"] = value;
        if (priority == 'S' &&
            config["Generic"]["plot_name"].as<string>() == "Default plot")
            config["Generic"]["plot_name"] = value;
        
        return true;
    }
    if (option == "Variable Name=") {
        if (priority == 'O' && value != "")
            config["Generic"]["variable_name"] = value;
        if (priority == 'S' &&
            config["Generic"]["variable_name"].as<string>() == "Default variable")
            config["Generic"]["variable_name"] = value;
        
        return true;
    }
    if (option == "Data Name=") {
        if (priority == 'O' && value != "")
            config["Generic"]["data_name"] = value;
        if (priority == 'S' &&
            config["Generic"]["data_name"].as<string>() == "Data")
            config["Generic"]["data_name"] = value;
        
        return true;
    }
    if (option == "Number of data columns in a file=" && value != "") {
        if (priority == 'O' && value != "")
            config["Generic"]["columns_in_file"] = str2int(value);
        if (priority == 'S' &&
            config["Generic"]["columns_in_file"].as<int>() == 2)
            config["Generic"]["columns_in_file"] = str2int(value);
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_output(const char priority,
                                           const string &option,
                                           const string &value)
{
    if (option == "File Name=") {
        if (priority == 'O' && value != "")
            config["Output"]["filename"] = value;
        if (priority == 'S' &&
            config["Output"]["filename"].as<string>() == "Default file")
            config["Output"]["filename"] = value;
        
        return true;
    }
    if (option == "Plot prefix=") {
        if (priority == 'O' && value != "")
            config["Output"]["filename_prefix"] = value;
        if (priority == 'S' &&
            config["Output"]["filename_prefix"].as<string>() == "Histogram")
            config["Output"]["filename_prefix"] = value;
        
        return true;
    }
    if (option == "Graphical output format=") {
        if (priority == 'O' && value != "") {
            config["Output"]["type"] = "." + value;
        }
        if (priority == 'S' &&
            config["Output"]["type"].as<string>() == "") {
            config["Output"]["type"] = "." + value;
        }
        
        return true;
    }
    if (option == "Plot with .root output=") {
        if (priority == 'O' && value != "")
            config["Output"]["add_root_file"] = value;
        if (priority == 'S' &&
            config["Output"]["add_root_file"].as<string>() == "")
            config["Output"]["add_root_file"] = value;
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_axes(const char priority,
                                         const string &option,
                                         const string &value)
{
    if (option == "AxisX=") {
        if (priority == 'O' && value != "") {
            if (value == "empty!" || value == "Empty!")
                config["Axes"]["label_x"] = "";
            else
                config["Axes"]["label_x"] = value;
        }
        if (priority == 'S' &&
            config["Axes"]["label_x"].as<string>() == "X axis") {
            if (value == "empty!" || value == "Empty!")
                config["Axes"]["label_x"] = "";
            else
                config["Axes"]["label_x"] = value;
        }
        
        return true;
    }
    if (option == "AxisY=") {
        if (priority == 'O' && value != "") {
            if (value == "empty!" || value == "Empty!")
                config["Axes"]["label_y"] = "";
            else
                config["Axes"]["label_y"] = value;
        }
        if (priority == 'S' &&
            config["Axes"]["label_y"].as<string>() == "Y axis") {
            if (value == "empty!" || value == "Empty!")
                config["Axes"]["label_y"] = "";
            else
                config["Axes"]["label_y"] = value;
        }
        
        return true;
    }
    if (option == "AxisZ=") {
        if (priority == 'O' && value != "") {
            if (value == "empty!" || value == "Empty!")
                config["Axes"]["label_z"] = "";
            else
                config["Axes"]["label_z"] = value;
        }
        if (priority == 'S' &&
            config["Axes"]["label_z"].as<string>() == "Z axis") {
            if (value == "empty!" || value == "Empty!")
                config["Axes"]["label_z"] = "";
            else
                config["Axes"]["label_z"] = value;
        }
        
        return true;
    }
    if (option == "LogScaleX=" && priority == 'O') {
        if (value == "Y")
            config["Axes"]["log_x"] = true;
        else
            config["Axes"]["log_x"] = false;
        
        return true;
    }
    if (option == "LogScaleY=" && priority == 'O') {
        if (value == "Y")
            config["Axes"]["log_y"] = true;
        else
            config["Axes"]["log_y"] = false;
        
        return true;
    }
    if (option == "LogScaleZ=" && priority == 'O') {
        if (value == "Y")
            config["Axes"]["log_z"] = true;
        else
            config["Axes"]["log_z"] = false;
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_bins(const char priority,
                                         const string &option,
                                         const string &value)
{
    if (option == "Number Of BinsX=") {
        if (priority == 'O' && value != "")
            config["Bins"]["n_x"] = static_cast<unsigned int>(str2int(value));
        if (priority == 'S' && config["Bins"]["n_x"].as<unsigned int>() == 1)
            config["Bins"]["n_x"] = static_cast<unsigned int>(str2int(value));
        
        return true;
    }
    if (option == "Number Of BinsY=") {
        if (priority == 'O' && value != "")
            config["Bins"]["n_y"] = static_cast<unsigned int>(str2int(value));
        if (priority == 'S' && config["Bins"]["n_y"].as<unsigned int>() == 1)
            config["Bins"]["n_y"] = static_cast<unsigned int>(str2int(value));
        
        return true;
    }
    if (option == "Use Custom Binning=" && priority == 'O') {
        if (value == "Y")
            config["Bins"]["custom_binning"] = true;
        
        return true;
    }
    if (option == "Custom BinsX=") {
        if (priority == 'O' && value != "") {
            stringstream ss;

            ss << value;
            double buff;
            vector<double> v;
            while (ss.good()) {
                ss >> buff;
                v.push_back(buff);
            }
            config["Bins"]["custom_x"] = v;
        }
        if (priority == 'S' &&
            config["Bins"]["custom_x"].as<vector<double>>().size() == 0) {
            stringstream ss;

            ss << value;
            double buff;
            vector<double> v;
            while (ss.good()) {
                ss >> buff;
                v.push_back(buff);
            }
            config["Bins"]["custom_x"] = v;
        }
        
        return true;
    }
    if (option == "Custom BinsY=") {
        if (priority == 'O' && value != "") {
            stringstream ss;

            ss << value;
            double buff;
            vector<double> v;
            while (ss.good()) {
                ss >> buff;
                v.push_back(buff);
            }
            config["Bins"]["custom_y"] = v;
        }
        if (priority == 'S' &&
            config["Bins"]["custom_y"].as<vector<double>>().size() == 0) {
            stringstream ss;

            ss << value;
            double buff;
            vector<double> v;
            while (ss.good()) {
                ss >> buff;
                v.push_back(buff);
            }
            config["Bins"]["custom_y"] = v;
        }
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_range(const char priority,
                                          const string &option,
                                          const string &value)
{
    // BLOCK 2
    if (option == "MinimumX=") {
        if (priority == 'O' && value != "")
            config["Range"]["x_min"] = str2double(value);
        if (priority == 'S' && config["Range"]["x_min"].as<double>() == 0)
            config["Range"]["x_min"] = str2double(value);
        
        return true;
    }
    if (option == "MaximumX=") {
        if (priority == 'O' && value != "")
            config["Range"]["x_max"] = str2double(value);
        if (priority == 'S' && config["Range"]["x_max"].as<double>() == 10)
            config["Range"]["x_max"] = str2double(value);
        
        return true;
    }
    if (option == "MinimumY=") {
        if (priority == 'O' && value != "")
            config["Range"]["y_min"] = str2double(value);
        if (priority == 'S' && config["Range"]["y_min"].as<double>() == 0)
            config["Range"]["y_min"] = str2double(value);
        
        return true;
    }
    if (option == "MaximumY=") {
        if (priority == 'O' && value != "")
            config["Range"]["y_max"] = str2double(value);
        if (priority == 'S' && config["Range"]["y_max"].as<double>() == 10)
            config["Range"]["y_max"] = str2double(value);
        
        return true;
    }
    if (option == "MinimumZ=") {
        if (priority == 'O' && value != "")
            config["Range"]["z_min"] = str2double(value);
        if (priority == 'S' && config["Range"]["z_min"].as<double>() == 0)
            config["Range"]["z_min"] = str2double(value);
        
        return true;
    }
    if (option == "MaximumZ=") {
        if (priority == 'O' && value != "")
            config["Range"]["z_max"] = str2double(value);
        if (priority == 'S' && config["Range"]["z_max"].as<double>() == 10)
            config["Range"]["z_max"] = str2double(value);
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_auto(const char priority,
                                         const string &option,
                                         const string &value)
{
    if (option == "Rescaling Factor=") {
        if (priority == 'O' && value != "")
            config["Auto"]["rescale"] = str2double(value);
        if (priority == 'S' && config["Auto"]["rescale"].as<double>() == 1)
            config["Auto"]["rescale"] = str2double(value);
        
        return true;
    }
    if (option == "NormalizationZ=" && priority == 'O') {
        if (value == "Y")
            config["Auto"]["normalize"] = true;
        else
            config["Auto"]["normalize"] = false;
        
        return true;
    }
    if (option == "AutoscaleZMin=" && priority == 'O') {
        if (value == "Y")
            config["Auto"]["scale_z_min"] = true;
        else
            config["Auto"]["scale_z_min"] = false;
        
        return true;
    }
    if (option == "AutoscaleZMax=" && priority == 'O') {
        if (value == "Y")
            config["Auto"]["scale_z_max"] = true;
        else
            config["Auto"]["scale_z_max"] = false;
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_smooth(const char priority,
                                           const string &option,
                                           const string &value)
{
    if (option == "Use smoothing=" && priority == 'O') {
        if (value == "Y")
            config["Smooth"]["use"] = true;
        else
            config["Smooth"]["use"] = false;
        
        return true;
    }
    if (option == "Smooth Nr. Of Times=" && value != "") {
        if (priority == 'O' && value != "")
            config["Smooth"]["times"] = str2int(value);
        if (priority == 'S' &&
            config["Smooth"]["times"].as<int>() == 1)
            config["Smooth"]["times"] = str2int(value);
        
        return true;
    }
    if (option == "Smoothing Algorithm in 2D=") {
        if (priority == 'O' && value != "")
            config["Smooth"]["algorithm_2D"] = value;
        if (priority == 'S' && config["Smooth"]["algorithm_2D"].as<string>() == "k5a")
            config["Smooth"]["algorithm_2D"] = value;
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_floor(const char priority,
                                         const string &option,
                                         const string &value)
{
    if (option == "Add a unitary floor=" && priority == 'O') {
        if (value == "Y")
            config["Floor"]["add"] = true;
        else
            config["Floor"]["add"] = false;
    }
    if (option == "Unitary floor magnitude=" && value != "") {
        if (priority == 'O' && value != "")
            config["Floor"]["value"] = str2double(value);
        if (priority == 'S' && config["Floor"]["value"].as<double>() == 1)
            config["Floor"]["value"] = str2double(value);
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_other(const char priority,
                                         const string &option,
                                         const string &value)
{
    if (option == "PadGridX=" && priority == 'O') {
        if (value == "Y")
            config["Other"]["pad_grid_x"] = true;
        else
            config["Other"]["pad_grid_x"] = false;
        
        return true;
    }
    if (option == "PadGridY=" && priority == 'O') {
        if (value == "Y")
            config["Other"]["pad_grid_y"] = true;
        else
            config["Other"]["pad_grid_y"] = false;
        
        return true;
    }
    if (option == "PadGridZ=" && priority == 'O') {
        if (value == "Y")
            config["Other"]["pad_grid_z"] = true;
        else
            config["Other"]["pad_grid_z"] = false;
        
        return true;
    }
    if (option == "AngleA=") {
        if (priority == 'O' && value != "")
            config["Other"]["angleA_3D"] = value;
        if (priority == 'S' && config["Other"]["angleA_3D"].as<string>() == "0")
            config["Other"]["angleA_3D"] = value;
        
        return true;
    }
    if (option == "AngleB=") {
        if (priority == 'O' && value != "")
            config["Other"]["angleB_3D"] = value;
        if (priority == 'S' && config["Other"]["angleB_3D"].as<string>() == "0")
            config["Other"]["angleB_3D"] = value;
        
        return true;
    }
    if (option == "AngleZ=") {
        if (priority == 'O' && value != "")
            config["Other"]["angleZ_3D"] = value;
        if (priority == 'S' && config["Other"]["angleZ_3D"].as<string>() == "0")
            config["Other"]["angleZ_3D"] = value;
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_draw_options(const char priority,
                                         const string &option,
                                         const string &value)
{
    if (option == "GraphOptions=") {
        if (priority == 'O' && value != "")
            config["Draw_options"]["graph"] = value;
        if (priority == 'S' &&
            config["Draw_options"]["graph"].as<string>() == "lego2")
            config["Draw_options"]["graph"] = value;
        
        return true;
    }
    if (option == "2DGraphOptions=") {
        if (priority == 'O' && value != "")
            config["Draw_options"]["graph_2D"] = value;
        if (priority == 'S' &&
            config["Draw_options"]["graph_2D"].as<string>() == "lego2")
            config["Draw_options"]["graph_2D"] = value;
        
        return true;
    }
    if (option == "HistogramOptions=") {
        if (priority == 'O' && value != "")
            config["Draw_options"]["histogram"] = value;
        if (priority == 'S' &&
            config["Draw_options"]["histogram"].as<string>() == "lego2")
            config["Draw_options"]["histogram"] = value;
        
        return true;
    }
    if (option == "2DHistogramOptions=") {
        if (priority == 'O' && value != "")
            config["Draw_options"]["histogram_2D"] = value;
        if (priority == 'S' &&
            config["Draw_options"]["histogram_2D"].as<string>() == "lego2")
            config["Draw_options"]["histogram_2D"] = value;
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_curves_and_fill(const char priority,
                                         const string &option,
                                         const string &value)
{
    if (option == "Fill color=" && value != "") {
        if (priority == 'O' && value != "")
            config["Curves_and_fill"]["fill"]["color"] = str2int(value);
        if (priority == 'S' &&
            config["Curves_and_fill"]["fill"]["color"].as<int>() == -1)
            config["Curves_and_fill"]["fill"]["color"] = str2int(value);
        
        return true;
    }
    if (option == "Fill style=" && value != "") {
        if (priority == 'O' && value != "")
            config["Curves_and_fill"]["fill"]["style"] = str2int(value);
        if (priority == 'S' &&
            config["Curves_and_fill"]["fill"]["style"].as<int>() == -1)
            config["Curves_and_fill"]["fill"]["style"] = str2int(value);
        
        return true;
    }
    if (option == "Marker size=" && value != "") {
        if (priority == 'O' && value != "")
            config["Curves_and_fill"]["marker"]["size"] = str2double(value);
        if (priority == 'S' &&
            config["Curves_and_fill"]["marker"]["size"].as<double>() == -1)
            config["Curves_and_fill"]["marker"]["size"] = str2double(value);
        
        return true;
    }
    if (option == "Marker style=" && value != "") {
        if (priority == 'O' && value != "")
            config["Curves_and_fill"]["marker"]["style"] = str2int(value);
        if (priority == 'S' &&
            config["Curves_and_fill"]["marker"]["style"].as<int>() == -1)
            config["Curves_and_fill"]["marker"]["style"] = str2int(value);
        
        return true;
    }
    if (option == "Line width=" && value != "") {
        if (priority == 'O' && value != "")
            config["Curves_and_fill"]["line"]["width"] = str2double(value);
        if (priority == 'S' &&
            config["Curves_and_fill"]["line"]["width"].as<double>() == -1)
            config["Curves_and_fill"]["line"]["width"] = str2double(value);
        
        return true;
    }
    if (option == "Line style=" && value != "") {
        if (priority == 'O' && value != "")
            config["Curves_and_fill"]["line"]["style"] = str2int(value);
        if (priority == 'S' &&
            config["Curves_and_fill"]["line"]["style"].as<int>() == -1)
            config["Curves_and_fill"]["line"]["style"] = str2int(value);
        
        return true;
    }
    if (option == "Line and marker color=" && value != "") {
        if (priority == 'O' && value != "")
            config["Curves_and_fill"]["line_marker_color"] = str2int(value);
        if (priority == 'S' &&
            config["Curves_and_fill"]["line_marker_color"].as<int>() == 1)
            config["Curves_and_fill"]["line_marker_color"] = str2int(value);
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_formatting(const char priority,
                                         const string &option,
                                         const string &value)
{
    if (option == "Style=") {
        config["Formatting"]["style"] = value;
        
        return true;
    }
    if (option == "ResolutionX=") {
        if (priority == 'O' && value != "")
            config["Formatting"]["x_resolution"] = str2double(value);
        if (priority == 'S' &&
            config["Formatting"]["x_resolution"].as<double>() == 600)
            config["Formatting"]["x_resolution"] = str2double(value);
        
        return true;
    }
    if (option == "ResolutionY=") {
        if (priority == 'O' && value != "")
            config["Formatting"]["y_resolution"] = str2double(value);
        if (priority == 'S' &&
            config["Formatting"]["y_resolution"].as<double>() == 600)
            config["Formatting"]["y_resolution"] = str2double(value);
        
        return true;
    }
    if (option == "Top Margin=") {
        if (priority == 'O' && value != "")
            config["Formatting"]["margin"]["top"] = str2double(value);
        if (priority == 'S' &&
            config["Formatting"]["margin"]["top"].as<double>() == 0.05)
            config["Formatting"]["margin"]["top"] = str2double(value);
        
        return true;
    }
    if (option == "Bottom Margin=") {
        if (priority == 'O' && value != "")
            config["Formatting"]["margin"]["bottom"] = str2double(value);
        if (priority == 'S' &&
            config["Formatting"]["margin"]["bottom"].as<double>() == 0.13)
            config["Formatting"]["margin"]["bottom"] = str2double(value);
        
        return true;
    };
    if (option == "Left Margin=") {
        if (priority == 'O' && value != "")
            config["Formatting"]["margin"]["left"] = str2double(value);
        if (priority == 'S' &&
            config["Formatting"]["margin"]["left"].as<double>() == 0.16)
            config["Formatting"]["margin"]["left"] = str2double(value);
        
        return true;
    }
    if (option == "Right Margin=") {
        if (priority == 'O' && value != "")
            config["Formatting"]["margin"]["right"] = str2double(value);
        if (priority == 'S' &&
            config["Formatting"]["margin"]["right"].as<double>() == 0.02)
            config["Formatting"]["margin"]["right"] = str2double(value);
        
        return true;
    }
    if (option == "Palette number=") {
        if (priority == 'O' && value != "")
            config["Formatting"]["pallete_nr"] = str2double(value);
        if (priority == 'S' &&
            config["Formatting"]["pallete_nr"].as<double>() == 1)
            config["Formatting"]["pallete_nr"] = str2double(value);
        
        return true;
    }
    if (option == "XaxisTitleOffset=" && value != "") {
        if (priority == 'O' && value != "")
            config["Formatting"]["title_offset"]["x"] = str2double(value);
        if (priority == 'S' &&
            config["Formatting"]["title_offset"]["x"].as<double>() == 1)
            config["Formatting"]["title_offset"]["x"] = str2double(value);
        
        return true;
    }
    if (option == "YaxisTitleOffset=" && value != "") {
        if (priority == 'O' && value != "")
            config["Formatting"]["title_offset"]["y"] = str2double(value);
        if (priority == 'S' &&
            config["Formatting"]["title_offset"]["y"].as<double>() == 1)
            config["Formatting"]["title_offset"]["y"] = str2double(value);
        
        return true;
    }
    if (option == "ZaxisTitleOffset=" && value != "") {
        if (priority == 'O' && value != "")
            config["Formatting"]["title_offset"]["z"] = str2double(value);
        if (priority == 'S' &&
            config["Formatting"]["title_offset"]["z"].as<double>() == 1)
            config["Formatting"]["title_offset"]["z"] = str2double(value);
        
        return true;
    }
    
    return false;
}

inline bool AGraph::Change_variable_legend(const char priority,
                                         const string &option,
                                         const string &value)
{
//     Legend:
//         # Legend may have additional options, at the moment 0 or 1. BL: bottom
//         # left; TR: top right.
//         add: true
//         marker: 'LP'
//         placement:
//             # |
//             # ---
//             BL_x: 0.2
//             BL_y: 0.75
//             # ___
//             #   |
//             TR_x: 0.4
//             TR_y: 0.9
    if (option == "Legend=") {
        if (priority == 'O' && value != "") {
            if (value == "1")
                config["Legend"]["add"] = true;
            else
                config["Legend"]["add"] = false;
        }
        if (priority == 'S' && config["Legend"]["add"].as<bool>()) {
            if (value != "1")
                config["Legend"]["add"] = false;
        }
        
        return true;
    }
    if (option == "Legend marker options=") {
        if (priority == 'O' && value != "")
            config["Legend"]["marker"] = value;
        if (priority == 'S' &&
            config["Legend"]["marker"].as<string>() == "LP")
            config["Legend"]["marker"] = value;
        
        return true;
    }
    if (option == "Legend Corner BL X coord=") {
        if (priority == 'O' && value != "")
            config["Legend"]["placement"]["BL_x"] = str2double(value);
        if (priority == 'S' &&
            config["Legend"]["placement"]["BL_x"].as<double>() == 0.55)
            config["Legend"]["placement"]["BL_x"] = str2double(value);
        
        return true;
    }
    if (option == "Legend Corner BL Y coord=") {
        if (priority == 'O' && value != "")
            config["Legend"]["placement"]["BL_y"] = str2double(value);
        if (priority == 'S' &&
            config["Legend"]["placement"]["BL_y"].as<double>() == 0.6)
            config["Legend"]["placement"]["BL_y"] = str2double(value);
        
        return true;
    }
    if (option == "Legend Corner TR X coord=") {
        if (priority == 'O' && value != "")
            config["Legend"]["placement"]["TR_x"] = str2double(value);
        if (priority == 'S' &&
            config["Legend"]["placement"]["TR_x"].as<double>() == 0.95)
            config["Legend"]["placement"]["TR_x"] = str2double(value);
        
        return true;
    }
    if (option == "Legend Corner TR Y coord=") {
        if (priority == 'O' && value != "")
            config["Legend"]["placement"]["TR_y"] = str2double(value);
        if (priority == 'S' &&
            config["Legend"]["placement"]["TR_x"].as<double>() == 0.75)
            config["Legend"]["placement"]["TR_y"] = str2double(value);
        
        return true;
    }
    
    return false;
}


void AGraph::Change_variable(const string &input, const char priority)
{
    string option = "", value = "";

    ExtractOptionAndValue(input, option, value);
    
    if(Change_variable_generic(priority, option, value))
        return;
    if(Change_variable_output(priority, option, value))
        return;
    if(Change_variable_axes(priority, option, value))
        return;
    if(Change_variable_bins(priority, option, value))
        return;
    if(Change_variable_range(priority, option, value))
        return;
    if(Change_variable_auto(priority, option, value))
        return;
    if(Change_variable_smooth(priority, option, value))
        return;
    if(Change_variable_floor(priority, option, value))
        return;
    if(Change_variable_other(priority, option, value))
        return;
    if(Change_variable_draw_options(priority, option, value))
        return;
    if(Change_variable_curves_and_fill(priority, option, value))
        return;
    if(Change_variable_formatting(priority, option, value))
        return;
    if(Change_variable_legend(priority, option, value))
        return;
}

#endif
