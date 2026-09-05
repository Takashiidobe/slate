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
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct cursor {
// COMMON-LOWERING-NEXT:     p: *mut u8,
// COMMON-LOWERING-NEXT:     buf: [u8; 8],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut c: cursor = cursor {
// COMMON-LOWERING-NEXT:         p: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         buf: [0; 8],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 1;
// COMMON-LOWERING-NEXT:     fill(std::ptr::addr_of_mut!(c), {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u8 = c.buf[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn fill({{arg[0-9]+}}: *mut cursor, {{arg[0-9]+}}: u8) {
// COMMON-LOWERING-NEXT:     let mut c: *mut cursor = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut n: u8 = 0;
// COMMON-LOWERING-NEXT:     c = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     n = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut cursor = c;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).buf) }) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut cursor = c;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         (*{{__v[0-9]+}}).p = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut cursor = c;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut u8 = unsafe { (*{{__v[0-9]+}}).p };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut cursor = c;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:                 < unsafe { std::ptr::addr_of_mut!(*(*{{__v[0-9]+}}).buf.as_mut_ptr().add({{__v[0-9]+}} as usize)) };
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u8 = n;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut cursor = c;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut u8 = unsafe { (*{{__v[0-9]+}}).p };
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u8 = n;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                 n = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut cursor = c;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut u8 = unsafe { (*{{__v[0-9]+}}).p };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     (*{{__v[0-9]+}}).p = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct cursor {
// COMMON-REWRITES-NEXT:     p: *mut u8,
// COMMON-REWRITES-NEXT:     buf: [u8; 8],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut c: cursor = cursor {
// COMMON-REWRITES-NEXT:         p: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         buf: [0; 8],
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     fill(unsafe { &mut (*std::ptr::addr_of_mut!(c)) }, 1);
// COMMON-REWRITES-NEXT:     for i in 0..8 {
// COMMON-REWRITES-NEXT:         total += c.buf[((i as i64) as usize)] as i32;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), total) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn fill({{arg[0-9]+}}: &mut cursor, mut n: u8) {
// COMMON-REWRITES-NEXT:     let mut c: *mut cursor = {{arg[0-9]+}} as *mut cursor;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*c).buf) }) as *mut u8;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         (*c).p = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { (*c).p };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut cursor = c;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:             < unsafe { std::ptr::addr_of_mut!(*(*{{__v[0-9]+}}).buf.as_mut_ptr().add((8 as u64) as usize)) };
// COMMON-REWRITES-NEXT:         if !{{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *unsafe { (*c).p } = n;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         n += 1;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut cursor = c;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { (*{{__v[0-9]+}}).p };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             (*{{__v[0-9]+}}).p = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
