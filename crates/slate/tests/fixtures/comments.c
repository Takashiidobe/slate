/** selects an operating mode */
enum Mode {
  /** disable processing */
  MODE_OFF = 0,
  /// enable processing
  MODE_ON  = 1,
};

/** stores a selected mode */
struct Holder {
  /** current mode value */
  enum Mode mode;
};

/** names holder records */
typedef struct Holder Holder;

/** counts completed operations */
static int completed_count = 1;

/** increments a value and records the operation */
static int increment(int value) {
  /** stores the intermediate result */
  volatile int next = value + 1;
  completed_count++;
  return next;
}

int main(void) {
  struct Holder holder = {MODE_ON};
  return holder.mode == MODE_ON && increment(1) == 2 && completed_count == 2
             ? 0
             : 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: // selects an operating mode
// CHECK-NEXT: /// stores a selected mode
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Holder {
// CHECK-NEXT:     /// current mode value
// CHECK-NEXT:     mode: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // names holder records
// CHECK-NEXT: /// counts completed operations
// CHECK-NEXT: static mut completed_count: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: /// increments a value and records the operation
// CHECK-NEXT: fn increment(value: i32) -> i32 {
// CHECK-NEXT:     // stores the intermediate result
// CHECK-NEXT:     let next: i32 = value + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         completed_count = completed_count + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(next)) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let holder: Holder = Holder { mode: 1 };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (holder.mode as u32) == 1 {
// CHECK-NEXT:         [[V_0]] = increment(1 as i32) == 2;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] && (unsafe { completed_count }) == 2 {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1
// CHECK-NEXT:     }
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
