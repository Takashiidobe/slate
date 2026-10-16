/* PR c++/34459 */

extern void  abort(void);
extern void *memset(void *s, int c, __SIZE_TYPE__ n);

struct S {
  char s[25];
};

struct S *p;

void __attribute__((noinline, noclone)) foo(struct S *x, int set) {
  int i;
  for (i = 0; i < sizeof(x->s); ++i)
    if (x->s[i] != 0)
      abort();
    else if (set)
      x->s[i] = set;
  p = x;
}

void __attribute__((noinline, noclone)) test1(void) {
  struct S a;
  memset(&a.s, '\0', sizeof(a.s));
  foo(&a, 0);
  struct S b = a;
  foo(&b, 1);
  b = a;
  b = b;
  foo(&b, 0);
}

void __attribute__((noinline, noclone)) test2(void) {
  struct S a;
  memset(&a.s, '\0', sizeof(a.s));
  foo(&a, 0);
  struct S b = a;
  foo(&b, 1);
  b = a;
  b = *p;
  foo(&b, 0);
}

void __attribute__((noinline, noclone)) test3(void) {
  struct S a;
  memset(&a.s, '\0', sizeof(a.s));
  foo(&a, 0);
  struct S b = a;
  foo(&b, 1);
  *p = a;
  *p = b;
  foo(&b, 0);
}

int main(void) {
  test1();
  test2();
  test3();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c++/34459
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn memset(s: *mut (), c: i32, n: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     s: [i8; 25],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut p: *mut S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *mut S, mut set: i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (25 as u64) {
// CHECK-NEXT:         if ((unsafe { *unsafe { unsafe { (*x).s.as_mut_ptr() as *mut i8 }.offset(i as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if set != (0 as i32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { unsafe { (*x).s.as_mut_ptr() as *mut i8 }.offset(i as isize) } =
// CHECK-NEXT:                         set as i8;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p = x;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1() {
// CHECK-NEXT:     let mut a: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { memset(std::ptr::addr_of_mut!(a.s) as *mut (), 0 as i32, 25 as u64) };
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(a), 0 as i32);
// CHECK-NEXT:     let mut b: S = a;
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(b), 1 as i32);
// CHECK-NEXT:     b = a;
// CHECK-NEXT:     b = b;
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(b), 0 as i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2() {
// CHECK-NEXT:     let mut a: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { memset(std::ptr::addr_of_mut!(a.s) as *mut (), 0 as i32, 25 as u64) };
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(a), 0 as i32);
// CHECK-NEXT:     let mut b: S = a;
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(b), 1 as i32);
// CHECK-NEXT:     b = a;
// CHECK-NEXT:     b = unsafe { *unsafe { p } };
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(b), 0 as i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3() {
// CHECK-NEXT:     let mut a: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { memset(std::ptr::addr_of_mut!(a.s) as *mut (), 0 as i32, 25 as u64) };
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(a), 0 as i32);
// CHECK-NEXT:     let mut b: S = a;
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(b), 1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { p } = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { p } = b;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(b), 0 as i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test1();
// CHECK-NEXT:     test2();
// CHECK-NEXT:     test3();
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
