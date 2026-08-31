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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Data {
// LOWERING-NEXT:     value: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut process_c: i8 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn process({{arg[0-9]+}}: i32, {{arg[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i32)>, {{arg[0-9]+}}: *mut Data) {
// LOWERING-NEXT:     let mut flag: i32 = 0;
// LOWERING-NEXT:     let mut handler: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i32)> = None;
// LOWERING-NEXT:     flag = {{arg[0-9]+}};
// LOWERING-NEXT:     handler = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = flag;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i32)> = handler;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(process_c) as *mut core::ffi::c_void;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:             return;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i32)> = handler;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:     unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn print_handler({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut p: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut extra: i32 = 0;
// LOWERING-NEXT:     p = {{arg[0-9]+}};
// LOWERING-NEXT:     extra = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = extra;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut core::ffi::c_void = p;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"zero %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:             return;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = p;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Data = {{_v[0-9]+}} as *mut Data;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = extra;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut d: Data = Data { value: 0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     d = Data { value: 7 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     process({{_v[0-9]+}}, Some(print_handler), std::ptr::addr_of_mut!(d));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     process({{_v[0-9]+}}, Some(print_handler), std::ptr::addr_of_mut!(d));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Data {
// REWRITES-NEXT:     value: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut process_c: i8 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn process({{arg[0-9]+}}: i32, {{arg[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i32)>, {{arg[0-9]+}}: *mut Data) {
// REWRITES-NEXT: let mut flag: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut handler: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i32)> = {{arg[0-9]+}};
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = flag != 0;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i32)> = handler;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(process_c) as *mut core::ffi::c_void;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:                     unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT:                     return;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 42;
// REWRITES-NEXT: unsafe { handler.unwrap()({{arg[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}}) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn print_handler({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: i32) {
// REWRITES-NEXT: let mut p: *mut core::ffi::c_void = {{arg[0-9]+}};
// REWRITES-NEXT: let mut extra: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = extra == {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = p as *mut i8;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b"zero %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     unsafe { printf({{_v[0-9]+}} as *const i8, (unsafe { *{{_v[0-9]+}} }) as i32) };
// REWRITES-NEXT:                     return;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut Data = p as *mut Data;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).value };
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, extra) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut d: Data = Data { value: 7 };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: process({{_v[0-9]+}}, Some(print_handler), std::ptr::addr_of_mut!(d));
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: process({{_v[0-9]+}}, Some(print_handler), std::ptr::addr_of_mut!(d));
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
