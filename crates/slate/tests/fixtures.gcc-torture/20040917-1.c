/* submitted by kenneth zadeck */

void abort(void);

static int test_var;

/* the idea here is that not only is inlinable, inlinable but since it
   is static, the cgraph node will not be marked as output.  The
   current version of the code ignores these cgraph nodes.  */

void not_inlinable() __attribute__((noinline));

static void inlinable() { test_var = -10; }

void not_inlinable() { inlinable(); }

int main(void) {
  test_var = 10;
  /* Variable test_var should be considered call-clobbered by the call
     to not_inlinable().  */
  not_inlinable();
  if (test_var == 10)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! submitted by kenneth zadeck
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut test_var: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: // the idea here is that not only is inlinable, inlinable but since it
// CHECK-NEXT: // is static, the cgraph node will not be marked as output.  The
// CHECK-NEXT: // current version of the code ignores these cgraph nodes.
// CHECK-NEXT: fn inlinable() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         test_var = -(10 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn not_inlinable() {
// CHECK-NEXT:     inlinable();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         test_var = 10 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     // Variable test_var should be considered call-clobbered by the call
// CHECK-NEXT:     // to not_inlinable().
// CHECK-NEXT:     not_inlinable();
// CHECK-NEXT:     if (unsafe { test_var }) == (10 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
