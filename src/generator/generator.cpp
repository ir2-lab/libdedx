#include <iostream>
#include <fstream>

#include <cmath>
#include <filesystem>

#include "generator.h"

namespace fs = std::filesystem;
using namespace std;

#ifdef WIN32
const char endline[] = "\n";
#else
// in Linux or mac, where SRModule is called using Wine, we must feed an input file containing
// return+linefeed
const char endline[] = "\r\n";
#endif

const char *preample = "// file generated for dedx project - do not edit\n\n";

vector<float> E(N);

int write_erg(const std::vector<float> &);

string data_folder;
string srmodule_folder;
string srmodule_command;
string dpass_folder;
string dpass_command;

const char *usage = R"(
Usage: generator srim96_data_dir srmodule_dir srmodule_cmd dpass_dir dpass_cmd

arguments:
- srim96_data_dir   dir containing SRIM96 files scoef.95a and scoef.95b
- srmodule_dir      directory of SRIM's SR_module.exe
- srmodule_cmd      SR_module.exe command invocation, e.g. "wine SR_module.exe"
- dpass_dir         DPASS directory, contains DPASS.exe
- dpass_cmd         DPASS.exe command invocation, e.g. "wine DPASS.exe"
)";

int main(int argc, char *argv[])
{

    if (argc != 6) {
        cout << usage << endl;
        return -1;
    }
    data_folder = argv[1];
    srmodule_folder = argv[2];
    srmodule_command = argv[3];
    dpass_folder = argv[4];
    dpass_command = argv[5];

    if (readSRIM96(data_folder) != 0) {
        cerr << "readSRIM96 failed" << endl;
        return -1;
    }

    E[0] = 1.0f;
    E[N - 1] = 1e6f;
    for (int i = 1; i < N - 1; ++i) {
        double e = 1.0f * pow(1.0e6, 1.0 * i / (N - 1));
        E[i] = e;
    }

    if (write_erg(E) != 0) {
        cerr << "Failed to create erg.cpp." << endl;
        return -1;
    }

    if (create_files("srim96", E, srim96_Se) != 0) {
        cerr << "Failed to create srim96 data." << endl;
        return -1;
    }

    if (create_files("srim13", E, srim13_Se) != 0) {
        cerr << "Failed to create srim13 data." << endl;
        return -1;
    }

    if (create_files("dpass22", E, dpass22_Se) != 0) {
        cerr << "Failed to create dpass22 data." << endl;
        return -1;
    }

    return 0;
}

int write_erg(const std::vector<float> &E)
{
    ofstream cpp("erg.c");
    if (!cpp.is_open()) {
        cerr << "Could not create file erg.c" << endl;
        return -1;
    }
    cpp << preample;
    cpp << "const float erg[" << N << "] = ";
    print_vector(cpp, E, 0);
    cpp << ";" << endl << endl;
    return 0;
}

int srim13_Se(int Z1, int Z2, const std::vector<float> &E, std::vector<float> &Se)
{
    cout << "srim13 gen, Z1,Z2=" << Z1 << "," << Z2 << endl;

    // store current folder and change to srim srmodule folder
    fs::path work_dir(fs::current_path());
    fs::current_path(srmodule_folder);

    float M1 = mai_weight(Z1);

    // create SR.IN file
    {
        // write SRModule input file
        ofstream os("SR.IN");
        os << endline << endline << "SR_OUTPUT.txt" << endline << endline << Z1 << ' ' << M1
           << endline << endline << " 0 1 0" << endline << endline << "1" << endline << endline
           << Z2 << " \"X\" 1 1" << endline << endline << " 7" << endline << endline << " 0 0 "
           << endline;

        for (const float &erg : E)
            os << erg * M1 << endline;

        os << " 0 ";
    }

    // call srmodule
    if (system(srmodule_command.c_str()) == 0) {
        ifstream is("SR_OUTPUT.txt");
        char buff[4096];
        is.getline(buff, 4096);
        is.getline(buff, 4096);
        is.getline(buff, 4096);
        is.getline(buff, 4096);

        int i = 0;
        while (is.good() && i < E.size()) {
            float d;
            is >> d >> Se[i];
            is.getline(buff, 4096);
            i++;
        }

        if (!is.good()) {
            cerr << "Failed to read SR_OUTPUT.txt" << endl;
            return -1;
        }
    } else {
        cerr << "Call to SR_module failed" << endl;
        return -1;
    }

    // return to workdir
    fs::current_path(work_dir);

    return 0;
}

int dpass22_Se(int Z1, int Z2, const std::vector<float> &E, std::vector<float> &Se)
{
    cout << "dpass22 gen, Z1,Z2=" << Z1 << "," << Z2 << endl;

    // store current folder and change to srim srmodule folder
    fs::path work_dir(fs::current_path());
    fs::current_path(dpass_folder);

    // create DPASS.job file
    {
        // write SRModule input file
        ofstream os("DPASS.job");
        os << "[Main]" << endline;
        os << "Projectile = " << Z1 << endline;
        os << "Target     = " << Z2 << endline;
        os << "[Options]" << endline;
        os << "Header = 0" << endline;
        os << "[Spline]" << endline;
        os << "Raw = 0" << endline;
        os << "Epts = " << N << endline;
    }

    // call srmodule
    if (system(dpass_command.c_str()) == 0) {
        ifstream is("DPASS.out");
        int i = 0;
        while (is.good() && i < E.size()) {
            float d;
            is >> d >> Se[i];
            i++;
        }

        if (!is.good()) {
            cerr << "Failed to read DPASS.out" << endl;
            return -1;
        }
    } else {
        cerr << "Call to DPASS failed" << endl;
        return -1;
    }

    // return to workdir
    fs::current_path(work_dir);

    return 0;
}
