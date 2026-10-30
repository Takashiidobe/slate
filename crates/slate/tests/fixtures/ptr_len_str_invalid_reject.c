#include <stdio.h>
#include <string.h>

static int first_byte(const unsigned char *bytes, int len) {
  (void)strlen((const char *)bytes);
  int value = 0;
  for (int i = 0; i < len; ++i)
    value += bytes[i];
  return value;
}

int main(void) {
  const unsigned char bytes[] = {0xc3, 0xa9, 0};
  int                 value   = first_byte(bytes, 1);
  printf("%d\n", value);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strlen([[ARG_0]]: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: fn first_byte(bytes: *const u8, len: i32) -> i32 {
// CHECK-NEXT:     let __bytes_view = unsafe {
// CHECK-NEXT:         std::slice::from_raw_parts(bytes as *const u8, len as usize)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { strlen(bytes as *const i8) };
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         value += ((unsafe { __bytes_view[i as usize] }) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut bytes: [u8; 3] = [195u8, 169u8, 0];
// CHECK-NEXT:     let value: i32 = first_byte(bytes.as_ptr() as *const u8, 1i32);
// CHECK-NEXT:     println!("{}", value);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
