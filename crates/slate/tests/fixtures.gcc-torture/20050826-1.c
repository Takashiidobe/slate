/* PR rtl-optimization/23561 */

struct A {
  char a1[1];
  char a2[5];
  char a3[1];
  char a4[2048 - 7];
} a;

typedef __SIZE_TYPE__ size_t;
extern void          *memset(void *, int, size_t);
extern void          *memcpy(void *, const void *, size_t);
extern int            memcmp(const void *, const void *, size_t);
extern void           abort(void);

void bar(struct A *x) {
  size_t i;
  if (memcmp(x, "\1HELLO\1", sizeof "\1HELLO\1"))
    abort();
  for (i = 0; i < sizeof(x->a4); i++)
    if (x->a4[i])
      abort();
}

int foo(void) {
  memset(&a, 0, sizeof(a));
  a.a1[0] = 1;
  memcpy(a.a2, "HELLO", sizeof "HELLO");
  a.a3[0] = 1;
  bar(&a);
  return 0;
}

int main(void) {
  foo();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/23561
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn memcpy([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: *const (), [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn memcmp([[V_6:__v[0-9]+]]: *const (), [[V_7:__v[0-9]+]]: *const (), [[V_8:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     a1: [i8; 1],
// CHECK-NEXT:     a2: [i8; 5],
// CHECK-NEXT:     a3: [i8; 1],
// CHECK-NEXT:     a4: [i8; 2041],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: A = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: *mut A) {
// CHECK-NEXT:     let mut i: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             x as *const (),
// CHECK-NEXT:             b"\x01HELLO\x01\0".as_ptr() as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2041 as u64) {
// CHECK-NEXT:         if (unsafe { *unsafe { unsafe { (*x).a4.as_mut_ptr() as *mut i8 }.offset(i as isize) } })
// CHECK-NEXT:             != (0 as i8)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: u64 = [[V_9]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:         i = [[V_10]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(a) }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             2048 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.a1) as *mut i8 }.offset((0 as i32) as isize)
// CHECK-NEXT:         } = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(a.a2) as *mut i8 }) as *mut (),
// CHECK-NEXT:             b"HELLO\0".as_ptr() as *const (),
// CHECK-NEXT:             6 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.a3) as *mut i8 }.offset((0 as i32) as isize)
// CHECK-NEXT:         } = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(unsafe { std::ptr::addr_of_mut!(a) });
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
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
