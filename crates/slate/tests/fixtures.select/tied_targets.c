extern int helper_value(void);

int value(void) {
    return VARIANT + helper_value();
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}build target static: 2 files, other targets: example, shared
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file helper.rs
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn helper_value() -> i32 {
// CHECK-NEXT:     2 * 10
// CHECK-NEXT: }
// CHECK-NEXT: file lib.rs
// CHECK-NEXT: #![allow(non_camel_case_types, rustdoc::all)]
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "helper.rs"]
// CHECK-NEXT: mod __slate_unit_helper;
// CHECK-NEXT: #[path = "tied_targets.rs"]
// CHECK-NEXT: mod __slate_unit_tied_targets;
// CHECK-NEXT: file tied_targets.rs
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn helper_value() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn value() -> i32 {
// CHECK-NEXT:     2 + unsafe { helper_value() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
