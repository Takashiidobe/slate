#include <dirent.h>
#include <stdio.h>
#include <string.h>

static unsigned long entry_size(struct dirent *entry) {
  return sizeof(*entry);
}

int main(void) {
  struct dirent entry = {0};
  strcpy(entry.d_name, "slate");
  printf("%lu %s\n", entry_size(&entry), entry.d_name);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: use slate_support::strcpy;
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn entry_size(entry: &slate_support::dirent) -> u64 {
// CHECK-NEXT:     std::mem::size_of::<slate_support::dirent>() as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut entry: slate_support::dirent = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: slate_support::dirent = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].d_ino = 0u64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             (entry.d_name.as_mut_ptr() as *mut i8) as *mut core::ffi::c_char,
// CHECK-NEXT:             c"slate".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lu %s\n".as_ptr(),
// CHECK-NEXT:             entry_size(&(*std::ptr::addr_of_mut!(entry))),
// CHECK-NEXT:             entry.d_name.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
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
