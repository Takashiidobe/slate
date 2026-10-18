#include <stddef.h>

int main(void) {
  char *buffer = __builtin_malloc(32);
  __builtin_strcpy(buffer, "slate");
  char copy[8];
  __builtin_strncpy(copy, buffer, sizeof copy);
  char formatted[32];
  __builtin_sprintf(formatted, "%s-%d", copy, (int)__builtin_strlen(buffer));
  char bounded[4];
  int wanted = __builtin_snprintf(bounded, sizeof bounded, "%s", formatted);
  const char *found = __builtin_memchr(formatted, '-', __builtin_strlen(formatted));
  __builtin_printf("%s %s %d %d %td\n", formatted, bounded, wanted,
                   __builtin_strcmp(buffer, "slate"), found - formatted);
  __builtin_printf("%d %d %d %d %d %d\n", __builtin_ffs(0), __builtin_ffs(12),
                   __builtin_ffs(-2147483647 - 1), __builtin_ffsl(1L << 20),
                   __builtin_ffsll(1LL << 40), __builtin_ffsll(0));
  __builtin_printf("%d %ld\n", __builtin_abs(-7), __builtin_labs(-9L));
  __builtin_free(buffer);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "malloc"]
// CHECK-NEXT:     fn __builtin_malloc([[V_0:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "strcpy"]
// CHECK-NEXT:     fn __builtin_strcpy([[V_1:__v[0-9]+]]: *mut i8, [[V_2:__v[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT:     #[link_name = "strncpy"]
// CHECK-NEXT:     fn __builtin_strncpy([[V_3:__v[0-9]+]]: *mut i8, [[V_4:__v[0-9]+]]: *const i8, [[V_5:__v[0-9]+]]: u64) -> *mut i8;
// CHECK-NEXT:     #[link_name = "sprintf"]
// CHECK-NEXT:     fn __builtin_sprintf([[V_6:__v[0-9]+]]: *mut i8, [[V_7:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     #[link_name = "strlen"]
// CHECK-NEXT:     fn __builtin_strlen([[V_8:__v[0-9]+]]: *const i8) -> u64;
// CHECK-NEXT:     #[link_name = "snprintf"]
// CHECK-NEXT:     fn __builtin_snprintf([[V_9:__v[0-9]+]]: *mut i8, [[V_10:__v[0-9]+]]: u64, [[V_11:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     #[link_name = "memchr"]
// CHECK-NEXT:     fn __builtin_memchr([[V_12:__v[0-9]+]]: *const (), [[V_13:__v[0-9]+]]: i32, [[V_14:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "printf"]
// CHECK-NEXT:     fn __builtin_printf([[V_15:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     #[link_name = "strcmp"]
// CHECK-NEXT:     fn __builtin_strcmp([[V_16:__v[0-9]+]]: *const i8, [[V_17:__v[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT:     #[link_name = "abs"]
// CHECK-NEXT:     fn __builtin_abs([[V_18:__v[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     #[link_name = "labs"]
// CHECK-NEXT:     fn __builtin_labs([[V_19:__v[0-9]+]]: i64) -> i64;
// CHECK-NEXT:     #[link_name = "free"]
// CHECK-NEXT:     fn __builtin_free([[V_20:__v[0-9]+]]: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let buffer: *mut i8 = (unsafe { __builtin_malloc(32 as u64) }) as *mut i8;
// CHECK-NEXT:     unsafe { __builtin_strcpy(buffer, c"slate".as_ptr()) };
// CHECK-NEXT:     let mut copy: [i8; 8] = [0 as i8; 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_strncpy(copy.as_mut_ptr() as *mut i8, buffer as *const i8, 8 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut formatted: __SlateAlign16<[i8; 32]> = __SlateAlign16([0 as i8; 32]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_sprintf(
// CHECK-NEXT:             formatted.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             c"%s-%d".as_ptr(),
// CHECK-NEXT:             copy.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             (__builtin_strlen(buffer as *const i8) as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut bounded: [i8; 4] = [0 as i8; 4];
// CHECK-NEXT:     let wanted: i32 = unsafe {
// CHECK-NEXT:         __builtin_snprintf(
// CHECK-NEXT:             bounded.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:             c"%s".as_ptr(),
// CHECK-NEXT:             formatted.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let found: *const i8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(
// CHECK-NEXT:             formatted.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             45 as i32,
// CHECK-NEXT:             __builtin_strlen(formatted.0.as_mut_ptr() as *const i8),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as *const i8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_printf(
// CHECK-NEXT:             c"%s %s %d %d %td\n".as_ptr(),
// CHECK-NEXT:             formatted.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             bounded.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             wanted,
// CHECK-NEXT:             __builtin_strcmp(buffer as *const i8, c"slate".as_ptr()),
// CHECK-NEXT:             found.offset_from(formatted.0.as_mut_ptr() as *const i8) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", { let [[T_0:__t[0-9]+]] = 0 as i32; if [[T_0]] == 0 { 0 as i32 } else {
// CHECK-NEXT:         ([[T_0]].trailing_zeros() as i32) + 1 } }, { let [[T_1:__t[0-9]+]] = 12 as i32; if [[T_1]] == 0 { 0
// CHECK-NEXT:         as i32 } else { ([[T_1]].trailing_zeros() as i32) + 1 } }, { let [[T_2:__t[0-9]+]] = -
// CHECK-NEXT:         (2147483647 as i32) - (1 as i32); if [[T_2]] == 0 { 0 as i32 } else { ([[T_2]]
// CHECK-NEXT:         .trailing_zeros() as i32) + 1 } }, { let [[T_3:__t[0-9]+]] = (1 as i64) << (20 as i32); if
// CHECK-NEXT:         [[T_3]] == 0 { 0 as i32 } else { ([[T_3]].trailing_zeros() as i32) + 1 } }, { let [[T_4:__t[0-9]+]]
// CHECK-NEXT:         = (1 as i64) << (40 as i32); if [[T_4]] == 0 { 0 as i32 } else { ([[T_4]]
// CHECK-NEXT:         .trailing_zeros() as i32) + 1 } }, { let [[T_5:__t[0-9]+]] = 0 as i64; if [[T_5]] == 0 { 0 as i32
// CHECK-NEXT:         } else { ([[T_5]].trailing_zeros() as i32) + 1 } }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_printf(
// CHECK-NEXT:             c"%d %ld\n".as_ptr(),
// CHECK-NEXT:             (-(7 as i32)).wrapping_abs(),
// CHECK-NEXT:             (-(9 as i64)).wrapping_abs(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { __builtin_free(buffer as *mut ()) };
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
