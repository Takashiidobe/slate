/* { dg-options "/Zc:preprocessor /source-charset:utf-8 /execution-charset:windows-1252" } */
void abort(void);

static void check(int got, int want) {
  if (got != want)
    abort();
}

int main(void) {
  static const char text[] = "é€";
  check(sizeof(text), 3);
  check((unsigned char)text[0], 0xE9);
  check((unsigned char)text[1], 0x80);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor /source-charset:utf-8 /execution-charset:windows-1252" }
// CHECK-NEXT: fn check(got: i32, want: i32) {
// CHECK-NEXT:     if got != want {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     check(3i32, 3i32);
// CHECK-NEXT:     check((((unsafe { text[0] }) as u8) as u32) as i32, 233i32);
// CHECK-NEXT:     check((((unsafe { text[1] }) as u8) as u32) as i32, 128i32);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut text: [i8; 3] = [-23, -128, 0];
// CHECK-EMPTY:
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
