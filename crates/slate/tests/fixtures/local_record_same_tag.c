#include <stdio.h>

enum Err { ERR_NONE = 0, ERR_BAD = 5 };

static int first(void) {
  struct Rec {
    const char *text;
    enum Err    code;
  };
  struct Rec items[] = {{"a", ERR_NONE}, {"bb", ERR_BAD}};
  int        total   = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].code + (int)items[i].text[0];
  return total;
}

static int second(void) {
  struct Rec {
    unsigned long n;
    const char   *text;
    enum Err      code;
  };
  struct Rec items[] = {{5, "x", ERR_NONE}, {6, "y", ERR_BAD}};
  int        total   = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].n + (int)items[i].code + (int)items[i].text[0];
  return total;
}

int main(void) {
  printf("%d %d\n", first(), second());
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
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum Err_ {
// LOWERING-NEXT:     ERR_NONE = 0,
// LOWERING-NEXT:     ERR_BAD = 5,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Rec {
// LOWERING-NEXT:     text: *mut i8,
// LOWERING-NEXT:     code: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Rec_0 {
// LOWERING-NEXT:     n: u64,
// LOWERING-NEXT:     text: *mut i8,
// LOWERING-NEXT:     code: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn first() -> i32 {
// LOWERING-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Rec; 2]> = aligned::Aligned(
// LOWERING-NEXT:         [Rec {
// LOWERING-NEXT:             text: std::ptr::null_mut(),
// LOWERING-NEXT:             code: 0,
// LOWERING-NEXT:         }; 2],
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     *items = [
// LOWERING-NEXT:         Rec {
// LOWERING-NEXT:             text: b"a\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             code: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         Rec {
// LOWERING-NEXT:             text: b"bb\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             code: 5,
// LOWERING-NEXT:         },
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: u32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = std::mem::size_of::<Rec>() as u64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = items[({{_v[0-9]+}} as usize)].code;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = items[({{_v[0-9]+}} as usize)].text;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             total = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn second() -> i32 {
// LOWERING-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Rec_0; 2]> = aligned::Aligned(
// LOWERING-NEXT:         [Rec_0 {
// LOWERING-NEXT:             n: 0,
// LOWERING-NEXT:             text: std::ptr::null_mut(),
// LOWERING-NEXT:             code: 0,
// LOWERING-NEXT:         }; 2],
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     *items = [
// LOWERING-NEXT:         Rec_0 {
// LOWERING-NEXT:             n: 5,
// LOWERING-NEXT:             text: b"x\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             code: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         Rec_0 {
// LOWERING-NEXT:             n: 6,
// LOWERING-NEXT:             text: b"y\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             code: 5,
// LOWERING-NEXT:         },
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: u32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = 48;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = 24;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = items[({{_v[0-9]+}} as usize)].n;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = items[({{_v[0-9]+}} as usize)].code;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = items[({{_v[0-9]+}} as usize)].text;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             total = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = first();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = second();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum Err_ {
// REWRITES-NEXT:     ERR_NONE = 0,
// REWRITES-NEXT:     ERR_BAD = 5,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Rec {
// REWRITES-NEXT:     text: *mut i8,
// REWRITES-NEXT:     code: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Rec_0 {
// REWRITES-NEXT:     n: u64,
// REWRITES-NEXT:     text: *mut i8,
// REWRITES-NEXT:     code: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn first() -> i32 {
// REWRITES-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Rec; 2]> = aligned::Aligned(
// REWRITES-NEXT:         [Rec {
// REWRITES-NEXT:             text: std::ptr::null_mut(),
// REWRITES-NEXT:             code: 0,
// REWRITES-NEXT:         }; 2],
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     *items = [
// REWRITES-NEXT:         Rec {
// REWRITES-NEXT:             text: c"a".as_ptr() as *mut i8,
// REWRITES-NEXT:             code: 0,
// REWRITES-NEXT:         },
// REWRITES-NEXT:         Rec {
// REWRITES-NEXT:             text: c"bb".as_ptr() as *mut i8,
// REWRITES-NEXT:             code: 5,
// REWRITES-NEXT:         },
// REWRITES-NEXT:     ];
// REWRITES-NEXT:     let mut i: u32 = 0;
// REWRITES-NEXT:     i = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = i as u64;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 32;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = std::mem::size_of::<Rec>() as u64;
// REWRITES-NEXT:         if !({{_v[0-9]+}} < {{_v[0-9]+}} / {{_v[0-9]+}}) {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = items[((i as u64) as usize)].code as i32;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = items[((i as u64) as usize)].text;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:         total += {{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32);
// REWRITES-NEXT:         i += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn second() -> i32 {
// REWRITES-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Rec_0; 2]> = aligned::Aligned(
// REWRITES-NEXT:         [Rec_0 {
// REWRITES-NEXT:             n: 0,
// REWRITES-NEXT:             text: std::ptr::null_mut(),
// REWRITES-NEXT:             code: 0,
// REWRITES-NEXT:         }; 2],
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     *items = [
// REWRITES-NEXT:         Rec_0 {
// REWRITES-NEXT:             n: 5,
// REWRITES-NEXT:             text: c"x".as_ptr() as *mut i8,
// REWRITES-NEXT:             code: 0,
// REWRITES-NEXT:         },
// REWRITES-NEXT:         Rec_0 {
// REWRITES-NEXT:             n: 6,
// REWRITES-NEXT:             text: c"y".as_ptr() as *mut i8,
// REWRITES-NEXT:             code: 5,
// REWRITES-NEXT:         },
// REWRITES-NEXT:     ];
// REWRITES-NEXT:     let mut i: u32 = 0;
// REWRITES-NEXT:     i = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 24;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (i as u64) < 48 / {{_v[0-9]+}};
// REWRITES-NEXT:         if !{{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 =
// REWRITES-NEXT:             (items[((i as u64) as usize)].n as i32) + (items[((i as u64) as usize)].code as i32);
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = items[((i as u64) as usize)].text;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:         total += {{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32);
// REWRITES-NEXT:         i += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), first(), second()) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
