#include <iostream>
#include <fstream>
#include <filesystem>

#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <charconv>

using namespace std;
namespace fs = std::filesystem;

#define ACOLS 16
#define BCOLS 38
#define NCOLS 54
#define NELEM 92

static float SCOEF95[NELEM + 2][NCOLS + 1];

int readSRIM96(const string &srim96_dir)
{
    // Read scoef95A
    {
        ifstream f(fs::path(srim96_dir) / "scoef.95a");
        if (!f.is_open())
            return -1;
        string s;
        getline(f, s); // skip header
        getline(f, s);

        for (int i = 1; i <= NELEM + 1; i++)
            for (int j = 1; j <= ACOLS; j++)
                f >> SCOEF95[i][j];
    }

    // Read scoef95B
    {
        ifstream f(fs::path(srim96_dir) / "scoef.95b");
        if (!f.is_open())
            return -1;
        string s;
        getline(f, s); // skip header
        getline(f, s);

        for (int i = 1; i <= NELEM + 1; i++)
            for (int j = ACOLS + 1; j <= NCOLS; j++)
                f >> SCOEF95[i][j];
    }

    return 0;
}

float mai_weight(int z)
{
    return SCOEF95[z][3];
}
float natural_weight(int z)
{
    return SCOEF95[z][4];
}

double pstop(int z2, double E)
{
    double sp, x, pe0, pe, sl, sh;

    pe0 = 10.0;

    if (E > 1.0e4) {
        x = std::log(E) / E;
        sp = SCOEF95[z2][17] + (SCOEF95[z2][18] * x) + (SCOEF95[z2][19] * x * x)
                + (SCOEF95[z2][20] / x);
    } else {
        pe = std::max(pe0, E);
        sl = SCOEF95[z2][9] * std::pow(pe, 1.0 * SCOEF95[z2][10])
                + SCOEF95[z2][11] * std::pow(pe, 1.0 * SCOEF95[z2][12]);
        sh = SCOEF95[z2][13] / std::pow(pe, 1.0 * SCOEF95[z2][14])
                * std::log(1.0 * SCOEF95[z2][15] / pe + 1.0 * SCOEF95[z2][16] * pe);
        sp = sl * sh / (sl + sh);
        if (E <= pe0) {
            double ppow = z2 < 7 ? 0.35 : 0.45;
            sp *= std::pow(E / pe0, ppow);
        }
    }

    return (sp);
}

double hestop(int z2, double E)
{
    double se, he0, he, a, b, heh, sp;

    he0 = 1.0;

    he = std::max(he0, E);

    b = std::log(he);
    a = 0.2865 + 0.1266 * b - 0.001429 * b * b + 0.02402 * b * b * b - 0.01135 * b * b * b * b
            + 0.001475 * b * b * b * b * b;
    a = std::min(a, 30.0);
    heh = 1.0 - std::exp(-a);

    he = std::max(he, 1.0);
    a = 1.0 + (0.007 + 0.00005 * z2) * std::exp(-std::pow(7.6 - log(he), 2.0));
    heh *= a * a;
    sp = pstop(z2, he);
    se = sp * heh * 4.0;

    if (E <= he0)
        se *= std::sqrt(E / he0);

    return (se);
}

