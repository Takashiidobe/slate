#include <stdint.h>
#include <stdio.h>

struct Point {
  int x;
  int y;
};

_Alignas(64) static int counter = 3;
_Alignas(32) static uint32_t values[3] = {4, 5, 6};
static double weights[2] __attribute__((aligned(128))) = {0.5, 1.5};
_Alignas(16) static struct Point origin = {7, 8};
_Alignas(4096) static char page[16];
_Alignas(16) static char small;

static uint32_t *values_tail = &values[2];
static struct Point *origin_ptr = &origin;
static int *counter_ptr = &counter;

static int aligned(const void *pointer, uintptr_t alignment) {
  return (uintptr_t)pointer % alignment == 0;
}

static uint32_t total(const uint32_t *items, int count) {
  uint32_t sum = 0;
  for (int i = 0; i < count; i++) {
    sum += items[i];
  }
  return sum;
}

static int bump(void) {
  _Alignas(256) static int calls;
  calls += 1;
  return calls + aligned(&calls, 256);
}

int main(void) {
  counter += 10;
  values[1] = 50;
  *values_tail += 600;
  weights[0] *= 4.0;
  origin.y = 80;
  origin_ptr->x += 1;
  page[3] = 'p';
  small = 's';
  *counter_ptr *= 2;
  bump();
  printf("%d %d %d %d %d %d\n", aligned(&counter, 64), aligned(values, 32),
         aligned(weights, 128), aligned(&origin, 16), aligned(page, 4096),
         aligned(&small, 16));
  printf("%d %u %u %u %u\n", counter, values[0], values[1], values[2],
         total(values, 3));
  printf("%.2f %.2f %d %d %c %c %d\n", weights[0], weights[1], origin.x,
         origin.y, page[3], small, bump());
  printf("%zu %zu %zu\n", sizeof counter, sizeof values, sizeof origin);
  return counter % 9;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: struct __SlateAlign32<T>(T);
// CHECK-NEXT: #[repr(C, align(64))]
// CHECK-NEXT: struct __SlateAlign64<T>(T);
// CHECK-NEXT: #[repr(C, align(128))]
// CHECK-NEXT: struct __SlateAlign128<T>(T);
// CHECK-NEXT: #[repr(C, align(256))]
// CHECK-NEXT: struct __SlateAlign256<T>(T);
// CHECK-NEXT: #[repr(C, align(4096))]
// CHECK-NEXT: struct __SlateAlign4096<T>(T);
// CHECK-NEXT: static mut counter: __SlateAlign64<i32> = __SlateAlign64(3 as i32);
// CHECK-NEXT: static mut values: __SlateAlign32<[u32; 3]> = __SlateAlign32([
// CHECK-NEXT:     (4 as i32) as u32,
// CHECK-NEXT:     (5 as i32) as u32,
// CHECK-NEXT:     (6 as i32) as u32,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut weights: __SlateAlign128<[f64; 2]> = __SlateAlign128([0.5f64, 1.5f64]);
// CHECK-NEXT: static mut origin: __SlateAlign16<Point> = __SlateAlign16(Point {
// CHECK-NEXT:     x: 7 as i32,
// CHECK-NEXT:     y: 8 as i32,
// CHECK-NEXT: });
// CHECK-NEXT: static mut page: __SlateAlign4096<[i8; 16]> = __SlateAlign4096(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut small: __SlateAlign16<i8> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT: static mut values_tail: *mut u32 = unsafe {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(values.0) as *mut u32 }.offset((2 as i32) as isize)
// CHECK-NEXT: };
// CHECK-NEXT: static mut origin_ptr: *mut Point = unsafe { std::ptr::addr_of_mut!(origin.0) };
// CHECK-NEXT: static mut counter_ptr: *mut i32 = unsafe { std::ptr::addr_of_mut!(counter.0) };
// CHECK-NEXT: fn aligned(pointer: *const (), alignment: u64) -> i32 {
// CHECK-NEXT:     ((pointer as u64) % alignment == 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn total(items: &[u32]) -> u32 {
// CHECK-NEXT:     let mut sum: u32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < (items.len() as i32) {
// CHECK-NEXT:         sum = sum.wrapping_add(unsafe { items[i as usize] });
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn bump() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls.0 = calls.0 + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { calls.0 })
// CHECK-NEXT:         + aligned((unsafe { std::ptr::addr_of_mut!(calls.0) }) as *const (), 256 as u64)
// CHECK-NEXT: }
// CHECK-NEXT: static mut calls: __SlateAlign256<i32> = __SlateAlign256(unsafe { std::mem::zeroed() });
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         counter.0 = counter.0 + 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         values.0[1] = 50;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut u32 = unsafe { values_tail };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u32 = unsafe { *[[V_0]] };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = [[V_1]].wrapping_add(600);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: *mut f64 = unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!(weights.0) as *mut f64).offset(0 as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: f64 = (unsafe { *[[V_2]] }) * 4.0f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_2]] = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         origin.0.y = 80;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: *mut Point = unsafe { origin_ptr };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_4]]).x = (*[[V_4]]).x + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         page.0[3] = 112;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         small.0 = 115;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: *mut i32 = unsafe { counter_ptr };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_5]] = *[[V_5]] * 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     bump();
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", aligned((unsafe { std::ptr::addr_of_mut!(counter.0) }) as *
// CHECK-NEXT:         const (), 64 as u64), aligned((unsafe { std::ptr::addr_of_mut!(values.0) as * mut
// CHECK-NEXT:         u32 }) as * const (), 32 as u64), aligned((unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(weights.0) as * mut f64 }) as * const (), 128 as u64),
// CHECK-NEXT:         aligned((unsafe { std::ptr::addr_of_mut!(origin.0) }) as * const (), 16 as u64),
// CHECK-NEXT:         aligned((unsafe { std::ptr::addr_of_mut!(page.0) as * mut i8 }) as * const (),
// CHECK-NEXT:         4096 as u64), aligned((unsafe { std::ptr::addr_of_mut!(small.0) }) as * const (),
// CHECK-NEXT:         16 as u64)
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %u %u %u %u\n".as_ptr(),
// CHECK-NEXT:             counter.0,
// CHECK-NEXT:             values.0[0],
// CHECK-NEXT:             values.0[1],
// CHECK-NEXT:             values.0[2],
// CHECK-NEXT:             total(
// CHECK-NEXT:                 std::slice::from_raw_parts(
// CHECK-NEXT:                     (std::ptr::addr_of_mut!(values.0) as *mut u32) as *const u32,
// CHECK-NEXT:                     3 as usize,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.2f %.2f %d %d %c %c %d\n".as_ptr(),
// CHECK-NEXT:             weights.0[0],
// CHECK-NEXT:             weights.0[1],
// CHECK-NEXT:             origin.0.x,
// CHECK-NEXT:             origin.0.y,
// CHECK-NEXT:             page.0[3] as i32,
// CHECK-NEXT:             small.0 as i32,
// CHECK-NEXT:             bump(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu\n".as_ptr(), 4 as u64, 12 as u64, 8 as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     (unsafe { counter.0 }) % 9
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
