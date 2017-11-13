#include <cstdio>
#include <string>
#include <fstream>  // for reading list file
#include <sstream>  // stringstream

#include "AGraph/AGraphM.h"
#include "helpers/type_conversion.h"
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

// 0 - program name; 1 - destination (data) folder; 2 - automatically generated
// list of files (each name - new line)
int main(int argc, char *argv[])
{
    if (argc == 1) {
        cerr << "Wrong number of arguments passed.\n";
        cerr << "Usage: ./plotter <file with a list>/<config .yaml>\n";
        return 1;
    }
    const string filename(argv[1]);
    const string outpath(argv[2]);
    
    const string file_basename = fs::basename(filename);
    
    if (filename.compare(filename.size() - 5, 5, ".yaml") == 0) {
        const string outf = outpath + file_basename.substr(0, file_basename.size() - 5);
        AGraphM graphs(filename);
        auto &cf = graphs[-1]->config;
        if (cf["Output"]["filename"].as<string>().size() < 1)
            cf["Output"]["filename"] = outf;
        cout << "Plot Name: " << cf["Output"]["filename"].as<string>() << "\n";
        cout.setf(ios_base::scientific);
        cout.precision(8);

//         graphs[0]->Print_Graph_wErrors();
        graphs.Draw();
        return 0;
    }
    
    fstream list_file(filename);
    if (list_file.is_open()) {
        string current_file;
        getline(list_file, current_file);
        while (!list_file.eof()) {
            cout << "Current file: " << current_file << endl;

            AGraphM graphs(current_file);
            graphs[-1]->config["Output"]["filename"] = current_file;
            cout << "Plot Name: " <<
                graphs[-1]->config["Output"]["filename"].as<string>() << "\n";
            cout.setf(ios_base::scientific);
            cout.precision(8);

            graphs[0]->Print_Graph_wErrors();
            graphs.Draw();

            getline(list_file, current_file);
        }
        list_file.close();
    } else {
        cerr << "Error opening list\n";
        return 1;
    }
    
    return 0;
}
