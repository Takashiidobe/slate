/* PR tree-optimization/47538 */

struct S {
  double        a, b, *c;
  unsigned long d;
};

__attribute__((noinline, noclone)) void foo(struct S *x, const struct S *y) {
  const unsigned long n = y->d + 1;
  const double        m = 0.25 * (y->b - y->a);
  x->a                  = y->a;
  x->b                  = y->b;
  if (n == 1) {
    x->c[0] = 0.;
  } else if (n == 2) {
    x->c[1] = m * y->c[0];
    x->c[0] = 2.0 * x->c[1];
  } else {
    double        o = 0.0, p = 1.0;
    unsigned long i;

    for (i = 1; i <= n - 2; i++) {
      x->c[i]  = m * (y->c[i - 1] - y->c[i + 1]) / (double)i;
      o       += p * x->c[i];
      p        = -p;
    }
    x->c[n - 1]  = m * y->c[n - 2] / (n - 1.0);
    o           += p * x->c[n - 1];
    x->c[0]      = 2.0 * o;
  }
}

int main(void) {
  struct S x, y;
  double   c[4] = {10, 20, 30, 40}, d[4], e[4] = {118, 118, 118, 118};

  y.a = 10;
  y.b = 6;
  y.c = c;
  x.c = d;
  y.d = 3;
  __builtin_memcpy(d, e, sizeof d);
  foo(&x, &y);
  if (d[0] != 0 || d[1] != 20 || d[2] != 10 || d[3] != -10)
    __builtin_abort();
  y.d = 2;
  __builtin_memcpy(d, e, sizeof d);
  foo(&x, &y);
  if (d[0] != 60 || d[1] != 20 || d[2] != -10 || d[3] != 118)
    __builtin_abort();
  y.d = 1;
  __builtin_memcpy(d, e, sizeof d);
  foo(&x, &y);
  if (d[0] != -20 || d[1] != -10 || d[2] != 118 || d[3] != 118)
    __builtin_abort();
  y.d = 0;
  __builtin_memcpy(d, e, sizeof d);
  foo(&x, &y);
  if (d[0] != 0 || d[1] != 118 || d[2] != 118 || d[3] != 118)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/47538
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: f64,
// CHECK-NEXT:     b: f64,
// CHECK-NEXT:     c: *mut f64,
// CHECK-NEXT:     d: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: &mut S, mut y: &S) {
// CHECK-NEXT:     let mut n: u64 = unsafe { (*(y as *const S)).d }.wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:     let mut m: f64 =
// CHECK-NEXT:         0.25f64 * ((unsafe { (*(y as *const S)).b }) - unsafe { (*(y as *const S)).a });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(x as *mut S)).a = unsafe { (*(y as *const S)).a };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(x as *mut S)).b = unsafe { (*(y as *const S)).b };
// CHECK-NEXT:     }
// CHECK-NEXT:     if n == (((1 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*(x as *mut S)).c }.offset((0 as i32) as isize) } = 0.0f64;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if n == (((2 as i32) as i64) as u64) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { (*(x as *mut S)).c }.offset((1 as i32) as isize) } = m
// CHECK-NEXT:                     * unsafe {
// CHECK-NEXT:                         *unsafe { unsafe { (*(y as *const S)).c }.offset((0 as i32) as isize) }
// CHECK-NEXT:                     };
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { (*(x as *mut S)).c }.offset((0 as i32) as isize) } = 2.0f64
// CHECK-NEXT:                     * unsafe {
// CHECK-NEXT:                         *unsafe { unsafe { (*(x as *mut S)).c }.offset((1 as i32) as isize) }
// CHECK-NEXT:                     };
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let mut o: f64 = 0.0f64;
// CHECK-NEXT:             let mut p: f64 = 1.0f64;
// CHECK-NEXT:             let mut i: u64 = ((1 as i32) as i64) as u64;
// CHECK-NEXT:             '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= n.wrapping_sub(((2 as i32) as i64) as u64) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { unsafe { (*(x as *mut S)).c }.offset(i as isize) } =
// CHECK-NEXT:                         m * ((unsafe {
// CHECK-NEXT:                             *unsafe {
// CHECK-NEXT:                                 unsafe { (*(y as *const S)).c }
// CHECK-NEXT:                                     .offset(i.wrapping_sub(((1 as i32) as i64) as u64) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }) - unsafe {
// CHECK-NEXT:                             *unsafe {
// CHECK-NEXT:                                 unsafe { (*(y as *const S)).c }
// CHECK-NEXT:                                     .offset(i.wrapping_add(((1 as i32) as i64) as u64) as isize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }) / (i as f64);
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: f64 = o;
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: f64 = [[V_3]]
// CHECK-NEXT:                     + p * unsafe { *unsafe { unsafe { (*(x as *mut S)).c }.offset(i as isize) } };
// CHECK-NEXT:                 o = [[V_4]];
// CHECK-NEXT:                 p = -p;
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:                 let mut [[V_6:__v[0-9]+]]: u64 = [[V_5]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:                 i = [[V_6]];
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { (*(x as *mut S)).c }
// CHECK-NEXT:                         .offset(n.wrapping_sub(((1 as i32) as i64) as u64) as isize)
// CHECK-NEXT:                 } = m * unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { (*(y as *const S)).c }
// CHECK-NEXT:                             .offset(n.wrapping_sub(((2 as i32) as i64) as u64) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 } / ((n as f64) - 1.0f64);
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: f64 = o;
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: f64 = [[V_7]]
// CHECK-NEXT:                 + p * unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { (*(x as *mut S)).c }
// CHECK-NEXT:                             .offset(n.wrapping_sub(((1 as i32) as i64) as u64) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:             o = [[V_8]];
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { (*(x as *mut S)).c }.offset((0 as i32) as isize) } = 2.0f64 * o;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut y: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut c: __SlateAlign16<[f64; 4]> = __SlateAlign16([
// CHECK-NEXT:         (10 as i32) as f64,
// CHECK-NEXT:         (20 as i32) as f64,
// CHECK-NEXT:         (30 as i32) as f64,
// CHECK-NEXT:         (40 as i32) as f64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut d: __SlateAlign16<[f64; 4]> = __SlateAlign16([0 as f64; 4]);
// CHECK-NEXT:     let mut e: __SlateAlign16<[f64; 4]> = __SlateAlign16([
// CHECK-NEXT:         (118 as i32) as f64,
// CHECK-NEXT:         (118 as i32) as f64,
// CHECK-NEXT:         (118 as i32) as f64,
// CHECK-NEXT:         (118 as i32) as f64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     y.a = (10 as i32) as f64;
// CHECK-NEXT:     y.b = (6 as i32) as f64;
// CHECK-NEXT:     y.c = c.0.as_mut_ptr() as *mut f64;
// CHECK-NEXT:     x.c = d.0.as_mut_ptr() as *mut f64;
// CHECK-NEXT:     y.d = ((3 as i32) as i64) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             d.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             e.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             32 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(x)) }, unsafe {
// CHECK-NEXT:         &(*(std::ptr::addr_of_mut!(y) as *const S))
// CHECK-NEXT:     });
// CHECK-NEXT:     if (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((0 as i32) as isize) } })
// CHECK-NEXT:         != ((0 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((1 as i32) as isize) } })
// CHECK-NEXT:             != ((20 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((2 as i32) as isize) } })
// CHECK-NEXT:             != ((10 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((3 as i32) as isize) } })
// CHECK-NEXT:             != (-(10 as i32) as f64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     y.d = ((2 as i32) as i64) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             d.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             e.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             32 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(x)) }, unsafe {
// CHECK-NEXT:         &(*(std::ptr::addr_of_mut!(y) as *const S))
// CHECK-NEXT:     });
// CHECK-NEXT:     if (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((0 as i32) as isize) } })
// CHECK-NEXT:         != ((60 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((1 as i32) as isize) } })
// CHECK-NEXT:             != ((20 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((2 as i32) as isize) } })
// CHECK-NEXT:             != (-(10 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((3 as i32) as isize) } })
// CHECK-NEXT:             != ((118 as i32) as f64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     y.d = ((1 as i32) as i64) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             d.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             e.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             32 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(x)) }, unsafe {
// CHECK-NEXT:         &(*(std::ptr::addr_of_mut!(y) as *const S))
// CHECK-NEXT:     });
// CHECK-NEXT:     if (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((0 as i32) as isize) } })
// CHECK-NEXT:         != (-(20 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((1 as i32) as isize) } })
// CHECK-NEXT:             != (-(10 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((2 as i32) as isize) } })
// CHECK-NEXT:             != ((118 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((3 as i32) as isize) } })
// CHECK-NEXT:             != ((118 as i32) as f64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     y.d = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             d.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             e.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             32 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(x)) }, unsafe {
// CHECK-NEXT:         &(*(std::ptr::addr_of_mut!(y) as *const S))
// CHECK-NEXT:     });
// CHECK-NEXT:     if (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((0 as i32) as isize) } })
// CHECK-NEXT:         != ((0 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((1 as i32) as isize) } })
// CHECK-NEXT:             != ((118 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((2 as i32) as isize) } })
// CHECK-NEXT:             != ((118 as i32) as f64)
// CHECK-NEXT:         || (unsafe { *unsafe { (d.0.as_mut_ptr() as *mut f64).offset((3 as i32) as isize) } })
// CHECK-NEXT:             != ((118 as i32) as f64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
