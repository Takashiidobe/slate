/* PR c/29154 */

extern void abort(void);

void foo(int **p, int *q) { *(*p++)++ = *q++; }

void bar(int **p, int *q) {
  **p = *q++;
  *(*p++)++;
}

void baz(int **p, int *q) {
  **p = *q++;
  (*p++)++;
}

int main(void) {
  int  i = 42, j = 0;
  int *p = &i;
  foo(&p, &j);
  if (p - 1 != &i || j != 0 || i != 0)
    abort();
  i = 43;
  p = &i;
  bar(&p, &j);
  if (p - 1 != &i || j != 0 || i != 0)
    abort();
  i = 44;
  p = &i;
  baz(&p, &j);
  if (p - 1 != &i || j != 0 || i != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/29154
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: *mut *mut i32, mut q: *mut i32) {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut i32 = q;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut i32 = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:     q = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut *mut i32 = p;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut *mut i32 = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:     p = [[V_3]];
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: *mut i32 = unsafe { *[[V_2]] };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: *mut i32 = unsafe { [[V_4]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_2]] = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_4]] = unsafe { *[[V_0]] };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut p: *mut *mut i32, mut q: *mut i32) {
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: *mut i32 = q;
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: *mut i32 = unsafe { [[V_6]].offset((1 as i32) as isize) };
// CHECK-NEXT:     q = [[V_7]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { *p } = unsafe { *[[V_6]] };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: *mut *mut i32 = p;
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: *mut *mut i32 = unsafe { [[V_8]].offset((1 as i32) as isize) };
// CHECK-NEXT:     p = [[V_9]];
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: *mut i32 = unsafe { *[[V_8]] };
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: *mut i32 = unsafe { [[V_10]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_8]] = [[V_11]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut p: *mut *mut i32, mut q: *mut i32) {
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: *mut i32 = q;
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: *mut i32 = unsafe { [[V_12]].offset((1 as i32) as isize) };
// CHECK-NEXT:     q = [[V_13]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { *p } = unsafe { *[[V_12]] };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: *mut *mut i32 = p;
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: *mut *mut i32 = unsafe { [[V_14]].offset((1 as i32) as isize) };
// CHECK-NEXT:     p = [[V_15]];
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: *mut i32 = unsafe { *[[V_14]] };
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: *mut i32 = unsafe { [[V_16]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_14]] = [[V_17]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 42 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut p: *mut i32 = std::ptr::addr_of_mut!(i);
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(p), std::ptr::addr_of_mut!(j));
// CHECK-NEXT:     if (unsafe { p.offset(-((1 as i32) as isize)) }) != std::ptr::addr_of_mut!(i)
// CHECK-NEXT:         || j != (0 as i32)
// CHECK-NEXT:         || i != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 43 as i32;
// CHECK-NEXT:     p = std::ptr::addr_of_mut!(i);
// CHECK-NEXT:     bar(std::ptr::addr_of_mut!(p), std::ptr::addr_of_mut!(j));
// CHECK-NEXT:     if (unsafe { p.offset(-((1 as i32) as isize)) }) != std::ptr::addr_of_mut!(i)
// CHECK-NEXT:         || j != (0 as i32)
// CHECK-NEXT:         || i != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 44 as i32;
// CHECK-NEXT:     p = std::ptr::addr_of_mut!(i);
// CHECK-NEXT:     baz(std::ptr::addr_of_mut!(p), std::ptr::addr_of_mut!(j));
// CHECK-NEXT:     if (unsafe { p.offset(-((1 as i32) as isize)) }) != std::ptr::addr_of_mut!(i)
// CHECK-NEXT:         || j != (0 as i32)
// CHECK-NEXT:         || i != (0 as i32)
// CHECK-NEXT:     {
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
