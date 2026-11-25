// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c23

typedef signed _BitInt(257) i257;
typedef unsigned _BitInt(257) u257;
typedef unsigned short u16x4 __attribute__((vector_size(8)));
typedef signed short i16x4 __attribute__((vector_size(8)));

enum { reversed_byte = __builtin_elementwise_bitreverse((unsigned char)0x12) };
constexpr signed char reversed_signed = __builtin_elementwise_bitreverse((signed char)1);
unsigned char byte = __builtin_elementwise_bitreverse((unsigned char)0x12);
u257 wide = __builtin_elementwise_bitreverse(((u257)1 << 256) | ((u257)1 << 192) | 1);
u16x4 lanes = __builtin_elementwise_bitreverse((u16x4){1, 2, 3, 4} + 1);
i16x4 partial = __builtin_elementwise_bitreverse((i16x4){1, -2, 3});
u16x4 nested = __builtin_elementwise_bitreverse(
    __builtin_elementwise_bitreverse((u16x4){1, 2, 3, 4}));

_Static_assert(reversed_byte == 0x48, "the byte stays eight bits");
_Static_assert(reversed_signed == -128, "the reversed sign bit");
_Static_assert(__builtin_elementwise_bitreverse((signed _BitInt(17))1) == -65536, "narrow bit-int width");
_Static_assert(__builtin_elementwise_bitreverse((i257)-2) == (i257)(((u257)1 << 256) - 1),
               "wide signed bit reversal");

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
// DEFAULT-NEXT:     type @type[[TYPE_i257:[0-9]+]] i257 = i257b;
// DEFAULT-NEXT:     type @type[[TYPE_u257:[0-9]+]] u257 = u257b;
// DEFAULT-NEXT:     type @type[[TYPE_u16x4:[0-9]+]] u16x4 = vector<u16, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_i16x4:[0-9]+]] i16x4 = vector<i16, 4>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_reversed_byte:[0-9]+]] reversed_byte = const<i32>(72);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_reversed_signed:[0-9]+]] reversed_signed: i8 [storage=static] [const] [constexpr] = const<i8>(-128) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_byte:[0-9]+]] byte: u8 [storage=static] = const<u8>(72) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: u257b [storage=static] = const<u257b>(115792089237316195423570985008687907853269984665640564039476030751986839191553) [c="u257"] [linkage=external] [c="u257"];
// DEFAULT-NEXT:     global %[[VALUE_lanes:[0-9]+]] lanes: vector<u16, 4> [storage=static] = aggregate<vector<u16, 4>, zero_fill=false>(index0 = const<u16>(16384), index1 = const<u16>(49152), index2 = const<u16>(8192), index3 = const<u16>(40960)) [c="u16x4"] [linkage=external] [c="u16x4"];
// DEFAULT-NEXT:     global %[[VALUE_partial:[0-9]+]] partial: vector<i16, 4> [storage=static] = aggregate<vector<i16, 4>, zero_fill=false>(index0 = const<i16>(-32768), index1 = const<i16>(32767), index2 = const<i16>(-16384), index3 = const<i16>(0)) [c="i16x4"] [linkage=external] [c="i16x4"];
// DEFAULT-NEXT:     global %[[VALUE_nested:[0-9]+]] nested: vector<u16, 4> [storage=static] = aggregate<vector<u16, 4>, zero_fill=false>(index0 = const<u16>(1), index1 = const<u16>(2), index2 = const<u16>(3), index3 = const<u16>(4)) [c="u16x4"] [linkage=external] [c="u16x4"];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
