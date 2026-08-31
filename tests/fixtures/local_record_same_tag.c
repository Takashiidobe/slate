#include <stdio.h>

enum Err { ERR_NONE = 0, ERR_BAD = 5 };

static int first(void) {
  struct Rec {
    const char *text;
    enum Err code;
  };
  struct Rec items[] = {{"a", ERR_NONE}, {"bb", ERR_BAD}};
  int total = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].code + (int)items[i].text[0];
  return total;
}

static int second(void) {
  struct Rec {
    unsigned long n;
    const char *text;
    enum Err code;
  };
  struct Rec items[] = {{5, "x", ERR_NONE}, {6, "y", ERR_BAD}};
  int total = 0;
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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum Err_ {
// LOWERING-NEXT:     ERR_NONE = 0,
// LOWERING-NEXT:     ERR_BAD = 5,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Rec {
// LOWERING-NEXT:     text: *mut i8,
// LOWERING-NEXT:     code: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Rec_0 {
// LOWERING-NEXT:     n: u64,
// LOWERING-NEXT:     text: *mut i8,
// LOWERING-NEXT:     code: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn first() -> i32 {
// LOWERING-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Rec; 2]> = aligned::Aligned([Rec { text: std::ptr::null_mut(), code: 0 }; 2]);
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     *items = [Rec { text: b"a\0".as_ptr() as *mut i8, code: 0 }, Rec { text: b"bb\0".as_ptr() as *mut i8, code: 5 }];
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
// LOWERING-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Rec_0; 2]> = aligned::Aligned([Rec_0 { n: 0, text: std::ptr::null_mut(), code: 0 }; 2]);
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     *items = [Rec_0 { n: 5, text: b"x\0".as_ptr() as *mut i8, code: 0 }, Rec_0 { n: 6, text: b"y\0".as_ptr() as *mut i8, code: 5 }];
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum Err_ {
// REWRITES-NEXT:     ERR_NONE = 0,
// REWRITES-NEXT:     ERR_BAD = 5,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Rec {
// REWRITES-NEXT:     text: *mut i8,
// REWRITES-NEXT:     code: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Rec_0 {
// REWRITES-NEXT:     n: u64,
// REWRITES-NEXT:     text: *mut i8,
// REWRITES-NEXT:     code: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn first() -> i32 {
// REWRITES-NEXT: let mut items: aligned::Aligned<aligned::A16, [Rec; 2]> = aligned::Aligned([Rec { text: std::ptr::null_mut(), code: 0 }; 2]);
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: *items = [Rec { text: b"a\0".as_ptr() as *mut i8, code: 0 }, Rec { text: b"bb\0".as_ptr() as *mut i8, code: 5 }];
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: u32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: u64 = i as u64;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: u64 = 32;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: u64 = std::mem::size_of::<Rec>() as u64;
// REWRITES-NEXT:                     if !({{_v[0-9]+}} < {{_v[0-9]+}} / {{_v[0-9]+}}) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = items[((i as u64) as usize)].code as i32;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = items[((i as u64) as usize)].text;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:                     total = total + ({{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32));
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn second() -> i32 {
// REWRITES-NEXT: let mut items: aligned::Aligned<aligned::A16, [Rec_0; 2]> = aligned::Aligned([Rec_0 { n: 0, text: std::ptr::null_mut(), code: 0 }; 2]);
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: *items = [Rec_0 { n: 5, text: b"x\0".as_ptr() as *mut i8, code: 0 }, Rec_0 { n: 6, text: b"y\0".as_ptr() as *mut i8, code: 5 }];
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: u32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: u64 = 48;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: u64 = 24;
// REWRITES-NEXT:                     if !((i as u64) < {{_v[0-9]+}} / {{_v[0-9]+}}) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = (items[((i as u64) as usize)].n as i32) + (items[((i as u64) as usize)].code as i32);
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = items[((i as u64) as usize)].text;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:                     total = total + ({{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32));
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = first();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = second();
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
