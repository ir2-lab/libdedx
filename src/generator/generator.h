#ifndef GENERATOR_H_
#define GENERATOR_H_

#define NELEM 92
#define N 121

#include <vector>

typedef int (*gen_func)(int, int, const std::vector<float> &, std::vector<float> &);

int print_vector(std::ostream &cpp, const std::vector<float> &data, int indent);
int create_files(const std::string &prefix, const std::vector<float> &E, gen_func F);

int readSRIM96(const std::string &srim96_dir);
float mai_weight(int z);
float natural_weight(int z);

int srim96_Se(int Z1, int Z2, const std::vector<float> &E, std::vector<float> &Se);
int srim13_Se(int Z1, int Z2, const std::vector<float> &E, std::vector<float> &Se);
int dpass22_Se(int Z1, int Z2, const std::vector<float> &E, std::vector<float> &Se);

#endif