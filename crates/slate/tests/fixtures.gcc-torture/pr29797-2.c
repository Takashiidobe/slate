extern void abort(void);

unsigned long bar(void) { return 32768; }

int main() {
  unsigned long nStyle = bar();
  if (nStyle & 32768)
    nStyle |= 65536;
  if (nStyle != (32768 | 65536))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn bar() -> u64 {
// CHECK-NEXT:     ((32768 as i32) as i64) as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut nStyle: u64 = bar();
// CHECK-NEXT:     if nStyle & (((32768 as i32) as i64) as u64) != (0 as u64) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u64 = nStyle;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]] | (((65536 as i32) as i64) as u64);
// CHECK-NEXT:         nStyle = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if nStyle != ((((32768 as i32) | (65536 as i32)) as i64) as u64) {
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
