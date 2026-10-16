/* Make sure that language + abi extensions in passing S interoperate.  */

static long long __attribute__((noinline)) foo(unsigned short s) {
  return (short)s;
}

unsigned short s = 0xFFFF;

int main(void) { return foo(s) + 1 != 0; }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Make sure that language + abi extensions in passing S interoperate.
// CHECK-NEXT: fn foo(mut [[S_0:s_[0-9]+]]: u16) -> i64 {
// CHECK-NEXT:     ([[S_0]] as i16) as i64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: u16 = ((65535 as i32) as i16) as u16;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     (foo(unsafe { s }) + ((1 as i32) as i64) != ((0 as i32) as i64)) as i32
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
