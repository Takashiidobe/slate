/* PR c/3444
   This used to fail because bitwise xor was improperly computed in char type
   and sign extended to int type.  */

extern void abort();
extern void exit(int);

signed char c = (signed char)0xffffffff;

int foo(void) { return (unsigned short)c ^ (signed char)0x99999999; }

int main(void) {
  if ((unsigned char)-1 != 0xff || sizeof(short) != 2 || sizeof(int) != 4)
    exit(0);
  if (foo() != (int)0xffff0066)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/3444
// CHECK-NEXT: //! This used to fail because bitwise xor was improperly computed in char type
// CHECK-NEXT: //! and sign extended to int type.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i8 = ((4294967295 as u32) as u8) as i8;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     (((((unsafe { c }) as i16) as u16) as u32) as i32)
// CHECK-NEXT:         ^ ((((2576980377 as u32) as u8) as i8) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((((-(1 as i32) as i8) as u8) as u32) as i32) != (255 as i32)
// CHECK-NEXT:         || (2 as u64) != (((2 as i32) as i64) as u64)
// CHECK-NEXT:         || (4 as u64) != (((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo() != ((4294901862 as u32) as i32) {
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
