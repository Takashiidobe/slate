#include <stdio.h>

struct inner {
  int start;
  int end;
};

struct outer {
  struct inner buf;
  int          error;
};

static void bump(int *p) { *p = *p + 10; }

static void init(struct outer *o) {
  o->buf.start = 1;
  o->buf.end   = 2;
  o->error     = 0;
  bump(&o->buf.start);
}

int main(void) {
  struct outer o;
  init(&o);
  printf("%d %d %d\n", o.buf.start, o.buf.end, o.error);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct inner {
// LOWERING-NEXT:     start: i32,
// LOWERING-NEXT:     end: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct outer {
// LOWERING-NEXT:     buf: inner,
// LOWERING-NEXT:     error: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn bump(arg1: *mut i32) {
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     p = arg1;
// LOWERING-NEXT:     let _v0: *mut i32 = p;
// LOWERING-NEXT:     let _v1: i32 = unsafe { *_v0 };
// LOWERING-NEXT:     let _v2: i32 = 10;
// LOWERING-NEXT:     let _v3: i32 = _v1 + _v2;
// LOWERING-NEXT:     let _v4: *mut i32 = p;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v4 = _v3;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn init(arg0: *mut outer) {
// LOWERING-NEXT:     let mut o: *mut outer = std::ptr::null_mut();
// LOWERING-NEXT:     o = arg0;
// LOWERING-NEXT:     let _v0: i32 = 1;
// LOWERING-NEXT:     let _v1: *mut outer = o;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v1).buf.start = _v0;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v2: i32 = 2;
// LOWERING-NEXT:     let _v3: *mut outer = o;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v3).buf.end = _v2;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v4: i32 = 0;
// LOWERING-NEXT:     let _v5: *mut outer = o;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v5).error = _v4;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v6: *mut outer = o;
// LOWERING-NEXT:     bump(unsafe { std::ptr::addr_of_mut!((*_v6).buf.start) });
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut o: outer = outer { buf: inner { start: 0, end: 0 }, error: 0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     init(std::ptr::addr_of_mut!(o));
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = o.buf.start;
// LOWERING-NEXT:     let _v3: i32 = o.buf.end;
// LOWERING-NEXT:     let _v4: i32 = o.error;
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4) };
// LOWERING-NEXT:     let _v6: i32 = 0;
// LOWERING-NEXT:     __retval = _v6;
// LOWERING-NEXT:     let _v7: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v7 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct inner {
// REWRITES-NEXT:     start: i32,
// REWRITES-NEXT:     end: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct outer {
// REWRITES-NEXT:     buf: inner,
// REWRITES-NEXT:     error: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn bump(arg1: *mut i32) {
// REWRITES-NEXT: let _v2: i32 = 10;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *arg1 = (unsafe { *arg1 }) + _v2;
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn init(arg0: &outer) {
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*((arg0 as *const outer) as *mut outer)).buf.start = 1;
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*((arg0 as *const outer) as *mut outer)).buf.end = 2;
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*((arg0 as *const outer) as *mut outer)).error = 0;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v6: *mut outer = (arg0 as *const outer) as *mut outer;
// REWRITES-NEXT: bump(unsafe { std::ptr::addr_of_mut!((*_v6).buf.start) });
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut o: outer = outer { buf: inner { start: 0, end: 0 }, error: 0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: init(unsafe { &(*std::ptr::addr_of_mut!(o)) });
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = o.buf.start;
// REWRITES-NEXT: let _v3: i32 = o.buf.end;
// REWRITES-NEXT: let _v4: i32 = o.error;
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