double histop(int z1, int z2, double E)
{
    double yrmin, vrmin, v, vfermi, vr, yr, a, q, lambda0, lambda1, zeta0, zeta;
    double sp, se, hipower, eee, eion, l, vfcorr0, vfcorr1, vmin;
    int j;

    yrmin = 0.13;
    vrmin = 1.0;
    vfermi = SCOEF95[z2][7];

    v = std::sqrt(E / 25.0) / vfermi;
    if (v < 1.0)
        vr = (3.0 * vfermi / 4.0) * (1.0 + (2.0 * v * v / 3.0) - (std::pow(v, 4.0) / 15.0));
    else
        vr = v * vfermi * (1.0 + 1.0 / (5.0 * v * v));

    yr = std::max(vr / std::pow(1.0 * z1, 0.6667), yrmin);
    yr = std::max(yr, vrmin / std::pow(1.0 * z1, 0.6667));

    a = -0.803 * std::pow(yr, 0.3) + 1.3167 * std::pow(yr, 0.6) + 0.38157 * yr + 0.008983 * yr * yr;

    a = std::min(a, 50.0);

    q = 1.0 - std::exp(-a);

    q = std::max(q, 0.0);
    q = std::min(q, 1.0);

    for (j = 22; j <= 39 && q > SCOEF95[93][j]; j++)
        ;
    j--;

    j = std::max(j, 22);
    j = std::min(j, 38);

    lambda0 = SCOEF95[z1][j];
    lambda1 = 1.0 * (q - SCOEF95[93][j]) * (SCOEF95[z1][j + 1] - SCOEF95[z1][j])
            / (SCOEF95[93][j + 1] - SCOEF95[93][j]);
    l = (lambda0 + lambda1) / std::pow(1.0 * z1, 0.33333);
    zeta0 = q
            + (1.0 / (2.0 * std::pow(vfermi, 2.0))) * (1.0 - q)
                    * std::log(1.0 + std::pow(4.0 * l * vfermi / 1.919, 2.0));
    a = std::log(E);
    a = std::max(a, 0.0);

    zeta = zeta0
            * (1.0 + (1.0 / (z1 * z1)) * (0.08 + 0.0015 * z2) * std::exp(-std::pow(7.6 - a, 2.0)));
    a = vrmin / std::pow(1.0 * z1, 0.6667);
    a = std::max(a, yrmin);

    if (yr > a) {
        sp = pstop(z2, E);
        se = sp * pow(zeta * z1, 2.0);
        eion = std::min(E, 9999.0);
        for (j = 41; j <= 53 && eion >= SCOEF95[93][j]; j++)
            ;
        j--;
        j = std::max(j, 41);
        j = std::min(j, 53);

        vfcorr0 = SCOEF95[z2][j];
        vfcorr1 = (eion - SCOEF95[93][j]) * (SCOEF95[z2][j + 1] - SCOEF95[z2][j])
                / (SCOEF95[93][j + 1] - SCOEF95[93][j]);
        se *= (vfcorr0 + vfcorr1);
    } else {
        vrmin = std::max(vrmin, yrmin * std::pow(1.0 * z1, 0.6667));
        a = std::pow(vrmin, 2.0) - 0.8 * std::pow(vfermi, 2.0);
        a = std::max(a, 0.0);

        vmin = 0.5 * (vrmin + std::sqrt(a));
        eee = 25.0 * vmin * vmin;
        sp = pstop(z2, eee);
        eion = std::min(eee, 9999.0);

        for (j = 41; j <= 53 && eion >= SCOEF95[93][j]; j++)
            ;
        j--;

        j = std::max(j, 41);
        j = std::min(j, 53);

        vfcorr0 = SCOEF95[z2][j];
        vfcorr1 = (eion - SCOEF95[93][j]) * (SCOEF95[z2][j + 1] - SCOEF95[z2][j])
                / (SCOEF95[93][j + 1] - SCOEF95[93][j]);
        sp = sp * (vfcorr0 + vfcorr1);

        hipower = 0.47;
        if (z1 == 3)
            hipower = 0.55;
        else if (z2 < 7)
            hipower = 0.375;
        else if (z1 < 18 && (z2 == 14 || z2 == 32))
            hipower = 0.375;
        se = (sp * std::pow(zeta * z1, 2.0)) * std::pow(E / eee, hipower);
    }

    return se;
}

/**
 * @brief Calc electronic stopping XS of Z1 in Z2
 *
 * @param Z1 projectile atomic number
 * @param Z2 target atomic number
 * @param E keV/amu
 * @return double Se in 10^-15 eV-cm^2
 */
double srim96_Se(int Z1, int Z2, double E)
{
    switch (Z1) {
    case 1:
        return pstop(Z2, E);
        break;
    case 2:
        return hestop(Z2, E);
        break;
    default:
        return histop(Z1, Z2, E);
        break;
    }
}

/**
 * @brief Calc electronic stopping XS of Z1 in Z2
 *
 * @param Z1 projectile atomic number
 * @param Z2 target atomic number
 * @param E projectile energy in keV/amu
 * @param Se stopping XS in 10^-15 eV-cm^2
 * @return int 0 if succesfull
 */
int srim96_Se(int Z1, int Z2, const vector<float> &E, vector<float> &Se)
{
    for (int i = 0; i < E.size(); ++i)
        Se[i] = srim96_Se(Z1, Z2, 1.0 * E[i]);
    return 0;
}