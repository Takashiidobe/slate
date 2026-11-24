#define MAX(a, b) ((a) > (b) ? (a) : (b))

static int maximum(const int *value, int floor) {
    return MAX(*value, floor);
}

int main(void) {
    int value = 4;
    return !(maximum(&value, 3) == 4 && maximum(&value, 5) == 5);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! MAX {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         if ($a) > $b { $a } else { $b }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn maximum(value: *const i32, floor: i32) -> i32 {
// CHECK-NEXT:     unsafe { MAX!(*value, floor) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 4;
// CHECK-NEXT:     !(maximum(core::ptr::addr_of_mut!(value) as *const i32, 3i32) == 4
// CHECK-NEXT:         && maximum(core::ptr::addr_of_mut!(value) as *const i32, 5i32) == 5) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
