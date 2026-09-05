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
// LOWERING-NEXT: struct Data {
// LOWERING-NEXT:     value: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: static mut process_c: i8 = 0;
// LOWERING-AARCH64-GNU-NEXT: static mut process_c: u8 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut d: Data = Data { value: 0 };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: Data = Data { value: 7 };
// LOWERING-NEXT:     d = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     process({{__v[0-9]+}}, Some(print_handler), std::ptr::addr_of_mut!(d));
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     process({{__v[0-9]+}}, Some(print_handler), std::ptr::addr_of_mut!(d));
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn process(
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)>,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut Data,
// LOWERING-NEXT: ) {
// LOWERING-NEXT:     let mut flag: i32 = 0;
// LOWERING-NEXT:     let mut handler: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)> = None;
// LOWERING-NEXT:     flag = {{arg[0-9]+}};
// LOWERING-NEXT:     handler = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = flag;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)> = handler;
// LOWERING-NEXT:             let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:                 std::ptr::addr_of_mut!(process_c) as *mut core::ffi::c_void;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:             return;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)> = handler;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:     unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn print_handler({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut p: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut extra: i32 = 0;
// LOWERING-NEXT:     p = {{arg[0-9]+}};
// LOWERING-NEXT:     extra = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = extra;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: *mut core::ffi::c_void = p;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"zero %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"zero %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:             return;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = p;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Data = {{__v[0-9]+}} as *mut Data;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).value };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = extra;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     return;
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
// REWRITES-NEXT: struct Data {
// REWRITES-NEXT:     value: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-X86_64-GNU-NEXT: static mut process_c: i8 = 0;
// REWRITES-AARCH64-GNU-NEXT: static mut process_c: u8 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut d: Data = Data { value: 7 };
// REWRITES-NEXT:     process(1, Some(print_handler), std::ptr::addr_of_mut!(d));
// REWRITES-NEXT:     process(0, Some(print_handler), std::ptr::addr_of_mut!(d));
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn process(
// REWRITES-NEXT:     mut {{__v[0-9]+}}: i32,
// REWRITES-NEXT:     mut handler: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)>,
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut Data,
// REWRITES-NEXT: ) {
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)> = handler;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:             std::ptr::addr_of_mut!(process_c) as *mut core::ffi::c_void;
// REWRITES-NEXT:         unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, 0 as i32) };
// REWRITES-NEXT:         return;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { handler.unwrap()({{arg[0-9]+}} as *mut core::ffi::c_void, 42 as i32) };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn print_handler(mut p: *mut core::ffi::c_void, mut extra: i32) {
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = extra == 0;
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:         unsafe { printf(c"zero %d\n".as_ptr(), (unsafe { *(p as *mut i8) }) as i32) };
// REWRITES-AARCH64-GNU-NEXT:         unsafe { printf(c"zero %d\n".as_ptr(), (unsafe { *(p as *mut u8) }) as i32) };
// REWRITES-NEXT:         return;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*(p as *mut Data)).value };
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), {{__v[0-9]+}}, extra) };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
