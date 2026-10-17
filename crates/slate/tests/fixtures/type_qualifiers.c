#include <stdio.h>

static const int   global_bias    = 3;
static _Atomic int atomic_counter = 4;

static int add_const_param(const int value) {
  const int local_bias = 5;
  return value + local_bias + global_bias;
}

static int add_restrict_pointers(int *restrict lhs, int *restrict rhs) {
  return *lhs + *rhs;
}

static int add_atomic_value(_Atomic int value) {
  _Atomic int local = value + atomic_counter;
  return local;
}

int main(void) {
  int left  = 7;
  int right = 11;
  printf("%d\n", add_const_param(2));
  printf("%d\n", add_restrict_pointers(&left, &right));
  printf("%d\n", add_atomic_value(6));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut global_bias: i32 = 3 as i32;
// CHECK-NEXT: static mut atomic_counter: i32 = 4 as i32;
// CHECK-NEXT: fn add_const_param(value: i32) -> i32 {
// CHECK-NEXT:     let local_bias: i32 = 5;
// CHECK-NEXT:     value + local_bias + unsafe { global_bias }
// CHECK-NEXT: }
// CHECK-NEXT: fn add_restrict_pointers(lhs: &i32, rhs: &i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(lhs as *const i32) }) + unsafe { *(rhs as *const i32) }
// CHECK-NEXT: }
// CHECK-NEXT: fn add_atomic_value(value: i32) -> i32 {
// CHECK-NEXT:     let local: i32 = (unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of!(value) as *mut i32)
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     })
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     std::ptr::addr_of!(atomic_counter) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of!(local) as *mut i32)
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut left: i32 = 7;
// CHECK-NEXT:     let mut right: i32 = 11;
// CHECK-NEXT:     println!("{}", add_const_param(2 as i32));
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", add_restrict_pointers(unsafe { & (* std::ptr::addr_of_mut!(left)) }, unsafe
// CHECK-NEXT:         { & (* std::ptr::addr_of_mut!(right)) })
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", add_atomic_value(6 as i32));
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
