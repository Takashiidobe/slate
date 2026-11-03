// SLATE-BUILD-TARGETS: x86_64-unknown-linux-gnu
#include <dirent.h>

unsigned long entry_size(struct dirent *entry) { return sizeof(*entry); }
char *entry_name(struct dirent *entry) { return entry->d_name; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn entry_size(entry: *mut slate_support::dirent) -> u64 {
// CHECK-NEXT:     std::mem::size_of::<slate_support::dirent>() as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn entry_name(entry: *mut slate_support::dirent) -> *mut i8 {
// CHECK-NEXT:     unsafe { (*entry).d_name.as_mut_ptr() as *mut i8 }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
