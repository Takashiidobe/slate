static int check_rotate(unsigned _BitInt(17) value, unsigned long long count) {
  unsigned left  = __builtin_stdc_rotate_left(value, count);
  unsigned right = __builtin_stdc_rotate_right(value, count);
  return (left != 1) + 2 * (right != 1);
}

int main(void) {
  return check_rotate(1, 0xffffffffffffffffULL);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu armv7-unknown-linux-gnueabihf
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn check_rotate(value: u32, count: u64) -> i32 {
// CHECK-NEXT:     let left: u32 = ({
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = value;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = ((count as u64) % 17) as u32;
// CHECK-NEXT:         (([[T_0]] << [[T_1]] | [[T_0]] >> (17 - [[T_1]]) % 17) as u32) & 131071
// CHECK-NEXT:     }) as u32;
// CHECK-NEXT:     let right: u32 = ({
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = value;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = ((count as u64) % 17) as u32;
// CHECK-NEXT:         (([[T_2]] >> [[T_3]] | [[T_2]] << (17 - [[T_3]]) % 17) as u32) & 131071
// CHECK-NEXT:     }) as u32;
// CHECK-NEXT:     ((left != 1) as i32) + 2 * ((right != 1) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     check_rotate(1u32, 18446744073709551615u64)
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
