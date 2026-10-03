// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

int abs(int);
void _mm_pause(void);

int spin(int value) {
  _mm_pause();
  __builtin_trap();
  return abs(value);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_abs:[0-9]+]] @abs(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> i32 [linkage=external] [memory=none] [c="int(int)"] [c_builtin="abs"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// IR-NEXT:     fn %[[VALUE__mm_pause:[0-9]+]] @_mm_pause() -> void [linkage=external] [c="void(void)"] [c_builtin="_mm_pause"] [c_builtin_kind="library"] [c_builtin_header="emmintrin.h"];
// IR-NEXT:     fn %[[VALUE___builtin_trap:[0-9]+]] @__builtin_trap() -> void [linkage=external] [noreturn] [c_builtin="__builtin_trap"] [c_builtin_kind="builtin"];
// IR-NEXT:     fn %[[VALUE_spin:[0-9]+]] @spin(%[[VALUE_value:[0-9]+]] value: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// IR-NEXT:         call<void, signature=fn() -> void>(%[[VALUE__mm_pause]]);
// IR-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___builtin_trap]]);
// IR-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_abs]], read<i32>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
