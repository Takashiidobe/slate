#include <stdint.h>

#include <stdio.h>

struct OverAligned {
  _Alignas(32) int value;
};

int main(void) {
  struct OverAligned object = {7};
  _Alignas(64) int   local  = 11;
  printf("%zu %zu %zu %d %d\n", _Alignof(struct OverAligned),
         (uintptr_t)&object % 32U, (uintptr_t)&local % 64U, object.value,
         local);
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
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct OverAligned {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(64))]
// CHECK-NEXT: struct __SlateAlign64<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut object: OverAligned = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: OverAligned = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].value = 7 as i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut local: __SlateAlign64<i32> = __SlateAlign64(11 as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu %zu %d %d\n".as_ptr(),
// CHECK-NEXT:             32 as u64,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(object) as u64) % 32,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(local.0) as u64) % 64,
// CHECK-NEXT:             object.value,
// CHECK-NEXT:             local.0,
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
