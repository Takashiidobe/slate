#include <stdio.h>

int main(void) {
  int          ri  = 0;
  unsigned int ru  = 0;
  long long    rll = 0;

  int add_i = __builtin_add_overflow(2147483647, 1, &ri);
  printf("%d %d\n", add_i, ri);

  int sub_i = __builtin_sub_overflow((-2147483647 - 1), 1, &ri);
  printf("%d %d\n", sub_i, ri);

  int mul_i = __builtin_mul_overflow(1073741824, 2, &ri);
  printf("%d %d\n", mul_i, ri);

  int add_u = __builtin_add_overflow(4294967295u, 1u, &ru);
  printf("%d %u\n", add_u, ru);

  int mul_ll = __builtin_mul_overflow(3037000500LL, 3037000500LL, &rll);
  printf("%d %lld\n", mul_ll, rll);

  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut ri: i32 = 0;
// CHECK-NEXT:     let mut ru: u32 = 0;
// CHECK-NEXT:     let mut rll: i64 = 0;
// CHECK-NEXT:     let add_i: i32 = ({
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i128 = 2147483647 as i128;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: i128 = 1 as i128;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(ri) = [[T_2]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as i32) as i128) != [[T_2]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     println!("{} {}", add_i, ri);
// CHECK-NEXT:     let sub_i: i32 = ({
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: i128 = -2147483648i32 as i128;
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: i128 = 1 as i128;
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: i128 = [[T_3]].wrapping_sub([[T_4]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(ri) = [[T_5]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_3]].checked_sub([[T_4]]).is_none() || (([[T_5]] as i32) as i128) != [[T_5]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     println!("{} {}", sub_i, ri);
// CHECK-NEXT:     let mul_i: i32 = ({
// CHECK-NEXT:         let [[T_6:__t[0-9]+]]: i128 = 1073741824 as i128;
// CHECK-NEXT:         let [[T_7:__t[0-9]+]]: i128 = 2 as i128;
// CHECK-NEXT:         let [[T_8:__t[0-9]+]]: i128 = [[T_6]].wrapping_mul([[T_7]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(ri) = [[T_8]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].checked_mul([[T_7]]).is_none() || (([[T_8]] as i32) as i128) != [[T_8]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     println!("{} {}", mul_i, ri);
// CHECK-NEXT:     let add_u: i32 = ({
// CHECK-NEXT:         let [[T_9:__t[0-9]+]]: i128 = 4294967295 as i128;
// CHECK-NEXT:         let [[T_10:__t[0-9]+]]: i128 = 1 as i128;
// CHECK-NEXT:         let [[T_11:__t[0-9]+]]: i128 = [[T_9]].wrapping_add([[T_10]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(ru) = [[T_11]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_9]].checked_add([[T_10]]).is_none() || (([[T_11]] as u32) as i128) != [[T_11]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %u\n".as_ptr(), add_u, ru) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mul_ll: i32 = ({
// CHECK-NEXT:         let [[T_12:__t[0-9]+]]: i128 = 3037000500 as i128;
// CHECK-NEXT:         let [[T_13:__t[0-9]+]]: i128 = 3037000500 as i128;
// CHECK-NEXT:         let [[T_14:__t[0-9]+]]: i128 = [[T_12]].wrapping_mul([[T_13]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(rll) = [[T_14]] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_12]].checked_mul([[T_13]]).is_none() || (([[T_14]] as i64) as i128) != [[T_14]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %lld\n".as_ptr(), mul_ll, rll) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
