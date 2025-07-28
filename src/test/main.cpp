#include "libdedx.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cout << "Usage: testdedx Z1 Z2" << endl;
        cout << "1 <= Z1,Z2 <= 92" << endl;
        return -1;
    }

    int Z1 = atoi(argv[1]);
    int Z2 = atoi(argv[2]);

    const float *e, *Se[3];
    int n;
    stopping_xs(SM_SRIM96, Z1, Z2, &e, &Se[0], &n);
    stopping_xs(SM_SRIM13, Z1, Z2, &e, &Se[1], &n);
    stopping_xs(SM_DPASS22, Z1, Z2, &e, &Se[2], &n);

    for (int i = 0; i < n; ++i) {
        cout << e[i] << '\t';
        cout << Se[0][i] << '\t';
        cout << Se[1][i] << '\t';
        cout << Se[2][i] << endl;
    }

    return 0;
}
