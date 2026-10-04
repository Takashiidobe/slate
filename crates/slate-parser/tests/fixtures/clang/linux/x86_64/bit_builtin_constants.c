// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT

enum {
  CLZ = __builtin_clz(1U),
  CLZ_NEGATIVE = __builtin_clz(-1),
  CLZL = __builtin_clzl(1),
  CLZLL = 63 - __builtin_clzll(0x400000000ULL),
  CTZ = __builtin_ctz(8),
  POPCOUNT = __builtin_popcount(-1),
  PARITY = __builtin_parity(7U),
  FFS_ZERO = __builtin_ffs(0),
  FFSLL = __builtin_ffsll(0x100000000LL),
  CLRSB_ZERO = __builtin_clrsb(0),
  CLRSB_NEGATIVE = __builtin_clrsb(-1),
  CLRSB_ONE = __builtin_clrsb(1),
  BSWAP16 = __builtin_bswap16(0x1234),
  CLZG_FALLBACK = __builtin_clzg(0U, 7),
  CLZG_CHAR = __builtin_clzg((unsigned char)1),
  CTZG_FALLBACK = __builtin_ctzg(0ULL, -1),
};

struct ilog2 {
  int a[__builtin_constant_p(0x400000000ULL) ? 63 - __builtin_clzll(0x400000000ULL) : 1];
};

static const char *const names[] = {[__builtin_ctz(4)] = "four"};

_Static_assert(__builtin_bswap32(0x12345678U) == 0x78563412U, "bswap32");
_Static_assert(__builtin_bswap64(1ULL) == 0x0100000000000000ULL, "bswap64");

int values[] = {CLZ, CLZ_NEGATIVE, CLZL, CLZLL, CTZ, POPCOUNT, PARITY, FFS_ZERO, FFSLL,
                CLRSB_ZERO, CLRSB_NEGATIVE, CLRSB_ONE, BSWAP16, CLZG_FALLBACK, CLZG_CHAR,
                CTZG_FALLBACK};
int ilog2_size = sizeof(struct ilog2);
int names_size = sizeof(names);

enum {
  BITREVERSE8 = __builtin_bitreverse8(1),
  CLZS = __builtin_clzs(1),
};
int clang_values[] = {BITREVERSE8, CLZS};

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : i32 {
// DEFAULT-NEXT:         %[[VALUE_CLZ:[0-9]+]] CLZ = const<i32>(31);
// DEFAULT-NEXT:         %[[VALUE_CLZ_NEGATIVE:[0-9]+]] CLZ_NEGATIVE = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_CLZL:[0-9]+]] CLZL = const<i32>(63);
// DEFAULT-NEXT:         %[[VALUE_CLZLL:[0-9]+]] CLZLL = const<i32>(34);
// DEFAULT-NEXT:         %[[VALUE_CTZ:[0-9]+]] CTZ = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_POPCOUNT:[0-9]+]] POPCOUNT = const<i32>(32);
// DEFAULT-NEXT:         %[[VALUE_PARITY:[0-9]+]] PARITY = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_FFS_ZERO:[0-9]+]] FFS_ZERO = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_FFSLL:[0-9]+]] FFSLL = const<i32>(33);
// DEFAULT-NEXT:         %[[VALUE_CLRSB_ZERO:[0-9]+]] CLRSB_ZERO = const<i32>(31);
// DEFAULT-NEXT:         %[[VALUE_CLRSB_NEGATIVE:[0-9]+]] CLRSB_NEGATIVE = const<i32>(31);
// DEFAULT-NEXT:         %[[VALUE_CLRSB_ONE:[0-9]+]] CLRSB_ONE = const<i32>(30);
// DEFAULT-NEXT:         %[[VALUE_BSWAP16:[0-9]+]] BSWAP16 = const<i32>(13330);
// DEFAULT-NEXT:         %[[VALUE_CLZG_FALLBACK:[0-9]+]] CLZG_FALLBACK = const<i32>(7);
// DEFAULT-NEXT:         %[[VALUE_CLZG_CHAR:[0-9]+]] CLZG_CHAR = const<i32>(7);
// DEFAULT-NEXT:         %[[VALUE_CTZG_FALLBACK:[0-9]+]] CTZG_FALLBACK = const<i32>(-1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_ilog2:[0-9]+]] ilog2 = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 34>;
// DEFAULT-NEXT:     } [size=136, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_CLZ]] BITREVERSE8 = const<i32>(128);
// DEFAULT-NEXT:         %[[VALUE_CLZ_NEGATIVE]] CLZS = const<i32>(15);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 111, 117, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_names:[0-9]+]] names: array<ptr<const i8>, 3> [storage=static] [const] [align=16] = aggregate<array<ptr<const i8>, 3>, zero_fill=true>(index2 = pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]]))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_values:[0-9]+]] values: array<i32, 16> [storage=static] [align=16] = aggregate<array<i32, 16>, zero_fill=false>(index0 = const<i32>(31), index1 = const<i32>(0), index2 = const<i32>(63), index3 = const<i32>(34), index4 = const<i32>(3), index5 = const<i32>(32), index6 = const<i32>(1), index7 = const<i32>(0), index8 = const<i32>(33), index9 = const<i32>(31), index10 = const<i32>(31), index11 = const<i32>(30), index12 = const<i32>(13330), index13 = const<i32>(7), index14 = const<i32>(7), index15 = const<i32>(-1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ilog2_size:[0-9]+]] ilog2_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(136))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_names_size:[0-9]+]] names_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(24))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_clang_values:[0-9]+]] clang_values: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(128), index1 = const<i32>(15)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
