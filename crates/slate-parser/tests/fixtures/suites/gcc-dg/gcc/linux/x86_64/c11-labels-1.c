/* Tests for labels before declarations and at ends of compound statements.  */
/* { dg-do compile } */
/* { dg-options "-std=c11" } */

int f(int x) 
{ 
	goto b;
	a: int i = 2 * x;
           goto c;
	b: goto a;
	{ i *= 3; c: }
	return i;
        d:
}


// SLATE-FILECHECK-STD DEFAULT c11
// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         goto %[[VALUE_b:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_a:[0-9]+]] a:
// DEFAULT-NEXT:             let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         goto %[[VALUE_c:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_b]] b:
// DEFAULT-NEXT:             goto %[[VALUE_a]];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(3));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:             label %[[VALUE_c]] c:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         label %[[VALUE_d:[0-9]+]] d:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
