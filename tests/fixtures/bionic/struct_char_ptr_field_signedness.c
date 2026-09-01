#include <string.h>

struct pw_entry {
  char *pw_name;
};

char *entry_name(struct pw_entry *entry) {
  char *local = entry->pw_name;
  return local;
}

int main(void) {
  struct pw_entry entry;
  entry.pw_name = "root";
  return strcmp(entry_name(&entry), "root") == 0 ? 0 : 1;
}

// SLATE-FILECHECK-BEGIN lowering-bionic-aarch64
// LOWERING-BIONIC-AARCH64: #![allow(
// LOWERING-BIONIC-AARCH64-NEXT:     dead_code,
// LOWERING-BIONIC-AARCH64-NEXT:     unused,
// LOWERING-BIONIC-AARCH64-NEXT:     non_camel_case_types,
// LOWERING-BIONIC-AARCH64-NEXT:     non_snake_case,
// LOWERING-BIONIC-AARCH64-NEXT:     non_upper_case_globals,
// LOWERING-BIONIC-AARCH64-NEXT:     arithmetic_overflow,
// LOWERING-BIONIC-AARCH64-NEXT:     unconditional_panic,
// LOWERING-BIONIC-AARCH64-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-BIONIC-AARCH64-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-BIONIC-AARCH64-NEXT:     unused_comparisons
// LOWERING-BIONIC-AARCH64-NEXT: )]
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: #[repr(C)]
// LOWERING-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// LOWERING-BIONIC-AARCH64-NEXT: struct pw_entry {
// LOWERING-BIONIC-AARCH64-NEXT:     pw_name: *mut u8,
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: unsafe extern "C" {
// LOWERING-BIONIC-AARCH64-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut u8 {
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { (*{{arg[0-9]+}}).pw_name };
// LOWERING-BIONIC-AARCH64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn main() {
// LOWERING-BIONIC-AARCH64-NEXT:     let mut entry: pw_entry = pw_entry {
// LOWERING-BIONIC-AARCH64-NEXT:         pw_name: std::ptr::null_mut(),
// LOWERING-BIONIC-AARCH64-NEXT:     };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = b"root\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-AARCH64-NEXT:     entry.pw_name = {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = b"root\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-BIONIC-AARCH64-NEXT:         strcmp(
// LOWERING-BIONIC-AARCH64-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-BIONIC-AARCH64-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-BIONIC-AARCH64-NEXT:         )
// LOWERING-BIONIC-AARCH64-NEXT:     };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-BIONIC-AARCH64-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-aarch64

// SLATE-FILECHECK-BEGIN lowering-bionic-x86_64
// LOWERING-BIONIC-X86_64: #![allow(
// LOWERING-BIONIC-X86_64-NEXT:     dead_code,
// LOWERING-BIONIC-X86_64-NEXT:     unused,
// LOWERING-BIONIC-X86_64-NEXT:     non_camel_case_types,
// LOWERING-BIONIC-X86_64-NEXT:     non_snake_case,
// LOWERING-BIONIC-X86_64-NEXT:     non_upper_case_globals,
// LOWERING-BIONIC-X86_64-NEXT:     arithmetic_overflow,
// LOWERING-BIONIC-X86_64-NEXT:     unconditional_panic,
// LOWERING-BIONIC-X86_64-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-BIONIC-X86_64-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-BIONIC-X86_64-NEXT:     unused_comparisons
// LOWERING-BIONIC-X86_64-NEXT: )]
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: #[repr(C)]
// LOWERING-BIONIC-X86_64-NEXT: #[derive(Clone, Copy)]
// LOWERING-BIONIC-X86_64-NEXT: struct pw_entry {
// LOWERING-BIONIC-X86_64-NEXT:     pw_name: *mut i8,
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: unsafe extern "C" {
// LOWERING-BIONIC-X86_64-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut i8 {
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { (*{{arg[0-9]+}}).pw_name };
// LOWERING-BIONIC-X86_64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn main() {
// LOWERING-BIONIC-X86_64-NEXT:     let mut entry: pw_entry = pw_entry {
// LOWERING-BIONIC-X86_64-NEXT:         pw_name: std::ptr::null_mut(),
// LOWERING-BIONIC-X86_64-NEXT:     };
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"root\0".as_ptr() as *mut i8;
// LOWERING-BIONIC-X86_64-NEXT:     entry.pw_name = {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"root\0".as_ptr() as *mut i8;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-BIONIC-X86_64-NEXT:         strcmp(
// LOWERING-BIONIC-X86_64-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-BIONIC-X86_64-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-BIONIC-X86_64-NEXT:         )
// LOWERING-BIONIC-X86_64-NEXT:     };
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-BIONIC-X86_64-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-BIONIC-X86_64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-x86_64

// SLATE-FILECHECK-BEGIN rewrites-bionic-aarch64
// REWRITES-BIONIC-AARCH64: #![allow(
// REWRITES-BIONIC-AARCH64-NEXT:     dead_code,
// REWRITES-BIONIC-AARCH64-NEXT:     unused,
// REWRITES-BIONIC-AARCH64-NEXT:     non_camel_case_types,
// REWRITES-BIONIC-AARCH64-NEXT:     non_snake_case,
// REWRITES-BIONIC-AARCH64-NEXT:     non_upper_case_globals,
// REWRITES-BIONIC-AARCH64-NEXT:     arithmetic_overflow,
// REWRITES-BIONIC-AARCH64-NEXT:     unconditional_panic,
// REWRITES-BIONIC-AARCH64-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-BIONIC-AARCH64-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-BIONIC-AARCH64-NEXT:     unused_comparisons
// REWRITES-BIONIC-AARCH64-NEXT: )]
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: #[repr(C)]
// REWRITES-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// REWRITES-BIONIC-AARCH64-NEXT: struct pw_entry {
// REWRITES-BIONIC-AARCH64-NEXT:     pw_name: *mut u8,
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: unsafe extern "C" {
// REWRITES-BIONIC-AARCH64-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut u8 {
// REWRITES-BIONIC-AARCH64-NEXT:     return unsafe { (*{{arg[0-9]+}}).pw_name };
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn main() {
// REWRITES-BIONIC-AARCH64-NEXT:     let mut entry: pw_entry = pw_entry {
// REWRITES-BIONIC-AARCH64-NEXT:         pw_name: std::ptr::null_mut(),
// REWRITES-BIONIC-AARCH64-NEXT:     };
// REWRITES-BIONIC-AARCH64-NEXT:     entry.pw_name = b"root\0".as_ptr() as *mut u8;
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const core::ffi::c_char, c"root".as_ptr()) };
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} == {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-BIONIC-AARCH64-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-aarch64

// SLATE-FILECHECK-BEGIN rewrites-bionic-x86_64
// REWRITES-BIONIC-X86_64: #![allow(
// REWRITES-BIONIC-X86_64-NEXT:     dead_code,
// REWRITES-BIONIC-X86_64-NEXT:     unused,
// REWRITES-BIONIC-X86_64-NEXT:     non_camel_case_types,
// REWRITES-BIONIC-X86_64-NEXT:     non_snake_case,
// REWRITES-BIONIC-X86_64-NEXT:     non_upper_case_globals,
// REWRITES-BIONIC-X86_64-NEXT:     arithmetic_overflow,
// REWRITES-BIONIC-X86_64-NEXT:     unconditional_panic,
// REWRITES-BIONIC-X86_64-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-BIONIC-X86_64-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-BIONIC-X86_64-NEXT:     unused_comparisons
// REWRITES-BIONIC-X86_64-NEXT: )]
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: #[repr(C)]
// REWRITES-BIONIC-X86_64-NEXT: #[derive(Clone, Copy)]
// REWRITES-BIONIC-X86_64-NEXT: struct pw_entry {
// REWRITES-BIONIC-X86_64-NEXT:     pw_name: *mut i8,
// REWRITES-BIONIC-X86_64-NEXT: }
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: unsafe extern "C" {
// REWRITES-BIONIC-X86_64-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-BIONIC-X86_64-NEXT: }
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut i8 {
// REWRITES-BIONIC-X86_64-NEXT:     return unsafe { (*{{arg[0-9]+}}).pw_name };
// REWRITES-BIONIC-X86_64-NEXT: }
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: fn main() {
// REWRITES-BIONIC-X86_64-NEXT:     let mut entry: pw_entry = pw_entry {
// REWRITES-BIONIC-X86_64-NEXT:         pw_name: std::ptr::null_mut(),
// REWRITES-BIONIC-X86_64-NEXT:     };
// REWRITES-BIONIC-X86_64-NEXT:     entry.pw_name = b"root\0".as_ptr() as *mut i8;
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const core::ffi::c_char, c"root".as_ptr()) };
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} == {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-BIONIC-X86_64-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-BIONIC-X86_64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-x86_64
