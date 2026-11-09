#include <stdio.h>
#include <string.h>

void __clear_cache(void *beginning, void *end);

static unsigned char code[64];

int main(void) {
  memset(code, 0xd5, sizeof code);
  __clear_cache(code, code + sizeof code);
  __clear_cache(code + 8, code + 16);
  printf("%u %u\n", code[0], code[63]);
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.clear_cache"]
// CHECK-NEXT:     fn __slate_intrinsic_7eb885185d29c159([[ARG_0:arg[0-9]+]]: *mut (), [[ARG_1:arg[0-9]+]]: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut code: [u8; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: use slate_support::memset;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             (((std::ptr::addr_of_mut!(code) as *mut u8) as *mut ())
// CHECK-NEXT:                 as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (213i32 as i32) as u8,
// CHECK-NEXT:             ((std::mem::size_of::<[u8; 64]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_intrinsic_7eb885185d29c159(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(code) as *mut u8) as *mut (),
// CHECK-NEXT:             (std::ptr::addr_of_mut!(code) as *mut u8)
// CHECK-NEXT:                 .offset(std::mem::size_of::<[u8; 64]>() as isize) as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_intrinsic_7eb885185d29c159(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(code) as *mut u8).add(8) as *mut (),
// CHECK-NEXT:             (std::ptr::addr_of_mut!(code) as *mut u8).add(16) as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u\n".as_ptr(), code[0] as i32, code[63] as i32)
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
