#ifndef AGraph__read_config_cpp
#define AGraph__read_config_cpp

#include "AGraph/AGraph.h"
#include "AGraph/extra.h"
#include <fstream>

int AGraph::Read_config() { return Read_config(""); }

int AGraph::Read_config(const string &filename)
{
    if (filename.size() < 1) {
        string msg = "AGraph::Read_config(...)";
        msg += ": Error opening config file: no filename given.\n";
        cerr << msg;
        return 1;
    }
    if (filename.compare(filename.size()-5, 5, ".yaml") == 0) {
        config = YAML::LoadFile(filename);
        return 0;
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
        return 0;
    } else {
        cerr << "Error opening config file: " << filename + ".pcfg\n";
        return 1;
    }
}

#endif
