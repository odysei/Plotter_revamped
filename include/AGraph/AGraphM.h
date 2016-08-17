#ifndef AGraph__AGraphM_h
#define AGraph__AGraphM_h

#include <string>
#include "include/AGraph/AGraph.h"
#include "yaml-cpp/yaml.h"

using namespace std;

class AGraphM
{
  public:
    inline void AGraphM_(); // common piece
    AGraphM();
    AGraphM(const string &);
    AGraphM(const AGraph &);
    ~AGraphM();
    
    unsigned int Nr_of_graphs;
    YAML::Node config;

    // from a data file: string, AGraph
    template <class cfg_or_graph> int Add_AGraph(const cfg_or_graph &in);
    int Draw();
    int Print_graphs();
    int Print_graphs_wErrors();
    
    void Load_config(const string &);

    AGraph *operator[](int i);

  private:
    AGraph core;
    AGraph **graphs;
    
    inline void Load_config_(const YAML::Node &config);
    inline int Add_AGraph_0(const AGraph &graph);

    // legacy:
    void Change_variable(const string &, const char);
};

#endif
