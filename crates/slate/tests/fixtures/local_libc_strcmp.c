#include <stdio.h>
#include <string.h>

static int cmp_texts(const char *a, int alen, const char *b, int blen) {
  int sa = 0, sb = 0;
  for (int i = 0; i < alen; i++)
    sa += a[i];
  for (int i = 0; i < blen; i++)
    sb += b[i];
  int order = strcmp(a, b);
  int sign  = (order > 0) - (order < 0);
  int eq    = strcmp(a, b) == 0;
  return sign * 1000 + eq * 100 + (sa - sb);
}

int main(void) {
  const char x[] = "abc";
  const char y[] = "abd";
  printf("%d %d %d\n", cmp_texts(x, 3, y, 3), cmp_texts(y, 3, x, 3),
         cmp_texts(x, 3, x, 3));
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
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     y = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = cmp_texts({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = cmp_texts({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = cmp_texts({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut alen: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut blen: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut sa: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut sb: i32 = 0;
// COMMON-LOWERING-NEXT:     a = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     alen = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     b = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     blen = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     sa = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     sb = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = alen;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = sa;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             sa = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i2: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = blen;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = sb;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             sb = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1000;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sa;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sb;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut x: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let mut y: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 99, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 100, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = y.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = y.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: fn cmp_texts({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut a: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut b: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = a;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut x: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut y: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 99, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = x.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = y.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = y.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = x.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = x.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = x.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT: fn cmp_texts({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-AARCH64-GNU-NEXT:     let mut a: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut b: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = a;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b;
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
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     x = [97, 98, 99, 0];
// COMMON-REWRITES-NEXT:     y = [97, 98, 100, 0];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = cmp_texts(
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const u8,
// COMMON-REWRITES-NEXT:                 (3 as i32) as usize,
// COMMON-REWRITES-NEXT:             ))
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const u8,
// COMMON-REWRITES-NEXT:                 (3 as i32) as usize,
// COMMON-REWRITES-NEXT:             ))
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = cmp_texts(
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const u8,
// COMMON-REWRITES-NEXT:                 (3 as i32) as usize,
// COMMON-REWRITES-NEXT:             ))
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const u8,
// COMMON-REWRITES-NEXT:                 (3 as i32) as usize,
// COMMON-REWRITES-NEXT:             ))
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             cmp_texts(
// COMMON-REWRITES-NEXT:                 unsafe {
// COMMON-REWRITES-NEXT:                     std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// COMMON-REWRITES-NEXT:                         {{__v[0-9]+}} as *const u8,
// COMMON-REWRITES-NEXT:                         (3 as i32) as usize,
// COMMON-REWRITES-NEXT:                     ))
// COMMON-REWRITES-NEXT:                 },
// COMMON-REWRITES-NEXT:                 unsafe {
// COMMON-REWRITES-NEXT:                     std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// COMMON-REWRITES-NEXT:                         {{__v[0-9]+}} as *const u8,
// COMMON-REWRITES-NEXT:                         (3 as i32) as usize,
// COMMON-REWRITES-NEXT:                     ))
// COMMON-REWRITES-NEXT:                 },
// COMMON-REWRITES-NEXT:             ),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn cmp_texts({{arg[0-9]+}}: &str, {{arg[0-9]+}}: &str) -> i32 {
// COMMON-REWRITES-NEXT:     let mut alen: i32 = {{arg[0-9]+}}.len() as i32;
// COMMON-REWRITES-NEXT:     let mut blen: i32 = {{arg[0-9]+}}.len() as i32;
// COMMON-REWRITES-NEXT:     let mut sa: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut sb: i32 = 0;
// COMMON-REWRITES-NEXT:     for i in 0..alen {
// COMMON-REWRITES-NEXT:         sa += (unsafe { *{{__v[0-9]+}} }) as i32;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     for i2 in 0..blen {
// COMMON-REWRITES-NEXT:         sb += (unsafe { *{{__v[0-9]+}} }) as i32;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{arg[0-9]+}}.cmp({{arg[0-9]+}}) as i32 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (({{__v[0-9]+}} > 0) as i32) - (({{__v[0-9]+}} < 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{arg[0-9]+}}.cmp({{arg[0-9]+}}) as i32 };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} * 1000 + (({{__v[0-9]+}} == 0) as i32) * 100 + (sa - sb)
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut x: [i8; 4] = [0; 4];
// REWRITES-X86_64-GNU-NEXT:     let mut y: [i8; 4] = [0; 4];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = y.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = y.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let mut a: *mut i8 = {{arg[0-9]+}}.as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let mut b: *mut i8 = {{arg[0-9]+}}.as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset((i2 as i64) as isize) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut x: [u8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut y: [u8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = x.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = y.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = y.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = x.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = x.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = x.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut a: *mut u8 = {{arg[0-9]+}}.as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut b: *mut u8 = {{arg[0-9]+}}.as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset((i2 as i64) as isize) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
