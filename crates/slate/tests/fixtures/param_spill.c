#include <stdio.h>

// @check-fn-begin
int adjust(_Atomic int value, int delta) {
  value = value + delta;
  return value;
}
// @check-fn-end

int main(void) {
  printf("%d\n", adjust(4, 3));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn adjust(mut value: i32, delta: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(
// CHECK-NEXT:                 core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                         core::ptr::addr_of!(value) as *mut i32,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(core::sync::atomic::Ordering::SeqCst) + delta,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(core::ptr::addr_of!(value) as *mut i32)
// CHECK-NEXT:             .load(core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
