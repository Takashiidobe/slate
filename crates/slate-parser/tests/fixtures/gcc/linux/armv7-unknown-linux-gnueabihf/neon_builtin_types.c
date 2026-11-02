// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES NEON
typedef __simd64_int8_t int8x8_t;
unsigned int fpscr(void) { return __builtin_arm_get_fpscr(); }
int8x8_t abs8(int8x8_t a) { return __builtin_neon_vabsv8qi(a); }

// SLATE-FILECHECK-BEGIN NEON
// NEON: module {
// NEON-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// NEON-NEXT:         endian = little;
// NEON-NEXT:         pointer [size=4, align=4];
// NEON-NEXT:         stack_alignment = 8;
// NEON-NEXT:         long_double = f64;
// NEON-NEXT:         storage bool [size=1, align=1];
// NEON-NEXT:         storage i8, u8 [size=1, align=1];
// NEON-NEXT:         storage i16, u16 [size=2, align=2];
// NEON-NEXT:         storage i32, u32 [size=4, align=4];
// NEON-NEXT:         storage i64, u64 [size=8, align=8];
// NEON-NEXT:         storage bf16 [size=2, align=2];
// NEON-NEXT:         storage f16 [size=2, align=2];
// NEON-NEXT:         storage f32 [size=4, align=4];
// NEON-NEXT:         storage f64 [size=8, align=8];
// NEON-NEXT:         storage f128 [size=16, align=16];
// NEON-NEXT:         storage d32 [size=4, align=4];
// NEON-NEXT:         storage d64 [size=8, align=8];
// NEON-NEXT:         storage d128 [size=16, align=16];
// NEON-NEXT:     }
// NEON-NEXT:     type @type[[TYPE_int8x8_t:[0-9]+]] int8x8_t = vector<i8, 8>;
// NEON-NEXT:     fn %[[VALUE___builtin_arm_get_fpscr:[0-9]+]] @__builtin_arm_get_fpscr() -> u32 [linkage=external];
// NEON-NEXT:     fn %[[VALUE_fpscr:[0-9]+]] @fpscr() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// NEON-NEXT:         return call<u32>(%[[VALUE___builtin_arm_get_fpscr]]);
// NEON-NEXT:     }
// NEON-NEXT:     fn %[[VALUE___builtin_neon_vabsv8qi:[0-9]+]] @__builtin_neon_vabsv8qi(%[[VALUE0:[0-9]+]] <unnamed>: vector<i8, 8>) -> vector<i8, 8> [linkage=external] [abi=aapcs32_hard_float(direct) -> direct];
// NEON-NEXT:     fn %[[VALUE_abs8:[0-9]+]] @abs8(%[[VALUE_a:[0-9]+]] a: vector<i8, 8> [c="int8x8_t"]) -> vector<i8, 8> [linkage=external] [abi=aapcs32_hard_float(direct) -> direct] [fallthrough=ub_if_used] [c="int8x8_t(int8x8_t)"] {
// NEON-NEXT:         return call<vector<i8, 8>, abi=aapcs32_hard_float(direct) -> direct>(%[[VALUE___builtin_neon_vabsv8qi]], read<vector<i8, 8>>(%[[VALUE_a]])) [c="int8x8_t"];
// NEON-NEXT:     }
// NEON-NEXT: }
// SLATE-FILECHECK-END NEON
