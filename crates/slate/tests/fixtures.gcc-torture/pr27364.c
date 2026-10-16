void exit(int);

int f(unsigned number_of_digits_to_use) {
  if (number_of_digits_to_use > 1294)
    return 0;
  return (number_of_digits_to_use * 3321928 / 1000000 + 1) / 16;
}

int main(void) {
  if (f(11) != 2)
    __builtin_abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut number_of_digits_to_use: u32) -> i32 {
// CHECK-NEXT:     if number_of_digits_to_use > ((1294 as i32) as u32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     ((number_of_digits_to_use.wrapping_mul((3321928 as i32) as u32)
// CHECK-NEXT:         / ((1000000 as i32) as u32))
// CHECK-NEXT:         .wrapping_add((1 as i32) as u32) / ((16 as i32) as u32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f((11 as i32) as u32) != (2 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
