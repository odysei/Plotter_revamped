#ifndef AGprah__AGraphM_cpp
#define AGprah__AGraphM_cpp

#include "AGraph/AGraphM.h"
#include "AGraph/extra.h"
#include <fstream>

inline void AGraphM::AGraphM_() { Nr_of_graphs = 0; }

AGraphM::AGraphM() { AGraphM_(); }

AGraphM::AGraphM(const string &file) : core(file)
{
    AGraphM_();
    Add_AGraph(file);
    Load_config(file);
}

AGraphM::AGraphM(const AGraph &in_AGraph) : core(in_AGraph)
{
    AGraphM_();

    Add_AGraph(in_AGraph);
}

AGraphM::~AGraphM()
{
    for (unsigned int i = 0; i < Nr_of_graphs; i++)
        delete graphs[i];
    if (Nr_of_graphs != 0)
        delete[] graphs;
}

AGraph *AGraphM::operator[](int i)
{
    if (i == -1)
        return &core;
    if (static_cast<int>(Nr_of_graphs) >= i && i >= 0)
        return graphs[i];
    return nullptr;
}

int AGraphM::Draw()
{
    if (Nr_of_graphs > 0)
        core.Draw(graphs, Nr_of_graphs);
    else
        return 1;

    return 0;
}

int AGraphM::Print_graphs()
{
    int error = 0;

    if (Nr_of_graphs > 0) {
        core.Print_Instead_Of_Draw = true;
        error = core.Draw(graphs, Nr_of_graphs);
        core.Print_Instead_Of_Draw = false;
    } else
        return 1;

    return error;
}

int AGraphM::Print_graphs_wErrors()
{
    int error = 0;

    if (Nr_of_graphs > 0) {
        core.Print_Instead_Of_Draw = true;
        core.Print_wErrors = true;
        error = core.Draw(graphs, Nr_of_graphs);
        core.Print_Instead_Of_Draw = false;
        core.Print_wErrors = false;
    } else
        return 1;

    return error;
}

inline int AGraphM::Add_AGraph_0(const AGraph &graph)
{
    graphs = new AGraph *[1];
    graphs[0] = new AGraph(graph);
    ++Nr_of_graphs;

    return 0;
}

template <class cfg_or_graph> int AGraphM::Add_AGraph(const cfg_or_graph &in)
{
    if (Nr_of_graphs == 0)
        return Add_AGraph_0(core);

    AGraph **tmp;
    tmp = new AGraph *[Nr_of_graphs + 1];
    for (unsigned int i = 0; i < Nr_of_graphs; ++i)
        tmp[i] = graphs[i];
    tmp[Nr_of_graphs] = new AGraph(in);

    delete[] graphs;
    graphs = tmp;
    ++Nr_of_graphs;

    return 0;
}

// int AGraphM::Add_AGraph(const string &path)
// {
//     if (Nr_of_graphs == 0)
//         return Add_AGraph_0(core);
//
//     AGraph **tmp;
//     tmp = new AGraph *[Nr_of_graphs + 1];
//     for (unsigned int i = 0; i < Nr_of_graphs; ++i)
//         tmp[i] = graphs[i];
//     tmp[Nr_of_graphs] = new AGraph(path);
//
//     delete[] graphs;
//     graphs = tmp;
//     ++Nr_of_graphs;
//
//     return 0;
// }
//
// int AGraphM::Add_AGraph(const AGraph &in_graph)
// {
//     if (Nr_of_graphs == 0)
//         return Add_AGraph_0(core);
//
//     AGraph **tmp;
//     tmp = new AGraph *[Nr_of_graphs + 1];
//     for (unsigned int i = 0; i < Nr_of_graphs; ++i)
//         tmp[i] = graphs[i];
//     tmp[Nr_of_graphs] = new AGraph(in_graph);
//
//     delete[] graphs;
//     graphs = tmp;
//     ++Nr_of_graphs;
//
//     return 0;
// }

inline void AGraphM::Load_config_(const YAML::Node &config)
{
    auto &extra = config["Generic"]["extra_graphs"].as<vector<string>>();
    if (extra.size() == 0)
        return;
    for (auto i : extra) {
        if (i.size() > 0)
            Add_AGraph(i);
    }
    return;
}

void AGraphM::Load_config(const string &filename)
{
    if (filename.size() < 1) {
        string msg = "AGraphM::Load_config(...)";
        msg += ": Error opening config file: no filename given.\n";
        cerr << msg;
        return;
    }
    if (filename.compare(filename.size() - 5, 5, ".yaml") == 0) {
        config = YAML::LoadFile(filename);
        Load_config_(config);
        return;
    }

    // ***legacy***
    fstream config_file;
    config_file.open(filename + ".pcfg");

    if (config_file.is_open()) {
        string read_line;
        char priority = 'S';

        getline(config_file, read_line);
        while (!config_file.eof()) {
            if (read_line[0] != '/' && read_line[1] != '/') {
                read_line = ExtractSentence(read_line);
                if (read_line == "\\Main Settings")
                    priority = 'O';
                if (read_line == "\\Manual Override")
                    priority = 'O';
                if (read_line == "\\Default Settings")
                    priority = 'S';
                Change_variable(read_line, priority);
            }
            getline(config_file, read_line);
        }
        config_file.close();
        return;
    } else {
        cerr << "Error opening config file: " << filename + ".pcfg\n";
        return;
    }
}

/*
 * Legacy
 *
 */
void AGraphM::Change_variable(const string &input, const char priority)
{
    string option = "", value = "";

    ExtractOptionAndValue(input, option, value);

    if (option == "Add extra graph=") {
        if (priority == 'O' && value != "")
            config["Generic"]["extra_graphs"].push_back(value);
    }
}

#endif
