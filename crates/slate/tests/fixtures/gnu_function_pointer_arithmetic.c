#include <stddef.h>
#include <stdio.h>

static int target(void) { return 42; }

int main(void) {
  int       (*base)(void) = target;
  ptrdiff_t forward       = (base + 3) - base;
  ptrdiff_t backward      = (base - 2) - base;
  ptrdiff_t difference    = base - (base + 3);
  int       unchanged     = base + 0 == base;
  printf("%td %td %td %d\n", forward, backward, difference, unchanged);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.target")]
// CHECK-NEXT: extern "C-unwind" fn target() -> i32 {
// CHECK-NEXT:     42
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let base: Option<unsafe extern "C-unwind" fn() -> i32> = Some(target);
// CHECK-NEXT:     let forward: i64 = ((unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:             *const u8,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const u8,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                         *const u8,
// CHECK-NEXT:                     >(base)
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .wrapping_offset(3 as isize),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     }) as isize)
// CHECK-NEXT:         .wrapping_sub(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                     *const u8,
// CHECK-NEXT:                 >(base)
// CHECK-NEXT:             }) as isize,
// CHECK-NEXT:         ) as i64;
// CHECK-NEXT:     let backward: i64 = ((unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:             *const u8,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const u8,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                         *const u8,
// CHECK-NEXT:                     >(base)
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .wrapping_offset(-(2 as isize)),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     }) as isize)
// CHECK-NEXT:         .wrapping_sub(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                     *const u8,
// CHECK-NEXT:                 >(base)
// CHECK-NEXT:             }) as isize,
// CHECK-NEXT:         ) as i64;
// CHECK-NEXT:     let difference: i64 = ((unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:             *const u8,
// CHECK-NEXT:         >(base)
// CHECK-NEXT:     }) as isize)
// CHECK-NEXT:         .wrapping_sub(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                     *const u8,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         *const u8,
// CHECK-NEXT:                         Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                     >(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                                 *const u8,
// CHECK-NEXT:                             >(base)
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .wrapping_offset(3 as isize),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 })
// CHECK-NEXT:             }) as isize,
// CHECK-NEXT:         ) as i64;
// CHECK-NEXT:     let unchanged: i32 = ((unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const u8,
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                     *const u8,
// CHECK-NEXT:                 >(base)
// CHECK-NEXT:             }
// CHECK-NEXT:                 .wrapping_offset(0 as isize),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == base) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%td %td %td %d\n".as_ptr(), forward, backward, difference, unchanged)
// CHECK-NEXT:     };
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
