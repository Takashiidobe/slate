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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct cursor {
// LOWERING-NEXT:     p: *mut u8,
// LOWERING-NEXT:     buf: [u8; 8],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill(arg0: *mut cursor, arg1: u8) {
// LOWERING-NEXT:     let mut c: *mut cursor = std::ptr::null_mut();
// LOWERING-NEXT:     let mut n: u8 = 0;
// LOWERING-NEXT:     c = arg0;
// LOWERING-NEXT:     n = arg1;
// LOWERING-NEXT:     let _v0: *mut cursor = c;
// LOWERING-NEXT:     let _v1: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*_v0).buf) }) as *mut u8;
// LOWERING-NEXT:     let _v2: *mut cursor = c;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v2).p = _v1;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v3: *mut cursor = c;
// LOWERING-NEXT:             let _v4: *mut u8 = unsafe { (*_v3).p };
// LOWERING-NEXT:             let _v5: u64 = 8;
// LOWERING-NEXT:             let _v6: *mut cursor = c;
// LOWERING-NEXT:             let _v7: bool = _v4 < unsafe { std::ptr::addr_of_mut!(*(*_v6).buf.as_mut_ptr().add(_v5 as usize)) };
// LOWERING-NEXT:             if !_v7 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v8: u8 = n;
// LOWERING-NEXT:                 let _v9: *mut cursor = c;
// LOWERING-NEXT:                 let _v10: *mut u8 = unsafe { (*_v9).p };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *_v10 = _v8;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v11: u8 = n;
// LOWERING-NEXT:                 let _v12: u8 = _v11 + 1;
// LOWERING-NEXT:                 n = _v12;
// LOWERING-NEXT:                 let _v13: *mut cursor = c;
// LOWERING-NEXT:                 let _v14: *mut u8 = unsafe { (*_v13).p };
// LOWERING-NEXT:                 let _v15: i32 = 1;
// LOWERING-NEXT:                 let _v16: *mut u8 = unsafe { _v14.add(1) };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     (*_v13).p = _v16;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut c: cursor = cursor { p: std::ptr::null_mut(), buf: [0; 8] };
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u8 = 1;
// LOWERING-NEXT:     fill(std::ptr::addr_of_mut!(c), _v1);
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     total = _v2;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v3: i32 = 0;
// LOWERING-NEXT:         i = _v3;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v4: i32 = i;
// LOWERING-NEXT:             let _v5: i32 = 8;
// LOWERING-NEXT:             let _v6: bool = _v4 < _v5;
// LOWERING-NEXT:             if !_v6 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v7: i32 = i;
// LOWERING-NEXT:                 let _v8: i64 = _v7 as i64;
// LOWERING-NEXT:                 let _v9: u8 = c.buf[(_v8 as usize)];
// LOWERING-NEXT:                 let _v10: i32 = _v9 as i32;
// LOWERING-NEXT:                 let _v11: i32 = total;
// LOWERING-NEXT:                 let _v12: i32 = _v11 + _v10;
// LOWERING-NEXT:                 total = _v12;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v13: i32 = i;
// LOWERING-NEXT:             let _v14: i32 = _v13 + 1;
// LOWERING-NEXT:             i = _v14;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v15: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: i32 = total;
// LOWERING-NEXT:     let _v17: i32 = unsafe { printf(_v15 as *const i8, _v16) };
// LOWERING-NEXT:     let _v18: i32 = 0;
// LOWERING-NEXT:     __retval = _v18;
// LOWERING-NEXT:     let _v19: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v19 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct cursor {
// REWRITES-NEXT:     p: *mut u8,
// REWRITES-NEXT:     buf: [u8; 8],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill(arg0: &cursor, arg1: u8) {
// REWRITES-NEXT: let mut c: *mut cursor = (arg0 as *const cursor) as *mut cursor;
// REWRITES-NEXT: let mut n: u8 = arg1;
// REWRITES-NEXT: let _v1: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*c).buf) }) as *mut u8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*c).p = _v1;
// REWRITES-NEXT: }
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let _v4: *mut u8 = unsafe { (*c).p };
// REWRITES-NEXT:         let _v5: u64 = 8;
// REWRITES-NEXT:         let _v6: *mut cursor = c;
// REWRITES-NEXT:         let _v7: bool = _v4 < unsafe { std::ptr::addr_of_mut!(*(*_v6).buf.as_mut_ptr().add(_v5 as usize)) };
// REWRITES-NEXT:         if !_v7 {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     *unsafe { (*c).p } = n;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     n = n + 1;
// REWRITES-NEXT:                     let _v13: *mut cursor = c;
// REWRITES-NEXT:                     let _v14: *mut u8 = unsafe { (*_v13).p };
// REWRITES-NEXT:                     let _v15: i32 = 1;
// REWRITES-NEXT:                     let _v16: *mut u8 = unsafe { _v14.add(1) };
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     (*_v13).p = _v16;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut c: cursor = cursor { p: std::ptr::null_mut(), buf: [0; 8] };
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u8 = 1;
// REWRITES-NEXT: fill(unsafe { &(*std::ptr::addr_of_mut!(c)) }, _v1);
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v5: i32 = 8;
// REWRITES-NEXT:                     if !(i < _v5) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     total = total + (c.buf[((i as i64) as usize)] as i32);
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v15: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v17: i32 = unsafe { printf(_v15 as *const i8, total) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
