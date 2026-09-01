#include <stdio.h>
#include <string.h>

int main(void) {
  remove("slate_stdio_gets_loop_unsupported.tmp");
  FILE *f = fopen("slate_stdio_gets_loop_unsupported.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("one\n", f);
  fputs("two\n", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_gets_loop_unsupported.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char line[64];
  int  count = 0;
  while (fgets(line, sizeof line, g) != NULL) {
    count += (int)strlen(line);
  }
  fclose(g);
  printf("%d\n", count);
  remove("slate_stdio_gets_loop_unsupported.tmp");
  return 0;
}

// LOWERING-DAG: fn strlen(_0: *const i8) -> usize;
// LOWERING-NOT: safe fn strlen(

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
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn remove(_0: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn fopen(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut libc::FILE;
// REWRITES-NEXT:     fn puts(_0: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn fputs(_0: *const core::ffi::c_char, _1: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fgets(_0: *mut core::ffi::c_char, _1: i32, _2: *mut libc::FILE) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT:     let mut g: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT:     let mut line: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-NEXT:     let mut count: i32 = 0;
// REWRITES-NEXT:     __retval = 0;
// REWRITES-NEXT:     unsafe { remove(c"slate_stdio_gets_loop_unsupported.tmp".as_ptr()) };
// REWRITES-NEXT:     f = unsafe {
// REWRITES-NEXT:         fopen(
// REWRITES-NEXT:             c"slate_stdio_gets_loop_unsupported.tmp".as_ptr(),
// REWRITES-NEXT:             c"w".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = f != std::ptr::null_mut();
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:             unsafe { puts(c"open-fail".as_ptr()) };
// REWRITES-NEXT:             __retval = 0;
// REWRITES-NEXT:             std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { fputs(c"one\n".as_ptr(), f as *mut libc::FILE) };
// REWRITES-NEXT:     unsafe { fputs(c"two\n".as_ptr(), f as *mut libc::FILE) };
// REWRITES-NEXT:     unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-NEXT:     g = unsafe {
// REWRITES-NEXT:         fopen(
// REWRITES-NEXT:             c"slate_stdio_gets_loop_unsupported.tmp".as_ptr(),
// REWRITES-NEXT:             c"r".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = g != std::ptr::null_mut();
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:             unsafe { puts(c"reopen-fail".as_ptr()) };
// REWRITES-NEXT:             __retval = 0;
// REWRITES-NEXT:             std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = line.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-NEXT:             fgets(
// REWRITES-NEXT:                 {{_v[0-9]+}} as *mut core::ffi::c_char,
// REWRITES-NEXT:                 64 as i32,
// REWRITES-NEXT:                 g as *mut libc::FILE,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         }) as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:         if !({{_v[0-9]+}} != ({{_v[0-9]+}} as *mut i8)) {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut i8 = line.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:             let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// REWRITES-NEXT:             count = count + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { fclose(g as *mut libc::FILE) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), count) };
// REWRITES-NEXT:     unsafe { remove(c"slate_stdio_gets_loop_unsupported.tmp".as_ptr()) };
// REWRITES-NEXT:     __retval = 0;
// REWRITES-NEXT:     std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
