/* { dg-require-effective-target int32plus } */
extern void abort(void);

unsigned int bar(void) { return 32768; }

int main() {
  unsigned int nStyle = bar();
  if (nStyle & 32768)
    nStyle |= 65536;
  if (nStyle != (32768 | 65536))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-require-effective-target int32plus }
// CHECK-NEXT: fn bar() -> u32 {
// CHECK-NEXT:     (32768 as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut nStyle: u32 = bar();
// CHECK-NEXT:     if nStyle & ((32768 as i32) as u32) != (0 as u32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = nStyle;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]] | ((65536 as i32) as u32);
// CHECK-NEXT:         nStyle = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if nStyle != (((32768 as i32) | (65536 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
