/* PR tree-optimization/59643 */

#define N 32

__attribute__((noinline, noclone)) void foo(double *a, double *b, double *c,
                                            double d, double e, int n) {
  int i;
  for (i = 1; i < n - 1; i++)
    a[i] = d * (b[i] + c[i] + a[i - 1] + a[i + 1]) + e * a[i];
}

double expected[] = {0.0,          10.0,         44.0,          110.0,
                     232.0,        490.0,        1020.0,        2078.0,
                     4152.0,       8314.0,       16652.0,       33326.0,
                     66664.0,      133354.0,     266748.0,      533534.0,
                     1067064.0,    2134138.0,    4268300.0,     8536622.0,
                     17073256.0,   34146538.0,   68293116.0,    136586270.0,
                     273172536.0,  546345082.0,  1092690188.0,  2185380398.0,
                     4370760808.0, 8741521642.0, 17483043324.0, 6.0};

int main() {
  int    i;
  double a[N], b[N], c[N];
  if (__DBL_MANT_DIG__ <= 35)
    return 0;
  for (i = 0; i < N; i++) {
    a[i] = (i & 3) * 2.0;
    b[i] = (i & 7) - 4;
    c[i] = i & 7;
  }
  foo(a, b, c, 2.0, 3.0, N);
  for (i = 0; i < N; i++)
    if (a[i] != expected[i])
      __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/59643
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: *mut f64, mut b: *mut f64, mut c: *mut f64, mut d: f64, mut e: f64, mut n: i32) {
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < n - (1 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { a.offset(i as isize) } = d
// CHECK-NEXT:                 * ((unsafe { *unsafe { b.offset(i as isize) } })
// CHECK-NEXT:                     + unsafe { *unsafe { c.offset(i as isize) } }
// CHECK-NEXT:                     + unsafe { *unsafe { a.offset((i - (1 as i32)) as isize) } }
// CHECK-NEXT:                     + unsafe { *unsafe { a.offset((i + (1 as i32)) as isize) } })
// CHECK-NEXT:                 + e * unsafe { *unsafe { a.offset(i as isize) } };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut expected: __SlateAlign16<[f64; 32]> = __SlateAlign16([
// CHECK-NEXT:     0.0f64,
// CHECK-NEXT:     10.0f64,
// CHECK-NEXT:     44.0f64,
// CHECK-NEXT:     110.0f64,
// CHECK-NEXT:     232.0f64,
// CHECK-NEXT:     490.0f64,
// CHECK-NEXT:     1020.0f64,
// CHECK-NEXT:     2078.0f64,
// CHECK-NEXT:     4152.0f64,
// CHECK-NEXT:     8314.0f64,
// CHECK-NEXT:     16652.0f64,
// CHECK-NEXT:     33326.0f64,
// CHECK-NEXT:     66664.0f64,
// CHECK-NEXT:     133354.0f64,
// CHECK-NEXT:     266748.0f64,
// CHECK-NEXT:     533534.0f64,
// CHECK-NEXT:     1067064.0f64,
// CHECK-NEXT:     2134138.0f64,
// CHECK-NEXT:     4268300.0f64,
// CHECK-NEXT:     8536622.0f64,
// CHECK-NEXT:     17073256.0f64,
// CHECK-NEXT:     34146538.0f64,
// CHECK-NEXT:     68293116.0f64,
// CHECK-NEXT:     136586270.0f64,
// CHECK-NEXT:     273172536.0f64,
// CHECK-NEXT:     546345082.0f64,
// CHECK-NEXT:     1092690188.0f64,
// CHECK-NEXT:     2185380398.0f64,
// CHECK-NEXT:     4370760808.0f64,
// CHECK-NEXT:     8741521642.0f64,
// CHECK-NEXT:     17483043324.0f64,
// CHECK-NEXT:     6.0f64,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut a: __SlateAlign16<[f64; 32]> = __SlateAlign16([0 as f64; 32]);
// CHECK-NEXT:     let mut b: __SlateAlign16<[f64; 32]> = __SlateAlign16([0 as f64; 32]);
// CHECK-NEXT:     let mut c: __SlateAlign16<[f64; 32]> = __SlateAlign16([0 as f64; 32]);
// CHECK-NEXT:     if (53 as i32) <= (35 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (32 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a.0.as_mut_ptr() as *mut f64).offset(i as isize) } =
// CHECK-NEXT:                 ((i & (3 as i32)) as f64) * 2.0f64;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (b.0.as_mut_ptr() as *mut f64).offset(i as isize) } =
// CHECK-NEXT:                 ((i & (7 as i32)) - (4 as i32)) as f64;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (c.0.as_mut_ptr() as *mut f64).offset(i as isize) } = (i & (7 as i32)) as f64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(
// CHECK-NEXT:         a.0.as_mut_ptr() as *mut f64,
// CHECK-NEXT:         b.0.as_mut_ptr() as *mut f64,
// CHECK-NEXT:         c.0.as_mut_ptr() as *mut f64,
// CHECK-NEXT:         2.0f64,
// CHECK-NEXT:         3.0f64,
// CHECK-NEXT:         32 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (32 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { (a.0.as_mut_ptr() as *mut f64).offset(i as isize) } })
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(expected.0) as *mut f64 }.offset(i as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
