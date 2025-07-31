// function __dedx__
//
// Call libdedx from OCTAVE

#include <octave/oct.h>

#include <iostream>

#include <libdedx.h>

using namespace std;

DEFUN_DLD(__dedx__, args, nargout, "Returns raw tables of electronic energy loss")
{
    int Z1 = args(0).int_value();
    int Z2 = args(1).int_value();
    int model = args(2).int_value();

    const float *E, *Se;
    int ndedx;

    int ret = stopping_xs((STOPPING_MODEL)model, Z1, Z2, &E, &Se, &ndedx);
    if (ret != 0) {
        RowVector e, se;
        octave_value_list retval(2);
        retval(0) = e;
        retval(1) = se;
        return retval;
    }

    octave_idx_type n = ndedx;
    RowVector e(n), se(n);
    for (octave_idx_type k = 0; k < n; k++) {
        e(k) = E[k];
        se(k) = Se[k];
    }

    octave_value_list retval(2);
    retval(0) = e;
    retval(1) = se;
    return retval;
}
