#include <stdio.h>

struct __attribute__((packed)) MixedBits {
  unsigned char tag;
  unsigned int  low       : 3;
  signed int    delta     : 6;
  unsigned int            : 0;
  unsigned long long wide : 35;
  unsigned int       tail : 17;
};

int main(void) {
  struct MixedBits bits = {0};
  bits.tag              = 0xa5;
  bits.low              = 7;
  bits.delta            = -17;
  bits.wide             = 0x712345678ULL;
  bits.tail             = 0x1abcd;
  printf("%u %u %d %llu %u %zu\n", bits.tag, bits.low, bits.delta, bits.wide,
         bits.tail, sizeof(bits));
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
// DEFAULT-NEXT:     type @type[[TYPE_MixedBits:[0-9]+]] MixedBits = struct {
// DEFAULT-NEXT:         field0 tag: u8;
// DEFAULT-NEXT:         field1 low: u32 : 3;
// DEFAULT-NEXT:         field2 delta: i32 : 6;
// DEFAULT-NEXT:         field3 <anonymous>: u32 : 0;
// DEFAULT-NEXT:         field4 wide: u64 : 35;
// DEFAULT-NEXT:         field5 tail: u32 : 17;
// DEFAULT-NEXT:     } [size=11, align=1, offsets=[0, 1, 1, 4, 4, 8], bit_offsets=[None, Some(8), Some(11), Some(32), Some(32), Some(67)], bit_units=[(1, 2), (4, 7)], field_units=[None, Some(0), Some(0), None, Some(1), Some(1)]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([37, 117, 32, 37, 117, 32, 37, 100, 32, 37, 108, 108, 117, 32, 37, 117, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_bits:[0-9]+]] bits: @type[[TYPE_MixedBits]] [storage=automatic] = aggregate<@type[[TYPE_MixedBits]], zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(field0(%[[VALUE_bits]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(165))));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=1..3, bits=0..3>(%[[VALUE_bits]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         write<i32>(bitfield2<unit=0, bytes=1..3, bits=3..9>(%[[VALUE_bits]]), neg<i32, overflow=ub>(const<i32>(17)));
// DEFAULT-NEXT:         write<u64>(bitfield4<unit=1, bytes=4..11, bits=0..35>(%[[VALUE_bits]]), const<u64>(30370190968));
// DEFAULT-NEXT:         write<u32>(bitfield5<unit=1, bytes=4..11, bits=35..52>(%[[VALUE_bits]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(109517)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(field0(%[[VALUE_bits]])))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=1..3, bits=0..3>(%[[VALUE_bits]]))), read<i32>(bitfield2<unit=0, bytes=1..3, bits=3..9>(%[[VALUE_bits]])), read<u64>(bitfield4<unit=1, bytes=4..11, bits=0..35>(%[[VALUE_bits]])), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield5<unit=1, bytes=4..11, bits=35..52>(%[[VALUE_bits]]))), const<u64>(11));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
