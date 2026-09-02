#include <setjmp.h>
#include <stdio.h>

typedef void Callback(int x);
typedef void Dispatch(int x, int y);

struct Ctx {
  Dispatch *run;
};

static jmp_buf   env;
static int       failures = 0;
static Callback *g_callback;

static void quiet_callback(int x) { printf("quiet %d\n", x); }

static void panicky_callback(int x) {
  if (x == 2) {
    longjmp(env, 1);
  }
  printf("panicky %d\n", x);
}

static void dispatcher(int x, int y) {
  (void)y;
  g_callback(x);
}

int main(void) {
  struct Ctx c;
  c.run = dispatcher;

  g_callback = quiet_callback;
  for (int i = 0; i < 2; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered quiet %d\n", i);
      continue;
    }
    c.run(i, 0);
  }

  g_callback = panicky_callback;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered panicky %d\n", i);
      continue;
    }
    c.run(i, 0);
  }

  printf("failures=%d\n", failures);
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
// LOWERING-NEXT: struct Ctx {
// LOWERING-NEXT:     run: Option<unsafe extern "C" fn(i32, i32)>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __slate_jmp_buf_tag {
// LOWERING-NEXT:     __regs: [i64; 8],
// LOWERING-NEXT:     __mask_was_saved: i32,
// LOWERING-NEXT:     __saved_mask: [u64; 16],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut env: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// LOWERING-NEXT:     [__slate_jmp_buf_tag {
// LOWERING-NEXT:         __regs: [0; 8],
// LOWERING-NEXT:         __mask_was_saved: 0,
// LOWERING-NEXT:         __saved_mask: [0; 16],
// LOWERING-NEXT:     }; 1],
// LOWERING-NEXT: );
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut failures: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut g_callback: Option<unsafe extern "C" fn(i32)> = None;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn dispatcher({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32)> = unsafe { g_callback };
// LOWERING-NEXT:     unsafe { {{_v[0-9]+}}.unwrap()({{arg[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn quiet_callback({{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"quiet %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{arg[0-9]+}}) };
// LOWERING-NEXT:     return;
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
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag =
// LOWERING-NEXT:                 std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"panicky %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut c: Ctx = Ctx { run: None };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     c.run = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32, i32)>>(
// LOWERING-NEXT:             dispatcher as *const (),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         g_callback = unsafe {
// LOWERING-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32)>>(
// LOWERING-NEXT:                 quiet_callback as *const (),
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         '__loop0: loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             '__continue0: {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag =
// LOWERING-NEXT:                             std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                             unsafe {
// LOWERING-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"recovered quiet %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 =
// LOWERING-NEXT:                                 unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:                             break '__continue0;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32, i32)> = c.run;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         g_callback = unsafe {
// LOWERING-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32)>>(
// LOWERING-NEXT:                 panicky_callback as *const (),
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:     }
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
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag =
// LOWERING-NEXT:                             std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                             unsafe {
// LOWERING-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"recovered panicky %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 =
// LOWERING-NEXT:                                 unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:                             break '__continue1;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32, i32)> = c.run;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i2 = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"failures=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { failures };
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
// REWRITES-NEXT: struct Ctx {
// REWRITES-NEXT:     run: Option<unsafe extern "C" fn(i32, i32)>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __slate_jmp_buf_tag {
// REWRITES-NEXT:     __regs: [i64; 8],
// REWRITES-NEXT:     __mask_was_saved: i32,
// REWRITES-NEXT:     __saved_mask: [u64; 16],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut env: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// REWRITES-NEXT:     [__slate_jmp_buf_tag {
// REWRITES-NEXT:         __regs: [0; 8],
// REWRITES-NEXT:         __mask_was_saved: 0,
// REWRITES-NEXT:         __saved_mask: [0; 16],
// REWRITES-NEXT:     }; 1],
// REWRITES-NEXT: );
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut failures: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut g_callback: Option<unsafe extern "C" fn(i32)> = None;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn dispatcher({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// REWRITES-NEXT:     unsafe { unsafe { g_callback }.unwrap()({{arg[0-9]+}}) };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn quiet_callback({{arg[0-9]+}}: i32) {
// REWRITES-NEXT:     unsafe { printf(c"quiet %d\n".as_ptr(), {{arg[0-9]+}}) };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn panicky_callback(mut x: i32) {
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = x == 2;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             longjmp(
// REWRITES-NEXT:                 std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>()
// REWRITES-NEXT:                     as *mut __slate_jmp_buf_tag,
// REWRITES-NEXT:                 1 as i32,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"panicky %d\n".as_ptr(), x) };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut c: Ctx = Ctx { run: None };
// REWRITES-NEXT:     c.run = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32, i32)>>(
// REWRITES-NEXT:             dispatcher as *const (),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         g_callback = unsafe {
// REWRITES-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32)>>(
// REWRITES-NEXT:                 quiet_callback as *const (),
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let mut i: i32 = 0;
// REWRITES-NEXT:     '__loop0: loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = i < 2;
// REWRITES-NEXT:         if !{{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag =
// REWRITES-NEXT:             std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:             unsafe {
// REWRITES-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             unsafe { printf(c"recovered quiet %d\n".as_ptr(), i) };
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             unsafe { c.run.unwrap()(i, 0 as i32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT:         i += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         g_callback = unsafe {
// REWRITES-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32)>>(
// REWRITES-NEXT:                 panicky_callback as *const (),
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let mut i2: i32 = 0;
// REWRITES-NEXT:     '__loop1: loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = i2 < 5;
// REWRITES-NEXT:         if !{{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag =
// REWRITES-NEXT:             std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:             unsafe {
// REWRITES-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             unsafe { printf(c"recovered panicky %d\n".as_ptr(), i2) };
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             unsafe { c.run.unwrap()(i2, 0 as i32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT:         i2 += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"failures=%d\n".as_ptr(), unsafe { failures }) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
