// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

unsigned long aligned_attribute(int q __attribute__((aligned(16)))) {
  return _Alignof(q);
}

unsigned long alignas_specifier(_Alignas(32) int p) { return _Alignof(p); }

unsigned long below_natural(int q __attribute__((aligned(1)))) {
  return _Alignof(q);
}

unsigned long repeated(int q __attribute__((aligned(8), aligned(16)))) {
  return _Alignof(q);
}

unsigned long plain(int r) { return _Alignof(r); }

unsigned long ignored(int r __attribute__((unused))) { return _Alignof(r); }

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
// IR-NEXT:     fn %[[VALUE_aligned_attribute:[0-9]+]] @aligned_attribute(%[[VALUE_q:[0-9]+]] q: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(16);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_alignas_specifier:[0-9]+]] @alignas_specifier(%[[VALUE_p:[0-9]+]] p: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(32);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_below_natural:[0-9]+]] @below_natural(%[[VALUE_q_2:[0-9]+]] q: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_repeated:[0-9]+]] @repeated(%[[VALUE_q_3:[0-9]+]] q: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(16);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_r:[0-9]+]] r: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(4);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_ignored:[0-9]+]] @ignored(%[[VALUE_r_2:[0-9]+]] r: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(4);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
