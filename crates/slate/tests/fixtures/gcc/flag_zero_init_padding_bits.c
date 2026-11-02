/* { dg-options "-fzero-init-padding-bits=all" } */
#include <stdio.h>
#include <string.h>

union small_first {
  char c;
  long long wide;
};

struct padded {
  char c;
  int i;
};

static void scribble(void) {
  volatile unsigned char junk[256];
  memset((void *)junk, 0xa5, sizeof junk);
}

static void inspect(void) {
  union small_first u = {.c = 1};
  struct padded p = {2, 3};
  unsigned char bytes[sizeof u];
  unsigned char pbytes[sizeof p];
  memcpy(bytes, &u, sizeof u);
  memcpy(pbytes, &p, sizeof p);
  for (unsigned i = 0; i < sizeof u; i++)
    printf("%02x", bytes[i]);
  printf(" ");
  for (unsigned i = 0; i < sizeof p; i++)
    printf("%02x", pbytes[i]);
  printf("\n");
}

int main(void) {
  scribble();
  inspect();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fzero-init-padding-bits=all" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memcpy([[ARG_0:arg[0-9]+]]: *mut (), [[ARG_1:arg[0-9]+]]: *const (), [[ARG_2:arg[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn memset([[ARG_0]]: *mut (), [[ARG_1]]: i32, [[ARG_2]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union small_first {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     wide: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct padded {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn scribble() {
// CHECK-NEXT:     let mut junk: __SlateAlign16<[u8; 256]> = __SlateAlign16([0u8; 256]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             junk.0.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             165u8,
// CHECK-NEXT:             std::mem::size_of::<[u8; 256]>(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn inspect() {
// CHECK-NEXT:     let mut u: small_first = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: small_first = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].c = 1i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: *mut u8 = (std::ptr::addr_of_mut!(u) as *mut _) as *mut u8;
// CHECK-NEXT:         [[T_1]].add(1usize).write_bytes(0u8, 7usize);
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut p: padded = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].c = 2i8;
// CHECK-NEXT:         [[T_2]].i = 3i32;
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: *mut u8 = (std::ptr::addr_of_mut!(p) as *mut _) as *mut u8;
// CHECK-NEXT:         [[T_3]].add(1usize).write_bytes(0u8, 3usize);
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut bytes: [u8; 8] = [0u8; 8];
// CHECK-NEXT:     let mut pbytes: [u8; 8] = [0u8; 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(u) as *const ()) as *const u8,
// CHECK-NEXT:             bytes.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             std::mem::size_of::<small_first>(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(p) as *const ()) as *const u8,
// CHECK-NEXT:             pbytes.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             std::mem::size_of::<padded>(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: u32 = 0;
// CHECK-NEXT:         while (i as u64) < (std::mem::size_of::<small_first>() as u64) {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(c"%02x".as_ptr(), (bytes[i as usize] as u32) as i32) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     print!(" ");
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < (std::mem::size_of::<padded>() as u64) {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%02x".as_ptr(), (pbytes[i as usize] as u32) as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     scribble();
// CHECK-NEXT:     inspect();
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
