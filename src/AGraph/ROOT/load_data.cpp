#ifndef AGraph__load_data_cpp
#define AGraph__load_data_cpp

#include "AGraph/AGraph.h"
#include <sstream>
#include <fstream>

/*void AGraph::LoadDataSimple(vector<int> input)
{
    ContainerI=input;
}*/

void AGraph::LoadDataSimple(const vector<double> &input)
{
    ContainerD = new vector<double>;

    *ContainerD = input;
}

void AGraph::LoadDataSimple(const vector<double> &input1,
                            const vector<double> &input2)
{
    // Graph_Type[0]='d';	//redundant in AGraph unification
    ContainerD = new vector<double>;
    ContainerDZ = new vector<double>;

    *ContainerD = input1;
    *ContainerDZ = input2;
}

void AGraph::LoadDataSimple(const vector<double> &input1,
                            const vector<double> &input2,
                            const vector<double> &input3)
{
    // Graph_Type[0]='D';	//redundant in AGraph unification
    ContainerD = new vector<double>;
    ContainerDY = new vector<double>;
    ContainerDZ = new vector<double>;

    *ContainerD = input1;
    *ContainerDY = input2;
    *ContainerDZ = input3;
}

int AGraph::LoadDataFromFile(const string &filename)
{
    const auto ncols = config["Generic"]["columns_in_file"].as<int>();
    if (ncols <= 0) {
        cerr << "Wrong number of columns specified in the reading file for "
                "AGraph! Exiting.\n";
        exit(1);
    }
    vector<double> *VectorArrayRead;
    VectorArrayRead = new vector<double>[ncols];

    // 		VectorArrayRead[0].push_back(10);
    // 		cout << "\ntest" << VectorArrayRead[0][0] << endl;

    stringstream decompose; // for extracting formatted data
    fstream data_file;

    double bufferD;
    unsigned long line_number = 0;

    data_file.open(filename); // open data file
    if (data_file.is_open()) {
        string input_line;

        while (!data_file.eof()) {
            getline(data_file, input_line);
            line_number++;
            // Clear error because of bad formating (possible EOL problem)
            decompose.clear();
            decompose.str(input_line);

            for (auto i = 0; i < ncols; ++i) {
                if (decompose.good()) {
                    decompose >> bufferD;
                    if (!decompose.fail())
                        VectorArrayRead[i].push_back(bufferD);
                    if (decompose.fail() && !decompose.eof()) {
                        cout << "WARNING! AGraph::LoadDataFromFile format "
                                "reading error. In the file "
                             << filename << " at line " << line_number
                             << ".\n";
                    }
                }
                if ((!decompose.good() && i < (ncols - 1)) ||
                    decompose.fail()) {
                    unsigned long shortest_vector = 0;
                    for (auto test_size = 0; test_size < ncols; ++test_size) {
                        if (VectorArrayRead[test_size].size() <
                            VectorArrayRead[shortest_vector].size())
                            shortest_vector = test_size;
                    }

                    /// let's pop too long tails
                    for (auto tail_popper_counter = 0;
                         tail_popper_counter < ncols; ++tail_popper_counter) {
                        if (VectorArrayRead[tail_popper_counter].size() >
                            VectorArrayRead[shortest_vector].size()) {
                            VectorArrayRead[tail_popper_counter].pop_back();
                            cout << "Too few valid elements in a line. "
                                    "Dropping a line.\n";
                        }
                    }
                }
            }
        }

        ContainerD = new vector<double>;
        *ContainerD = VectorArrayRead[0];
        if (ncols == 2) {
            bool if_fired = false;
            if (Graph_Type[0] == 'D') {
                ContainerDY = new vector<double>;
                *ContainerDY = VectorArrayRead[1];
                if_fired = true;
            }
            if (Graph_Type[0] == 'd' && Graph_Type.size() == 1) {
                ContainerDZ = new vector<double>;
                *ContainerDZ = VectorArrayRead[1];
                if_fired = true;
            }
            if (Graph_Type[0] == 'd' && Graph_Type.size() > 1 &&
                Graph_Type[1] == 'w') {
                ContainerDw = new vector<double>;
                *ContainerDw = VectorArrayRead[1];
                if_fired = true;
            }
            if (Graph_Type[0] == 'p') {
                ContainerDZ = new vector<double>;
                *ContainerDZ = VectorArrayRead[1];
                if_fired = true;
            }
            if (!if_fired) {
                cout << "File reading error. This special configuration is "
                        "not implemented.\n";
                exit(1);
            }
        }
        if (ncols == 3) {
            bool if_fired = false;
            if (Graph_Type[0] == 'D' && Graph_Type.size() == 1) {
                ContainerDY = new vector<double>;
                *ContainerDY = VectorArrayRead[1];
                ContainerDZ = new vector<double>;
                *ContainerDZ = VectorArrayRead[2];
                if_fired = true;
            }
            if (Graph_Type[0] == 'D' && Graph_Type.size() > 1 &&
                Graph_Type[1] == 'w') {
                ContainerDY = new vector<double>;
                *ContainerDY = VectorArrayRead[1];
                ContainerDw = new vector<double>;
                *ContainerDw = VectorArrayRead[2];
                if_fired = true;
            }
            if (Graph_Type[0] == 'P' && Graph_Type.size() == 1) {
                ContainerDY = new vector<double>;
                *ContainerDY = VectorArrayRead[1];
                ContainerDZ = new vector<double>;
                *ContainerDZ = VectorArrayRead[2];
                if_fired = true;
            }
            if (!if_fired) {
                cerr << "File reading error. This special configuration is "
                        "not implemented.\n";
                exit(1);
            }
        }
        if (ncols > 3) {
            ContainerDY = new vector<double>;
            *ContainerDY = VectorArrayRead[1];
            ContainerDZ = new vector<double>;
            *ContainerDZ = VectorArrayRead[2];
            ContainerDw = new vector<double>;
            *ContainerDw = VectorArrayRead[3];
        }

        data_file.close();
        return 0;
    } else {
        cerr << "Error while opening data file: " << filename << ".\n";
        return 1;
    }

    delete[] VectorArrayRead;
}

#endif
