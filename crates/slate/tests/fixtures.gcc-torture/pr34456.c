/* { dg-skip-if "requires qsort" { freestanding } }  */

#include <stdlib.h>

int __attribute__((noinline)) debug(void) { return 1; }
int                           errors;

struct s {
  int elt;
  int (*compare)(int);
};

static int compare(const void *x, const void *y) {
  const struct s *s1 = x, *s2 = y;
  int             (*compare1)(int);
  int             elt2;

  compare1 = s1->compare;
  elt2     = s2->elt;
  if (elt2 != 0 && debug() && compare1(s1->elt) != 0)
    errors++;
  return compare1(elt2);
}

int      bad_compare(int x) { return -x; }
struct s array[2] = {{1, bad_compare}, {-1, bad_compare}};

int main(void) {
  qsort(array, 2, sizeof(struct s), compare);
  return errors == 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-skip-if "requires qsort" { freestanding } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn qsort(
// CHECK-NEXT:         __base: *mut (),
// CHECK-NEXT:         __nmemb: u64,
// CHECK-NEXT:         __size: u64,
// CHECK-NEXT:         __compar: Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn debug() -> i32 {
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut errors: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     elt: i32,
// CHECK-NEXT:     compare: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn compare(mut x: *const (), mut y: *const ()) -> i32 {
// CHECK-NEXT:     let mut compare1: Option<unsafe extern "C-unwind" fn(i32) -> i32> =
// CHECK-NEXT:         unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut elt2: i32 = 0 as i32;
// CHECK-NEXT:     compare1 = unsafe { (*(x as *const s)).compare };
// CHECK-NEXT:     elt2 = unsafe { (*(y as *const s)).elt };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if elt2 != (0 as i32) {
// CHECK-NEXT:         [[V_0]] = debug() != (0 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = (unsafe { compare1.unwrap()(unsafe { (*(x as *const s)).elt }) }) != (0 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { errors };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             errors = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { compare1.unwrap()(elt2) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn bad_compare(mut x: i32) -> i32 {
// CHECK-NEXT:     -x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut array: __SlateAlign16<[s; 2]> = __SlateAlign16([
// CHECK-NEXT:     s {
// CHECK-NEXT:         elt: 1 as i32,
// CHECK-NEXT:         compare: Some(bad_compare),
// CHECK-NEXT:     },
// CHECK-NEXT:     s {
// CHECK-NEXT:         elt: -(1 as i32),
// CHECK-NEXT:         compare: Some(bad_compare),
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         qsort(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(array.0) as *mut s }) as *mut (),
// CHECK-NEXT:             ((2 as i32) as i64) as u64,
// CHECK-NEXT:             16 as u64,
// CHECK-NEXT:             Some(compare),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     ((unsafe { errors }) == (0 as i32)) as i32
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
