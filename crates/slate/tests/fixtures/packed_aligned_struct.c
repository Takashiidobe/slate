#include <stddef.h>
#include <stdio.h>

struct __attribute__((packed, aligned(4))) PackedAligned {
  char a;
  int  b;
};

int main(void) {
  struct PackedAligned s;
  s.a = 7;
  s.b = 0x1234;

  printf("%zu %zu\n", sizeof(struct PackedAligned),
         _Alignof(struct PackedAligned));
  printf("%zu %zu\n", offsetof(struct PackedAligned, a),
         offsetof(struct PackedAligned, b));
  printf("%d %x\n", s.a, s.b);

  s.b = s.b + 1;
  printf("%x\n", s.b);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct PackedAligned {
// CHECK-NEXT:     a: std::mem::MaybeUninit<[u8; 1]>,
// CHECK-NEXT:     b: std::mem::MaybeUninit<[u8; 4]>,
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: PackedAligned = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!(s.a) as *mut i8).write_unaligned(7i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!(s.b) as *mut i32).write_unaligned(4660i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%zu %zu\n".as_ptr(),
// CHECK-NEXT:             std::mem::size_of::<PackedAligned>() as u64,
// CHECK-NEXT:             std::mem::align_of::<PackedAligned>() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%zu %zu\n".as_ptr(), 0u64, 1u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %x\n".as_ptr(),
// CHECK-NEXT:             (std::ptr::addr_of!(s.a) as *const i8).read_unaligned() as i32,
// CHECK-NEXT:             (std::ptr::addr_of!(s.b) as *const i32).read_unaligned(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!(s.b) as *mut i32)
// CHECK-NEXT:             .write_unaligned(
// CHECK-NEXT:                 (std::ptr::addr_of!(s.b) as *const i32).read_unaligned() + 1,
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%x\n".as_ptr(),
// CHECK-NEXT:             (std::ptr::addr_of!(s.b) as *const i32).read_unaligned(),
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
