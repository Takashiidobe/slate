#include <stdint.h>
#include <stdio.h>

static uint32_t values[3]                      = {1, 2, 3};
_Alignas(32) static uint32_t aligned_values[3] = {4, 5, 6};

static uint32_t update(uint32_t *items) {
  items[0] += 10;
  return items[0] + items[2];
}

static uint32_t middle(uint32_t (*items)[3]) { return (*items)[1]; }

int main(void) {
  uint32_t first  = update(values);
  uint32_t second = update(aligned_values);
  printf("%u %u %u\n", first, second, middle(&values));
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: struct __SlateAlign32<T>(T);
// CHECK-NEXT: static mut values: [u32; 3] = [(1 as i32) as u32, (2 as i32) as u32, (3 as i32) as u32];
// CHECK-NEXT: static mut aligned_values: __SlateAlign32<[u32; 3]> = __SlateAlign32([
// CHECK-NEXT:     (4 as i32) as u32,
// CHECK-NEXT:     (5 as i32) as u32,
// CHECK-NEXT:     (6 as i32) as u32,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn update(items: *mut u32) -> u32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut u32 = unsafe { items.offset(0 as isize) };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u32 = unsafe { *[[V_0]] };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = [[V_1]].wrapping_add(10);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *unsafe { items.offset(0 as isize) } }
// CHECK-NEXT:         .wrapping_add(unsafe { *unsafe { items.offset(2 as isize) } })
// CHECK-NEXT: }
// CHECK-NEXT: fn middle(items: *mut [u32; 3]) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*items).as_mut_ptr() as *mut u32 }.offset(1 as isize) }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let first: u32 = update(unsafe { std::ptr::addr_of_mut!(values) as *mut u32 });
// CHECK-NEXT:     let second: u32 = update(unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(aligned_values.0) as *mut u32
// CHECK-NEXT:     });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %u\n".as_ptr(),
// CHECK-NEXT:             first,
// CHECK-NEXT:             second,
// CHECK-NEXT:             middle(unsafe { std::ptr::addr_of_mut!(values) }),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
