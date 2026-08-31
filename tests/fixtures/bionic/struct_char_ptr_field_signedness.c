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
// LOWERING-BIONIC-AARCH64: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: #[repr(C)]
// LOWERING-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// LOWERING-BIONIC-AARCH64-NEXT: struct pw_entry {
// LOWERING-BIONIC-AARCH64-NEXT:     pw_name: *mut u8,
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: unsafe extern "C" {
// LOWERING-BIONIC-AARCH64-NEXT:     fn strcmp(_0: *const u8, _1: *const u8) -> i32;
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut u8 {
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { (*{{arg[0-9]+}}).pw_name };
// LOWERING-BIONIC-AARCH64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn main() {
// LOWERING-BIONIC-AARCH64-NEXT:     let mut entry: pw_entry = pw_entry { pw_name: std::ptr::null_mut() };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = b"root\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-AARCH64-NEXT:     entry.pw_name = {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = b"root\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const u8, {{_v[0-9]+}} as *const u8) };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-BIONIC-AARCH64-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-aarch64

// SLATE-FILECHECK-BEGIN rewrites-bionic-aarch64
// REWRITES-BIONIC-AARCH64: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: #[repr(C)]
// REWRITES-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// REWRITES-BIONIC-AARCH64-NEXT: struct pw_entry {
// REWRITES-BIONIC-AARCH64-NEXT:     pw_name: *mut u8,
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: unsafe extern "C" {
// REWRITES-BIONIC-AARCH64-NEXT:     fn strcmp(_0: *const u8, _1: *const u8) -> i32;
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: unsafe fn entry_name({{arg[0-9]+}}: *mut pw_entry) -> *mut u8 {
// REWRITES-BIONIC-AARCH64-NEXT: return unsafe { (*{{arg[0-9]+}}).pw_name };
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn main() {
// REWRITES-BIONIC-AARCH64-NEXT: let mut entry: pw_entry = pw_entry { pw_name: std::ptr::null_mut() };
// REWRITES-BIONIC-AARCH64-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT: let {{_v[0-9]+}}: *mut u8 = b"root\0".as_ptr() as *mut u8;
// REWRITES-BIONIC-AARCH64-NEXT: entry.pw_name = {{_v[0-9]+}};
// REWRITES-BIONIC-AARCH64-NEXT: let {{_v[0-9]+}}: *mut u8 = unsafe { entry_name(std::ptr::addr_of_mut!(entry)) };
// REWRITES-BIONIC-AARCH64-NEXT: let {{_v[0-9]+}}: *mut u8 = b"root\0".as_ptr() as *mut u8;
// REWRITES-BIONIC-AARCH64-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const u8, {{_v[0-9]+}} as *const u8) };
// REWRITES-BIONIC-AARCH64-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-BIONIC-AARCH64-NEXT: let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} == {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-BIONIC-AARCH64-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-aarch64
