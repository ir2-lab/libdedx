# libdedx

A compilation of different parametrizations for the electronic stopping of ions in matter:
- SRIM version 1996, obtained from published source code
- SRIM version 2013, obtained from the `SR_module.exe` utility included in the SRIM distribution
- DPASS version 21.06, from the `DPASS.exe` utility included in the DPASS distribution

The electronic stopping cross-section $S_e$, in units of $10^{-15}$ eV-cm$^2$, is tabulated over a logarithic energy range from $1$ to $10^6$ keV/u, for $92 \times 92$ ion-target combinations and for all three parametrizations listed above.

The tables are compiled as a plain C shared library (`.so` or `.dll`) to facilitate integration into other software.

## Usage

The library provides a single function to access the data:
```c
int stopping_xs(STOPPING_MODEL model, int Z1, int Z2, const float **E, const float **Se, int *n);
```
where
- `STOPPING_MODEL` is an enumerator which can take up the values `SM_SRIM96`, `SM_SRIM13` and `SM_DPASS22` corresponding to the three stopping parametrizations
- `Z1, Z2` are the projectile and target atomic numbers, respectively
- `E` will receive a pointer to an array of energy values in keV/u
- `Se` will receive a pointer to an array of stopping cross-section values in $10^{-15}$ eV-cm$^2$  
- `*n` will be set to the number of array elements

The function returns 0 on success and -1 on error.
Error conditions are either Z out of range or an invalid model value.

The following code shows how to use the library in a C or C++ program.
```c
#include <libdedx.h>

int model = SM_SRIM13;
const float *E, *Se;
int n;
int Z1 = 1; // H projectile
int Z2 = 26; // Fe target

stopping_xs(model, Z1, Z2, &E, &Se, &n);

for(int i=0; i<n; i++) x = Se[i] ...
```

## Installation

### Build from source

The library can be built and installed with `cmake`.
This is done with the following commands (run from the project directory):

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
ninja  # or make, nmake etc
ninja install
```
On Linux the default install location is `$HOME/.local`, thus `sudo` is not required.
On Windows the files to be installed are copied into the folder `build/libdedx`.
Override the install behavior by setting the option `-DCMAKE_INSTALL_PREFIX="/your/install/location"` when calling `cmake`.

### Binaries

Binary files (installer or zip) are provided for Win64. They can be downloaded from the releases tab. 

## Generating the data

The folder `src/generator` contains source code for building the `generator` executable. 
This can be used to generate automatically the data files for building the `libdedx` library.

The files have already been generated and are included in `src/generated`. 

`generate.sh` is an example script showing how to run the `generator` app.

