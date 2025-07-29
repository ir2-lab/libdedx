#ifndef _LIBDEDX_H_
#define _LIBDEDX_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(_WIN32) || defined(__CYGWIN__))
#  ifdef LIBDEDX_EXPORTS
#    define LIBDEDX_API __declspec(dllexport)
#  else
#    define LIBDEDX_API __declspec(dllimport)
#  endif
#else
#  define LIBDEDX_API
#endif

/**
 * @brief The model used to calculate electronic stopping of ions
 *
 * The implementation of the different models is from Yang et al (1991) NIMB
 *
 *  @ingroup dedx
 */
typedef enum {
    SM_SRIM96 = 0, /**< SRIM version 1996 */
    SM_SRIM13 = 1, /**< SRIM version 2013 */
    SM_DPASS22 = 2 /**< DPASS 2.11 */
} STOPPING_MODEL;

#define ZMAX 92
#define N_DEDX 121

LIBDEDX_API int stopping_xs(STOPPING_MODEL model, int Z1, int Z2, const float **E, const float **Se,
                            int *n);

#ifdef __cplusplus
}
#endif

#endif
