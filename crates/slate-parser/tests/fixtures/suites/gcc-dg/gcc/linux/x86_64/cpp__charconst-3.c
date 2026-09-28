/* Copyright (C) 2001 Free Software Foundation, Inc.  */

/* { dg-do run } */
/* { dg-options -Wno-multichar } */

/* This tests values and signedness of multichar charconsts.

   Neil Booth, 5 May 2002.  */

#include <limits.h>

extern void abort (void);

int main ()
{
  /* These tests require at least 2-byte ints.  8-)  */
#if INT_MAX > 127
  int scale = (int) (unsigned char) -1 + 1;

  if ('ab' != (int) ((unsigned char) 'a' * scale + (unsigned char) 'b'))
    abort ();

  if ('\234b' != (int) ((unsigned char) '\234' * (unsigned int) scale + (unsigned char) 'b'))
    abort ();

  if ('b\234' != (int) ((unsigned char) 'b' * scale + (unsigned char) '\234'))
    abort ();
  /* Multichar charconsts have type int and should be signed.  */
#if INT_MAX == 32767
# if '\234a' > 0
#  error Preprocessor charconsts 1
# endif
  if ('\234a' > 0)
    abort ();
#elif INT_MAX == 2147483647
# if '\234aaa' > 0
#  error Preprocessor charconsts 2
# endif
  if ('\234aaa' > 0)
    abort ();
#elif INT_MAX == 9223372036854775807
# if '\234aaaaaaa' > 0
#  error Preprocessor charconsts 3
# endif
  if ('\234aaaaaaa' > 0)
    abort ();
#endif
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 scale: i32 [storage=automatic] = add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(widen<u32, reason=explicit>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(const<i32>(24930), add<i32, overflow=ub>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(97))))), read<i32>(%2)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(98)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(40034), reinterpret<i32, reason=explicit, fits=unknown>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-100)))))), reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%2))), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(98)))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(25244), add<i32, overflow=ub>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(98))))), read<i32>(%2)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-100)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if gt<i32>(const<i32>(-1671339679), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
