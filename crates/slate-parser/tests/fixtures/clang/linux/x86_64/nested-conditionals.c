int main() {
#if 0
  return 0;
#elif 1
#ifdef INNER
  return 1;
#else
  return 2;
#endif
#else
  return 3;
#endif
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES INNER INNER

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
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN INNER
// INNER: module {
// INNER-NEXT:     target "x86_64-unknown-linux-gnu" {
// INNER-NEXT:         endian = little;
// INNER-NEXT:         pointer [size=8, align=8];
// INNER-NEXT:         stack_alignment = 16;
// INNER-NEXT:         long_double = f80;
// INNER-NEXT:         storage bool [size=1, align=1];
// INNER-NEXT:         storage i8, u8 [size=1, align=1];
// INNER-NEXT:         storage i16, u16 [size=2, align=2];
// INNER-NEXT:         storage i32, u32 [size=4, align=4];
// INNER-NEXT:         storage i64, u64 [size=8, align=8];
// INNER-NEXT:         storage i128, u128 [size=16, align=16];
// INNER-NEXT:         storage bf16 [size=2, align=2];
// INNER-NEXT:         storage f16 [size=2, align=2];
// INNER-NEXT:         storage f32 [size=4, align=4];
// INNER-NEXT:         storage f64 [size=8, align=8];
// INNER-NEXT:         storage f80 [size=16, align=16];
// INNER-NEXT:         storage f128 [size=16, align=16];
// INNER-NEXT:         storage d32 [size=4, align=4];
// INNER-NEXT:         storage d64 [size=8, align=8];
// INNER-NEXT:         storage d128 [size=16, align=16];
// INNER-NEXT:     }
// INNER-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// INNER-NEXT:         return const<i32>(1);
// INNER-NEXT:     }
// INNER-NEXT: }
// SLATE-FILECHECK-END INNER
