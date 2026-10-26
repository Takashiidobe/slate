void f(int n) { int i;
#pragma omp for schedule(static,0)
for(i=0;i<8;i++) {}
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-WARNING DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wopenmp
// DEFAULT:   ⚠ chunk size value must be positive
// DEFAULT:    ╭─[tests/fixtures/gcc/linux/x86_64/openmp_chunk_warning.c:2:33]
// DEFAULT:  1 │ void f(int n) { int i;
// DEFAULT:  2 │ #pragma omp for schedule(static,0)
// DEFAULT:    ·                                 ─
// DEFAULT:  3 │ for(i=0;i<8;i++) {}
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f80;
// IR-DEFAULT-NEXT:         storage bool [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage f64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage f80 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// IR-DEFAULT-NEXT:         omp_for %[[VALUE0:[0-9]+]]
// IR-DEFAULT-NEXT:             schedule: static, chunk widen<i64, reason=explicit>(const<i32>(0))
// IR-DEFAULT-NEXT:             loop %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic]
// IR-DEFAULT-NEXT:                 lower: const<i32>(0)
// IR-DEFAULT-NEXT:                 condition: lt const<i32>(8)
// IR-DEFAULT-NEXT:                 step: add const<i32>(1)
// IR-DEFAULT-NEXT:             body:
// IR-DEFAULT-NEXT:                 {
// IR-DEFAULT-NEXT:                 }
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
