/* { dg-options "-fpascal-strings" } */
int printf(const char *, ...);

int main(void) {
  const unsigned char *greeting = (const unsigned char *)"\pHello";
  const unsigned char *empty = (const unsigned char *)"\p";
  printf("%d %s %d %zu\n", greeting[0], (const char *)greeting + 1, empty[0], sizeof("\pHello"));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-fpascal-strings" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let greeting: *const u8 = c"\x05Hello".as_ptr() as *const u8;
// CHECK-NEXT:     let empty: *const u8 = b"\0\0".as_ptr() as *const u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %s %d %zu\n".as_ptr(),
// CHECK-NEXT:             *greeting.add(0) as i32,
// CHECK-NEXT:             (greeting as *const i8).add(1),
// CHECK-NEXT:             *empty.add(0) as i32,
// CHECK-NEXT:             core::mem::size_of::<[i8; 7]>() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
