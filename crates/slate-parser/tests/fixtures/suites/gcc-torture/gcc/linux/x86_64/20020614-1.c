/* PR c/6677 */
/* Verify that GCC doesn't perform illegal simplifications
   when folding constants.  */

#include <limits.h>

extern void abort(void);
extern void exit(int);

int main(void) {
  int           i;
  signed char   j;
  unsigned char k;

  i = SCHAR_MAX;

  j = ((signed char)(i << 1)) / 2;

  if (j != -1)
    abort();

  j = ((signed char)(i * 2)) / 2;

  if (j != -1)
    abort();

  i = UCHAR_MAX;

  k = ((unsigned char)(i << 1)) / 2;

  if (k != UCHAR_MAX / 2)
    abort();

  k = ((unsigned char)(i * 2)) / 2;

  if (k != UCHAR_MAX / 2)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%6 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 j: i8 [storage=automatic];
// DEFAULT-NEXT:         let %5 k: u8 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(127));
// DEFAULT-NEXT:         write<i8>(%4, truncate<i8, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%3), const<i32>(1)))), const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%4)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i8>(%4, truncate<i8, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%3), const<i32>(2)))), const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%4)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%3, add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         write<u8>(%5, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%3), const<i32>(1)))))), const<i32>(2)))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%5))), div<i32, by_zero=ub, min_by_neg_one=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u8>(%5, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%3), const<i32>(2)))))), const<i32>(2)))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%5))), div<i32, by_zero=ub, min_by_neg_one=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
