struct X {
  struct Y {
    struct YY {
      struct Z {
        int i;
      } c;
    } bb;
  } b;
} a;
int __attribute__((noinline, noclone)) foo(struct Z *p) {
  int i = p->i;
  a.b   = (struct Y){};
  return p->i + i;
}
extern void abort(void);
int         main() {
  a.b.bb.c.i = 1;
  if (foo(&a.b.bb.c) != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct X {
// CHECK-NEXT:     b: Y,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Y {
// CHECK-NEXT:     bb: YY,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct YY {
// CHECK-NEXT:     c: Z,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Z {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: X = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut p: *mut Z) -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: Y = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = unsafe { (*p).i };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.b = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = {
// CHECK-NEXT:                     let mut [[T_0:__t[0-9]+]]: Y = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                     [[T_0]]
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { (*p).i }) + i
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.b.bb.c.i = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(unsafe { std::ptr::addr_of_mut!(a.b.bb.c) }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
