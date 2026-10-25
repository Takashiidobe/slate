/* { dg-options "/Zc:preprocessor /O2 /Ob2 /Oi /Ot /GS- /Gy /Gw /GF /Z7 /EHsc /GR- /W3 /wd4996 /WX- /Zc:wchar_t /Zc:forScope /Zc:inline /permissive- /MT /Qspectre /TC" } */
void abort(void);

static int square(int value) { return value * value; }

int main(void) {
  int total = 0;
  for (int i = 0; i < 5; i++)
    total += square(i);
  if (total != 30)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor /O2 /Ob2 /Oi /Ot /GS- /Gy /Gw /GF /Z7 /EHsc /GR- /W3 /wd4996 /WX- /Zc:wchar_t /Zc:forScope /Zc:inline /permissive- /MT /Qspectre /TC" }
// CHECK-NEXT: fn square(value: i32) -> i32 {
// CHECK-NEXT:     value * value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0i32..5i32 {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = square(i);
// CHECK-NEXT:         total += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if total != 30 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
