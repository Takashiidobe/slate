typedef __complex__ float complex_float;

struct complex_fields {
  _Complex signed char    c8;
  _Complex unsigned short u16;
  complex_float           f32;
  _Complex double         f64;
};

union complex_union {
  _Complex double    value;
  unsigned long long words[2];
};

int main(void) {
  struct complex_fields fields  = {0};
  union complex_union   overlay = {0};

  fields.c8     = 1 + 2i;
  fields.u16    = 3 + 4i;
  fields.f32    = 5.0f + 6.0fi;
  fields.f64    = 7.0 + 8.0i;
  overlay.value = fields.f64;

  int failed = __real__ fields.c8 != 1 || __imag__ fields.c8 != 2 ||
               __real__ fields.u16 != 3 || __imag__ fields.u16 != 4 ||
               __real__ fields.f32 != 5.0f || __imag__ fields.f32 != 6.0f ||
               __real__ overlay.value != 7.0 || __imag__ overlay.value != 8.0;
  return failed;
}
