#include <stdio.h>

int main(void) {
  char name[] = "slate_stdio_file_ownership.tmp";
  remove(name);
  FILE *f = fopen(name, "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("owned\n", f);
  fclose(f);

  FILE *g = fopen(name, "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char buf[16] = {0};
  fgets(buf, sizeof buf, g);
  fclose(g);
  fputs(buf, stdout);
  remove(name);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     static mut stdout: *mut libc::FILE;
// LOWERING-NEXT:     fn remove(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// LOWERING-NEXT:     fn puts(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn fgets(_0: *mut i8, _1: i32, _2: *mut libc::FILE) -> *mut i8;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut name: aligned::Aligned<aligned::A16, [i8; 31]> = aligned::Aligned([0; 31]);
// LOWERING-NEXT:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut g: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     *name = [115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 105, 108, 101, 95, 111, 119, 110, 101, 114, 115, 104, 105, 112, 46, 116, 109, 112, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = name.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { remove({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = name.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"w\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = unsafe { fopen({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     f = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = f;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"open-fail\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { puts({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"owned\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = f;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fputs({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = f;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fclose({{_v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = name.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"r\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = unsafe { fopen({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     g = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = g;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"reopen-fail\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { puts({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     *buf = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = g;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { fgets({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = g;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fclose({{_v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stdout };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fputs({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = name.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { remove({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     static mut stdout: *mut libc::FILE;
// REWRITES-NEXT:     fn remove(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// REWRITES-NEXT:     fn puts(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fgets(_0: *mut i8, _1: i32, _2: *mut libc::FILE) -> *mut i8;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut name: aligned::Aligned<aligned::A16, [i8; 31]> = aligned::Aligned([0; 31]);
// REWRITES-NEXT: let mut f: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut g: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut buf: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: *name = [115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 105, 108, 101, 95, 111, 119, 110, 101, 114, 115, 104, 105, 112, 46, 116, 109, 112, 0];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = name.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { remove({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = name.as_mut_ptr() as *mut i8;
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
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"owned\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { fputs({{_v[0-9]+}} as *const i8, f as *mut libc::FILE) };
// REWRITES-NEXT: unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = name.as_mut_ptr() as *mut i8;
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
// REWRITES-NEXT: *buf = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 16;
// REWRITES-NEXT: unsafe { fgets({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as i32, g as *mut libc::FILE) };
// REWRITES-NEXT: unsafe { fclose(g as *mut libc::FILE) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { fputs({{_v[0-9]+}} as *const i8, (unsafe { stdout }) as *mut libc::FILE) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = name.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { remove({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
