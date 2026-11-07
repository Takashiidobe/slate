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

struct nested {
  char tag;
  union small_first u;
  struct padded rows[2];
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
  struct padded __attribute__((aligned(32))) ap = {9, 10};
  unsigned char abytes[sizeof ap];
  memcpy(abytes, &ap, sizeof ap);
  for (unsigned i = 0; i < sizeof ap; i++)
    printf("%02x", abytes[i]);
  printf("\n");
  struct nested *lit = &(struct nested){1, {.c = 4}, {{5, 6}, {7, 8}}};
  unsigned char lbytes[sizeof *lit];
  memcpy(lbytes, lit, sizeof *lit);
  for (unsigned i = 0; i < sizeof *lit; i++)
    printf("%02x", lbytes[i]);
  printf("\n");
  struct nested n = {1, {.c = 4}, {{5, 6}, {7, 8}}};
  unsigned char nbytes[sizeof n];
  memcpy(nbytes, &n, sizeof n);
  for (unsigned i = 0; i < sizeof n; i++)
    printf("%02x", nbytes[i]);
  printf("\n");
}

int main(void) {
  scribble();
  inspect();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fzero-init-padding-bits=all" }
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct nested {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     u: small_first,
// CHECK-NEXT:     rows: [padded; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: struct __SlateAlign32<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: use slate_support::memset;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn scribble() {
// CHECK-NEXT:     let mut junk: __SlateAlign16<[u8; 256]> = __SlateAlign16([0u8; 256]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             ((junk.0.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (165i32 as i32) as u8,
// CHECK-NEXT:             ((std::mem::size_of::<[u8; 256]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn inspect() {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: nested = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut u: small_first = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     {
// CHECK-NEXT:         u.c = 1i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: *mut u8 = (std::ptr::addr_of_mut!(u) as *mut _) as *mut u8;
// CHECK-NEXT:         };
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut p: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     {
// CHECK-NEXT:         p.c = 2i8;
// CHECK-NEXT:         p.i = 3i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]]: *mut u8 = (std::ptr::addr_of_mut!(p) as *mut _) as *mut u8;
// CHECK-NEXT:             [[T_1]].add(1usize).write_bytes(0u8, 3usize);
// CHECK-NEXT:         };
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut bytes: [u8; 8] = [0u8; 8];
// CHECK-NEXT:     let mut pbytes: [u8; 8] = [0u8; 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(u) as *const ()) as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((bytes.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((std::mem::size_of::<small_first>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(p) as *const ()) as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((pbytes.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((std::mem::size_of::<padded>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: u32 = 0;
// CHECK-NEXT:         while (i as u64) < (std::mem::size_of::<small_first>() as u64) {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(c"%02x".as_ptr(), bytes[i as usize] as i32) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     print!(" ");
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: u32 = 0;
// CHECK-NEXT:         while (i as u64) < (std::mem::size_of::<padded>() as u64) {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(c"%02x".as_ptr(), pbytes[i as usize] as i32) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     let mut ap: __SlateAlign32<padded> = __SlateAlign32(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     {
// CHECK-NEXT:         ap.0.c = 9i8;
// CHECK-NEXT:         ap.0.i = 10i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_2:__t[0-9]+]]: *mut u8 = (std::ptr::addr_of_mut!(ap.0) as *mut _) as *mut u8;
// CHECK-NEXT:             [[T_2]].add(1usize).write_bytes(0u8, 3usize);
// CHECK-NEXT:         };
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut abytes: [u8; 8] = [0u8; 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(ap.0) as *const ()) as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((abytes.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((std::mem::size_of::<padded>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: u32 = 0;
// CHECK-NEXT:         while (i as u64) < (std::mem::size_of::<padded>() as u64) {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(c"%02x".as_ptr(), abytes[i as usize] as i32) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     let lit: *mut nested = unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(*{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]].tag = 1i8;
// CHECK-NEXT:             [[SLATE_COMPOUND_0]].u.c = 4i8;
// CHECK-NEXT:             [[SLATE_COMPOUND_0]].rows[0].c = 5i8;
// CHECK-NEXT:             [[SLATE_COMPOUND_0]].rows[0].i = 6i32;
// CHECK-NEXT:             [[SLATE_COMPOUND_0]].rows[1].c = 7i8;
// CHECK-NEXT:             [[SLATE_COMPOUND_0]].rows[1].i = 8i32;
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_3:__t[0-9]+]]: *mut u8 =
// CHECK-NEXT:                     (std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]]) as *mut _) as *mut u8;
// CHECK-NEXT:                 [[T_3]].add(1usize).write_bytes(0u8, 7usize);
// CHECK-NEXT:                 [[T_3]].add(17usize).write_bytes(0u8, 3usize);
// CHECK-NEXT:                 [[T_3]].add(25usize).write_bytes(0u8, 3usize);
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut lbytes: __SlateAlign16<[u8; 32]> = __SlateAlign16([0u8; 32]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((lit as *const ()) as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((lbytes.0.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((std::mem::size_of::<nested>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: u32 = 0;
// CHECK-NEXT:         while (i as u64) < (std::mem::size_of::<nested>() as u64) {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(c"%02x".as_ptr(), lbytes.0[i as usize] as i32) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     let mut n: nested = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     {
// CHECK-NEXT:         n.tag = 1i8;
// CHECK-NEXT:         n.u.c = 4i8;
// CHECK-NEXT:         n.rows[0].c = 5i8;
// CHECK-NEXT:         n.rows[0].i = 6i32;
// CHECK-NEXT:         n.rows[1].c = 7i8;
// CHECK-NEXT:         n.rows[1].i = 8i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_4:__t[0-9]+]]: *mut u8 = (std::ptr::addr_of_mut!(n) as *mut _) as *mut u8;
// CHECK-NEXT:             [[T_4]].add(1usize).write_bytes(0u8, 7usize);
// CHECK-NEXT:             [[T_4]].add(17usize).write_bytes(0u8, 3usize);
// CHECK-NEXT:             [[T_4]].add(25usize).write_bytes(0u8, 3usize);
// CHECK-NEXT:         };
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut nbytes: __SlateAlign16<[u8; 32]> = __SlateAlign16([0u8; 32]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(n) as *const ()) as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((nbytes.0.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((std::mem::size_of::<nested>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < (std::mem::size_of::<nested>() as u64) {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%02x".as_ptr(), nbytes.0[i as usize] as i32) };
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
