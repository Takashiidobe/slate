#include <stdarg.h>
#include <stdio.h>

static int format_into(char *buffer, size_t size, const char *fmt, va_list ap) {
  return vsnprintf(buffer, size, fmt, ap);
}

static int report(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int written = vprintf(fmt, ap);
  va_end(ap);
  return written;
}

static int render(char *buffer, size_t size, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int written = format_into(buffer, size, fmt, ap);
  va_end(ap);
  return written;
}

static long sum_ints(int count, ...) {
  va_list ap;
  va_start(ap, count);
  long total = 0;
  for (int i = 0; i < count; i++) {
    total += va_arg(ap, int);
  }
  va_end(ap);
  return total;
}

static double mixed(int count, ...) {
  va_list ap;
  va_start(ap, count);
  double total = 0;
  for (int i = 0; i < count; i++) {
    int kind = va_arg(ap, int);
    if (kind == 0) {
      total += va_arg(ap, int);
    } else if (kind == 1) {
      total += va_arg(ap, double);
    } else if (kind == 2) {
      const char *text = va_arg(ap, const char *);
      total += text[0];
    } else {
      total += (double)va_arg(ap, long long);
    }
  }
  va_end(ap);
  return total;
}

static int sum_twice(int count, ...) {
  va_list ap, copy;
  va_start(ap, count);
  va_copy(copy, ap);
  int first = 0;
  for (int i = 0; i < count; i++) {
    first += va_arg(ap, int);
  }
  int second = 0;
  for (int i = 0; i < count; i++) {
    second += va_arg(copy, int) * 2;
  }
  va_end(copy);
  va_end(ap);
  va_start(ap, count);
  int restarted = va_arg(ap, int);
  va_end(ap);
  return first + second + restarted;
}

static unsigned take_unsigned(va_list ap) { return va_arg(ap, unsigned); }

static unsigned unsigned_pair(int unused, ...) {
  va_list ap;
  va_start(ap, unused);
  unsigned high = take_unsigned(ap);
  va_end(ap);
  return high;
}

