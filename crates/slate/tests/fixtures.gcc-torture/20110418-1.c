typedef unsigned long long uint64_t;
void f(uint64_t *a, uint64_t aa) __attribute__((noinline));
void f(uint64_t *a, uint64_t aa) {
  uint64_t new_value  = aa;
  uint64_t old_value  = *a;
  int      bit_size   = 32;
  uint64_t mask       = (uint64_t)(unsigned)(-1);
  uint64_t tmp        = old_value & mask;
  new_value          &= mask;
  /* On overflow we need to add 1 in the upper bits */
  if (tmp > new_value)
    new_value += 1ull << bit_size;
  /* Add in the upper bits from the old value */
  new_value += old_value & ~mask;
  *a         = new_value;
}
int main(void) {
  uint64_t value, new_value, old_value;
  value     = 0x100000001;
  old_value = value;
  new_value = (value + 1) & (uint64_t)(unsigned)(-1);
  f(&value, new_value);
  if (value != old_value + 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut a: &mut u64, mut new_value: u64) {
// CHECK-NEXT:     let mut old_value: u64 = unsafe { *(a as *mut u64) };
// CHECK-NEXT:     let mut bit_size: i32 = 32 as i32;
// CHECK-NEXT:     let mut mask: u64 = (-(1 as i32) as u32) as u64;
// CHECK-NEXT:     let mut tmp: u64 = old_value & mask;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = new_value;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]] & mask;
// CHECK-NEXT:     new_value = [[V_1]];
// CHECK-NEXT:     // On overflow we need to add 1 in the upper bits
// CHECK-NEXT:     if tmp > new_value {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u64 = new_value;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u64 = [[V_2]].wrapping_add((1 as u64) << bit_size);
// CHECK-NEXT:         new_value = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // Add in the upper bits from the old value
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u64 = new_value;
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u64 = [[V_4]].wrapping_add(old_value & !mask);
// CHECK-NEXT:     new_value = [[V_5]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(a as *mut u64) = new_value;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: u64 = (4294967297 as i64) as u64;
// CHECK-NEXT:     let mut new_value: u64 = 0 as u64;
// CHECK-NEXT:     let mut old_value: u64 = value;
// CHECK-NEXT:     new_value = value.wrapping_add(((1 as i32) as i64) as u64) & ((-(1 as i32) as u32) as u64);
// CHECK-NEXT:     f(unsafe { &mut (*std::ptr::addr_of_mut!(value)) }, new_value);
// CHECK-NEXT:     if value != old_value.wrapping_add(((1 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
