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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct pw_entry {
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut entry: pw_entry = pw_entry {
// COMMON-LOWERING-NEXT:         pw_name: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     entry.pw_name = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-bionic-aarch64
// LOWERING-BIONIC-AARCH64-NEXT:     pw_name: *mut u8,
// LOWERING-BIONIC-AARCH64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut u8 {
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { (*{{arg[0-9]+}}).pw_name };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"root\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"root\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-bionic-aarch64

// SLATE-FILECHECK-BEGIN lowering-bionic-x86_64
// LOWERING-BIONIC-X86_64-NEXT:     pw_name: *mut i8,
// LOWERING-BIONIC-X86_64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut i8 {
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { (*{{arg[0-9]+}}).pw_name };
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"root\0".as_ptr() as *mut i8;
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"root\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-bionic-x86_64

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct pw_entry {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     unsafe { (*{{arg[0-9]+}}).pw_name }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut entry: pw_entry = pw_entry {
// COMMON-REWRITES-NEXT:         pw_name: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"root".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-bionic-aarch64
// REWRITES-BIONIC-AARCH64-NEXT:     pw_name: *mut u8,
// REWRITES-BIONIC-AARCH64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut u8 {
// REWRITES-BIONIC-AARCH64-NEXT:     entry.pw_name = c"root".as_ptr() as *mut u8;
// REWRITES-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// SLATE-FILECHECK-END rewrites-bionic-aarch64

// SLATE-FILECHECK-BEGIN rewrites-bionic-x86_64
// REWRITES-BIONIC-X86_64-NEXT:     pw_name: *mut i8,
// REWRITES-BIONIC-X86_64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut i8 {
// REWRITES-BIONIC-X86_64-NEXT:     entry.pw_name = c"root".as_ptr() as *mut i8;
// REWRITES-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// SLATE-FILECHECK-END rewrites-bionic-x86_64
