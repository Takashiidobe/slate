#include <stddef.h>
#include <stdio.h>
#define OFFSET(T, M) ((size_t)((char *)&((T *)0)->M - (char *)0))
struct Inner { int first; short second; };
struct Record { char prefix; double value; struct Inner nested; unsigned char count; };
int main(void) {
    printf("%zu %zu %zu %zu %zu\n", OFFSET(struct Record, value),
           OFFSET(struct Record, nested), OFFSET(struct Record, nested.second),
           OFFSET(struct Record, count), OFFSET(struct Inner, first));
    return OFFSET(struct Record, nested.second) != offsetof(struct Record, nested.second);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%zu %zu %zu %zu %zu\n".as_ptr(),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             16u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             24u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     false as i32
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
