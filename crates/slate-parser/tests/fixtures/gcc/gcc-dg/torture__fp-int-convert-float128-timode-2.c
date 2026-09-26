/* Test floating-point conversions.  __float128 type with TImode: bug
   53317.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do run } */
/* { dg-require-effective-target __float128 } */
/* { dg-require-effective-target base_quadfloat_support } */
/* { dg-require-effective-target int128 } */
/* { dg-options "" } */
/* { dg-add-options __float128 } */

extern void abort(void);
extern void exit(int);

int
main(void) {
  volatile unsigned long long a = 0x1000000000000ULL;
  volatile unsigned long long b = 0xffffffffffffffffULL;
  unsigned __int128           c = (((unsigned __int128)a) << 64) | b;
  __float128                  d = c;
  if (d != 0x1.000000000000ffffffffffffffffp112q)
    abort();
  exit(0);
}



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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 a: volatile u64 [storage=automatic] = const<u64>(281474976710656);
// DEFAULT-NEXT:         let %4 b: volatile u64 [storage=automatic] = const<u64>(18446744073709551615);
// DEFAULT-NEXT:         let %5 c: u128 [storage=automatic] = or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(read<u64, volatile>(%3)), const<i32>(64)), widen<u128, reason=usual_arith>(read<u64, volatile>(%4)));
// DEFAULT-NEXT:         let %6 d: f128 [storage=automatic] = int_to_float<f128, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u128>(%5));
// DEFAULT-NEXT:         if ne<f128, exceptions=ignore>(read<f128>(%6), const<f128>(5192296858534846075274570038771711))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
