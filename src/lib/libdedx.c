#include "libdedx.h"

extern const float (*srim96_data_[])[N_DEDX];
extern const float (*srim13_data_[])[N_DEDX];
extern const float (*dpass22_data_[])[N_DEDX];
extern const float erg[N_DEDX];

LIBDEDX_API int stopping_xs(STOPPING_MODEL model, int Z1, int Z2, const float **E, const float **Se,
                            int *n)
{
    if (!(Z1 >= 1 && Z1 <= ZMAX && Z2 >= 1 && Z2 <= ZMAX))
        return -1;

    switch (model) {
    case SM_SRIM96:
        *Se = (const float *)(&(srim13_data_[Z1][Z2]));
        break;
    case SM_SRIM13:
        *Se = (const float *)(&(srim13_data_[Z1][Z2]));
        break;
    case SM_DPASS22:
        *Se = (const float *)(&(dpass22_data_[Z1][Z2]));
        break;
    default:
        return -1;
    }

    *E = erg;
    *n = N_DEDX;

    return 0;
}
