/* { dg-options "--flavor=gcc -mms-bitfields" } */
int printf(const char *, ...);
struct plain { char a : 3; int b : 4; char c; };
struct __attribute__((gcc_struct)) gs { char a : 3; int b : 4; char c; };
#pragma ms_struct off
struct off { char a : 3; int b : 4; char c; };
#pragma ms_struct reset
struct reset { char a : 3; int b : 4; char c; };
#pragma ms_struct on
struct on { char a : 3; int b : 4; char c; };
int main(void) { printf("%zu %zu %zu %zu %zu\n", sizeof(struct plain), sizeof(struct gs), sizeof(struct off), sizeof(struct reset), sizeof(struct on)); }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "--flavor=gcc -mms-bitfields" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu %zu %zu %zu\n".as_ptr(),
// CHECK-NEXT:             12u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             12u64,
// CHECK-NEXT:             12u64,
// CHECK-NEXT:             12u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
