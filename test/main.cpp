#include "libdedx.h"

#include <iostream>
#include <iomanip>

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

    cout << scientific << setprecision(4);
    for (int i = 0; i < n; ++i) {
        cout << setw(12) << e[i] << ' ';
        cout << setw(12) << Se[0][i] << ' ';
        cout << setw(12) << Se[1][i] << ' ';
        cout << setw(12) << Se[2][i] << endl;
    }

    return 0;
}
