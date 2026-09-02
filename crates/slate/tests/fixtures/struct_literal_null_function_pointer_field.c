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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Handlers {
// LOWERING-NEXT:     label: *mut i8,
// LOWERING-NEXT:     onEvent: Option<unsafe extern "C" fn(i32) -> i32>,
// LOWERING-NEXT:     counter: *mut i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn report({{arg[0-9]+}}: *mut Handlers) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: Handlers = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let mut h: Handlers = Handlers {
// LOWERING-NEXT:         label: std::ptr::null_mut(),
// LOWERING-NEXT:         onEvent: None,
// LOWERING-NEXT:         counter: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     h = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = h.onEvent;
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = h.onEvent;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             total = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = h.counter;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = h.counter;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             total = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Handlers = Handlers {
// LOWERING-NEXT:         label: b"none\0".as_ptr() as *mut i8,
// LOWERING-NEXT:         onEvent: None,
// LOWERING-NEXT:         counter: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval: Handlers = Handlers {
// LOWERING-NEXT:         label: std::ptr::null_mut(),
// LOWERING-NEXT:         onEvent: None,
// LOWERING-NEXT:         counter: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = report(std::ptr::addr_of_mut!(byval));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Handlers {
// REWRITES-NEXT:     label: *mut i8,
// REWRITES-NEXT:     onEvent: Option<unsafe extern "C" fn(i32) -> i32>,
// REWRITES-NEXT:     counter: *mut i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn report({{arg[0-9]+}}: &Handlers) -> i32 {
// REWRITES-NEXT:     let mut h: Handlers = unsafe { *({{arg[0-9]+}} as *const Handlers) };
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = h.onEvent != None;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { h.onEvent.unwrap()(1 as i32) };
// REWRITES-NEXT:         total += {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = h.counter != std::ptr::null_mut();
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         total += unsafe { *h.counter };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     total
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: Handlers = Handlers {
// REWRITES-NEXT:         label: c"none".as_ptr() as *mut i8,
// REWRITES-NEXT:         onEvent: None,
// REWRITES-NEXT:         counter: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval: Handlers = {{_v[0-9]+}};
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             report(unsafe { &(*std::ptr::addr_of_mut!(byval)) }),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
