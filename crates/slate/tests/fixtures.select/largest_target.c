extern int helper_value(void);

int value(void) {
    return VARIANT + helper_value();
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}build target libz.so: 2 files, other targets: tool
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file helper.rs
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn helper_value() -> i32 {
// CHECK-NEXT:     4 * 10
// CHECK-NEXT: }
// CHECK-NEXT: file largest_target.rs
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn helper_value() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn value() -> i32 {
// CHECK-NEXT:     4 + unsafe { helper_value() }
// CHECK-NEXT: }
// CHECK-NEXT: file lib.rs
// CHECK-NEXT: #![allow(non_camel_case_types, rustdoc::all)]
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "helper.rs"]
// CHECK-NEXT: mod __slate_unit_helper;
// CHECK-NEXT: #[path = "largest_target.rs"]
// CHECK-NEXT: mod __slate_unit_largest_target;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
