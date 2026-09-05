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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct pw_entry {
// LOWERING-BIONIC-AARCH64-NEXT:     pw_name: *mut u8,
// LOWERING-BIONIC-X86_64-NEXT:     pw_name: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut u8 {
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { (*{{arg[0-9]+}}).pw_name };
// LOWERING-BIONIC-X86_64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut i8 {
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { (*{{arg[0-9]+}}).pw_name };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut entry: pw_entry = pw_entry {
// LOWERING-NEXT:         pw_name: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"root\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"root\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     entry.pw_name = {{__v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"root\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"root\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct pw_entry {
// REWRITES-BIONIC-AARCH64-NEXT:     pw_name: *mut u8,
// REWRITES-BIONIC-X86_64-NEXT:     pw_name: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut u8 {
// REWRITES-BIONIC-X86_64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut i8 {
// REWRITES-NEXT:     unsafe { (*{{arg[0-9]+}}).pw_name }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut entry: pw_entry = pw_entry {
// REWRITES-NEXT:         pw_name: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-BIONIC-AARCH64-NEXT:     entry.pw_name = c"root".as_ptr() as *mut u8;
// REWRITES-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// REWRITES-BIONIC-X86_64-NEXT:     entry.pw_name = c"root".as_ptr() as *mut i8;
// REWRITES-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"root".as_ptr()) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
