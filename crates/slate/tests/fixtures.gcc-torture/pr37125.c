extern void abort(void);

static inline unsigned int mod_rhs(int rhs) {
  if (rhs == 0)
    return 1;
  return rhs;
}

void func_44(unsigned int p_45);
void func_44(unsigned int p_45) {
  if (!((p_45 * -9) % mod_rhs(-9))) {
    abort();
  }
}

int main(void) {
  func_44(2);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn mod_rhs(mut rhs: i32) -> u32 {
// CHECK-NEXT:     if rhs == (0 as i32) {
// CHECK-NEXT:         return (1 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     rhs as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn func_44(mut p_45: u32) {
// CHECK-NEXT:     if !(p_45.wrapping_mul(-(9 as i32) as u32) % mod_rhs(-(9 as i32)) != (0 as u32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     func_44((2 as i32) as u32);
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
