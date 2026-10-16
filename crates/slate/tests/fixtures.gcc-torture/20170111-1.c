/* PR rtl-optimization/79032 */
/* Reported by Daniel Cederman <cederman@gaisler.com> */

extern void abort(void);

struct S {
  short          a;
  long long      b;
  short          c;
  char           d;
  unsigned short e;
  long          *f;
};

static long foo(struct S *s) __attribute__((noclone, noinline));

static long foo(struct S *s) {
  long a  = 1;
  a      /= s->e;
  s->f[a]--;
  return a;
}

int main(void) {
  long     val = 1;
  struct S s   = {0, 0, 0, 0, 2, &val};
  val          = foo(&s);
  if (val != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/79032
// CHECK-NEXT: //!
// CHECK-NEXT: //! Reported by Daniel Cederman <cederman@gaisler.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i16,
// CHECK-NEXT:     b: i64,
// CHECK-NEXT:     c: i16,
// CHECK-NEXT:     d: i8,
// CHECK-NEXT:     e: u16,
// CHECK-NEXT:     f: *mut i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut s: &S) -> i64 {
// CHECK-NEXT:     let mut a: i64 = (1 as i32) as i64;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i64 = a;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = [[V_0]] / ((((unsafe { (*(s as *const S)).e }) as u32) as i32) as i64);
// CHECK-NEXT:     a = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i64 = unsafe { unsafe { (*(s as *const S)).f }.offset(a as isize) };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i64 = unsafe { *[[V_2]] };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i64 = [[V_3]] - ((1 as i32) as i64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_2]] = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut val: i64 = (1 as i32) as i64;
// CHECK-NEXT:     let mut s: S = S {
// CHECK-NEXT:         a: (0 as i32) as i16,
// CHECK-NEXT:         b: (0 as i32) as i64,
// CHECK-NEXT:         c: (0 as i32) as i16,
// CHECK-NEXT:         d: (0 as i32) as i8,
// CHECK-NEXT:         e: ((2 as i32) as i16) as u16,
// CHECK-NEXT:         f: std::ptr::addr_of_mut!(val),
// CHECK-NEXT:     };
// CHECK-NEXT:     val = foo(unsafe { &(*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     if val != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
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
