
#include <stdio.h>
#include <time.h>

int main(void) {
  struct timespec value    = {0};
  int             result   = timespec_get(&value, TIME_UTC);
  int nanoseconds_in_range = value.tv_nsec >= 0 && value.tv_nsec < 1000000000L;
  printf("%d %d\n", result == TIME_UTC, nanoseconds_in_range);
  return 0;
}



// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE___time_t:[0-9]+]] __time_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___syscall_slong_t:[0-9]+]] __syscall_slong_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_timespec:[0-9]+]] timespec = struct {
// DEFAULT-NEXT:         field0 tv_sec: i64;
// DEFAULT-NEXT:         field1 tv_nsec: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_timespec_get:[0-9]+]] @timespec_get(%[[VALUE___ts:[0-9]+]] __ts: ptr<@type[[TYPE_timespec]]>, %[[VALUE___base:[0-9]+]] __base: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: @type[[TYPE_timespec]] [storage=automatic] = aggregate<@type[[TYPE_timespec]], zero_fill=true>(field0 = widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic] = call<i32, signature=fn(ptr<@type[[TYPE_timespec]]>, i32) -> i32>(%[[VALUE_timespec_get]], addr_of<ptr<@type[[TYPE_timespec]]>>(%[[VALUE_value]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_nanoseconds_in_range:[0-9]+]] nanoseconds_in_range: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(ge<i64>(read<i64>(field1(%[[VALUE_value]])), widen<i64, reason=usual_arith>(const<i32>(0))), lt<i64>(read<i64>(field1(%[[VALUE_value]])), const<i64>(1000000000))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), from_bool<i32, reason=vararg>(eq<i32>(read<i32>(%[[VALUE_result]]), const<i32>(1))), read<i32>(%[[VALUE_nanoseconds_in_range]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
