#include "bitfield_width_header_function.h"

#define WIDTH(n) (__builtin_constant_p(n) ? 31 - __builtin_clz(n) : header_log2(n))

struct entry {
  unsigned char index : WIDTH(8);
  unsigned char exp : 1 ? 2 : header_log2(4);
};

int read_index(struct entry *e) { return e->index + e->exp; }

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
// DEFAULT-NEXT:     type @type[[TYPE_entry:[0-9]+]] entry = struct {
// DEFAULT-NEXT:         field0 index: u8 : 3;
// DEFAULT-NEXT:         field1 exp: u8 : 2;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 0], bit_offsets=[Some(0), Some(3)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clz:[0-9]+]] @__builtin_clz(%[[VALUE0:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_header_log2:[0-9]+]] @header_log2(%[[VALUE_n:[0-9]+]] n: u32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(const<i32>(31), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], read<u32>(%[[VALUE_n]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_read_index:[0-9]+]] @read_index(%[[VALUE_e:[0-9]+]] e: ptr<@type[[TYPE_entry]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield0<unit=0, bytes=0..1, bits=0..3>(deref(read<ptr<@type[[TYPE_entry]]>>(%[[VALUE_e]])))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield1<unit=0, bytes=0..1, bits=3..5>(deref(read<ptr<@type[[TYPE_entry]]>>(%[[VALUE_e]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
