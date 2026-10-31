// select-args: --build-target example
extern int helper_value(void);

int value(void) {
    return VARIANT + helper_value();
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}build target example: 2 files, other targets: shared, static
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "requested_small_target.rs"]
// CHECK-NEXT: mod __slate_unit_requested_small_target;
// CHECK-EMPTY:
// CHECK-NEXT: use crate::__slate_unit_requested_small_target::value;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     (unsafe { value() }) + 3
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
// CHECK-NEXT: file requested_small_target.rs
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn helper_value() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // select-args: --build-target example
// CHECK-NEXT: pub fn value() -> i32 {
// CHECK-NEXT:     3 + unsafe { helper_value() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
