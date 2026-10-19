#include <stdio.h>

struct cursor {
  unsigned char *p;
  unsigned char  buf[8];
};

static void fill(struct cursor *c, unsigned char n) {
  c->p = c->buf;
  while (c->p < &c->buf[sizeof(c->buf)]) {
    *c->p = n;
    n++;
    c->p++;
  }
}

int main(void) {
  struct cursor c;
  fill(&c, 1);

  int total = 0;
  for (int i = 0; i < 8; i++) {
    total += c.buf[i];
  }
  printf("%d\n", total);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct cursor {
// CHECK-NEXT:     p: *mut u8,
// CHECK-NEXT:     buf: [u8; 8],
// CHECK-NEXT: }
// CHECK-NEXT: fn fill(c: &mut cursor, mut n: u8) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(c as *mut cursor)).p = (*(c as *mut cursor)).buf.as_mut_ptr() as *mut u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     while (unsafe { (*(c as *mut cursor)).p })
// CHECK-NEXT:         < unsafe {
// CHECK-NEXT:             ((*(c as *mut cursor)).buf.as_mut_ptr() as *mut u8).offset(8 as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(*(c as *mut cursor)).p = n;
// CHECK-NEXT:         }
// CHECK-NEXT:         n = ((((n as u32) as i32) + 1) as i8) as u8;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut u8 = unsafe { (*c).p };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*c).p = [[V_0]].offset(1 as isize);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: cursor = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     fill(unsafe { &mut (*std::ptr::addr_of_mut!(c)) }, 1u8);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         total += (c.buf[i as usize] as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", total);
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
