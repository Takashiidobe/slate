// SLATE-BUILD-TARGETS: x86_64-unknown-freebsd aarch64-unknown-freebsd
#include <dirent.h>

unsigned long entry_size(struct dirent *entry) { return sizeof(*entry); }
char *entry_name(struct dirent *entry) { return entry->d_name; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-freebsd aarch64-unknown-freebsd
// CHECK: {{^}}#[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct dirent {
// CHECK-NEXT:     d_fileno: u64,
// CHECK-NEXT:     d_off: i64,
// CHECK-NEXT:     d_reclen: u16,
// CHECK-NEXT:     d_type: u8,
// CHECK-NEXT:     d_pad0: u8,
// CHECK-NEXT:     d_namlen: u16,
// CHECK-NEXT:     d_pad1: u16,
// X86_64-UNKNOWN-FREEBSD-NEXT:     d_name: [i8; 256],
// AARCH64-UNKNOWN-FREEBSD-NEXT:     d_name: [u8; 256],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn entry_size(entry: *mut dirent) -> u64 {
// CHECK-NEXT:     std::mem::size_of::<dirent>() as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// X86_64-UNKNOWN-FREEBSD-NEXT: extern "C-unwind" fn entry_name(entry: *mut dirent) -> *mut i8 {
// X86_64-UNKNOWN-FREEBSD-NEXT:     unsafe { (*entry).d_name.as_mut_ptr() as *mut i8 }
// AARCH64-UNKNOWN-FREEBSD-NEXT: extern "C-unwind" fn entry_name(entry: *mut dirent) -> *mut u8 {
// AARCH64-UNKNOWN-FREEBSD-NEXT:     unsafe { (*entry).d_name.as_mut_ptr() as *mut u8 }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
