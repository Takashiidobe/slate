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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Data {
// COMMON-LOWERING-NEXT:     value: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut d: Data = Data { value: 0 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Data = Data { value: 7 };
// COMMON-LOWERING-NEXT:     d = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     process({{__v[0-9]+}}, Some(print_handler), std::ptr::addr_of_mut!(d));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     process({{__v[0-9]+}}, Some(print_handler), std::ptr::addr_of_mut!(d));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn process(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)>,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut Data,
// COMMON-LOWERING-NEXT: ) {
// COMMON-LOWERING-NEXT:     let mut flag: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut handler: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)> = None;
// COMMON-LOWERING-NEXT:     flag = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     handler = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = flag;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)> = handler;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:                 std::ptr::addr_of_mut!(process_c) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:             return;
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)> = handler;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:     unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn print_handler({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let mut p: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut extra: i32 = 0;
// COMMON-LOWERING-NEXT:     p = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     extra = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = extra;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut core::ffi::c_void = p;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:             return;
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = p;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Data = {{__v[0-9]+}} as *mut Data;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).value };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = extra;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: static mut process_c: i8 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"zero %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: static mut process_c: u8 = 0;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"zero %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Data {
// COMMON-REWRITES-NEXT:     value: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut d: Data = Data { value: 7 };
// COMMON-REWRITES-NEXT:     process(1, Some(print_handler), std::ptr::addr_of_mut!(d));
// COMMON-REWRITES-NEXT:     process(0, Some(print_handler), std::ptr::addr_of_mut!(d));
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn process(
// COMMON-REWRITES-NEXT:     mut {{__v[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     mut handler: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)>,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut Data,
// COMMON-REWRITES-NEXT: ) {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i32)> = handler;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(process_c) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, 0 as i32) };
// COMMON-REWRITES-NEXT:         return;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { handler.unwrap()({{arg[0-9]+}} as *mut core::ffi::c_void, 42 as i32) };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn print_handler(mut p: *mut core::ffi::c_void, mut extra: i32) {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = extra == 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         return;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*(p as *mut Data)).value };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), {{__v[0-9]+}}, extra) };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: static mut process_c: i8 = 0;
// REWRITES-X86_64-GNU-NEXT:         unsafe { printf(c"zero %d\n".as_ptr(), (unsafe { *(p as *mut i8) }) as i32) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: static mut process_c: u8 = 0;
// REWRITES-AARCH64-GNU-NEXT:         unsafe { printf(c"zero %d\n".as_ptr(), (unsafe { *(p as *mut u8) }) as i32) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
