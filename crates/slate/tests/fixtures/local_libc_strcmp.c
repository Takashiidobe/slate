#include <stdio.h>
#include <string.h>

static int cmp_texts(const char *a, int alen, const char *b, int blen) {
  int sa = 0, sb = 0;
  for (int i = 0; i < alen; i++) sa += a[i];
  for (int i = 0; i < blen; i++) sb += b[i];
  int order = strcmp(a, b);
  int sign = (order > 0) - (order < 0);
  int eq = strcmp(a, b) == 0;
  return sign * 1000 + eq * 100 + (sa - sb);
}

int main(void) {
  const char x[] = "abc";
  const char y[] = "abd";
  printf("%d %d %d\n", cmp_texts(x, 3, y, 3), cmp_texts(y, 3, x, 3),
         cmp_texts(x, 3, x, 3));
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
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn cmp_texts({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut a: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut alen: i32 = 0;
// LOWERING-NEXT:     let mut b: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut blen: i32 = 0;
// LOWERING-NEXT:     let mut sa: i32 = 0;
// LOWERING-NEXT:     let mut sb: i32 = 0;
// LOWERING-NEXT:     a = {{arg[0-9]+}};
// LOWERING-NEXT:     alen = {{arg[0-9]+}};
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     blen = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     sa = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     sb = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = alen;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = a;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = sa;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             sa = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i2: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i2 = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = blen;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = sb;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             sb = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i2 = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1000;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sa;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sb;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut x: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut y: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     x = [97, 98, 99, 0];
// LOWERING-NEXT:     y = [97, 98, 100, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = y.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = cmp_texts({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = y.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = cmp_texts({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = cmp_texts({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn cmp_texts({{arg[0-9]+}}: &str, {{arg[0-9]+}}: &str) -> i32 {
// REWRITES-NEXT:     let mut a: *mut i8 = {{arg[0-9]+}}.as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut alen: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     let mut b: *mut i8 = {{arg[0-9]+}}.as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut blen: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     let mut sa: i32 = 0;
// REWRITES-NEXT:     let mut sb: i32 = 0;
// REWRITES-NEXT:     sa = 0;
// REWRITES-NEXT:     sb = 0;
// REWRITES-NEXT:     {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:             if !(i < alen) {
// REWRITES-NEXT:                 break;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut i8 = a;
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:             sa = sa + ((unsafe { *{{_v[0-9]+}} }) as i32);
// REWRITES-NEXT:             i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {
// REWRITES-NEXT:         let mut i2: i32 = 0;
// REWRITES-NEXT:         i2 = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:             if !(i2 < blen) {
// REWRITES-NEXT:                 break;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut i8 = b;
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset((i2 as i64) as isize) };
// REWRITES-NEXT:             sb = sb + ((unsafe { *{{_v[0-9]+}} }) as i32);
// REWRITES-NEXT:             i2 = i2 + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{arg[0-9]+}}.cmp({{arg[0-9]+}}) as i32 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (({{_v[0-9]+}} > {{_v[0-9]+}}) as i32) - (({{_v[0-9]+}} < {{_v[0-9]+}}) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{arg[0-9]+}}.cmp({{arg[0-9]+}}) as i32 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1000;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// REWRITES-NEXT:     return {{_v[0-9]+}} * {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) * {{_v[0-9]+}} + (sa - sb);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut x: [i8; 4] = [97, 98, 99, 0];
// REWRITES-NEXT:     let mut y: [i8; 4] = [97, 98, 100, 0];
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = y.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = cmp_texts(
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// REWRITES-NEXT:                 {{_v[0-9]+}} as *const u8,
// REWRITES-NEXT:                 (3 as i32) as usize,
// REWRITES-NEXT:             ))
// REWRITES-NEXT:         },
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// REWRITES-NEXT:                 {{_v[0-9]+}} as *const u8,
// REWRITES-NEXT:                 (3 as i32) as usize,
// REWRITES-NEXT:             ))
// REWRITES-NEXT:         },
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = y.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = cmp_texts(
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// REWRITES-NEXT:                 {{_v[0-9]+}} as *const u8,
// REWRITES-NEXT:                 (3 as i32) as usize,
// REWRITES-NEXT:             ))
// REWRITES-NEXT:         },
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// REWRITES-NEXT:                 {{_v[0-9]+}} as *const u8,
// REWRITES-NEXT:                 (3 as i32) as usize,
// REWRITES-NEXT:             ))
// REWRITES-NEXT:         },
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = x.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             cmp_texts(
// REWRITES-NEXT:                 unsafe {
// REWRITES-NEXT:                     std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// REWRITES-NEXT:                         {{_v[0-9]+}} as *const u8,
// REWRITES-NEXT:                         (3 as i32) as usize,
// REWRITES-NEXT:                     ))
// REWRITES-NEXT:                 },
// REWRITES-NEXT:                 unsafe {
// REWRITES-NEXT:                     std::str::from_utf8_unchecked(std::slice::from_raw_parts(
// REWRITES-NEXT:                         {{_v[0-9]+}} as *const u8,
// REWRITES-NEXT:                         (3 as i32) as usize,
// REWRITES-NEXT:                     ))
// REWRITES-NEXT:                 },
// REWRITES-NEXT:             ),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
