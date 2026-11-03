// SLATE-BUILD-TARGETS: x86_64-apple-darwin aarch64-apple-darwin
#include <dirent.h>

unsigned long entry_size(struct dirent *entry) { return sizeof(*entry); }
char *entry_name(struct dirent *entry) { return entry->d_name; }

// SLATE-FILECHECK-BEGIN x86_64-apple-darwin aarch64-apple-darwin
// X86_64-APPLE-DARWIN: {{^}}#[repr(C)]
// X86_64-APPLE-DARWIN-NEXT: #[derive(Clone, Copy)]
// X86_64-APPLE-DARWIN-NEXT: struct dirent {
// X86_64-APPLE-DARWIN-NEXT:     d_ino: u32,
// X86_64-APPLE-DARWIN-NEXT:     d_reclen: u16,
// X86_64-APPLE-DARWIN-NEXT:     d_type: u8,
// X86_64-APPLE-DARWIN-NEXT:     d_namlen: u8,
// X86_64-APPLE-DARWIN-NEXT:     d_name: [i8; 256],
// AARCH64-APPLE-DARWIN: {{^}}#[unsafe(no_mangle)]
// AARCH64-APPLE-DARWIN-NEXT: extern "C-unwind" fn entry_size(entry: *mut slate_support::dirent) -> u64 {
// AARCH64-APPLE-DARWIN-NEXT:     std::mem::size_of::<slate_support::dirent>() as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// X86_64-APPLE-DARWIN-NEXT: extern "C-unwind" fn entry_size(entry: *mut dirent) -> u64 {
// X86_64-APPLE-DARWIN-NEXT:     std::mem::size_of::<dirent>() as u64
// X86_64-APPLE-DARWIN-NEXT: }
// X86_64-APPLE-DARWIN-EMPTY:
// X86_64-APPLE-DARWIN-NEXT: #[unsafe(no_mangle)]
// X86_64-APPLE-DARWIN-NEXT: extern "C-unwind" fn entry_name(entry: *mut dirent) -> *mut i8 {
// AARCH64-APPLE-DARWIN-NEXT: extern "C-unwind" fn entry_name(entry: *mut slate_support::dirent) -> *mut i8 {
// CHECK-NEXT:     unsafe { (*entry).d_name.as_mut_ptr() as *mut i8 }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
