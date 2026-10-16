/* PR rtl-optimization/117095 */

short a, b;
int   c, e, z;
long  f, g;
int  *h = &c, *i;
#ifdef __SIZEOF_INT128__
volatile __int128 j;
#else
volatile long long j;
#endif
char k, l;

char foo(char n, char o) { return n + o; }

char bar(char n, char o) { return o == 0 ? n : n / o; }

short baz(short n) { return n - a; }

int main() {
  char *q = &l;
  int **s = &i;
  *s      = &z;
  for (e = 0; e <= 5; e++)
    for (g = 1; g <= 5; g++) {
      k   = foo(9, *i);
      **s = bar(f > 1, (1 && j) ^ k);
    }
  b  = baz(q == &l);
  *h = b;
  if (c != 1)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/117095
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut z: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: *mut i32 = unsafe { std::ptr::addr_of_mut!(c) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut j: i128 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut l: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut n: i8, mut o: i8) -> i8 {
// CHECK-NEXT:     ((n as i32) + (o as i32)) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut n: i8, mut o: i8) -> i8 {
// CHECK-NEXT:     (if (o as i32) == (0 as i32) {
// CHECK-NEXT:         n as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (n as i32) / (o as i32)
// CHECK-NEXT:     }) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut n: i16) -> i16 {
// CHECK-NEXT:     ((n as i32) - ((unsafe { a }) as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut q: *mut i8 = unsafe { std::ptr::addr_of_mut!(l) };
// CHECK-NEXT:     let mut s: *mut *mut i32 = unsafe { std::ptr::addr_of_mut!(i) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *s = unsafe { std::ptr::addr_of_mut!(z) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { e }) <= (5 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             g = (1 as i32) as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { g }) <= ((5 as i32) as i64) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 k = foo((9 as i32) as i8, (unsafe { *unsafe { i } }) as i8);
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { *s } = bar(
// CHECK-NEXT:                     ((unsafe { f }) > ((1 as i32) as i64)) as i8,
// CHECK-NEXT:                     ((((1 as i32) != (0 as i32)
// CHECK-NEXT:                         && (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(j)) })
// CHECK-NEXT:                             != (0 as i128)) as i32)
// CHECK-NEXT:                         ^ ((unsafe { k }) as i32)) as i8,
// CHECK-NEXT:                 ) as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i64 = unsafe { g };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i64 = [[V_0]] + ((1 as i32) as i64);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 g = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { e };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = baz((q == unsafe { std::ptr::addr_of_mut!(l) }) as i16);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { h } = (unsafe { b }) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { c }) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
