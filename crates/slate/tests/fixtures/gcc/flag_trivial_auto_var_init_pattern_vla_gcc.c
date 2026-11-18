/* { dg-options "-ftrivial-auto-var-init=pattern" } */
int printf(const char *, ...);

static void show(int n) {
  int ints[n];
  float floats[n];
  unsigned char *a = (unsigned char *)ints;
  unsigned char *f = (unsigned char *)floats;
  printf("%02x %02x %02x %02x\n", a[0], a[4 * n - 1], f[0], f[4 * n - 1]);
}

int main(void) {
  show(3);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-ftrivial-auto-var-init=pattern" }
// CHECK-NEXT: fn show(n: i32) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = n as u64;
// I686-UNKNOWN-LINUX-GNU-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = n as u32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = n as u64;
// CHECK-NEXT:     let mut ints: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity([[V_0]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(254u8, [[V_0]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = n as u64;
// I686-UNKNOWN-LINUX-GNU-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = n as u32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = n as u64;
// CHECK-NEXT:     let mut floats: Vec<f32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<f32> = Vec::with_capacity([[V_1]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(254u8, [[V_1]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     let a: *mut u8 = ints.as_mut_ptr() as *mut u8;
// CHECK-NEXT:     let f: *mut u8 = floats.as_mut_ptr() as *mut u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%02x %02x %02x %02x\n".as_ptr(),
// CHECK-NEXT:             *a.add(0) as i32,
// CHECK-NEXT:             *a.offset((4 * n - 1) as isize) as i32,
// CHECK-NEXT:             *f.add(0) as i32,
// CHECK-NEXT:             *f.offset((4 * n - 1) as isize) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     show(3i32);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
