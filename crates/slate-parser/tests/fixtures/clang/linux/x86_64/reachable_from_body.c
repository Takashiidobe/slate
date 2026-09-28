#include "reachable_from_body/reach.h"

int main(void) {
  struct reach_point point = {1, 2};
  reach_size_t       size  = sizeof(struct reach_point);
  reach_counter += REACH_GREEN;
  return reach_called(&point) + (int)(reach_cast_t)size;
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
// DEFAULT-NEXT:     type @type0 reach_size_t = u64;
// DEFAULT-NEXT:     type @type1 reach_cast_t = i64;
// DEFAULT-NEXT:     type @type2 reach_point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type3 reach_color = enum : u32 {
// DEFAULT-NEXT:         %0 REACH_RED = const<i32>(0);
// DEFAULT-NEXT:         %1 REACH_GREEN = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     extern %7 reach_counter: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @reach_called(%11 point: ptr<@type2>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 point: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// DEFAULT-NEXT:         let %10 size: u64 [storage=automatic] = const<u64>(8);
// DEFAULT-NEXT:         let %12: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%13));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(ptr<@type2>) -> i32>(%6, addr_of<ptr<@type2>>(%9)), truncate<i32, reason=explicit, fits=unknown>(reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
