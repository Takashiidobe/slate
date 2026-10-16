void abort(void);
int  ii;
typedef struct {
} raw_spinlock_t;
typedef struct {
  raw_spinlock_t raw_lock;
} spinlock_t;
raw_spinlock_t one_raw_spinlock(void) {
  raw_spinlock_t raw_lock;
  ii++;
  return raw_lock;
}
int main(void) {
  spinlock_t lock = (spinlock_t){.raw_lock = one_raw_spinlock()};
  if (ii != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut ii: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     raw_lock: [[SLATERECORD_0]],
// CHECK-NEXT: }
// CHECK-NEXT: fn one_raw_spinlock() -> [[SLATERECORD_0]] {
// CHECK-NEXT:     let mut raw_lock: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { ii };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ii = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     return raw_lock;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = [[SLATERECORD_1]] {
// CHECK-NEXT:                 raw_lock: one_raw_spinlock(),
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe { ii }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
