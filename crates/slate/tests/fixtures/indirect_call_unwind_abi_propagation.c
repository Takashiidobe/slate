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
// COMMON-LOWERING-NEXT: struct Ctx {
// COMMON-LOWERING-NEXT:     run: Option<unsafe extern "C-unwind" fn(i32, i32)>,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct __slate_jmp_buf_tag {
// COMMON-LOWERING-NEXT:     __regs: [i64; 8],
// COMMON-LOWERING-NEXT:     __mask_was_saved: i32,
// COMMON-LOWERING-NEXT:     __saved_mask: [u64; 16],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     [__slate_jmp_buf_tag {
// COMMON-LOWERING-NEXT:         __regs: [0; 8],
// COMMON-LOWERING-NEXT:         __mask_was_saved: 0,
// COMMON-LOWERING-NEXT:         __saved_mask: [0; 16],
// COMMON-LOWERING-NEXT:     }; 1],
// COMMON-LOWERING-NEXT: );
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut failures: i32 = 0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut g_callback: Option<unsafe extern "C-unwind" fn(i32)> = None;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut c: Ctx = Ctx { run: None };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     c.run = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32, i32)>>(
// COMMON-LOWERING-NEXT:             dispatcher as *const (),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         g_callback = unsafe {
// COMMON-LOWERING-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32)>>(
// COMMON-LOWERING-NEXT:                 quiet_callback as *const (),
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         '__loop0: loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             '__continue0: {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: *mut __slate_jmp_buf_tag =
// COMMON-LOWERING-NEXT:                             std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = unsafe { setjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:                         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = unsafe { failures };
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                             unsafe {
// COMMON-LOWERING-NEXT:                                 failures = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                             }
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                                 unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                             break '__continue0;
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, i32)> = c.run;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                     unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         g_callback = unsafe {
// COMMON-LOWERING-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32)>>(
// COMMON-LOWERING-NEXT:                 panicky_callback as *const (),
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i2: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         '__loop1: loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             '__continue1: {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: *mut __slate_jmp_buf_tag =
// COMMON-LOWERING-NEXT:                             std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = unsafe { setjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:                         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = unsafe { failures };
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                             unsafe {
// COMMON-LOWERING-NEXT:                                 failures = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                             }
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                                 unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                             break '__continue1;
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, i32)> = c.run;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                     unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { failures };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn dispatcher({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32)> = unsafe { g_callback };
// COMMON-LOWERING-NEXT:     unsafe { {{__v[0-9]+}}.unwrap()({{arg[0-9]+}}) };
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn quiet_callback({{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{arg[0-9]+}}) };
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn panicky_callback({{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let mut x: i32 = 0;
// COMMON-LOWERING-NEXT:     x = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut __slate_jmp_buf_tag =
// COMMON-LOWERING-NEXT:                 std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:             unsafe { longjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag, {{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: static mut env: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:                             let {{__v[0-9]+}}: *mut i8 = b"recovered quiet %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{__v[0-9]+}}: *mut i8 = b"recovered panicky %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"failures=%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"quiet %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"panicky %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: static mut env: aligned::Aligned<aligned::A8, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:                             let {{__v[0-9]+}}: *mut u8 = b"recovered quiet %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                             let {{__v[0-9]+}}: *mut u8 = b"recovered panicky %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"failures=%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"quiet %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"panicky %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: struct Ctx {
// COMMON-REWRITES-NEXT:     run: Option<unsafe extern "C-unwind" fn(i32, i32)>,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct __slate_jmp_buf_tag {
// COMMON-REWRITES-NEXT:     __regs: [i64; 8],
// COMMON-REWRITES-NEXT:     __mask_was_saved: i32,
// COMMON-REWRITES-NEXT:     __saved_mask: [u64; 16],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     [__slate_jmp_buf_tag {
// COMMON-REWRITES-NEXT:         __regs: [0; 8],
// COMMON-REWRITES-NEXT:         __mask_was_saved: 0,
// COMMON-REWRITES-NEXT:         __saved_mask: [0; 16],
// COMMON-REWRITES-NEXT:     }; 1],
// COMMON-REWRITES-NEXT: );
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut failures: i32 = 0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut g_callback: Option<unsafe extern "C-unwind" fn(i32)> = None;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut c: Ctx = Ctx { run: None };
// COMMON-REWRITES-NEXT:     c.run = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32, i32)>>(
// COMMON-REWRITES-NEXT:             dispatcher as *const (),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         g_callback = unsafe {
// COMMON-REWRITES-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32)>>(
// COMMON-REWRITES-NEXT:                 quiet_callback as *const (),
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let mut i: i32 = 0;
// COMMON-REWRITES-NEXT:     '__loop0: while i < 2 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut __slate_jmp_buf_tag =
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { setjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             unsafe {
// COMMON-REWRITES-NEXT:                 failures = (unsafe { failures }) + 1;
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:             unsafe { printf(c"recovered quiet %d\n".as_ptr(), i) };
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             unsafe { c.run.unwrap()(i, 0 as i32) };
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         i += 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         g_callback = unsafe {
// COMMON-REWRITES-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32)>>(
// COMMON-REWRITES-NEXT:                 panicky_callback as *const (),
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let mut i2: i32 = 0;
// COMMON-REWRITES-NEXT:     '__loop1: while i2 < 5 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut __slate_jmp_buf_tag =
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { setjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             unsafe {
// COMMON-REWRITES-NEXT:                 failures = (unsafe { failures }) + 1;
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:             unsafe { printf(c"recovered panicky %d\n".as_ptr(), i2) };
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             unsafe { c.run.unwrap()(i2, 0 as i32) };
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         i2 += 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"failures=%d\n".as_ptr(), unsafe { failures }) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn dispatcher({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// COMMON-REWRITES-NEXT:     unsafe { unsafe { g_callback }.unwrap()({{arg[0-9]+}}) };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn quiet_callback({{arg[0-9]+}}: i32) {
// COMMON-REWRITES-NEXT:     unsafe { printf(c"quiet %d\n".as_ptr(), {{arg[0-9]+}}) };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn panicky_callback(mut x: i32) {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = x == 2;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             longjmp(
// COMMON-REWRITES-NEXT:                 std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>()
// COMMON-REWRITES-NEXT:                     as *mut __slate_jmp_buf_tag,
// COMMON-REWRITES-NEXT:                 1 as i32,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"panicky %d\n".as_ptr(), x) };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: static mut env: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: static mut env: aligned::Aligned<aligned::A8, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// SLATE-FILECHECK-END rewrites-aarch64-gnu
