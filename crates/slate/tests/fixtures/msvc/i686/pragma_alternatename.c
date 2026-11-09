/* { dg-options "/Zc:preprocessor /O2 /MT /TC" } */
#include <stddef.h>
void abort(void);

int missing_defined(void);
size_t missing_libc(const char *);
int present(void);
int fallback_unused(void);

#pragma comment(linker, "/alternatename:_missing_defined=_fallback_defined")
#pragma comment(linker, "/alternatename:_missing_libc=_strlen")
#pragma comment(linker, "/alternatename:_present=_fallback_unused")

int fallback_defined(void) { return 7; }
int present(void) { return 1; }
int fallback_unused(void) { return 99; }

int main(void) {
  if (missing_defined() != 7)
    abort();
  if (missing_libc("four") != 4)
    abort();
  if (present() != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor /O2 /MT /TC" }
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "strlen"]
// CHECK-NEXT:     fn missing_libc([[ARG_0:arg[0-9]+]]: *const i8) -> u32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fallback_defined() -> i32 {
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn present() -> i32 {
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fallback_unused() -> i32 {
// CHECK-NEXT:     99
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if fallback_defined() != 7 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { missing_libc(c"four".as_ptr()) }) != 4 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if present() != 1 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
