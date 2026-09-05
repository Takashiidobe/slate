#include <stdio.h>

int main(void) {
  remove("slate_stdio_fread_short_read.tmp");
  FILE *f = fopen("slate_stdio_fread_short_read.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("abcdefghi", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_fread_short_read.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char   buf[16] = {0};
  size_t n       = fread(buf, 4, 4, g);
  printf("%zu %s\n", n, buf);
  fclose(g);
  remove("slate_stdio_fread_short_read.tmp");
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
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn remove(_0: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn fopen(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut libc::FILE;
// COMMON-LOWERING-NEXT:     fn puts(_0: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn fputs(_0: *const core::ffi::c_char, _1: *mut libc::FILE) -> i32;
// COMMON-LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// COMMON-LOWERING-NEXT:     fn fread(_0: *mut core::ffi::c_void, _1: usize, _2: usize, _3: *mut libc::FILE) -> usize;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut g: *mut libc::FILE = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { remove({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// COMMON-LOWERING-NEXT:         fopen(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     f = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut libc::FILE = f;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { puts({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = f;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fputs({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = f;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// COMMON-LOWERING-NEXT:         fopen(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     g = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut libc::FILE = g;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { puts({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = g;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         fread(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = g;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { remove({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate_stdio_fread_short_read.tmp\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate_stdio_fread_short_read.tmp\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"w\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"open-fail\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"abcdefghi\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate_stdio_fread_short_read.tmp\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"r\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"reopen-fail\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [0; 16];
// LOWERING-X86_64-GNU-NEXT:     *buf = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate_stdio_fread_short_read.tmp\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut buf: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate_stdio_fread_short_read.tmp\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate_stdio_fread_short_read.tmp\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"w\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"open-fail\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"abcdefghi\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate_stdio_fread_short_read.tmp\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"r\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"reopen-fail\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     buf = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %s\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate_stdio_fread_short_read.tmp\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn remove(_0: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn fopen(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut libc::FILE;
// COMMON-REWRITES-NEXT:     fn puts(_0: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn fputs(_0: *const core::ffi::c_char, _1: *mut libc::FILE) -> i32;
// COMMON-REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// COMMON-REWRITES-NEXT:     fn fread(_0: *mut core::ffi::c_void, _1: usize, _2: usize, _3: *mut libc::FILE) -> usize;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut g: *mut libc::FILE = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     unsafe { remove(c"slate_stdio_fread_short_read.tmp".as_ptr()) };
// COMMON-REWRITES-NEXT:     f = unsafe { fopen(c"slate_stdio_fread_short_read.tmp".as_ptr(), c"w".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !(f != std::ptr::null_mut());
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { puts(c"open-fail".as_ptr()) };
// COMMON-REWRITES-NEXT:         __retval = 0;
// COMMON-REWRITES-NEXT:         std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { fputs(c"abcdefghi".as_ptr(), f as *mut libc::FILE) };
// COMMON-REWRITES-NEXT:     unsafe { fclose(f as *mut libc::FILE) };
// COMMON-REWRITES-NEXT:     g = unsafe { fopen(c"slate_stdio_fread_short_read.tmp".as_ptr(), c"r".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !(g != std::ptr::null_mut());
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { puts(c"reopen-fail".as_ptr()) };
// COMMON-REWRITES-NEXT:         __retval = 0;
// COMMON-REWRITES-NEXT:         std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%zu %s\n".as_ptr(),
// COMMON-REWRITES-NEXT:             (unsafe {
// COMMON-REWRITES-NEXT:                 fread(
// COMMON-REWRITES-NEXT:                     {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:                     (4 as u64) as usize,
// COMMON-REWRITES-NEXT:                     (4 as u64) as usize,
// COMMON-REWRITES-NEXT:                     g as *mut libc::FILE,
// COMMON-REWRITES-NEXT:                 )
// COMMON-REWRITES-NEXT:             }) as u64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { fclose(g as *mut libc::FILE) };
// COMMON-REWRITES-NEXT:     unsafe { remove(c"slate_stdio_fread_short_read.tmp".as_ptr()) };
// COMMON-REWRITES-NEXT:     __retval = 0;
// COMMON-REWRITES-NEXT:     std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-X86_64-GNU-NEXT:     *buf = [0; 16];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:             buf.as_mut_ptr() as *mut i8,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut buf: [u8; 16] = [0; 16];
// REWRITES-AARCH64-GNU-NEXT:     buf = [0; 16];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:             buf.as_mut_ptr() as *mut u8,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
