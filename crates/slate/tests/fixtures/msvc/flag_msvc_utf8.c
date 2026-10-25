/* { dg-options "/Zc:preprocessor /utf-8" } */
void abort(void);

static void check(int got, int want) {
  if (got != want)
    abort();
}

int main(void) {
  static const char narrow[] = "é";
  check(sizeof(narrow), 3);
  check((unsigned char)narrow[0], 0xC3);
  check(sizeof(L"é") / sizeof(L"é"[0]), 2);
  check(L"é"[0], 0xE9);
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
// CHECK-NEXT: // { dg-options "/Zc:preprocessor /utf-8" }
// CHECK-NEXT: fn check(got: i32, want: i32) {
// CHECK-NEXT:     if got != want {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     check(3i32, 3i32);
// CHECK-NEXT:     check((((unsafe { narrow[0] }) as u8) as u32) as i32, 195i32);
// CHECK-NEXT:     check(2i32, 2i32);
// CHECK-NEXT:     check(((unsafe { __slate_str6[0] }) as u32) as i32, 233i32);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut narrow: [i8; 3] = [-61, -87, 0];
// CHECK-EMPTY:
// CHECK-NEXT: static mut __slate_str6: [u16; 2] = [233, 0];
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
