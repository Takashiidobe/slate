// select-args: --build-target shared
extern int helper_value(void);

int value(void) {
    return VARIANT + helper_value();
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}build target shared: 2 files, other targets: example, static
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file helper.rs
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn helper_value() -> i32 {
// CHECK-NEXT:     1 * 10
// CHECK-NEXT: }
// CHECK-NEXT: file lib.rs
// CHECK-NEXT: #![allow(non_camel_case_types, rustdoc::all)]
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "helper.rs"]
// CHECK-NEXT: mod __slate_unit_helper;
// CHECK-NEXT: #[path = "requested_target.rs"]
// CHECK-NEXT: mod __slate_unit_requested_target;
// CHECK-NEXT: file requested_target.rs
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn helper_value() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // select-args: --build-target shared
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn value() -> i32 {
// CHECK-NEXT:     1 + unsafe { helper_value() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
