/* { dg-options "/Zc:preprocessor /std:c11" } */
#if !defined(_MSC_VER) || !defined(_WIN64) || !defined(_M_ARM64) || defined(_M_X64) || _MSVC_TRADITIONAL
#error "wrong arm64 msvc predefines"
#endif

struct mixed {
  char c;
  long l;
  long long ll;
};

_Static_assert(sizeof(long) == 4, "long");
_Static_assert(sizeof(void *) == 8, "pointer");
_Static_assert(sizeof(L'a') == 2, "wchar_t");
_Static_assert(sizeof(struct mixed) == 16, "struct mixed");
_Static_assert(sizeof(long double) == 8, "long double");

int main(void) {
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor /std:c11" }
// CHECK-NEXT: fn __slate_main() -> i32 {
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
