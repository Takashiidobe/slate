#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define weak __attribute__((__weak__))
#define hidden __attribute__((__visibility__("hidden")))

extern weak hidden char __ehdr_start[];
extern weak hidden void (*const __init_array_start)(void), (*const __init_array_end)(void);
extern weak int missing_counter;
extern weak const size_t missing_table[];

static int read_counter(void) { return &missing_counter ? missing_counter : -1; }

int main(void) {
    printf("ehdr %d\n", __ehdr_start && memcmp(__ehdr_start, "\177ELF", 4) == 0);
    printf("init %d\n", &__init_array_start <= &__init_array_end);
    printf("missing %d %d\n", &missing_counter == NULL, missing_table == NULL);
    printf("counter %d\n", read_counter());
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(linkage)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[linkage = "extern_weak"]
// CHECK-NEXT:     static __ehdr_start: *mut [i8; 0];
// CHECK-NEXT:     #[linkage = "extern_weak"]
// CHECK-NEXT:     static __init_array_start: *mut Option<unsafe extern "C-unwind" fn()>;
// CHECK-NEXT:     #[linkage = "extern_weak"]
// CHECK-NEXT:     static __init_array_end: *mut Option<unsafe extern "C-unwind" fn()>;
// CHECK-NEXT:     #[linkage = "extern_weak"]
// CHECK-NEXT:     static missing_counter: *mut i32;
// CHECK-NEXT:     #[linkage = "extern_weak"]
// CHECK-NEXT:     static missing_table: *mut [u64; 0];
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::memcmp;
// CHECK-NEXT: fn read_counter() -> i32 {
// CHECK-NEXT:     if !unsafe { std::ptr::addr_of_mut!(* missing_counter) }.is_null() {
// CHECK-NEXT:         unsafe { *missing_counter }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         -1
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "ehdr {}", (! unsafe { std::ptr::addr_of_mut!(* __ehdr_start) as * mut i8 }
// CHECK-NEXT:         .is_null() && (unsafe { memcmp(((std::ptr::addr_of_mut!(* __ehdr_start) as * mut
// CHECK-NEXT:         i8) as * const ()) as * const core::ffi::c_void, (b"\x7fELF\0".as_ptr() as *
// CHECK-NEXT:         const ()) as * const core::ffi::c_void, 4u64 as usize) as i32 }) == 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "init {}", ((unsafe { std::ptr::addr_of!(* __init_array_start) }) <= unsafe {
// CHECK-NEXT:         std::ptr::addr_of!(* __init_array_end) }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "missing {} {}", unsafe { std::ptr::addr_of_mut!(* missing_counter) } .is_null()
// CHECK-NEXT:         as i32, unsafe { std::ptr::addr_of!(* missing_table) as * const u64 } .is_null()
// CHECK-NEXT:         as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("counter {}", read_counter());
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
