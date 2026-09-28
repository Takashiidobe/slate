/* PR target/114310 */
/* { dg-do run { target int128 } } */

volatile __attribute__((aligned (sizeof (__int128_t)))) __int128_t v = 10;

int
main ()
{
#if __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16
  if (__sync_val_compare_and_swap (&v, (__int128_t) 10, (__int128_t) 0) != 10)
    __builtin_abort ();
  if (__sync_val_compare_and_swap (&v, (__int128_t) 10, (__int128_t) 15) != 0)
    __builtin_abort ();
  if (__sync_val_compare_and_swap (&v, (__int128_t) 0, (__int128_t) 42) != 0)
    __builtin_abort ();
  if (__sync_val_compare_and_swap (&v, (__int128_t) 31, (__int128_t) 35) != 42)
    __builtin_abort ();
#endif
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     global %0 v: volatile i128 [storage=static] = widen<i128, reason=assign>(const<i32>(10)) [linkage=external];
// DEFAULT-NEXT:     fn %1 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
