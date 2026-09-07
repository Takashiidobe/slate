/* Test C2Y complex increment and decrement.  */
/* { dg-do run } */
/* { dg-options "-std=c2y -pedantic-errors" } */

extern void abort(void);
extern void exit(int);

_Complex float       a, ax;
_Complex double      b, bx;
_Complex long double c, cx;

int
// @lowering-fn-begin
// @rewrite-fn-begin
main() {
  ax = a++;
  if (ax != 0 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = a++;
  if (ax != 0 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = 0;
  ax = ++a;
  if (ax != 1 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = ++a;
  if (ax != 1 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = 0;
  ax = a--;
  if (ax != 0 || a != -1 || __builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = a--;
  if (ax != 0 || a != -1 || __builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = 0;
  ax = --a;
  if (ax != -1 || a != -1 || !__builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = --a;
  if (ax != -1 || a != -1 || !__builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();

  bx = b++;
  if (bx != 0 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0, -0.0);
  bx = b++;
  if (bx != 0 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = 0;
  bx = ++b;
  if (bx != 1 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0, -0.0);
  bx = ++b;
  if (bx != 1 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = 0;
  bx = b--;
  if (bx != 0 || b != -1 || __builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0f, -0.0f);
  bx = b--;
  if (bx != 0 || b != -1 || __builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = 0;
  bx = --b;
  if (bx != -1 || b != -1 || !__builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0f, -0.0f);
  bx = --b;
  if (bx != -1 || b != -1 || !__builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();

  cx = c++;
  if (cx != 0 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = c++;
  if (cx != 0 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = 0;
  cx = ++c;
  if (cx != 1 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = ++c;
  if (cx != 1 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = 0;
  cx = c--;
  if (cx != 0 || c != -1 || __builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = c--;
  if (cx != 0 || c != -1 || __builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = 0;
  cx = --c;
  if (cx != -1 || c != -1 || !__builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = --c;
  if (cx != -1 || c != -1 || !__builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();

  exit(0);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         ax = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: -0.0 };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         ax = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         ax = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: -0.0 };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         ax = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         ax = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: -0.0 };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         ax = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         ax = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: -0.0 };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         ax = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         bx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: -0.0 };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         bx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         bx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: -0.0 };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         bx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         bx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: -0.0 };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re as f64;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im as f64;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         bx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         bx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: -0.0 };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re as f64;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im as f64;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         bx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = 0.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         cx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-DAG:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-DAG:         im: -0.000000e+00f128,
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         cx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         cx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-DAG:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-DAG:         im: -0.000000e+00f128,
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         cx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         cx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-DAG:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-DAG:         im: -0.000000e+00f128,
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         cx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         cx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-DAG:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-DAG:         im: -0.000000e+00f128,
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         cx = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:             re: {{__v[0-9]+}},
// LOWERING-DAG:             im: {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}},
// LOWERING-DAG:                 im: {{__v[0-9]+}},
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}}.re + 1.0,
// REWRITES-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         ax = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (0 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { ax })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = (1 as i32) as f32;
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { a })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex { re: 0.0, im: -0.0 };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}}.re + 1.0,
// REWRITES-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         ax = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (0 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { ax })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = (1 as i32) as f32;
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { a })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (0 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// REWRITES-DAG:         re: {{__v[0-9]+}}.re + 1.0,
// REWRITES-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         ax = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (1 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { ax })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = (1 as i32) as f32;
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { a })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex { re: 0.0, im: -0.0 };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// REWRITES-DAG:         re: {{__v[0-9]+}}.re + 1.0,
// REWRITES-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         ax = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (1 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { ax })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = (1 as i32) as f32;
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { a })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (0 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}}.re - 1.0,
// REWRITES-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         ax = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (0 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { ax })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = (-1 as i32) as f32;
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { a })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex { re: 0.0, im: -0.0 };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}}.re - 1.0,
// REWRITES-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         ax = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (0 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { ax })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = (-1 as i32) as f32;
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { a })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (0 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// REWRITES-DAG:         re: {{__v[0-9]+}}.re - 1.0,
// REWRITES-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         ax = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (-1 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { ax })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = (-1 as i32) as f32;
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { a })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = num_complex::Complex { re: 0.0, im: -0.0 };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// REWRITES-DAG:         re: {{__v[0-9]+}}.re - 1.0,
// REWRITES-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         a = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         ax = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = (-1 as i32) as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { ax })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = (-1 as i32) as f32;
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { a })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { ax };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { a };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}}.re + 1.0,
// REWRITES-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         bx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (0 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { bx })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = (1 as i32) as f64;
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { b })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex { re: 0.0, im: -0.0 };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}}.re + 1.0,
// REWRITES-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         bx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (0 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { bx })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = (1 as i32) as f64;
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { b })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (0 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// REWRITES-DAG:         re: {{__v[0-9]+}}.re + 1.0,
// REWRITES-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         bx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (1 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { bx })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = (1 as i32) as f64;
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { b })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex { re: 0.0, im: -0.0 };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// REWRITES-DAG:         re: {{__v[0-9]+}}.re + 1.0,
// REWRITES-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         bx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (1 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { bx })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = (1 as i32) as f64;
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { b })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (0 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}}.re - 1.0,
// REWRITES-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         bx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (0 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { bx })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = (-1 as i32) as f64;
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { b })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: -0.0 };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}}.re as f64,
// REWRITES-DAG:             im: {{__v[0-9]+}}.im as f64,
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}}.re - 1.0,
// REWRITES-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         bx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (0 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { bx })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = (-1 as i32) as f64;
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { b })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (0 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// REWRITES-DAG:         re: {{__v[0-9]+}}.re - 1.0,
// REWRITES-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         bx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (-1 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { bx })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = (-1 as i32) as f64;
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { b })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: -0.0 };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}}.re as f64,
// REWRITES-DAG:             im: {{__v[0-9]+}}.im as f64,
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// REWRITES-DAG:         re: {{__v[0-9]+}}.re - 1.0,
// REWRITES-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         bx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = (-1 as i32) as f64;
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if ((unsafe { bx })
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = (-1 as i32) as f64;
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = 0.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { b })
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { bx };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = unsafe { b };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:             re: {{__v[0-9]+}}.re + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-AARCH64-GNU-DAG:             re: {{__v[0-9]+}} + {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         cx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(0 as i32);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (0 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = if ({{__v[0-9]+}}
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(1 as i32);
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = (1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:         im: -0.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:     };
// REWRITES-AARCH64-GNU-DAG:     unsafe {
// REWRITES-AARCH64-GNU-DAG:         c = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-DAG:     }
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:             re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-X86_64-GNU-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:     unsafe {
// REWRITES-X86_64-GNU-DAG:         c = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:             re: {{__v[0-9]+}}.re + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-AARCH64-GNU-DAG:             re: {{__v[0-9]+}} + {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         cx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(0 as i32);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (0 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = if ({{__v[0-9]+}}
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(1 as i32);
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = (1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(0 as i32);
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (0 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.0f128;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:         re: {{__v[0-9]+}}.re + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:         re: {{__v[0-9]+}} + {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         im: {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         cx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(1 as i32);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = if ({{__v[0-9]+}}
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(1 as i32);
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = (1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:         im: -0.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-X86_64-GNU-DAG:         c = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:             re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-AARCH64-GNU-DAG:         c = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:         re: {{__v[0-9]+}}.re + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:         re: {{__v[0-9]+}} + {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         im: {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         cx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(1 as i32);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = if ({{__v[0-9]+}}
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(1 as i32);
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = (1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(0 as i32);
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (0 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.0f128;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:             re: {{__v[0-9]+}}.re - LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-AARCH64-GNU-DAG:             re: {{__v[0-9]+}} - {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         cx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(0 as i32);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (0 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = if ({{__v[0-9]+}}
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(-1 as i32);
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = (-1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:         im: -0.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:     };
// REWRITES-AARCH64-GNU-DAG:     unsafe {
// REWRITES-AARCH64-GNU-DAG:         c = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-DAG:     }
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:             re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-X86_64-GNU-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:     unsafe {
// REWRITES-X86_64-GNU-DAG:         c = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:             re: {{__v[0-9]+}}.re - LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-DAG:             im: {{__v[0-9]+}}.im,
// REWRITES-AARCH64-GNU-DAG:             re: {{__v[0-9]+}} - {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         cx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(0 as i32);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (0 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = if ({{__v[0-9]+}}
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(-1 as i32);
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = (-1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(0 as i32);
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (0 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.0f128;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-DAG:         };
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:         re: {{__v[0-9]+}}.re - LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:         re: {{__v[0-9]+}} - {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         im: {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         cx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(-1 as i32);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (-1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = if ({{__v[0-9]+}}
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(-1 as i32);
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = (-1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:         im: -0.000000e+00f128,
// REWRITES-AARCH64-GNU-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-X86_64-GNU-DAG:         c = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:             re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-AARCH64-GNU-DAG:         c = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:         re: {{__v[0-9]+}}.re - LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-DAG:         im: {{__v[0-9]+}}.im,
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:         re: {{__v[0-9]+}} - {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         im: {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         cx = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(-1 as i32);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = (-1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = if ({{__v[0-9]+}}
// REWRITES-DAG:         != num_complex::Complex {
// REWRITES-DAG:             re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:             im: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-DAG:         };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-DAG:             im: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:         }) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(-1 as i32);
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = (-1 as i32) as f128;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// REWRITES-DAG:             != num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0; 10]),
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { cx };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cx };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { c };
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_signbit({{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { c };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
