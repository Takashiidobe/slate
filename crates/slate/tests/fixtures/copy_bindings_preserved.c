int copy_param(int source) {
    int copy = source;
    return copy + 1;
}

int copy_pointer(int *source) {
    int *copy = source;
    return *copy + (source != (void *)0);
}

int main(void) {
    int value = 10;
    return copy_param(value) != 11 || copy_pointer(&value) != 11;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn copy_param(source: i32) -> i32 {
// CHECK-NEXT:     let copy: i32 = source;
// CHECK-NEXT:     copy + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn copy_pointer(source: *mut i32) -> i32 {
// CHECK-NEXT:     let copy: *mut i32 = source;
// CHECK-NEXT:     (unsafe { *copy }) + (!source.is_null() as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 10;
// CHECK-NEXT:     (copy_param(value) != 11 || copy_pointer(core::ptr::addr_of_mut!(value)) != 11)
// CHECK-NEXT:         as i32
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
