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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut fields: complex_fields = complex_fields {
// LOWERING-DAG:         c8: num_complex::Complex { re: 0, im: 0 },
// LOWERING-DAG:         u16: num_complex::Complex { re: 0, im: 0 },
// LOWERING-DAG:         f32: num_complex::Complex { re: 0.0, im: 0.0 },
// LOWERING-DAG:         f64: num_complex::Complex { re: 0.0, im: 0.0 },
// LOWERING-DAG:     };
// LOWERING-DAG:     let mut overlay: complex_union = unsafe { std::mem::zeroed::<complex_union>() };
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: complex_fields = complex_fields {
// LOWERING-DAG:         c8: num_complex::Complex { re: 0, im: 0 },
// LOWERING-DAG:         u16: num_complex::Complex { re: 0, im: 0 },
// LOWERING-DAG:         f32: num_complex::Complex { re: 0.0, im: 0.0 },
// LOWERING-DAG:         f64: num_complex::Complex { re: 0.0, im: 0.0 },
// LOWERING-DAG:     };
// LOWERING-DAG:     fields = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: complex_union = complex_union {
// LOWERING-DAG:         value: num_complex::Complex { re: 0.0, im: 0.0 },
// LOWERING-DAG:     };
// LOWERING-DAG:     overlay = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 2 };
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex {
// LOWERING-DAG:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// LOWERING-DAG:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: i8 = {{_v[0-9]+}}.re as i8;
// LOWERING-DAG:     let {{_v[0-9]+}}: i8 = {{_v[0-9]+}}.im as i8;
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i8> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-DAG:     fields.c8 = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 4 };
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex {
// LOWERING-DAG:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// LOWERING-DAG:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: u16 = {{_v[0-9]+}}.re as u16;
// LOWERING-DAG:     let {{_v[0-9]+}}: u16 = {{_v[0-9]+}}.im as u16;
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<u16> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-DAG:     fields.u16 = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: f32 = 5.0;
// LOWERING-DAG:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:     let {{_v[0-9]+}}: f32 = 6.0;
// LOWERING-DAG:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-DAG:     fields.f32 = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: f64 = 7.0;
// LOWERING-DAG:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:     let {{_v[0-9]+}}: f64 = 8.0;
// LOWERING-DAG:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-DAG:     fields.f64 = {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<f64> = fields.f64;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         overlay.value = {{_v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i8> = fields.c8;
// LOWERING-DAG:     let {{_v[0-9]+}}: i8 = {{_v[0-9]+}}.re;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     } else {
// LOWERING-DAG:         let {{_v[0-9]+}}: num_complex::Complex<i8> = fields.c8;
// LOWERING-DAG:         let {{_v[0-9]+}}: i8 = {{_v[0-9]+}}.im;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     } else {
// LOWERING-DAG:         let {{_v[0-9]+}}: num_complex::Complex<u16> = fields.u16;
// LOWERING-DAG:         let {{_v[0-9]+}}: u16 = {{_v[0-9]+}}.re;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     } else {
// LOWERING-DAG:         let {{_v[0-9]+}}: num_complex::Complex<u16> = fields.u16;
// LOWERING-DAG:         let {{_v[0-9]+}}: u16 = {{_v[0-9]+}}.im;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 4;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     } else {
// LOWERING-DAG:         let {{_v[0-9]+}}: num_complex::Complex<f32> = fields.f32;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.re;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = 5.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     } else {
// LOWERING-DAG:         let {{_v[0-9]+}}: num_complex::Complex<f32> = fields.f32;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.im;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = 6.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     } else {
// LOWERING-DAG:         let {{_v[0-9]+}}: num_complex::Complex<f64> = unsafe { overlay.value };
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 7.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     } else {
// LOWERING-DAG:         let {{_v[0-9]+}}: num_complex::Complex<f64> = unsafe { overlay.value };
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 8.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         {{_v[0-9]+}}
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut fields: complex_fields = complex_fields {
// REWRITES-DAG:         c8: num_complex::Complex { re: 0, im: 0 },
// REWRITES-DAG:         u16: num_complex::Complex { re: 0, im: 0 },
// REWRITES-DAG:         f32: num_complex::Complex { re: 0.0, im: 0.0 },
// REWRITES-DAG:         f64: num_complex::Complex { re: 0.0, im: 0.0 },
// REWRITES-DAG:     };
// REWRITES-DAG:     let mut overlay: complex_union = unsafe { std::mem::zeroed::<complex_union>() };
// REWRITES-DAG:     fields = complex_fields {
// REWRITES-DAG:         c8: num_complex::Complex { re: 0, im: 0 },
// REWRITES-DAG:         u16: num_complex::Complex { re: 0, im: 0 },
// REWRITES-DAG:         f32: num_complex::Complex { re: 0.0, im: 0.0 },
// REWRITES-DAG:         f64: num_complex::Complex { re: 0.0, im: 0.0 },
// REWRITES-DAG:     };
// REWRITES-DAG:     overlay = complex_union {
// REWRITES-DAG:         value: num_complex::Complex { re: 0.0, im: 0.0 },
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 2 };
// REWRITES-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex {
// REWRITES-DAG:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// REWRITES-DAG:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// REWRITES-DAG:     };
// REWRITES-DAG:     fields.c8 = num_complex::Complex {
// REWRITES-DAG:         re: {{_v[0-9]+}}.re as i8,
// REWRITES-DAG:         im: {{_v[0-9]+}}.im as i8,
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 4 };
// REWRITES-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex {
// REWRITES-DAG:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// REWRITES-DAG:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// REWRITES-DAG:     };
// REWRITES-DAG:     fields.u16 = num_complex::Complex {
// REWRITES-DAG:         re: {{_v[0-9]+}}.re as u16,
// REWRITES-DAG:         im: {{_v[0-9]+}}.im as u16,
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     let {{_v[0-9]+}}: f32 = 6.0;
// REWRITES-DAG:     fields.f32 = num_complex::Complex {
// REWRITES-DAG:         re: 5.0 + {{_v[0-9]+}},
// REWRITES-DAG:         im: {{_v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     let {{_v[0-9]+}}: f64 = 8.0;
// REWRITES-DAG:     fields.f64 = num_complex::Complex {
// REWRITES-DAG:         re: 7.0 + {{_v[0-9]+}},
// REWRITES-DAG:         im: {{_v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         overlay.value = fields.f64;
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: num_complex::Complex<i8> = fields.c8;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if ({{_v[0-9]+}}.re as i32) != 1 {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: num_complex::Complex<i8> = fields.c8;
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = ({{_v[0-9]+}}.im as i32) != 2;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: num_complex::Complex<u16> = fields.u16;
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = ({{_v[0-9]+}}.re as i32) != 3;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: num_complex::Complex<u16> = fields.u16;
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = ({{_v[0-9]+}}.im as i32) != 4;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: num_complex::Complex<f32> = fields.f32;
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.re != 5.0;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: num_complex::Complex<f32> = fields.f32;
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.im != 6.0;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: num_complex::Complex<f64> = unsafe { overlay.value };
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.re != 7.0;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: num_complex::Complex<f64> = unsafe { overlay.value };
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.im != 8.0;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
