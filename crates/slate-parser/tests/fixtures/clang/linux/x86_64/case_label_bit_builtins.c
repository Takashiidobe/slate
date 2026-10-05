#define htons(x) ((unsigned short)__builtin_bswap16((unsigned short)(x)))

int ethertype(unsigned short protocol) {
  switch (protocol) {
  case htons(0x0800):
    return 4;
  case htons(0x86DD):
    return 6;
  }
  return 0;
}

int word(unsigned int value) {
  switch (value) {
  case __builtin_bswap32(1u):
    return 32;
  case __builtin_popcount(7):
    return 3;
  }
  return 0;
}

int quad(unsigned long long value) {
  switch (value) {
  case __builtin_bswap64(1ull):
    return 64;
  case __builtin_ctzll(0x100ull):
    return 8;
  }
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
// DEFAULT-NEXT:     fn %[[VALUE_ethertype:[0-9]+]] @ethertype(%[[VALUE_protocol:[0-9]+]] protocol: u16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_protocol]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(8):
// DEFAULT-NEXT:                     return const<i32>(4);
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(56710):
// DEFAULT-NEXT:                     return const<i32>(6);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_word:[0-9]+]] @word(%[[VALUE_value:[0-9]+]] value: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] read<u32>(%[[VALUE_value]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<u32>(16777216):
// DEFAULT-NEXT:                     return const<i32>(32);
// DEFAULT-NEXT:                 case %[[VALUE1]] const<u32>(3):
// DEFAULT-NEXT:                     return const<i32>(3);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_quad:[0-9]+]] @quad(%[[VALUE_value_2:[0-9]+]] value: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE2:[0-9]+]] read<u64>(%[[VALUE_value_2]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE2]] const<u64>(72057594037927936):
// DEFAULT-NEXT:                     return const<i32>(64);
// DEFAULT-NEXT:                 case %[[VALUE2]] const<u64>(8):
// DEFAULT-NEXT:                     return const<i32>(8);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
