#include <stdio.h>

typedef int (*Callback)(int);

struct Handlers {
  const char *label;
  Callback    onEvent;
  int        *counter;
};

static int report(struct Handlers h) {
  int total = 0;
  if (h.onEvent != NULL) {
    total += h.onEvent(1);
  }
  if (h.counter != NULL) {
    total += *h.counter;
  }
  return total;
}

int main(void) {
  struct Handlers h = {"none", NULL, NULL};
  printf("%d\n", report(h));
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
// CHECK-NEXT: struct Handlers {
// CHECK-NEXT:     label: *const i8,
// CHECK-NEXT:     onEvent: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:     counter: *mut i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn report(mut h: Handlers) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     if h.onEvent != None {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe { h.onEvent.unwrap()(1 as i32) };
// CHECK-NEXT:         total += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if !h.counter.is_null() {
// CHECK-NEXT:         total += unsafe { *h.counter };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let h: Handlers = Handlers {
// CHECK-NEXT:         label: c"none".as_ptr(),
// CHECK-NEXT:         onEvent: None,
// CHECK-NEXT:         counter: std::ptr::null_mut::<i32>(),
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", report(h));
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
