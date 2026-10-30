/* { dg-options "-ftrivial-auto-var-init=pattern" } */
int printf(const char *, ...);

struct record {
  int count;
  char name[4];
};

static void show(void) {
  int scalar;
  struct record r;
  unsigned char *bytes = (unsigned char *)&r;
  printf("%x %02x %02x\n", (unsigned)scalar, bytes[0], bytes[7]);
}

int main(void) {
  show();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-ftrivial-auto-var-init=pattern" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct record {
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     name: [i8; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn show() {
// CHECK-NEXT:     let scalar: i32 = -1431655766i32;
// CHECK-NEXT:     let mut r: record = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(r) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let bytes: *mut u8 = std::ptr::addr_of_mut!(r) as *mut u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%x %02x %02x\n".as_ptr(),
// CHECK-NEXT:             scalar as u32,
// CHECK-NEXT:             (*bytes.add(0) as u32) as i32,
// CHECK-NEXT:             (*bytes.add(7) as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     show();
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
