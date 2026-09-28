/* { dg-do compile } */
/* { dg-options "-O2" } */

extern int ffs (int) __asm ("__GI_ffs") __attribute__ ((nothrow, const));

int
ffsll (long long int i)
{
  unsigned long long int x = i & -i;
  
  if (x <= 0xffffffff)
    return ffs (i);
  else
    return 32 + ffs (i >> 32);
}

/* { dg-final { scan-assembler-not "\nffs\n|\nffs\[^a-zA-Z0-9_\]|\[^a-zA-Z0-9_\]ffs\n" } } */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %0 @ffs(%4 <unnamed>: i32) -> i32 [linkage=external] [asm_name="__GI_ffs"] [memory=none];
// DEFAULT-NEXT:     fn %1 @ffsll(%2 i: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 x: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(and<i64>(read<i64>(%2), neg<i64, overflow=ub>(read<i64>(%2))));
// DEFAULT-NEXT:         if le<u64>(read<u64>(%3), widen<u64, reason=usual_arith>(const<u32>(4294967295)))
// DEFAULT-NEXT:             return call<i32, signature=fn(i32) -> i32>(%0, truncate<i32, reason=arg, fits=unknown>(read<i64>(%2)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return add<i32, overflow=ub>(const<i32>(32), call<i32, signature=fn(i32) -> i32>(%0, truncate<i32, reason=arg, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%2), const<i32>(32)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
