__attribute__((weak)) int fallback(void) { return 1; }
__attribute__((weak)) int both_weak(void) { return 10; }
__attribute__((weak)) int lone_weak(void) { return 30; }
__attribute__((weak)) int threshold = 4;
__attribute__((weak)) int shared_weak = 50;

int defaults_used(void) { return fallback() * 100 + threshold; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}use crate::__slate_unit_overrides::fallback;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut threshold: i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: #[linkage = "weak"]
// CHECK-NEXT: extern "C-unwind" fn both_weak() -> i32 {
// CHECK-NEXT:     10
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: #[linkage = "weak"]
// CHECK-NEXT: extern "C-unwind" fn lone_weak() -> i32 {
// CHECK-NEXT:     30
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[linkage = "weak"]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut shared_weak: i32 = 50;
// CHECK-EMPTY:
// CHECK-NEXT: pub fn defaults_used() -> i32 {
// CHECK-NEXT:     (unsafe { fallback() }) * 100 + unsafe { threshold }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
