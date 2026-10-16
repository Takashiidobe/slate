/* Two of these types will, on current gcc targets, have the same
   mode but have different alias sets.  DOIT tries to get gcse to
   invalidly hoist one of the values out of the loop.  */

void abort(void);
void exit(int);

typedef int       T0;
typedef long      T1;
typedef long long T2;

int doit(int sel, int n, void *p) {
  T0 *const p0 = p;
  T1 *const p1 = p;
  T2 *const p2 = p;

  switch (sel) {
  case 0:
    do
      *p0 += *p0;
    while (--n);
    return *p0 == 0;

  case 1:
    do
      *p1 += *p1;
    while (--n);
    return *p1 == 0;

  case 2:
    do
      *p2 += *p2;
    while (--n);
    return *p2 == 0;

  default:
    abort();
  }
}

int main() {
  T0 v0;
  T1 v1;
  T2 v2;

  v0 = 1;
  doit(0, 5, &v0);
  v1 = 1;
  doit(1, 5, &v1);
  v2 = 1;
  doit(2, 5, &v2);

  if (v0 != 32)
    abort();
  if (v1 != 32)
    abort();
  if (v2 != 32)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Two of these types will, on current gcc targets, have the same
// CHECK-NEXT: //! mode but have different alias sets.  DOIT tries to get gcse to
// CHECK-NEXT: //! invalidly hoist one of the values out of the loop.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn doit(mut sel: i32, mut n: i32, mut p: *mut ()) -> i32 {
// CHECK-NEXT:     match sel {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 let mut [[V_1:__v[0-9]+]]: i32 = unsafe { *(p as *mut i32) };
// CHECK-NEXT:                 let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + unsafe { *(p as *mut i32) };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *(p as *mut i32) = [[V_2]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] - (1 as i32);
// CHECK-NEXT:                 n = [[V_4]];
// CHECK-NEXT:                 if !([[V_4]] != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             return ((unsafe { *(p as *mut i32) }) == (0 as i32)) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: i64 = unsafe { *(p as *mut i64) };
// CHECK-NEXT:                 let mut [[V_6:__v[0-9]+]]: i64 = [[V_5]] + unsafe { *(p as *mut i64) };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *(p as *mut i64) = [[V_6]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_7:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                 let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] - (1 as i32);
// CHECK-NEXT:                 n = [[V_8]];
// CHECK-NEXT:                 if !([[V_8]] != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             return ((unsafe { *(p as *mut i64) }) == ((0 as i32) as i64)) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 let mut [[V_9:__v[0-9]+]]: i64 = unsafe { *(p as *mut i64) };
// CHECK-NEXT:                 let mut [[V_10:__v[0-9]+]]: i64 = [[V_9]] + unsafe { *(p as *mut i64) };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *(p as *mut i64) = [[V_10]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_11:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:                 let mut [[V_12:__v[0-9]+]]: i32 = [[V_11]] - (1 as i32);
// CHECK-NEXT:                 n = [[V_12]];
// CHECK-NEXT:                 if !([[V_12]] != (0 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             return ((unsafe { *(p as *mut i64) }) == ((0 as i32) as i64)) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut v0: i32 = 1 as i32;
// CHECK-NEXT:     let mut v1: i64 = (1 as i32) as i64;
// CHECK-NEXT:     let mut v2: i64 = (1 as i32) as i64;
// CHECK-NEXT:     doit(0 as i32, 5 as i32, std::ptr::addr_of_mut!(v0) as *mut ());
// CHECK-NEXT:     doit(1 as i32, 5 as i32, std::ptr::addr_of_mut!(v1) as *mut ());
// CHECK-NEXT:     doit(2 as i32, 5 as i32, std::ptr::addr_of_mut!(v2) as *mut ());
// CHECK-NEXT:     if v0 != (32 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if v1 != ((32 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if v2 != ((32 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