int main(void) {
  char buffer[64];
  int written = report("%s=%d %.1f\n", "value", 42, 2.5);
  int rendered = render(buffer, sizeof buffer, "[%d|%s|%c]", 7, "seven", 'x');
  printf("%d %d %s\n", written, rendered, buffer);
  printf("%ld\n", sum_ints(5, 1, 2, 3, 4, 5));
  printf("%.2f\n", mixed(4, 0, 10, 1, 0.5, 2, "A", 3, 1000000000000LL));
  printf("%d\n", sum_twice(3, 4, 5, 6));
  printf("%u\n", unsigned_pair(0, 4000000000u));
  return (int)(sum_ints(3, 10, 20, 30) % 13);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn vprintf(__format: *const i8, __arg: core::ffi::VaList<'_>) -> i32;
// CHECK-NEXT:     fn vsnprintf(
// CHECK-NEXT:         __s: *mut i8,
// CHECK-NEXT:         __maxlen: u64,
// CHECK-NEXT:         __format: *const i8,
// CHECK-NEXT:         __arg: core::ffi::VaList<'_>,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn format_into(
// CHECK-NEXT:     buffer: *mut i8,
// CHECK-NEXT:     size: u64,
// CHECK-NEXT:     fmt: *const i8,
// CHECK-NEXT:     mut ap: core::ffi::VaList<'_>,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { vsnprintf(buffer, size, fmt, ap.clone()) }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn report(fmt: *const i8, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     let written: i32 = unsafe { vprintf(fmt, ap.clone()) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     written
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn render(
// CHECK-NEXT:     buffer: *mut i8,
// CHECK-NEXT:     size: u64,
// CHECK-NEXT:     fmt: *const i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     let written: i32 = format_into(buffer, size, fmt, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT:     written
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn sum_ints(count: i32, mut __va_args: ...) -> i64 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i64 = total;
// CHECK-NEXT:         total = [[V_0]] + ((unsafe { ap.next_arg::<i32>() }) as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn mixed(count: i32, mut __va_args: ...) -> f64 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     let mut total: f64 = (0 as i32) as f64;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         let kind: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         if kind == 0 {
// CHECK-NEXT:             let [[V_1:__v[0-9]+]]: f64 = total;
// CHECK-NEXT:             total = [[V_1]] + ((unsafe { ap.next_arg::<i32>() }) as f64);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if kind == 1 {
// CHECK-NEXT:                 let [[V_2:__v[0-9]+]]: f64 = total;
// CHECK-NEXT:                 total = [[V_2]] + unsafe { ap.next_arg::<f64>() };
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if kind == 2 {
// CHECK-NEXT:                     let text: *const i8 = unsafe { ap.next_arg::<*const i8>() };
// CHECK-NEXT:                     total
// CHECK-NEXT:                         += ((unsafe { *unsafe { text.offset(0 as isize) } }) as i32)
// CHECK-NEXT:                             as f64;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     let [[V_3:__v[0-9]+]]: f64 = total;
// CHECK-NEXT:                     total = [[V_3]] + ((unsafe { ap.next_arg::<i64>() }) as f64);
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn sum_twice(count: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut copy: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     copy = ap.clone();
// CHECK-NEXT:     let mut first: i32 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         let [[V_4:__v[0-9]+]]: i32 = first;
// CHECK-NEXT:         first = [[V_4]] + unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut second: i32 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: i32 = second;
// CHECK-NEXT:         second = [[V_5]] + (unsafe { copy.next_arg::<i32>() }) * 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     {}
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     let restarted: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:     {}
// CHECK-NEXT:     first + second + restarted
// CHECK-NEXT: }
// CHECK-NEXT: fn take_unsigned(mut ap: core::ffi::VaList<'_>) -> u32 {
// CHECK-NEXT:     unsafe { ap.next_arg::<u32>() }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn unsigned_pair(unused: i32, mut __va_args: ...) -> u32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     let high: u32 = take_unsigned(ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT:     high
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buffer: __SlateAlign16<[i8; 64]> = __SlateAlign16([0 as i8; 64]);
// CHECK-NEXT:     let written: i32 = unsafe {
// CHECK-NEXT:         report(c"%s=%d %.1f\n".as_ptr(), c"value".as_ptr() as *mut i8, 42 as i32, 2.5f64)
// CHECK-NEXT:     };
// CHECK-NEXT:     let rendered: i32 = unsafe {
// CHECK-NEXT:         render(
// CHECK-NEXT:             buffer.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             64 as u64,
// CHECK-NEXT:             c"[%d|%s|%c]".as_ptr(),
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             c"seven".as_ptr() as *mut i8,
// CHECK-NEXT:             120 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %s\n".as_ptr(),
// CHECK-NEXT:             written,
// CHECK-NEXT:             rendered,
// CHECK-NEXT:             buffer.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 sum_ints(5 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32)
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.2f\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 mixed(
// CHECK-NEXT:                     4 as i32,
// CHECK-NEXT:                     0 as i32,
// CHECK-NEXT:                     10 as i32,
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     0.5f64,
// CHECK-NEXT:                     2 as i32,
// CHECK-NEXT:                     c"A".as_ptr() as *mut i8,
// CHECK-NEXT:                     3 as i32,
// CHECK-NEXT:                     1000000000000 as i64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d\n".as_ptr(),
// CHECK-NEXT:             unsafe { sum_twice(3 as i32, 4 as i32, 5 as i32, 6 as i32) },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%u\n".as_ptr(), unsafe { unsigned_pair(0 as i32, 4000000000 as u32) })
// CHECK-NEXT:     };
// CHECK-NEXT:     ((unsafe { sum_ints(3 as i32, 10 as i32, 20 as i32, 30 as i32) }) % 13) as i32
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
