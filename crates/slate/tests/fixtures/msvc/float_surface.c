#include <float.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                              \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(_SW_INVALID == 0x10, "_SW_INVALID");
_Static_assert(_EM_DENORMAL == 0x00080000, "_EM_DENORMAL");
_Static_assert(_MCW_RC == 0x00000300, "_MCW_RC");
_Static_assert(_RC_CHOP == 0x00000300, "_RC_CHOP");
_Static_assert(_MCW_DN == 0x03000000, "_MCW_DN");
_Static_assert(_FPE_MULTIPLE_FAULTS == 0x8e, "_FPE_MULTIPLE_FAULTS");
_Static_assert(_FPCLASS_PINF == 0x0200, "_FPCLASS_PINF");
_Static_assert(_CW_DEFAULT == 0x0008001f, "_CW_DEFAULT");

#ifdef DBL_NORM_MAX
#error "MSVC float.h must not expose C23 DBL_NORM_MAX"
#endif
#ifdef LDBL_SNAN
#error "MSVC float.h must not expose C23 LDBL_SNAN"
#endif

TYPE_IS(&_clearfp, unsigned int (*)(void));
TYPE_IS(&_controlfp, unsigned int (*)(unsigned int, unsigned int));
TYPE_IS(&_set_controlfp, void (*)(unsigned int, unsigned int));
TYPE_IS(&_controlfp_s,
        errno_t (*)(unsigned int *, unsigned int, unsigned int));
TYPE_IS(&_statusfp, unsigned int (*)(void));
TYPE_IS(&_fpreset, void (*)(void));
TYPE_IS(&_control87, unsigned int (*)(unsigned int, unsigned int));
TYPE_IS(&__fpecode, int *(*)(void));
TYPE_IS(&__fpe_flt_rounds, int (*)(void));
TYPE_IS(&_scalb, double (*)(double, long));
TYPE_IS(&_logb, double (*)(double));
TYPE_IS(&_nextafter, double (*)(double, double));
TYPE_IS(&_finite, int (*)(double));
TYPE_IS(&_isnan, int (*)(double));
TYPE_IS(&_fpclass, int (*)(double));
TYPE_IS(&_scalbf, float (*)(float, long));

int main(void) { return 0; }
