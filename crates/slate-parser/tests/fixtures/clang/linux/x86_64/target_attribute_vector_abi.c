// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

typedef float m256 __attribute__((vector_size(32)));
typedef float m512 __attribute__((vector_size(64)));

void plain256(m256);
__attribute__((target("avx"))) void avx256(m256);
__attribute__((target("avx2"))) void avx2_256(m256);
__attribute__((target("avx,no-avx"))) void cancelled256(m256);
__attribute__((target("arch=skylake-avx512"))) void arch512(m512);
__attribute__((target("avx"))) void avx512_wide(m512);
__attribute__((target("avx512f"))) void avx512f_wide(m512);

__attribute__((target("avx"))) void defined256(m256 v) {
  avx256(v);
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
// IR-NEXT:     type @type[[TYPE_m256:[0-9]+]] m256 = vector<f32, 8>;
// IR-NEXT:     type @type[[TYPE_m512:[0-9]+]] m512 = vector<f32, 16>;
// IR-NEXT:     fn %[[VALUE_plain256:[0-9]+]] @plain256(%[[VALUE0:[0-9]+]] <unnamed>: vector<f32, 8>) -> void [linkage=external] [abi=sysv64(byval<align=32>) -> void];
// IR-NEXT:     fn %[[VALUE_avx256:[0-9]+]] @avx256(%[[VALUE1:[0-9]+]] <unnamed>: vector<f32, 8>) -> void [linkage=external] [target=+avx] [abi=sysv64(direct) -> void];
// IR-NEXT:     fn %[[VALUE_avx2_256:[0-9]+]] @avx2_256(%[[VALUE2:[0-9]+]] <unnamed>: vector<f32, 8>) -> void [linkage=external] [target=+avx2] [abi=sysv64(direct) -> void];
// IR-NEXT:     fn %[[VALUE_cancelled256:[0-9]+]] @cancelled256(%[[VALUE3:[0-9]+]] <unnamed>: vector<f32, 8>) -> void [linkage=external] [target=+avx,-avx] [abi=sysv64(byval<align=32>) -> void];
// IR-NEXT:     fn %[[VALUE_arch512:[0-9]+]] @arch512(%[[VALUE4:[0-9]+]] <unnamed>: vector<f32, 16>) -> void [linkage=external] [target=arch=skylake-avx512] [abi=sysv64(direct) -> void];
// IR-NEXT:     fn %[[VALUE_avx512_wide:[0-9]+]] @avx512_wide(%[[VALUE5:[0-9]+]] <unnamed>: vector<f32, 16>) -> void [linkage=external] [target=+avx] [abi=sysv64(byval<align=64>) -> void];
// IR-NEXT:     fn %[[VALUE_avx512f_wide:[0-9]+]] @avx512f_wide(%[[VALUE6:[0-9]+]] <unnamed>: vector<f32, 16>) -> void [linkage=external] [target=+avx512f] [abi=sysv64(direct) -> void];
// IR-NEXT:     fn %[[VALUE_defined256:[0-9]+]] @defined256(%[[VALUE_v:[0-9]+]] v: vector<f32, 8>) -> void [linkage=external] [target=+avx] [abi=sysv64(direct) -> void] [fallthrough=ret_void] {
// IR-NEXT:         call<void, abi=sysv64(direct) -> void>(%[[VALUE_avx256]], read<vector<f32, 8>>(%[[VALUE_v]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
