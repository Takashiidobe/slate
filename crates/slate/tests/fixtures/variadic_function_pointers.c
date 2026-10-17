#include <stdarg.h>
#include <stdio.h>

typedef int (*Callback)(int, ...);
struct Dispatch { Callback callback; };

static int total(int count, ...) {
  va_list args;
  va_start(args, count);
  int result = va_arg(args, int);
  result += (int)va_arg(args, double);
  result += va_arg(args, int);
  va_end(args);
  return count + result;
}

static Callback select_callback(Callback callback) { return callback; }

int main(void) {
  struct Dispatch dispatch = {total};
  Callback callback = select_callback(dispatch.callback);
  signed char a = 7;
  float b = 2.5f;
  unsigned short c = 19;
  if (callback(3, a, b, c) != 31) return 1;
  callback = 0;
  if (callback != 0) return 2;
  int (*format)(char *, size_t, const char *, ...) = snprintf;
  char buffer[32];
  if (format(buffer, sizeof(buffer), "%d %.1f", a, b) != 5) return 3;
  puts(buffer);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn snprintf(__s: *mut i8, __maxlen: u64, __format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn puts(__s: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Dispatch {
// CHECK-NEXT:     callback: Option<unsafe extern "C-unwind" fn(i32, ...) -> i32>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C-unwind" fn total(count: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut args: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     args = __va_args.clone();
// CHECK-NEXT:     let mut result: i32 = unsafe { args.next_arg::<i32>() };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = result;
// CHECK-NEXT:     result = [[V_0]] + ((unsafe { args.next_arg::<f64>() }) as i32);
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = result;
// CHECK-NEXT:     result = [[V_1]] + unsafe { args.next_arg::<i32>() };
// CHECK-NEXT:     {}
// CHECK-NEXT:     count + result
// CHECK-NEXT: }
// CHECK-NEXT: fn select_callback(
// CHECK-NEXT:     callback: Option<unsafe extern "C-unwind" fn(i32, ...) -> i32>,
// CHECK-NEXT: ) -> Option<unsafe extern "C-unwind" fn(i32, ...) -> i32> {
// CHECK-NEXT:     callback
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let dispatch: Dispatch = Dispatch { callback: Some(total) };
// CHECK-NEXT:     let mut callback: Option<unsafe extern "C-unwind" fn(i32, ...) -> i32> = select_callback(
// CHECK-NEXT:         dispatch.callback,
// CHECK-NEXT:     );
// CHECK-NEXT:     let a: i8 = 7;
// CHECK-NEXT:     let b: f32 = 2.5f32;
// CHECK-NEXT:     let c: u16 = 19;
// CHECK-NEXT:     if (unsafe { callback.unwrap()(3 as i32, a as i32, b as f64, (c as u32) as i32) })
// CHECK-NEXT:         != 31
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     callback = None;
// CHECK-NEXT:     if callback != None {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     let format: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(*mut i8, u64, *const i8, ...) -> i32,
// CHECK-NEXT:     > = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(*mut i8, u64, *const i8, ...) -> i32>,
// CHECK-NEXT:         >(snprintf as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut buffer: __SlateAlign16<[i8; 32]> = __SlateAlign16([0 as i8; 32]);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         format
// CHECK-NEXT:             .unwrap()(
// CHECK-NEXT:             buffer.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             32 as u64,
// CHECK-NEXT:             c"%d %.1f".as_ptr(),
// CHECK-NEXT:             a as i32,
// CHECK-NEXT:             b as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != 5
// CHECK-NEXT:     {
// CHECK-NEXT:         return 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(buffer.0.as_mut_ptr() as *const i8) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
