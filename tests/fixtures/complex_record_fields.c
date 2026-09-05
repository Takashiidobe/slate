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

// @lowering-fn-begin
// @rewrite-fn-begin
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
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let mut fields: complex_fields = complex_fields {
// COMMON-LOWERING-DAG:         c8: num_complex::Complex { re: 0, im: 0 },
// COMMON-LOWERING-DAG:         u16: num_complex::Complex { re: 0, im: 0 },
// COMMON-LOWERING-DAG:         f32: num_complex::Complex { re: 0.0, im: 0.0 },
// COMMON-LOWERING-DAG:         f64: num_complex::Complex { re: 0.0, im: 0.0 },
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let mut overlay: complex_union = unsafe { std::mem::zeroed::<complex_union>() };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: complex_fields = complex_fields {
// COMMON-LOWERING-DAG:         c8: num_complex::Complex { re: 0, im: 0 },
// COMMON-LOWERING-DAG:         u16: num_complex::Complex { re: 0, im: 0 },
// COMMON-LOWERING-DAG:         f32: num_complex::Complex { re: 0.0, im: 0.0 },
// COMMON-LOWERING-DAG:         f64: num_complex::Complex { re: 0.0, im: 0.0 },
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     fields = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: complex_union = complex_union {
// COMMON-LOWERING-DAG:         value: num_complex::Complex { re: 0.0, im: 0.0 },
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     overlay = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 2 };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex {
// COMMON-LOWERING-DAG:         re: {{__v[0-9]+}}.re + {{__v[0-9]+}}.re,
// COMMON-LOWERING-DAG:         im: {{__v[0-9]+}}.im + {{__v[0-9]+}}.im,
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i8 = {{__v[0-9]+}}.re as i8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i8 = {{__v[0-9]+}}.im as i8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i8> = num_complex::Complex {
// COMMON-LOWERING-DAG:         re: {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         im: {{__v[0-9]+}},
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     fields.c8 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex {
// COMMON-LOWERING-DAG:         re: {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         im: {{__v[0-9]+}},
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 4 };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex {
// COMMON-LOWERING-DAG:         re: {{__v[0-9]+}}.re + {{__v[0-9]+}}.re,
// COMMON-LOWERING-DAG:         im: {{__v[0-9]+}}.im + {{__v[0-9]+}}.im,
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u16 = {{__v[0-9]+}}.re as u16;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u16 = {{__v[0-9]+}}.im as u16;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<u16> = num_complex::Complex {
// COMMON-LOWERING-DAG:         re: {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         im: {{__v[0-9]+}},
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     fields.u16 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 5.0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 6.0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-LOWERING-DAG:         re: {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         im: {{__v[0-9]+}},
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     fields.f32 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 7.0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 8.0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-LOWERING-DAG:         re: {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         im: {{__v[0-9]+}},
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     fields.f64 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = fields.f64;
// COMMON-LOWERING-DAG:     unsafe {
// COMMON-LOWERING-DAG:         overlay.value = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i8> = fields.c8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i8 = {{__v[0-9]+}}.re;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<i8> = fields.c8;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i8 = {{__v[0-9]+}}.im;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<u16> = fields.u16;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: u16 = {{__v[0-9]+}}.re;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<u16> = fields.u16;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: u16 = {{__v[0-9]+}}.im;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = fields.f32;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 5.0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = fields.f32;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 6.0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { overlay.value };
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: f64 = 7.0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { overlay.value };
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: f64 = 8.0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut fields: complex_fields = complex_fields {
// COMMON-REWRITES-DAG:         c8: num_complex::Complex { re: 0, im: 0 },
// COMMON-REWRITES-DAG:         u16: num_complex::Complex { re: 0, im: 0 },
// COMMON-REWRITES-DAG:         f32: num_complex::Complex { re: 0.0, im: 0.0 },
// COMMON-REWRITES-DAG:         f64: num_complex::Complex { re: 0.0, im: 0.0 },
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let mut overlay: complex_union = unsafe { std::mem::zeroed::<complex_union>() };
// COMMON-REWRITES-DAG:     fields = complex_fields {
// COMMON-REWRITES-DAG:         c8: num_complex::Complex { re: 0, im: 0 },
// COMMON-REWRITES-DAG:         u16: num_complex::Complex { re: 0, im: 0 },
// COMMON-REWRITES-DAG:         f32: num_complex::Complex { re: 0.0, im: 0.0 },
// COMMON-REWRITES-DAG:         f64: num_complex::Complex { re: 0.0, im: 0.0 },
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     overlay = complex_union {
// COMMON-REWRITES-DAG:         value: num_complex::Complex { re: 0.0, im: 0.0 },
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 2 };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex {
// COMMON-REWRITES-DAG:         re: {{__v[0-9]+}}.re + {{__v[0-9]+}}.re,
// COMMON-REWRITES-DAG:         im: {{__v[0-9]+}}.im + {{__v[0-9]+}}.im,
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     fields.c8 = num_complex::Complex {
// COMMON-REWRITES-DAG:         re: {{__v[0-9]+}}.re as i8,
// COMMON-REWRITES-DAG:         im: {{__v[0-9]+}}.im as i8,
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex {
// COMMON-REWRITES-DAG:         re: {{__v[0-9]+}},
// COMMON-REWRITES-DAG:         im: {{__v[0-9]+}},
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 4 };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex {
// COMMON-REWRITES-DAG:         re: {{__v[0-9]+}}.re + {{__v[0-9]+}}.re,
// COMMON-REWRITES-DAG:         im: {{__v[0-9]+}}.im + {{__v[0-9]+}}.im,
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     fields.u16 = num_complex::Complex {
// COMMON-REWRITES-DAG:         re: {{__v[0-9]+}}.re as u16,
// COMMON-REWRITES-DAG:         im: {{__v[0-9]+}}.im as u16,
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 6.0;
// COMMON-REWRITES-DAG:     fields.f32 = num_complex::Complex {
// COMMON-REWRITES-DAG:         re: 5.0 + {{__v[0-9]+}},
// COMMON-REWRITES-DAG:         im: {{__v[0-9]+}},
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 8.0;
// COMMON-REWRITES-DAG:     fields.f64 = num_complex::Complex {
// COMMON-REWRITES-DAG:         re: 7.0 + {{__v[0-9]+}},
// COMMON-REWRITES-DAG:         im: {{__v[0-9]+}},
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         overlay.value = fields.f64;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<i8> = fields.c8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ({{__v[0-9]+}}.re as i32) != 1 {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<i8> = fields.c8;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = ({{__v[0-9]+}}.im as i32) != 2;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<u16> = fields.u16;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = ({{__v[0-9]+}}.re as i32) != 3;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<u16> = fields.u16;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = ({{__v[0-9]+}}.im as i32) != 4;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = fields.f32;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.re != 5.0;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = fields.f32;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.im != 6.0;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { overlay.value };
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.re != 7.0;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { overlay.value };
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.im != 8.0;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
