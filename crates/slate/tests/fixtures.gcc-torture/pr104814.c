/* PR rtl-optimization/104814 */

short       a = 0;
static long b = 0;
int         c = 7;
char        d = 0;
short      *e = &a;
long        f = 0;

unsigned long foo(unsigned long h, long j) { return j == 0 ? h : h / j; }

int main() {
  long k = f;
  for (; c; --c) {
    for (int i = 0; i < 7; ++i)
      ;
    long m = foo(f, --b);
    d      = ((char)m | *e) <= 43165;
  }
  if (b != -7)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/104814
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i16 = (0 as i32) as i16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i64 = (0 as i32) as i64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = 7 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i8 = (0 as i32) as i8;
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: *mut i16 = unsafe { std::ptr::addr_of_mut!(a) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i64 = (0 as i32) as i64;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut h: u64, mut j: i64) -> u64 {
// CHECK-NEXT:     if j == ((0 as i32) as i64) {
// CHECK-NEXT:         h
// CHECK-NEXT:     } else {
// CHECK-NEXT:         h / (j as u64)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { c }) != (0 as i32) {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (7 as i32) {
// CHECK-NEXT:             {}
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             i = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut m: i64 = 0 as i64;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i64 = unsafe { b };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i64 = [[V_2]] - ((1 as i32) as i64);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         m = foo((unsafe { f }) as u64, [[V_3]]) as i64;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = (((m as i8) as i32) | ((unsafe { *unsafe { e } }) as i32) <= (43165 as i32)) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { b }) != (-(7 as i32) as i64) {
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
