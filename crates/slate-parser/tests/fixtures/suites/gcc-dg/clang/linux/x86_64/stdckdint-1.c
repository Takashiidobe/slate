/* Test C23 Checked Integer Arithmetic macros in <stdckdint.h>.  */
/* { dg-do run } */
/* { dg-options "-std=c23" } */

#include <stdckdint.h>

#if __STDC_VERSION_STDCKDINT_H__ != 202311L
# error __STDC_VERSION_STDCKDINT_H__ not defined to 202311L
#endif

extern void abort (void);

int
main ()
{
  unsigned int a;
  if (ckd_add (&a, 1, 2) || a != 3)
    abort ();
  if (ckd_add (&a, ~2U, 2) || a != ~0U)
    abort ();
  if (!ckd_add (&a, ~2U, 4) || a != 1)
    abort ();
  if (ckd_sub (&a, 42, 2) || a != 40)
    abort ();
  if (!ckd_sub (&a, 11, ~0ULL) || a != 12)
    abort ();
  if (ckd_mul (&a, 42, 16U) || a != 672)
    abort ();
  if (ckd_mul (&a, ~0UL, 0) || a != 0)
    abort ();
  if (ckd_mul (&a, 1, ~0U) || a != ~0U)
    abort ();
  if (ckd_mul (&a, ~0UL, 1) != (~0UL > ~0U) || a != ~0U)
    abort ();
  static_assert (_Generic (ckd_add (&a, 1, 1), bool: 1, default: 0));
  static_assert (_Generic (ckd_sub (&a, 1, 1), bool: 1, default: 0));
  static_assert (_Generic (ckd_mul (&a, 1, 1), bool: 1, default: 0));
  signed char b;
  if (ckd_add (&b, 8, 12) || b != 20)
    abort ();
  if (ckd_sub (&b, 8UL, 12ULL) || b != -4)
    abort ();
  if (ckd_mul (&b, 2, 3) || b != 6)
    abort ();
  unsigned char c;
  if (ckd_add (&c, 8, 12) || c != 20)
    abort ();
  if (ckd_sub (&c, 8UL, 12ULL) != (-4ULL > (unsigned char) -4U)
      || c != (unsigned char) -4U)
    abort ();
  if (ckd_mul (&c, 2, 3) || c != 6)
    abort ();
  long long d;
  if (ckd_add (&d, ~0U, ~0U) != (~0U + 1ULL < ~0U)
      || d != (long long) (2 * (unsigned long long) ~0U))
    abort ();
  if (ckd_sub (&d, 0, 0) || d != 0)
    abort ();
  if (ckd_mul (&d, 16, 1) || d != 16)
    abort ();
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 a: u32 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(overflow_add<bool>(const<i32>(1), const<i32>(2), deref(addr_of<ptr<u32>>(%2))), ne<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(overflow_add<bool>(not<u32>(const<u32>(2)), const<i32>(2), deref(addr_of<ptr<u32>>(%2))), ne<u32>(read<u32>(%2), not<u32>(const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(not<bool>(overflow_add<bool>(not<u32>(const<u32>(2)), const<i32>(4), deref(addr_of<ptr<u32>>(%2)))), ne<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(overflow_sub<bool>(const<i32>(42), const<i32>(2), deref(addr_of<ptr<u32>>(%2))), ne<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(40))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(not<bool>(overflow_sub<bool>(const<i32>(11), not<u64>(const<u64>(0)), deref(addr_of<ptr<u32>>(%2)))), ne<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(overflow_mul<bool>(const<i32>(42), const<u32>(16), deref(addr_of<ptr<u32>>(%2))), ne<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(672))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(overflow_mul<bool>(not<u64>(const<u64>(0)), const<i32>(0), deref(addr_of<ptr<u32>>(%2))), ne<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(overflow_mul<bool>(const<i32>(1), not<u32>(const<u32>(0)), deref(addr_of<ptr<u32>>(%2))), ne<u32>(read<u32>(%2), not<u32>(const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(from_bool<i32, reason=promotion>(overflow_mul<bool>(not<u64>(const<u64>(0)), const<i32>(1), deref(addr_of<ptr<u32>>(%2)))), from_bool<i32, reason=promotion>(gt<u64>(not<u64>(const<u64>(0)), widen<u64, reason=usual_arith>(not<u32>(const<u32>(0)))))), ne<u32>(read<u32>(%2), not<u32>(const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %3 b: i8 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(overflow_add<bool>(const<i32>(8), const<i32>(12), deref(addr_of<ptr<i8>>(%3))), ne<i32>(widen<i32, reason=promotion>(read<i8>(%3)), const<i32>(20)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(overflow_sub<bool>(const<u64>(8), const<u64>(12), deref(addr_of<ptr<i8>>(%3))), ne<i32>(widen<i32, reason=promotion>(read<i8>(%3)), neg<i32, overflow=ub>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(overflow_mul<bool>(const<i32>(2), const<i32>(3), deref(addr_of<ptr<i8>>(%3))), ne<i32>(widen<i32, reason=promotion>(read<i8>(%3)), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %4 c: u8 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(overflow_add<bool>(const<i32>(8), const<i32>(12), deref(addr_of<ptr<u8>>(%4))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%4))), const<i32>(20)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(from_bool<i32, reason=promotion>(overflow_sub<bool>(const<u64>(8), const<u64>(12), deref(addr_of<ptr<u8>>(%4)))), from_bool<i32, reason=promotion>(gt<u64>(neg<u64, overflow=wrap>(const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(neg<u32, overflow=wrap>(const<u32>(4)))))))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%4))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(neg<u32, overflow=wrap>(const<u32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(overflow_mul<bool>(const<i32>(2), const<i32>(3), deref(addr_of<ptr<u8>>(%4))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%4))), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %5 d: i64 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(from_bool<i32, reason=promotion>(overflow_add<bool>(not<u32>(const<u32>(0)), not<u32>(const<u32>(0)), deref(addr_of<ptr<i64>>(%5)))), from_bool<i32, reason=promotion>(lt<u64>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(not<u32>(const<u32>(0))), const<u64>(1)), widen<u64, reason=usual_arith>(not<u32>(const<u32>(0)))))), ne<i64>(read<i64>(%5), reinterpret<i64, reason=explicit, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), widen<u64, reason=explicit>(not<u32>(const<u32>(0)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(overflow_sub<bool>(const<i32>(0), const<i32>(0), deref(addr_of<ptr<i64>>(%5))), ne<i64>(read<i64>(%5), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(overflow_mul<bool>(const<i32>(16), const<i32>(1), deref(addr_of<ptr<i64>>(%5))), ne<i64>(read<i64>(%5), widen<i64, reason=usual_arith>(const<i32>(16))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
