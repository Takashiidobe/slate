// SLATE-BUILD-TARGETS: x86_64-apple-darwin aarch64-apple-darwin
#include <mach/mach_time.h>

uint64_t clock_ticks(void) { return mach_absolute_time(); }
uint64_t continuous_ticks(void) { return mach_continuous_time(); }
int clock_wait(uint64_t deadline) { return mach_wait_until(deadline); }
int clock_scale(mach_timebase_info_t info) { return mach_timebase_info(info); }
unsigned int clock_numerator(mach_timebase_info_data_t *info) { return info->numer; }
unsigned int clock_denominator(struct mach_timebase_info *info) { return info->denom; }

// SLATE-FILECHECK-BEGIN x86_64-apple-darwin aarch64-apple-darwin
// CHECK: {{^}}use slate_support::mach::mach_time::mach_absolute_time;
// CHECK-NEXT: use slate_support::mach::mach_time::mach_continuous_time;
// CHECK-NEXT: use slate_support::mach::mach_time::mach_timebase_info;
// CHECK-NEXT: use slate_support::mach::mach_time::mach_wait_until;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn clock_ticks() -> u64 {
// CHECK-NEXT:     unsafe { mach_absolute_time() as u64 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn continuous_ticks() -> u64 {
// CHECK-NEXT:     unsafe { mach_continuous_time() as u64 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn clock_wait(deadline: u64) -> i32 {
// CHECK-NEXT:     unsafe { mach_wait_until(deadline as u64) as i32 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn clock_scale(
// CHECK-NEXT:     info: *mut slate_support::mach::mach_time::mach_timebase_info,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         mach_timebase_info(info as *mut slate_support::mach::mach_time::mach_timebase_info) as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn clock_numerator(
// CHECK-NEXT:     info: *mut slate_support::mach::mach_time::mach_timebase_info,
// CHECK-NEXT: ) -> u32 {
// CHECK-NEXT:     unsafe { (*info).numer }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn clock_denominator(
// CHECK-NEXT:     info: *mut slate_support::mach::mach_time::mach_timebase_info,
// CHECK-NEXT: ) -> u32 {
// CHECK-NEXT:     unsafe { (*info).denom }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
