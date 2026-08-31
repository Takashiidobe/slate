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

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn remove(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// REWRITES-NEXT:     fn puts(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fgets(_0: *mut i8, _1: i32, _2: *mut libc::FILE) -> *mut i8;
// REWRITES-NEXT:     fn strlen(_0: *const i8) -> usize;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut f: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut g: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut line: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-NEXT: let mut count: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate_stdio_gets_loop_unsupported.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { remove({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate_stdio_gets_loop_unsupported.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"w\0".as_ptr() as *mut i8;
// REWRITES-NEXT: f = unsafe { fopen({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = f != std::ptr::null_mut();
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b"open-fail\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     unsafe { puts({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"one\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { fputs({{_v[0-9]+}} as *const i8, f as *mut libc::FILE) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"two\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { fputs({{_v[0-9]+}} as *const i8, f as *mut libc::FILE) };
// REWRITES-NEXT: unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate_stdio_gets_loop_unsupported.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"r\0".as_ptr() as *mut i8;
// REWRITES-NEXT: g = unsafe { fopen({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = g != std::ptr::null_mut();
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b"reopen-fail\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     unsafe { puts({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = line.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 64;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = unsafe { fgets({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as i32, g as *mut libc::FILE) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:         if !({{_v[0-9]+}} != ({{_v[0-9]+}} as *mut i8)) {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = line.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const i8) }) as u64;
// REWRITES-NEXT:                     count = count + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe { fclose(g as *mut libc::FILE) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, count) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate_stdio_gets_loop_unsupported.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { remove({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites

// LOWERING-DAG: fn strlen(_0: *const i8) -> usize;
// LOWERING-NOT: safe fn strlen(
