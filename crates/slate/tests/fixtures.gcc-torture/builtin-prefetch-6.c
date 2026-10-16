/* Test that __builtin_prefetch does no harm.

   Data prefetch should not fault if used with an invalid address.  */

#include <limits.h>

void exit(int);

#define ARRSIZE 65
int *bad_addr[ARRSIZE];
int  arr_used;

/* Fill bad_addr with a range of values in the hopes that on any target
   some will be invalid addresses.  */
void init_addrs(void) {
  int i;
  int bits_per_ptr = sizeof(void *) * 8;
  for (i = 0; i < bits_per_ptr; i++)
    bad_addr[i] = (void *)(1UL << i);
  arr_used = bits_per_ptr + 1; /* The last element used is zero.  */
}

void prefetch_for_read(void) {
  int i;
  for (i = 0; i < ARRSIZE; i++)
    __builtin_prefetch(bad_addr[i], 0, 0);
}

void prefetch_for_write(void) {
  int i;
  for (i = 0; i < ARRSIZE; i++)
    __builtin_prefetch(bad_addr[i], 1, 0);
}

int main() {
  init_addrs();
  prefetch_for_read();
  prefetch_for_write();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test that __builtin_prefetch does no harm.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Data prefetch should not fault if used with an invalid address.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn __builtin_prefetch([[V_1:__v[0-9]+]]: *const (), ...);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut bad_addr: __SlateAlign16<[*mut i32; 65]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut arr_used: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: /// Fill bad_addr with a range of values in the hopes that on any target
// CHECK-NEXT: /// some will be invalid addresses.
// CHECK-NEXT: fn init_addrs() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut bits_per_ptr: i32 = ((8 as u64).wrapping_mul(((8 as i32) as i64) as u64) as u32) as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < bits_per_ptr {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(bad_addr.0) as *mut *mut i32 }.offset(i as isize)
// CHECK-NEXT:             } = (((1 as u64) << i) as *mut ()) as *mut i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         arr_used = bits_per_ptr + (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     // The last element used is zero.
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn prefetch_for_read() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (65 as i32) {
// CHECK-NEXT:         {};
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn prefetch_for_write() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (65 as i32) {
// CHECK-NEXT:         {};
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     init_addrs();
// CHECK-NEXT:     prefetch_for_read();
// CHECK-NEXT:     prefetch_for_write();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
