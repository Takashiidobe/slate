

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
// DEFAULT-NEXT:     type @type0 __time_t = i64;
// DEFAULT-NEXT:     type @type1 time_t = i64;
// DEFAULT-NEXT:     type @type2 tm = struct {
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
// DEFAULT-NEXT:     global %22 .str22: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%17 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @gmtime_r(%18 __timer: ptr<const i64> [restrict], %19 __tp: ptr<@type2> [restrict]) -> ptr<@type2> [linkage=external];
// DEFAULT-NEXT:     fn %10 @localtime_r(%20 __timer: ptr<const i64> [restrict], %21 __tp: ptr<@type2> [restrict]) -> ptr<@type2> [linkage=external];
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 timestamp: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %13 utc: @type2 [storage=automatic] = aggregate<@type2, zero_fill=true>();
// DEFAULT-NEXT:         let %14 local: @type2 [storage=automatic] = aggregate<@type2, zero_fill=true>();
// DEFAULT-NEXT:         let %15 utc_result: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<ptr<@type2>>(call<ptr<@type2>, signature=fn(ptr<const i64>, ptr<@type2>) -> ptr<@type2>>(%7, pointer_cast<ptr<const i64>, reason=arg>(addr_of<ptr<i64>>(%12)), addr_of<ptr<@type2>>(%13)), addr_of<ptr<@type2>>(%13)));
// DEFAULT-NEXT:         let %16 local_result: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<ptr<@type2>>(call<ptr<@type2>, signature=fn(ptr<const i64>, ptr<@type2>) -> ptr<@type2>>(%10, pointer_cast<ptr<const i64>, reason=arg>(addr_of<ptr<i64>>(%12)), addr_of<ptr<@type2>>(%14)), addr_of<ptr<@type2>>(%14)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%22)), read<i32>(%15), read<i32>(%16));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
