/* { dg-options "-fpack-struct=2" } */
int printf(const char *, ...);
struct ua { char c; int i __attribute__((aligned(8))); };
#pragma pack(4)
struct p4 { char c; double d; };
#pragma pack()
struct reset { char c; double d; };
#pragma pack(push, 8)
struct p8 { char c; double d; };
#pragma pack(pop)
struct popped { char c; double d; };
struct __attribute__((aligned(16))) al16 { char c; int i; };
union u { char c; double d; };
struct bf { char c; int x : 4; int y : 20; };
int main(void) {
  printf("%zu %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu\n",
    sizeof(struct ua), __builtin_offsetof(struct ua, i), _Alignof(struct ua),
    sizeof(struct p4), _Alignof(struct p4), sizeof(struct reset), _Alignof(struct reset),
    sizeof(struct p8), _Alignof(struct p8), sizeof(struct popped), _Alignof(struct popped),
    sizeof(struct al16), _Alignof(struct al16), sizeof(union u), _Alignof(union u),
    sizeof(struct bf), _Alignof(struct bf));
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-fpack-struct=2" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu\n"
// CHECK-NEXT:                 .as_ptr(),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             6u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             2u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             2u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             12u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             4u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             10u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             2u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             16u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             8u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             10u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             2u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             16u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             16u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             8u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             2u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             4u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             2u64,
// I686-UNKNOWN-LINUX-GNU-NEXT:             6u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             2u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             2u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             12u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             4u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             10u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             2u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             12u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             4u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             10u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             2u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             16u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             16u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             8u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             2u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             4u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             2u32,
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
