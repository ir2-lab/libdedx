#include <cmath>
#include <cfloat>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "generator.h"

namespace fs = std::filesystem;
using namespace std;

void indent_line(ostream &cpp, int n)
{
    const char *tabspace = "    ";
    for (int i = 0; i < n; ++i)
        cpp << tabspace;
}

int print_vector(ostream &cpp, const vector<float> &data, int n)
{
    indent_line(cpp, n);
    cpp << "{";

    int oldp = cpp.precision();
    // get number of digits for a float -> text -> float round-trip
    static constexpr auto _n_d_ = std::numeric_limits<float>::max_digits10;
    cpp << std::setprecision(_n_d_);

    const int val_per_line = 5;
    for (int i = 0; i < data.size(); ++i) {
        if (i % val_per_line == 0) {
            cpp << endl;
            indent_line(cpp, n + 1);
        }

        cpp << setw(_n_d_ + 5) << data[i] << 'f';

        if (i < data.size() - 1)
            cpp << ", ";
    }
    cpp << endl;

    indent_line(cpp, n);
    cpp << "}";

    cpp << std::setprecision(oldp);

    return 0;
}

#define NELEM_TEST 92

int create_files(const string &prefix, const vector<float> &E, gen_func F)
{
    const char *preample = "// file generated for dedx project - do not edit\n\n";
    vector<float> Se(E.size());
    char name[128];

    fs::create_directory(fs::path(prefix));

    for (int Z1 = 1; Z1 <= NELEM_TEST; Z1++) {

        sprintf(name, "Z%02d.c", Z1);
        ofstream cpp(fs::path(prefix) / name);
        if (!cpp.is_open()) {
            cerr << "Could not create file " << (fs::path(prefix) / name) << endl;
        }
        cpp << preample;

        sprintf(name, "const float %s_Z%02d_[%d][%d] = { {},", prefix.c_str(), Z1, NELEM_TEST + 1,
                N);
        cpp << name << endl;

        for (int Z2 = 1; Z2 <= NELEM_TEST; Z2++) {

            if (F(Z1, Z2, E, Se) != 0) {
                cerr << "Calculation failed for Z1,Z2=" << Z1 << "," << Z2 << ", " << prefix
                     << endl;
                return -1;
            }

            print_vector(cpp, Se, 1);
            if (Z2 < NELEM_TEST)
                cpp << ",";
            cpp << endl;
        }
        cpp << "}; " << endl << endl;
    }

    { // generate master file all.c
        ofstream cpp(fs::path(prefix) / "all.c");
        if (!cpp.is_open()) {
            cerr << "Could not create file " << (fs::path(prefix) / "all.cpp") << endl;
            return -1;
        }
        cpp << preample;

        for (int Z1 = 1; Z1 <= NELEM_TEST; Z1++) {
            sprintf(name, "extern const float %s_Z%02d_[%d][%d];", prefix.c_str(), Z1,
                    NELEM_TEST + 1, N);
            cpp << name << endl;
        }
        cpp << endl;

        sprintf(name, "const float (*%s_data_[])[%d] = { 0,", prefix.c_str(), N);
        cpp << name << endl;
        for (int Z1 = 1; Z1 <= NELEM_TEST; Z1++) {
            sprintf(name, "%s_Z%02d_", prefix.c_str(), Z1);
            cpp << "    " << name;
            if (Z1 < NELEM_TEST)
                cpp << ",";
            cpp << endl;
        }
        cpp << "};" << endl << endl;
    }

    return 0;
}
