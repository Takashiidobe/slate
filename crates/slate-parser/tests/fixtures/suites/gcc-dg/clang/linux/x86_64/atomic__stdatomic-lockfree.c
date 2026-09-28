/* Test atomic_is_lock_free.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>
#include <stdint.h>

extern void abort (void);

_Atomic _Bool aba;
atomic_bool abt;
_Atomic char aca;
atomic_char act;
_Atomic __CHAR16_TYPE__ ac16a;
atomic_char16_t ac16t;
_Atomic __CHAR32_TYPE__ ac32a;
atomic_char32_t ac32t;
_Atomic __WCHAR_TYPE__ awca;
atomic_wchar_t awct;
_Atomic short asa;
atomic_short ast;
_Atomic int aia;
atomic_int ait;
_Atomic long ala;
atomic_long alt;
_Atomic long long alla;
atomic_llong allt;
void *_Atomic apa;

#define CHECK_TYPE(MACRO, V1, V2)		\
  do						\
    {						\
      int r1 = MACRO;				\
      int r2 = atomic_is_lock_free (&V1);	\
      int r3 = atomic_is_lock_free (&V2);	\
      if (r1 != 0 && r1 != 1 && r1 != 2)	\
	abort ();				\
      if (r2 != 0 && r2 != 1)			\
	abort ();				\
      if (r3 != 0 && r3 != 1)			\
	abort ();				\
      if (r1 == 2 && r2 != 1)			\
	abort ();				\
      if (r1 == 2 && r3 != 1)			\
	abort ();				\
      if (r1 == 0 && r2 != 0)			\
	abort ();				\
      if (r1 == 0 && r3 != 0)			\
	abort ();				\
    }						\
  while (0)

int
main ()
{
  CHECK_TYPE (ATOMIC_BOOL_LOCK_FREE, aba, abt);
  CHECK_TYPE (ATOMIC_CHAR_LOCK_FREE, aca, act);
  CHECK_TYPE (ATOMIC_CHAR16_T_LOCK_FREE, ac16a, ac16t);
  CHECK_TYPE (ATOMIC_CHAR32_T_LOCK_FREE, ac32a, ac32t);
  CHECK_TYPE (ATOMIC_WCHAR_T_LOCK_FREE, awca, awct);
  CHECK_TYPE (ATOMIC_SHORT_LOCK_FREE, asa, ast);
  CHECK_TYPE (ATOMIC_INT_LOCK_FREE, aia, ait);
  CHECK_TYPE (ATOMIC_LONG_LOCK_FREE, ala, alt);
  CHECK_TYPE (ATOMIC_LLONG_LOCK_FREE, alla, allt);
  CHECK_TYPE (ATOMIC_POINTER_LOCK_FREE, apa, apa);

  return 0;
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
// DEFAULT-NEXT:     type @type0 wchar_t = i32;
// DEFAULT-NEXT:     type @type1 __uint16_t = u16;
// DEFAULT-NEXT:     type @type2 __uint32_t = u32;
// DEFAULT-NEXT:     type @type3 __uint_least16_t = u16;
// DEFAULT-NEXT:     type @type4 __uint_least32_t = u32;
// DEFAULT-NEXT:     type @type5 uint_least16_t = u16;
// DEFAULT-NEXT:     type @type6 uint_least32_t = u32;
// DEFAULT-NEXT:     type @type7 atomic_bool = bool;
// DEFAULT-NEXT:     type @type8 atomic_char = i8;
// DEFAULT-NEXT:     type @type9 atomic_short = i16;
// DEFAULT-NEXT:     type @type10 atomic_int = i32;
// DEFAULT-NEXT:     type @type11 atomic_long = i64;
// DEFAULT-NEXT:     type @type12 atomic_llong = i64;
// DEFAULT-NEXT:     type @type13 atomic_char16_t = u16;
// DEFAULT-NEXT:     type @type14 atomic_char32_t = u32;
// DEFAULT-NEXT:     type @type15 atomic_wchar_t = i32;
// DEFAULT-NEXT:     global %17 aba: atomic bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 abt: atomic bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 aca: atomic i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %20 act: atomic i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %21 ac16a: atomic u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %22 ac16t: atomic u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %23 ac32a: atomic u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %24 ac32t: atomic u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %25 awca: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %26 awct: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %27 asa: atomic i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %28 ast: atomic i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %29 aia: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %30 ait: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %31 ala: atomic i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %32 alt: atomic i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %33 alla: atomic i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %34 allt: atomic i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %35 apa: atomic ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %16 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %36 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %67
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %37 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %38 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %39 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%37), const<i32>(0)), ne<i32>(read<i32>(%37), const<i32>(1))), ne<i32>(read<i32>(%37), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%38), const<i32>(0)), ne<i32>(read<i32>(%38), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%39), const<i32>(0)), ne<i32>(read<i32>(%39), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%37), const<i32>(2)), ne<i32>(read<i32>(%38), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%37), const<i32>(2)), ne<i32>(read<i32>(%39), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%37), const<i32>(0)), ne<i32>(read<i32>(%38), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%37), const<i32>(0)), ne<i32>(read<i32>(%39), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %68
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %40 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %41 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %42 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%40), const<i32>(0)), ne<i32>(read<i32>(%40), const<i32>(1))), ne<i32>(read<i32>(%40), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%41), const<i32>(0)), ne<i32>(read<i32>(%41), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%42), const<i32>(0)), ne<i32>(read<i32>(%42), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%40), const<i32>(2)), ne<i32>(read<i32>(%41), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%40), const<i32>(2)), ne<i32>(read<i32>(%42), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%40), const<i32>(0)), ne<i32>(read<i32>(%41), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%40), const<i32>(0)), ne<i32>(read<i32>(%42), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %69
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %43 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %44 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %45 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%43), const<i32>(0)), ne<i32>(read<i32>(%43), const<i32>(1))), ne<i32>(read<i32>(%43), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%44), const<i32>(0)), ne<i32>(read<i32>(%44), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%45), const<i32>(0)), ne<i32>(read<i32>(%45), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%43), const<i32>(2)), ne<i32>(read<i32>(%44), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%43), const<i32>(2)), ne<i32>(read<i32>(%45), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%43), const<i32>(0)), ne<i32>(read<i32>(%44), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%43), const<i32>(0)), ne<i32>(read<i32>(%45), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %70
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %46 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %47 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %48 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%46), const<i32>(0)), ne<i32>(read<i32>(%46), const<i32>(1))), ne<i32>(read<i32>(%46), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%47), const<i32>(0)), ne<i32>(read<i32>(%47), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%48), const<i32>(0)), ne<i32>(read<i32>(%48), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%46), const<i32>(2)), ne<i32>(read<i32>(%47), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%46), const<i32>(2)), ne<i32>(read<i32>(%48), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%46), const<i32>(0)), ne<i32>(read<i32>(%47), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%46), const<i32>(0)), ne<i32>(read<i32>(%48), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %71
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %49 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %50 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %51 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%49), const<i32>(0)), ne<i32>(read<i32>(%49), const<i32>(1))), ne<i32>(read<i32>(%49), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%50), const<i32>(0)), ne<i32>(read<i32>(%50), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%51), const<i32>(0)), ne<i32>(read<i32>(%51), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%49), const<i32>(2)), ne<i32>(read<i32>(%50), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%49), const<i32>(2)), ne<i32>(read<i32>(%51), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%49), const<i32>(0)), ne<i32>(read<i32>(%50), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%49), const<i32>(0)), ne<i32>(read<i32>(%51), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %72
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %52 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %53 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %54 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%52), const<i32>(0)), ne<i32>(read<i32>(%52), const<i32>(1))), ne<i32>(read<i32>(%52), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%53), const<i32>(0)), ne<i32>(read<i32>(%53), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%54), const<i32>(0)), ne<i32>(read<i32>(%54), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%52), const<i32>(2)), ne<i32>(read<i32>(%53), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%52), const<i32>(2)), ne<i32>(read<i32>(%54), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%52), const<i32>(0)), ne<i32>(read<i32>(%53), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%52), const<i32>(0)), ne<i32>(read<i32>(%54), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %73
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %55 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %56 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %57 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%55), const<i32>(0)), ne<i32>(read<i32>(%55), const<i32>(1))), ne<i32>(read<i32>(%55), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%56), const<i32>(0)), ne<i32>(read<i32>(%56), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%57), const<i32>(0)), ne<i32>(read<i32>(%57), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%55), const<i32>(2)), ne<i32>(read<i32>(%56), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%55), const<i32>(2)), ne<i32>(read<i32>(%57), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%55), const<i32>(0)), ne<i32>(read<i32>(%56), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%55), const<i32>(0)), ne<i32>(read<i32>(%57), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %74
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %58 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %59 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %60 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%58), const<i32>(0)), ne<i32>(read<i32>(%58), const<i32>(1))), ne<i32>(read<i32>(%58), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%59), const<i32>(0)), ne<i32>(read<i32>(%59), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%60), const<i32>(0)), ne<i32>(read<i32>(%60), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%58), const<i32>(2)), ne<i32>(read<i32>(%59), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%58), const<i32>(2)), ne<i32>(read<i32>(%60), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%58), const<i32>(0)), ne<i32>(read<i32>(%59), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%58), const<i32>(0)), ne<i32>(read<i32>(%60), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %75
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %61 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %62 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %63 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%61), const<i32>(0)), ne<i32>(read<i32>(%61), const<i32>(1))), ne<i32>(read<i32>(%61), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%62), const<i32>(0)), ne<i32>(read<i32>(%62), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%63), const<i32>(0)), ne<i32>(read<i32>(%63), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%61), const<i32>(2)), ne<i32>(read<i32>(%62), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%61), const<i32>(2)), ne<i32>(read<i32>(%63), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%61), const<i32>(0)), ne<i32>(read<i32>(%62), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%61), const<i32>(0)), ne<i32>(read<i32>(%63), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %76
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %64 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %65 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %66 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%64), const<i32>(0)), ne<i32>(read<i32>(%64), const<i32>(1))), ne<i32>(read<i32>(%64), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%65), const<i32>(0)), ne<i32>(read<i32>(%65), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%66), const<i32>(0)), ne<i32>(read<i32>(%66), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%64), const<i32>(2)), ne<i32>(read<i32>(%65), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%64), const<i32>(2)), ne<i32>(read<i32>(%66), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%64), const<i32>(0)), ne<i32>(read<i32>(%65), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%64), const<i32>(0)), ne<i32>(read<i32>(%66), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
