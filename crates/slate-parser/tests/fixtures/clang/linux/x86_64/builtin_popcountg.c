enum {
  BYTE = __builtin_popcountg((unsigned char)0xff),
  LONG = __builtin_popcountg(255UL),
  ALL = __builtin_popcountg(~0ULL),
  WIDE = __builtin_popcountg((unsigned __int128)-1),
};

int folded[BYTE == 8 && LONG == 8 && ALL == 64 && WIDE == 128 ? 1 : -1];

int count(unsigned long x) { return __builtin_popcountg(x); }

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_BYTE:[0-9]+]] BYTE = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE_LONG:[0-9]+]] LONG = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE_ALL:[0-9]+]] ALL = const<i32>(64);
// DEFAULT-NEXT:         %[[VALUE_WIDE:[0-9]+]] WIDE = const<i32>(128);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcountg:[0-9]+]] @__builtin_popcountg(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_count:[0-9]+]] @count(%[[VALUE_x:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountg]], read<u64>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
