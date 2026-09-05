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
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum Err_ {
// COMMON-LOWERING-NEXT:     ERR_NONE = 0,
// COMMON-LOWERING-NEXT:     ERR_BAD = 5,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Rec {
// COMMON-LOWERING-NEXT:     code: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Rec_0 {
// COMMON-LOWERING-NEXT:     n: u64,
// COMMON-LOWERING-NEXT:     code: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = first();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = second();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn first() -> i32 {
// COMMON-LOWERING-NEXT:         [Rec {
// COMMON-LOWERING-NEXT:             text: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             code: 0,
// COMMON-LOWERING-NEXT:         }; 2],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [Rec; 2] = [
// COMMON-LOWERING-NEXT:         Rec {
// COMMON-LOWERING-NEXT:             code: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         Rec {
// COMMON-LOWERING-NEXT:             code: 5,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     *items = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: u32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = std::mem::size_of::<Rec>() as u64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} / {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = items[({{__v[0-9]+}} as usize)].code;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn second() -> i32 {
// COMMON-LOWERING-NEXT:         [Rec_0 {
// COMMON-LOWERING-NEXT:             n: 0,
// COMMON-LOWERING-NEXT:             text: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             code: 0,
// COMMON-LOWERING-NEXT:         }; 2],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [Rec_0; 2] = [
// COMMON-LOWERING-NEXT:         Rec_0 {
// COMMON-LOWERING-NEXT:             n: 5,
// COMMON-LOWERING-NEXT:             code: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         Rec_0 {
// COMMON-LOWERING-NEXT:             n: 6,
// COMMON-LOWERING-NEXT:             code: 5,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     *items = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: u32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 48;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 24;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} / {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = items[({{__v[0-9]+}} as usize)].n;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = items[({{__v[0-9]+}} as usize)].code;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     text: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     text: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Rec; 2]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:             text: b"a\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             text: b"bb\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = items[({{__v[0-9]+}} as usize)].text;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Rec_0; 2]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:             text: b"x\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             text: b"y\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = items[({{__v[0-9]+}} as usize)].text;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     text: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     text: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A8, [Rec; 2]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:             text: b"a\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             text: b"bb\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = items[({{__v[0-9]+}} as usize)].text;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A8, [Rec_0; 2]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:             text: b"x\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             text: b"y\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = items[({{__v[0-9]+}} as usize)].text;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
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
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum Err_ {
// COMMON-REWRITES-NEXT:     ERR_NONE = 0,
// COMMON-REWRITES-NEXT:     ERR_BAD = 5,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Rec {
// COMMON-REWRITES-NEXT:     code: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Rec_0 {
// COMMON-REWRITES-NEXT:     n: u64,
// COMMON-REWRITES-NEXT:     code: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), first(), second()) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn first() -> i32 {
// COMMON-REWRITES-NEXT:         [Rec {
// COMMON-REWRITES-NEXT:             text: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             code: 0,
// COMMON-REWRITES-NEXT:         }; 2],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     *items = [
// COMMON-REWRITES-NEXT:         Rec {
// COMMON-REWRITES-NEXT:             code: 0,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         Rec {
// COMMON-REWRITES-NEXT:             code: 5,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     let mut i: u32 = 0;
// COMMON-REWRITES-NEXT:     i = 0;
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = i as u64;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = 32;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = std::mem::size_of::<Rec>() as u64;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} < {{__v[0-9]+}} / {{__v[0-9]+}}) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = items[((i as u64) as usize)].code as i32;
// COMMON-REWRITES-NEXT:         total += {{__v[0-9]+}} + ((unsafe { *{{__v[0-9]+}} }) as i32);
// COMMON-REWRITES-NEXT:         i += 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn second() -> i32 {
// COMMON-REWRITES-NEXT:         [Rec_0 {
// COMMON-REWRITES-NEXT:             n: 0,
// COMMON-REWRITES-NEXT:             text: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             code: 0,
// COMMON-REWRITES-NEXT:         }; 2],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     *items = [
// COMMON-REWRITES-NEXT:         Rec_0 {
// COMMON-REWRITES-NEXT:             n: 5,
// COMMON-REWRITES-NEXT:             code: 0,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         Rec_0 {
// COMMON-REWRITES-NEXT:             n: 6,
// COMMON-REWRITES-NEXT:             code: 5,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     let mut i: u32 = 0;
// COMMON-REWRITES-NEXT:     i = 0;
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = 24;
// COMMON-REWRITES-NEXT:         if !((i as u64) < 48 / {{__v[0-9]+}}) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 =
// COMMON-REWRITES-NEXT:             (items[((i as u64) as usize)].n as i32) + (items[((i as u64) as usize)].code as i32);
// COMMON-REWRITES-NEXT:         total += {{__v[0-9]+}} + ((unsafe { *{{__v[0-9]+}} }) as i32);
// COMMON-REWRITES-NEXT:         i += 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     text: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     text: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Rec; 2]> = aligned::Aligned(
// REWRITES-X86_64-GNU-NEXT:             text: c"a".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             text: c"bb".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = items[((i as u64) as usize)].text;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-X86_64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Rec_0; 2]> = aligned::Aligned(
// REWRITES-X86_64-GNU-NEXT:             text: c"x".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             text: c"y".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = items[((i as u64) as usize)].text;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     text: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     text: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A8, [Rec; 2]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-NEXT:             text: c"a".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             text: c"bb".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = items[((i as u64) as usize)].text;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-AARCH64-GNU-NEXT:     let mut items: aligned::Aligned<aligned::A8, [Rec_0; 2]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-NEXT:             text: c"x".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             text: c"y".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = items[((i as u64) as usize)].text;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
