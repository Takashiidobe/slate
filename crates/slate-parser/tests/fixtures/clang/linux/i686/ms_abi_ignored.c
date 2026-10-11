// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

long __attribute__((ms_abi)) ms_ignored(long a, ...);
long __attribute__((sysv_abi)) sysv_native(long a);

long use(void) { return ms_ignored(1, 2.0) + sysv_native(1); }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=4];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=4];
// IR-NEXT:         storage f80 [size=12, align=4];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_ms_ignored:[0-9]+]] @ms_ignored(%[[VALUE_a:[0-9]+]] a: i32, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_sysv_native:[0-9]+]] @sysv_native(%[[VALUE_a_2:[0-9]+]] a: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32>(call<i32>(%[[VALUE_ms_ignored]], const<i32>(1), const<f64>(2.0)), call<i32>(%[[VALUE_sysv_native]], const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
