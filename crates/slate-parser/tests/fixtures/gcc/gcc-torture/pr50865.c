/* PR middle-end/50865 */

#define INT64_MIN (-__LONG_LONG_MAX__ - 1)

int main() {
  volatile long long l1 = 1;
  volatile long long l2 = -1;
  volatile long long l3 = -1;

  if ((INT64_MIN % 1LL) != 0)
    __builtin_abort();
  if ((INT64_MIN % l1) != 0)
    __builtin_abort();
  if (l2 == -1) {
    if ((INT64_MIN % 1LL) != 0)
      __builtin_abort();
  } else if ((INT64_MIN % -l2) != 0)
    __builtin_abort();
  if ((INT64_MIN % -l3) != 0)
    __builtin_abort();

  return 0;
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
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 l1: volatile i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(1));
// DEFAULT-NEXT:         let %2 l2: volatile i64 [storage=automatic] = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         let %3 l3: volatile i64 [storage=automatic] = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<i64>(rem<i64, by_zero=ub, min_by_neg_one=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), const<i64>(1)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i64>(rem<i64, by_zero=ub, min_by_neg_one=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), read<i64, volatile>(%1)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if eq<i64>(read<i64, volatile>(%2), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i64>(rem<i64, by_zero=ub, min_by_neg_one=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), const<i64>(1)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i64>(rem<i64, by_zero=ub, min_by_neg_one=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), neg<i64, overflow=ub>(read<i64, volatile>(%2))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i64>(rem<i64, by_zero=ub, min_by_neg_one=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), neg<i64, overflow=ub>(read<i64, volatile>(%3))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
