#include <stdio.h>
#include <uchar.h>

int main(void) {
  mbstate_t state16        = {0};
  mbstate_t state32        = {0};
  char16_t  converted16    = 0;
  char32_t  converted32    = 0;
  char      multibyte16[4] = {0};
  char      multibyte32[4] = {0};

  size_t read16  = mbrtoc16(&converted16, "A", 1, &state16);
  size_t write16 = c16rtomb(multibyte16, u'A', &state16);
  size_t read32  = mbrtoc32(&converted32, "B", 1, &state32);
  size_t write32 = c32rtomb(multibyte32, U'B', &state32);

  printf("%zu %zu %u %d %zu %zu %u %d\n", read16, write16,
         (unsigned)converted16, multibyte16[0], read32, write32,
         (unsigned)converted32, multibyte32[0]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn mbrtoc16(
// CHECK-NEXT:         __pc16: *mut u16,
// CHECK-NEXT:         __s: *const i8,
// CHECK-NEXT:         __n: u64,
// CHECK-NEXT:         __p: *mut [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:     ) -> u64;
// CHECK-NEXT:     fn c16rtomb(__s: *mut i8, __c16: u16, __ps: *mut [[SLATERECORD_0]]) -> u64;
// CHECK-NEXT:     fn mbrtoc32(
// CHECK-NEXT:         __pc32: *mut u32,
// CHECK-NEXT:         __s: *const i8,
// CHECK-NEXT:         __n: u64,
// CHECK-NEXT:         __p: *mut [[SLATERECORD_0]],
// CHECK-NEXT:     ) -> u64;
// CHECK-NEXT:     fn c32rtomb(__s: *mut i8, __c32: u32, __ps: *mut [[SLATERECORD_0]]) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     __count: i32,
// CHECK-NEXT:     __value: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1]] {
// CHECK-NEXT:     __wch: u32,
// CHECK-NEXT:     __wchb: [i8; 4],
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut state16: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__count = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut state32: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].__count = 0i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut converted16: u16 = 0;
// CHECK-NEXT:     let mut converted32: u32 = 0;
// CHECK-NEXT:     let mut multibyte16: [i8; 4] = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: [i8; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]][0] = 0i8;
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut multibyte32: [i8; 4] = {
// CHECK-NEXT:         let mut [[T_3:__t[0-9]+]]: [i8; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]][0] = 0i8;
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let read16: u64 = unsafe {
// CHECK-NEXT:         mbrtoc16(
// CHECK-NEXT:             std::ptr::addr_of_mut!(converted16),
// CHECK-NEXT:             c"A".as_ptr(),
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             std::ptr::addr_of_mut!(state16),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let write16: u64 = unsafe {
// CHECK-NEXT:         c16rtomb(
// CHECK-NEXT:             multibyte16.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             65u16,
// CHECK-NEXT:             std::ptr::addr_of_mut!(state16),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let read32: u64 = unsafe {
// CHECK-NEXT:         mbrtoc32(
// CHECK-NEXT:             std::ptr::addr_of_mut!(converted32),
// CHECK-NEXT:             c"B".as_ptr(),
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             std::ptr::addr_of_mut!(state32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let write32: u64 = unsafe {
// CHECK-NEXT:         c32rtomb(
// CHECK-NEXT:             multibyte32.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             66u32,
// CHECK-NEXT:             std::ptr::addr_of_mut!(state32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu %u %d %zu %zu %u %d\n".as_ptr(),
// CHECK-NEXT:             read16,
// CHECK-NEXT:             write16,
// CHECK-NEXT:             converted16 as u32,
// CHECK-NEXT:             multibyte16[0] as i32,
// CHECK-NEXT:             read32,
// CHECK-NEXT:             write32,
// CHECK-NEXT:             converted32,
// CHECK-NEXT:             multibyte32[0] as i32,
// CHECK-NEXT:         )
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
