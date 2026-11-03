// SLATE-BUILD-TARGETS: x86_64-unknown-linux-gnu
#include <stdint.h>

struct mach_timebase_info {
    unsigned int numer;
    unsigned int denom;
};

uint64_t mach_absolute_time(void) { return 42; }
int mach_timebase_info(struct mach_timebase_info *info) {
    info->numer = 1;
    info->denom = 1;
    return 0;
}

uint64_t clock_ticks(void) { return mach_absolute_time(); }
int clock_scale(struct mach_timebase_info *info) {
    return mach_timebase_info(info);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mach_timebase_info {
// CHECK-NEXT:     numer: u32,
// CHECK-NEXT:     denom: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn mach_absolute_time() -> u64 {
// CHECK-NEXT:     42
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn mach_timebase_info(info: *mut mach_timebase_info) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*info).numer = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*info).denom = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn clock_ticks() -> u64 {
// CHECK-NEXT:     mach_absolute_time()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn clock_scale(info: *mut mach_timebase_info) -> i32 {
// CHECK-NEXT:     mach_timebase_info(info)
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
