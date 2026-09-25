// SLATE-FILECHECK-DEFINES DEFAULT

/* PR/12292
   http://gcc.gnu.org/ml/gcc-patches/2003-10/msg00143.html  */

char flags;

int bug12292(int t)
{
	flags &= ~(1 << (t + 4));
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
// DEFAULT-NEXT:     global %0 flags: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @bug12292(%2 t: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3: i8 [synthetic] = read<i8>(%0);
// DEFAULT-NEXT:         let %4: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(and<i32>(widen<i32, reason=promotion>(read<i8>(%3)), not<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), add<i32, overflow=ub>(read<i32>(%2), const<i32>(4))))));
// DEFAULT-NEXT:         write<i8>(%0, read<i8>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
