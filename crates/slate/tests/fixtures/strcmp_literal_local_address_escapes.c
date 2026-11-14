#include <stdio.h>
#include <string.h>

static int fill(char **out, const char *value) {
    if (!value)
        return 0;
    *out = (char *)value;
    return 1;
}

static void retarget(const char **slot) { *slot = "gpu"; }

static int check(const char *value) {
    char *device = "";
    if (!fill(&device, value))
        return -1;
    if (strcmp(device, "cpu") != 0)
        return 1;
    return 0;
}

static int check_const(void) {
    const char *device = "cpu";
    retarget(&device);
    return strcmp(device, "cpu") == 0;
}

static size_t length_after(void) {
    char *name = "";
    fill(&name, "numpy");
    return strlen(name);
}

int main(void) {
    printf("%d %d %d\n", check("cpu"), check("cuda"), check(NULL));
    printf("%d\n", check_const());
    printf("%zu\n", length_after());
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: use slate_support::strcmp;
// CHECK-NEXT: use slate_support::strlen;
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn fill(out: &mut *mut i8, value: *const i8) -> i32 {
// CHECK-NEXT:     if value.is_null() {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(out as *mut *mut i8) = value as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn retarget(slot: &mut *const i8) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(slot as *mut *const i8) = c"gpu".as_ptr();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn check(value: *const i8) -> i32 {
// CHECK-NEXT:     let mut device: *mut i8 = c"".as_ptr() as *mut i8;
// CHECK-NEXT:     if fill(unsafe { &mut (*std::ptr::addr_of_mut!(device)) }, value) == 0 {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strcmp(
// CHECK-NEXT:             (device as *const i8) as *const core::ffi::c_char,
// CHECK-NEXT:             c"cpu".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     }) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn check_const() -> i32 {
// CHECK-NEXT:     let mut device: *const i8 = c"cpu".as_ptr();
// CHECK-NEXT:     retarget(unsafe { &mut (*std::ptr::addr_of_mut!(device)) });
// CHECK-NEXT:     ((unsafe {
// CHECK-NEXT:         strcmp(
// CHECK-NEXT:             device as *const core::ffi::c_char,
// CHECK-NEXT:             c"cpu".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     }) == 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn length_after() -> u64 {
// CHECK-NEXT:     let mut name: *mut i8 = c"".as_ptr() as *mut i8;
// CHECK-NEXT:     fill(unsafe { &mut (*std::ptr::addr_of_mut!(name)) }, c"numpy".as_ptr());
// CHECK-NEXT:     unsafe { strlen((name as *const i8) as *const core::ffi::c_char) as u64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", check(c"cpu".as_ptr()), check(c"cuda".as_ptr()),
// CHECK-NEXT:         check(std::ptr::null::< i8 > ())
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", check_const());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%zu\n".as_ptr(), length_after()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
