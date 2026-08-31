#include <setjmp.h>
#include <stdio.h>

enum Status { STATUS_OK, STATUS_FAIL };
typedef enum Status Processor(int x);
typedef void        Callback(int x);

struct Dispatcher {
  Processor *run;
};

static jmp_buf   env;
static int       failures = 0;
static Callback *g_callback;

static enum Status risky(int x) {
  if (x == 3) {
    longjmp(env, 1);
  }
  return STATUS_OK;
}

static void panicky_callback(int x) {
  if (x == 2) {
    longjmp(env, 1);
  }
  printf("callback %d\n", x);
}

static enum Status content_like(int x) {
  g_callback(x);
  return STATUS_OK;
}

int main(void) {
  struct Dispatcher d;
  g_callback = panicky_callback;

  d.run = risky;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered risky %d\n", i);
      continue;
    }
    d.run(i);
  }

  d.run = content_like;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered content_like %d\n", i);
      continue;
    }
    d.run(i);
  }

  printf("failures=%d\n", failures);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum Status {
// LOWERING-NEXT:     STATUS_OK = 0,
// LOWERING-NEXT:     STATUS_FAIL = 1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Dispatcher {
// LOWERING-NEXT:     run: Option<unsafe extern "C" fn(i32) -> u32>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __slate_jmp_buf_tag {
// LOWERING-NEXT:     __regs: [i64; 8],
// LOWERING-NEXT:     __mask_was_saved: i32,
// LOWERING-NEXT:     __saved_mask: [u64; 16],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut env: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned([__slate_jmp_buf_tag { __regs: [0; 8], __mask_was_saved: 0, __saved_mask: [0; 16] }; 1]);
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut failures: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut g_callback: Option<unsafe extern "C" fn(i32)> = None;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn panicky_callback({{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     x = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"callback %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn risky({{arg[0-9]+}}: i32) -> u32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     x = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = Status::STATUS_OK as u32;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn content_like({{arg[0-9]+}}: i32) -> u32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32)> = unsafe { g_callback };
// LOWERING-NEXT:     unsafe { {{_v[0-9]+}}.unwrap()({{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = Status::STATUS_OK as u32;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut d: Dispatcher = Dispatcher { run: None };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         g_callback = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32)>>(panicky_callback as *const ()) };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     d.run = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> u32>>(risky as *const ()) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         '__loop0: loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             '__continue0: {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                             unsafe {
// LOWERING-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"recovered risky %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:                             break '__continue0;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> u32> = d.run;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     d.run = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> u32>>(content_like as *const ()) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i2: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i2 = {{_v[0-9]+}};
// LOWERING-NEXT:         '__loop1: loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             '__continue1: {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                             unsafe {
// LOWERING-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"recovered content_like %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:                             break '__continue1;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> u32> = d.run;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i2 = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"failures=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum Status {
// REWRITES-NEXT:     STATUS_OK = 0,
// REWRITES-NEXT:     STATUS_FAIL = 1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Dispatcher {
// REWRITES-NEXT:     run: Option<unsafe extern "C" fn(i32) -> u32>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __slate_jmp_buf_tag {
// REWRITES-NEXT:     __regs: [i64; 8],
// REWRITES-NEXT:     __mask_was_saved: i32,
// REWRITES-NEXT:     __saved_mask: [u64; 16],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut env: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned([__slate_jmp_buf_tag { __regs: [0; 8], __mask_was_saved: 0, __saved_mask: [0; 16] }; 1]);
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut failures: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut g_callback: Option<unsafe extern "C" fn(i32)> = None;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn panicky_callback({{arg[0-9]+}}: i32) {
// REWRITES-NEXT: let mut x: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = x == {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:                     unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"callback %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, x) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn risky({{arg[0-9]+}}: i32) -> u32 {
// REWRITES-NEXT: let mut x: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = x == {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:                     unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = Status::STATUS_OK as u32;
// REWRITES-NEXT: return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn content_like({{arg[0-9]+}}: i32) -> u32 {
// REWRITES-NEXT: unsafe { unsafe { g_callback }.unwrap()({{arg[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = Status::STATUS_OK as u32;
// REWRITES-NEXT: return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut d: Dispatcher = Dispatcher { run: None };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         g_callback = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32)>>(panicky_callback as *const ()) };
// REWRITES-NEXT: }
// REWRITES-NEXT: d.run = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> u32>>(risky as *const ()) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         '__loop0: loop {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT:                     if !(i < {{_v[0-9]+}}) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     '__continue0: {
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         {
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:                                                                                 if {{_v[0-9]+}} {
// REWRITES-NEXT:                                                                                                             unsafe {
// REWRITES-NEXT:                                                                                                                                             failures = (unsafe { failures }) + 1;
// REWRITES-NEXT:                                                                                                             }
// REWRITES-NEXT:                                                                                                             let {{_v[0-9]+}}: *mut i8 = b"recovered risky %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                                             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, i) };
// REWRITES-NEXT:                                                                                                             break '__continue0;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                                         let {{_v[0-9]+}}: u32 = unsafe { d.run.unwrap()(i) };
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: d.run = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> u32>>(content_like as *const ()) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i2: i32 = 0;
// REWRITES-NEXT:         i2 = 0;
// REWRITES-NEXT:         '__loop1: loop {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT:                     if !(i2 < {{_v[0-9]+}}) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     '__continue1: {
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         {
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:                                                                                 if {{_v[0-9]+}} {
// REWRITES-NEXT:                                                                                                             unsafe {
// REWRITES-NEXT:                                                                                                                                             failures = (unsafe { failures }) + 1;
// REWRITES-NEXT:                                                                                                             }
// REWRITES-NEXT:                                                                                                             let {{_v[0-9]+}}: *mut i8 = b"recovered content_like %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                                             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, i2) };
// REWRITES-NEXT:                                                                                                             break '__continue1;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                                         let {{_v[0-9]+}}: u32 = unsafe { d.run.unwrap()(i2) };
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i2 = i2 + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"failures=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, unsafe { failures }) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
