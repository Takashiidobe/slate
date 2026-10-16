/* { dg-require-stack-size "0x10000" } */

/* When generating o32 MIPS PIC, main's $gp save slot was out of range
   of a single load instruction.  */
struct big {
  int i[sizeof(int) >= 4 && sizeof(void *) >= 4 ? 0x4000 : 4];
};
struct big gb;
int        foo(struct big b, int x) { return b.i[x]; }
int        main(void) { return foo(gb, 0) + foo(gb, 1); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-stack-size "0x10000" }
// CHECK-NEXT: /// When generating o32 MIPS PIC, main's $gp save slot was out of range
// CHECK-NEXT: /// of a single load instruction.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct big {
// CHECK-NEXT:     i: [i32; 16384],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut gb: big = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut b: big, mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe { *unsafe { (b.i.as_mut_ptr() as *mut i32).offset(x as isize) } }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(unsafe { gb }, 0 as i32) + foo(unsafe { gb }, 1 as i32)
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
