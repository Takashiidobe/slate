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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut aligned_values: aligned::Aligned<aligned::A32, [u32; 3]> = aligned::Aligned([4, 5, 6]);
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut values: [u32; 3] = [1, 2, 3];
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn update(arg1: *mut u32) -> u32 {
// LOWERING-NEXT:     let mut items: *mut u32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: u32 = 0;
// LOWERING-NEXT:     items = arg1;
// LOWERING-NEXT:     let _v0: u32 = 10;
// LOWERING-NEXT:     let _v1: i64 = 0;
// LOWERING-NEXT:     let _v2: *mut u32 = items;
// LOWERING-NEXT:     let _v3: *mut u32 = unsafe { _v2.add(0) };
// LOWERING-NEXT:     let _v4: u32 = unsafe { *_v3 };
// LOWERING-NEXT:     let _v5: u32 = _v4 + _v0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v3 = _v5;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v6: i64 = 0;
// LOWERING-NEXT:     let _v7: *mut u32 = items;
// LOWERING-NEXT:     let _v8: *mut u32 = unsafe { _v7.add(0) };
// LOWERING-NEXT:     let _v9: u32 = unsafe { *_v8 };
// LOWERING-NEXT:     let _v10: i64 = 2;
// LOWERING-NEXT:     let _v11: *mut u32 = items;
// LOWERING-NEXT:     let _v12: *mut u32 = unsafe { _v11.add(2) };
// LOWERING-NEXT:     let _v13: u32 = unsafe { *_v12 };
// LOWERING-NEXT:     let _v14: u32 = _v9 + _v13;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: u32 = __retval;
// LOWERING-NEXT:     return _v15;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn middle(arg0: *mut [u32; 3]) -> u32 {
// LOWERING-NEXT:     let mut items: *mut [u32; 3] = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: u32 = 0;
// LOWERING-NEXT:     items = arg0;
// LOWERING-NEXT:     let _v0: i64 = 1;
// LOWERING-NEXT:     let _v1: *mut [u32; 3] = items;
// LOWERING-NEXT:     let _v2: u32 = unsafe { (*_v1)[(_v0 as usize)] };
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: u32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut first: u32 = 0;
// LOWERING-NEXT:     let mut second: u32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut u32 = std::ptr::addr_of_mut!(values).cast::<u32>();
// LOWERING-NEXT:     let _v2: u32 = update(_v1);
// LOWERING-NEXT:     first = _v2;
// LOWERING-NEXT:     let _v3: *mut u32 = std::ptr::addr_of_mut!(aligned_values).cast::<u32>();
// LOWERING-NEXT:     let _v4: u32 = update(_v3);
// LOWERING-NEXT:     second = _v4;
// LOWERING-NEXT:     let _v5: *mut i8 = b"%u %u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: u32 = first;
// LOWERING-NEXT:     let _v7: u32 = second;
// LOWERING-NEXT:     let _v8: u32 = middle(std::ptr::addr_of_mut!(values));
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v5 as *const i8, _v6, _v7, _v8) };
// LOWERING-NEXT:     let _v10: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v10 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut aligned_values: aligned::Aligned<aligned::A32, [u32; 3]> = aligned::Aligned([4, 5, 6]);
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut values: [u32; 3] = [1, 2, 3];
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn update(arg1: *mut u32) -> u32 {
// REWRITES-NEXT: let mut items: *mut u32 = arg1;
// REWRITES-NEXT: let mut __retval: u32 = 0;
// REWRITES-NEXT: let _v0: u32 = 10;
// REWRITES-NEXT: let _v1: i64 = 0;
// REWRITES-NEXT: let _v2: *mut u32 = items;
// REWRITES-NEXT: let _v3: *mut u32 = unsafe { _v2.add(0) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v3 = (unsafe { *_v3 }) + _v0;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v6: i64 = 0;
// REWRITES-NEXT: let _v7: *mut u32 = items;
// REWRITES-NEXT: let _v8: *mut u32 = unsafe { _v7.add(0) };
// REWRITES-NEXT: let _v9: u32 = unsafe { *_v8 };
// REWRITES-NEXT: let _v10: i64 = 2;
// REWRITES-NEXT: let _v11: *mut u32 = items;
// REWRITES-NEXT: let _v12: *mut u32 = unsafe { _v11.add(2) };
// REWRITES-NEXT: __retval = _v9 + unsafe { *_v12 };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn middle(arg0: &[u32; 3]) -> u32 {
// REWRITES-NEXT: let mut __retval: u32 = 0;
// REWRITES-NEXT: let _v0: i64 = 1;
// REWRITES-NEXT: __retval = unsafe { (*((arg0 as *const [u32; 3]) as *mut [u32; 3]))[(_v0 as usize)] };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut first: u32 = 0;
// REWRITES-NEXT: let mut second: u32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut u32 = std::ptr::addr_of_mut!(values).cast::<u32>();
// REWRITES-NEXT: first = update(_v1);
// REWRITES-NEXT: let _v3: *mut u32 = std::ptr::addr_of_mut!(aligned_values).cast::<u32>();
// REWRITES-NEXT: second = update(_v3);
// REWRITES-NEXT: let _v5: *mut i8 = b"%u %u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: u32 = first;
// REWRITES-NEXT: let _v7: u32 = second;
// REWRITES-NEXT: let _v8: u32 = middle(unsafe { &(*std::ptr::addr_of_mut!(values)) });
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v5 as *const i8, _v6, _v7, _v8) };
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
