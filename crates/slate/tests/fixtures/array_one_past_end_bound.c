#include <stdio.h>

struct cursor {
  unsigned char *p;
  unsigned char  buf[8];
};

static void fill(struct cursor *c, unsigned char n) {
  c->p = c->buf;
  while (c->p < &c->buf[sizeof(c->buf)]) {
    *c->p = n;
    n++;
    c->p++;
  }
}

int main(void) {
  struct cursor c;
  fill(&c, 1);

  int total = 0;
  for (int i = 0; i < 8; i++) {
    total += c.buf[i];
  }
  printf("%d\n", total);
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
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct cursor {
// LOWERING-NEXT:     p: *mut u8,
// LOWERING-NEXT:     buf: [u8; 8],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill({{arg[0-9]+}}: *mut cursor, {{arg[0-9]+}}: u8) {
// LOWERING-NEXT:     let mut c: *mut cursor = std::ptr::null_mut();
// LOWERING-NEXT:     let mut n: u8 = 0;
// LOWERING-NEXT:     c = {{arg[0-9]+}};
// LOWERING-NEXT:     n = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut cursor = c;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*{{_v[0-9]+}}).buf) }) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut cursor = c;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{_v[0-9]+}}).p = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut cursor = c;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut u8 = unsafe { (*{{_v[0-9]+}}).p };
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut cursor = c;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool =
// LOWERING-NEXT:                 {{_v[0-9]+}} < unsafe { std::ptr::addr_of_mut!(*(*{{_v[0-9]+}}).buf.as_mut_ptr().add({{_v[0-9]+}} as usize)) };
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u8 = n;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut cursor = c;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut u8 = unsafe { (*{{_v[0-9]+}}).p };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u8 = n;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                 n = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut cursor = c;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut u8 = unsafe { (*{{_v[0-9]+}}).p };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     (*{{_v[0-9]+}}).p = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut c: cursor = cursor {
// LOWERING-NEXT:         p: std::ptr::null_mut(),
// LOWERING-NEXT:         buf: [0; 8],
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 1;
// LOWERING-NEXT:     fill(std::ptr::addr_of_mut!(c), {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u8 = c.buf[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 total = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct cursor {
// REWRITES-NEXT:     p: *mut u8,
// REWRITES-NEXT:     buf: [u8; 8],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill({{arg[0-9]+}}: &mut cursor, {{arg[0-9]+}}: u8) {
// REWRITES-NEXT:     let mut c: *mut cursor = {{arg[0-9]+}} as *mut cursor;
// REWRITES-NEXT:     let mut n: u8 = {{arg[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*c).buf) }) as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*c).p = {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut u8 = unsafe { (*c).p };
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut cursor = c;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}
// REWRITES-NEXT:             < unsafe { std::ptr::addr_of_mut!(*(*{{_v[0-9]+}}).buf.as_mut_ptr().add((8 as u64) as usize)) };
// REWRITES-NEXT:         if !{{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *unsafe { (*c).p } = n;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         n = n + 1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut cursor = c;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut u8 = unsafe { (*{{_v[0-9]+}}).p };
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             (*{{_v[0-9]+}}).p = {{_v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut c: cursor = cursor {
// REWRITES-NEXT:         p: std::ptr::null_mut(),
// REWRITES-NEXT:         buf: [0; 8],
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     fill(unsafe { &mut (*std::ptr::addr_of_mut!(c)) }, 1);
// REWRITES-NEXT:     for i in 0..8 {
// REWRITES-NEXT:         total = total + (c.buf[((i as i64) as usize)] as i32);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), total) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
