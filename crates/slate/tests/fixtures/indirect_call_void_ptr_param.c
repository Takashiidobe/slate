#include <stdio.h>

struct Data {
  int value;
};

static void process(int flag, void (*handler)(const void *, int),
                    struct Data *d) {
  if (flag) {
    static const char c = '\0';
    handler(&c, 0);
    return;
  }
  handler(d, 42);
}

static void print_handler(const void *p, int extra) {
  if (extra == 0) {
    const char *c = (const char *)p;
    printf("zero %d\n", *c);
    return;
  }
  const struct Data *d = (const struct Data *)p;
  printf("%d %d\n", d->value, extra);
}

int main(void) {
  struct Data d = {7};
  process(1, print_handler, &d);
  process(0, print_handler, &d);
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
// CHECK-NEXT: struct Data {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn process(
// CHECK-NEXT:     flag: i32,
// CHECK-NEXT:     handler: Option<unsafe extern "C-unwind" fn(*const (), i32)>,
// CHECK-NEXT:     d: *mut Data,
// CHECK-NEXT: ) {
// CHECK-NEXT:     if flag != 0 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             handler.unwrap()((unsafe { std::ptr::addr_of!(c) }) as *const (), 0 as i32)
// CHECK-NEXT:         };
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { handler.unwrap()(d as *const (), 42 as i32) };
// CHECK-NEXT: }
// CHECK-NEXT: static mut c: i8 = (0 as i32) as i8;
// CHECK-NEXT: extern "C-unwind" fn print_handler(p: *const (), extra: i32) {
// CHECK-NEXT:     if extra == 0 {
// CHECK-NEXT:         println!("zero {}", (unsafe { * (p as * const i8) }) as i32);
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", unsafe { (* (p as * const Data)).value }, extra);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: Data = Data { value: 7 };
// CHECK-NEXT:     process(1 as i32, Some(print_handler), std::ptr::addr_of_mut!(d));
// CHECK-NEXT:     process(0 as i32, Some(print_handler), std::ptr::addr_of_mut!(d));
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
