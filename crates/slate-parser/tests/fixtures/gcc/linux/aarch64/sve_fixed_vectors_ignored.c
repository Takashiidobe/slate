typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(256)));
fixed_i32 identity(fixed_i32 value) { return value; }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -march=armv8.2-a+sve
// SLATE-FILECHECK-DEFINES SCALABLE
// SLATE-FILECHECK-DEFINES MISMATCH
// SLATE-FILECHECK-PREFIX-ARGS MISMATCH -msve-vector-bits=512
// SLATE-FILECHECK-WARNING SCALABLE
// SLATE-FILECHECK-WARNING MISMATCH

// SLATE-FILECHECK-BEGIN SCALABLE
// SCALABLE: -Wignored-attributes
// SCALABLE:   ⚠ unsupported SVE vector size
// SCALABLE:    ╭─[tests/fixtures/gcc/linux/aarch64/sve_fixed_vectors_ignored.c:1:64]
// SCALABLE:  1 │ typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(256)));
// SCALABLE:    ·                                                                ───
// SCALABLE:  2 │ fixed_i32 identity(fixed_i32 value) { return value; }
// SCALABLE:    ╰────
// SLATE-FILECHECK-END SCALABLE
// SLATE-FILECHECK-BEGIN MISMATCH
// MISMATCH: -Wignored-attributes
// MISMATCH:   ⚠ unsupported SVE vector size
// MISMATCH:    ╭─[tests/fixtures/gcc/linux/aarch64/sve_fixed_vectors_ignored.c:1:64]
// MISMATCH:  1 │ typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(256)));
// MISMATCH:    ·                                                                ───
// MISMATCH:  2 │ fixed_i32 identity(fixed_i32 value) { return value; }
// MISMATCH:    ╰────
// SLATE-FILECHECK-END MISMATCH
// SLATE-FILECHECK-BEGIN IR-SCALABLE
// IR-SCALABLE: module {
// IR-SCALABLE-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-SCALABLE-NEXT:         endian = little;
// IR-SCALABLE-NEXT:         pointer [size=8, align=8];
// IR-SCALABLE-NEXT:         stack_alignment = 16;
// IR-SCALABLE-NEXT:         long_double = f128;
// IR-SCALABLE-NEXT:         storage bool [size=1, align=1];
// IR-SCALABLE-NEXT:         storage i8, u8 [size=1, align=1];
// IR-SCALABLE-NEXT:         storage i16, u16 [size=2, align=2];
// IR-SCALABLE-NEXT:         storage i32, u32 [size=4, align=4];
// IR-SCALABLE-NEXT:         storage i64, u64 [size=8, align=8];
// IR-SCALABLE-NEXT:         storage i128, u128 [size=16, align=16];
// IR-SCALABLE-NEXT:         storage bf16 [size=2, align=2];
// IR-SCALABLE-NEXT:         storage f16 [size=2, align=2];
// IR-SCALABLE-NEXT:         storage f32 [size=4, align=4];
// IR-SCALABLE-NEXT:         storage f64 [size=8, align=8];
// IR-SCALABLE-NEXT:         storage f128 [size=16, align=16];
// IR-SCALABLE-NEXT:         storage d32 [size=4, align=4];
// IR-SCALABLE-NEXT:         storage d64 [size=8, align=8];
// IR-SCALABLE-NEXT:         storage d128 [size=16, align=16];
// IR-SCALABLE-NEXT:     }
// IR-SCALABLE-NEXT:     type @type[[TYPE_fixed_i32:[0-9]+]] fixed_i32 = sizeless<svint32_t>;
// IR-SCALABLE-NEXT:     fn %[[VALUE_identity:[0-9]+]] @identity(%[[VALUE_value:[0-9]+]] value: sizeless<svint32_t> [c="fixed_i32"]) -> sizeless<svint32_t> [linkage=external] [fallthrough=ub_if_used] [c="fixed_i32(fixed_i32)"] {
// IR-SCALABLE-NEXT:         return read<sizeless<svint32_t>>(%[[VALUE_value]]) [c="fixed_i32"];
// IR-SCALABLE-NEXT:     }
// IR-SCALABLE-NEXT: }
// SLATE-FILECHECK-END IR-SCALABLE
// SLATE-FILECHECK-BEGIN IR-MISMATCH
// IR-MISMATCH: module {
// IR-MISMATCH-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-MISMATCH-NEXT:         endian = little;
// IR-MISMATCH-NEXT:         pointer [size=8, align=8];
// IR-MISMATCH-NEXT:         stack_alignment = 16;
// IR-MISMATCH-NEXT:         long_double = f128;
// IR-MISMATCH-NEXT:         storage bool [size=1, align=1];
// IR-MISMATCH-NEXT:         storage i8, u8 [size=1, align=1];
// IR-MISMATCH-NEXT:         storage i16, u16 [size=2, align=2];
// IR-MISMATCH-NEXT:         storage i32, u32 [size=4, align=4];
// IR-MISMATCH-NEXT:         storage i64, u64 [size=8, align=8];
// IR-MISMATCH-NEXT:         storage i128, u128 [size=16, align=16];
// IR-MISMATCH-NEXT:         storage bf16 [size=2, align=2];
// IR-MISMATCH-NEXT:         storage f16 [size=2, align=2];
// IR-MISMATCH-NEXT:         storage f32 [size=4, align=4];
// IR-MISMATCH-NEXT:         storage f64 [size=8, align=8];
// IR-MISMATCH-NEXT:         storage f128 [size=16, align=16];
// IR-MISMATCH-NEXT:         storage d32 [size=4, align=4];
// IR-MISMATCH-NEXT:         storage d64 [size=8, align=8];
// IR-MISMATCH-NEXT:         storage d128 [size=16, align=16];
// IR-MISMATCH-NEXT:     }
// IR-MISMATCH-NEXT:     type @type[[TYPE_fixed_i32:[0-9]+]] fixed_i32 = sizeless<svint32_t>;
// IR-MISMATCH-NEXT:     fn %[[VALUE_identity:[0-9]+]] @identity(%[[VALUE_value:[0-9]+]] value: sizeless<svint32_t> [c="fixed_i32"]) -> sizeless<svint32_t> [linkage=external] [fallthrough=ub_if_used] [c="fixed_i32(fixed_i32)"] {
// IR-MISMATCH-NEXT:         return read<sizeless<svint32_t>>(%[[VALUE_value]]) [c="fixed_i32"];
// IR-MISMATCH-NEXT:     }
// IR-MISMATCH-NEXT: }
// SLATE-FILECHECK-END IR-MISMATCH
