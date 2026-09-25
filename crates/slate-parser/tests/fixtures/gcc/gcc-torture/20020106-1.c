// SLATE-FILECHECK-DEFINES DEFAULT

/* Origin: PR c/5279 from <wilco@equator.com>.  */     
	
int
foo ()
{
  extern long long Y;
  return (0 > Y++);
}

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
// DEFAULT-NEXT:     extern %1 Y: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:         let %3: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%3));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i64>(widen<i64, reason=usual_arith>(const<i32>(0)), read<i64>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
