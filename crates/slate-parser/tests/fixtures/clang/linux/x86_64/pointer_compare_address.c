

#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>

int main(void) {
  time_t    timestamp    = 0;
  struct tm utc          = {};
  struct tm local        = {};
  int       utc_result   = gmtime_r(&timestamp, &utc) == &utc;
  int       local_result = localtime_r(&timestamp, &local) == &local;
  printf("%d %d\n", utc_result, local_result);
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
// DEFAULT-NEXT:     type @type[[TYPE_time_t:[0-9]+]] time_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_tm:[0-9]+]] tm = struct {
// DEFAULT-NEXT:         field0 tm_sec: i32;
// DEFAULT-NEXT:         field1 tm_min: i32;
// DEFAULT-NEXT:         field2 tm_hour: i32;
// DEFAULT-NEXT:         field3 tm_mday: i32;
// DEFAULT-NEXT:         field4 tm_mon: i32;
// DEFAULT-NEXT:         field5 tm_year: i32;
// DEFAULT-NEXT:         field6 tm_wday: i32;
// DEFAULT-NEXT:         field7 tm_yday: i32;
// DEFAULT-NEXT:         field8 tm_isdst: i32;
// DEFAULT-NEXT:         field9 tm_gmtoff: i64;
// DEFAULT-NEXT:         field10 tm_zone: ptr<const i8>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 4, 8, 12, 16, 20, 24, 28, 32, 40, 48]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gmtime_r:[0-9]+]] @gmtime_r(%[[VALUE___timer:[0-9]+]] __timer: ptr<const i64> [restrict], %[[VALUE___tp:[0-9]+]] __tp: ptr<@type[[TYPE_tm]]> [restrict]) -> ptr<@type[[TYPE_tm]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_localtime_r:[0-9]+]] @localtime_r(%[[VALUE___timer_2:[0-9]+]] __timer: ptr<const i64> [restrict], %[[VALUE___tp_2:[0-9]+]] __tp: ptr<@type[[TYPE_tm]]> [restrict]) -> ptr<@type[[TYPE_tm]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_timestamp:[0-9]+]] timestamp: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_utc:[0-9]+]] utc: @type[[TYPE_tm]] [storage=automatic] = aggregate<@type[[TYPE_tm]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: @type[[TYPE_tm]] [storage=automatic] = aggregate<@type[[TYPE_tm]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_utc_result:[0-9]+]] utc_result: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<ptr<@type[[TYPE_tm]]>>(call<ptr<@type[[TYPE_tm]]>, signature=fn(ptr<const i64>, ptr<@type[[TYPE_tm]]>) -> ptr<@type[[TYPE_tm]]>>(%[[VALUE_gmtime_r]], pointer_cast<ptr<const i64>, reason=arg>(addr_of<ptr<i64>>(%[[VALUE_timestamp]])), addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_utc]])), addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_utc]])));
// DEFAULT-NEXT:         let %[[VALUE_local_result:[0-9]+]] local_result: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<ptr<@type[[TYPE_tm]]>>(call<ptr<@type[[TYPE_tm]]>, signature=fn(ptr<const i64>, ptr<@type[[TYPE_tm]]>) -> ptr<@type[[TYPE_tm]]>>(%[[VALUE_localtime_r]], pointer_cast<ptr<const i64>, reason=arg>(addr_of<ptr<i64>>(%[[VALUE_timestamp]])), addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_local]])), addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_local]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), read<i32>(%[[VALUE_utc_result]]), read<i32>(%[[VALUE_local_result]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
