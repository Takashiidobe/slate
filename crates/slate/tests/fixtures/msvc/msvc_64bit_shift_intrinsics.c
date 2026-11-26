// { dg-options "/Zc:preprocessor" }
#if defined(_M_IX86) || defined(_M_X64)
#include <intrin.h>
#include <stdlib.h>

#pragma intrinsic(__ll_lshift, __ll_rshift, __ull_rshift)

static int check(int count, unsigned __int64 left, __int64 signed_right,
                 unsigned __int64 unsigned_right) {
  return __ll_lshift(0x8000000000000001ULL, count) != left ||
         __ll_rshift(-16, count) != signed_right ||
         __ull_rshift(0x8000000000000000ULL, count) != unsigned_right;
}
#endif

int main(void) {
#if defined(_M_IX86) || defined(_M_X64)
  if (check(64, 0x8000000000000001ULL, -16, 0x8000000000000000ULL) ||
      check(65, 2, -8, 0x4000000000000000ULL))
    abort();
#ifdef _M_X64
  if (check(32, 0x100000000ULL, -1, 0x80000000ULL) ||
      check(-1, 0x8000000000000000ULL, -1, 1))
    abort();
#else
  if (check(32, 0x8000000000000001ULL, -16, 0x8000000000000000ULL) ||
      check(-1, 0x80000000ULL, -1, 0x100000000ULL))
    abort();
#endif
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor" }
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn check(count: i32, left: u64, signed_right: i64, unsigned_right: u64) -> i32 {
// I686-PC-WINDOWS-MSVC-NEXT: fn check(count: i32, left: u64, signed_right: i64, unsigned_right: u64) -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     (9223372036854775809u64 << (count & 63) != left
// X86_64-PC-WINDOWS-MSVC-NEXT:         || -16i64 >> (count & 63) != signed_right
// X86_64-PC-WINDOWS-MSVC-NEXT:         || 9223372036854775808u64 >> (count & 63) != unsigned_right) as i32
// I686-PC-WINDOWS-MSVC-NEXT:     (9223372036854775809u64 << (count & 31) != left
// I686-PC-WINDOWS-MSVC-NEXT:         || -16i64 >> (count & 31) != signed_right
// I686-PC-WINDOWS-MSVC-NEXT:         || 9223372036854775808u64 >> (count & 31) != unsigned_right) as i32
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     if check(64i32, 9223372036854775809u64, -16i64, 9223372036854775808u64) != 0
// I686-PC-WINDOWS-MSVC-NEXT:     if check(64i32, 9223372036854775809u64, -16i64, 9223372036854775808u64) != 0
// X86_64-PC-WINDOWS-MSVC-NEXT:         || check(65i32, 2u64, -8i64, 4611686018427387904u64) != 0 {
// I686-PC-WINDOWS-MSVC-NEXT:         || check(65i32, 2u64, -8i64, 4611686018427387904u64) != 0 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         std::process::abort();
// I686-PC-WINDOWS-MSVC-NEXT:         std::process::abort();
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     if check(32i32, 4294967296u64, -1i64, 2147483648u64) != 0
// X86_64-PC-WINDOWS-MSVC-NEXT:         || check(-1i32, 9223372036854775808u64, -1i64, 1u64) != 0 {
// I686-PC-WINDOWS-MSVC-NEXT:     if check(32i32, 9223372036854775809u64, -16i64, 9223372036854775808u64) != 0
// I686-PC-WINDOWS-MSVC-NEXT:         || check(-1i32, 2147483648u64, -1i64, 4294967296u64) != 0 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         std::process::abort();
// I686-PC-WINDOWS-MSVC-NEXT:         std::process::abort();
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// CHECK-NEXT:     0
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
