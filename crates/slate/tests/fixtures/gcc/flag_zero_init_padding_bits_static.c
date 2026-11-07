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

struct padded gp = {2, 3};
union small_first gu = {.c = 1};
struct nested gn = {1, {.c = 4}, {{5, 6}, {7, 8}}};
static struct nested sn = {1, {.c = 4}, {{5, 6}, {7, 8}}};
struct nested *gl = &(struct nested){1, {.c = 4}, {{5, 6}, {7, 8}}};

static void dump(const void *p, unsigned n) {
  unsigned char bytes[64];
  memcpy(bytes, p, n);
  for (unsigned i = 0; i < n; i++)
    printf("%02x", bytes[i]);
  printf("\n");
}

int main(void) {
  static union small_first lu = {.c = 7};
  dump(&gp, sizeof gp);
  dump(&gu, sizeof gu);
  dump(&gn, sizeof gn);
  dump(&sn, sizeof sn);
  dump(gl, sizeof *gl);
  dump(&lu, sizeof lu);
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
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut gp: padded = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].c = 2i8;
// CHECK-NEXT:     [[T_0]].i = 3i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut gu: small_first = {
// CHECK-NEXT:     let mut [[T_0]]: small_first = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].c = 1i8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut gn: nested = {
// CHECK-NEXT:     let mut [[T_0]]: nested = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].tag = 1i8;
// CHECK-NEXT:     [[T_0]].u = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: small_first = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].c = 4i8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     [[T_0]].rows = [
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].c = 5i8;
// CHECK-NEXT:             [[T_2]].i = 6i32;
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_3:__t[0-9]+]]: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_3]].c = 7i8;
// CHECK-NEXT:             [[T_3]].i = 8i32;
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut sn: nested = {
// CHECK-NEXT:     let mut [[T_0]]: nested = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].tag = 1i8;
// CHECK-NEXT:     [[T_0]].u = {
// CHECK-NEXT:         let mut [[T_1]]: small_first = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].c = 4i8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     [[T_0]].rows = [
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2]]: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].c = 5i8;
// CHECK-NEXT:             [[T_2]].i = 6i32;
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_3]]: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_3]].c = 7i8;
// CHECK-NEXT:             [[T_3]].i = 8i32;
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut gl: *mut nested = unsafe { std::ptr::addr_of_mut!(__slate_compound_gl_0) };
// CHECK-EMPTY:
// CHECK-NEXT: fn dump(p: *const (), n: u32) {
// CHECK-NEXT:     let mut bytes: __SlateAlign16<[u8; 64]> = __SlateAlign16([0u8; 64]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (p as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((bytes.0.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((n as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%02x".as_ptr(), bytes.0[i as usize] as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     dump(
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(gp) }) as *const (),
// CHECK-NEXT:         std::mem::size_of::<padded>() as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(gu) }) as *const (),
// CHECK-NEXT:         std::mem::size_of::<small_first>() as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(gn) }) as *const (),
// CHECK-NEXT:         std::mem::size_of::<nested>() as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(sn) }) as *const (),
// CHECK-NEXT:         std::mem::size_of::<nested>() as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         (unsafe { gl }) as *const (),
// CHECK-NEXT:         std::mem::size_of::<nested>() as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(lu) }) as *const (),
// CHECK-NEXT:         std::mem::size_of::<small_first>() as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut lu: small_first = {
// CHECK-NEXT:     let mut [[T_0]]: small_first = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].c = 7i8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut __slate_compound_gl_0: nested = {
// CHECK-NEXT:     let mut [[T_0]]: nested = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].tag = 1i8;
// CHECK-NEXT:     [[T_0]].u = {
// CHECK-NEXT:         let mut [[T_1]]: small_first = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].c = 4i8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     [[T_0]].rows = [
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2]]: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].c = 5i8;
// CHECK-NEXT:             [[T_2]].i = 6i32;
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_3]]: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_3]].c = 7i8;
// CHECK-NEXT:             [[T_3]].i = 8i32;
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
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
