// SLATE-BUILD-TARGETS: i686-unknown-linux-gnu armv7-unknown-linux-gnueabihf
// { dg-options "-D_FILE_OFFSET_BITS=64 -D_TIME_BITS=64" }
#include <sys/types.h>
#include <time.h>

_Static_assert(sizeof(off_t) == 8, "file offsets");
_Static_assert(sizeof(time_t) == 8, "time width");

off_t file_offset(off_t value) { return value; }
time_t timestamp(time_t value) { return value; }

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu armv7-unknown-linux-gnueabihf
// CHECK: {{^}}//! { dg-options "-D_FILE_OFFSET_BITS=64 -D_TIME_BITS=64" }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn file_offset(value: i64) -> i64 {
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn timestamp(value: i64) -> i64 {
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
