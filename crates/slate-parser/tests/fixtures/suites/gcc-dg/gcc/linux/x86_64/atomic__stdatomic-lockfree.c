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
// DEFAULT-NEXT:     type @type[[TYPE_atomic_bool:[0-9]+]] atomic_bool = bool;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_char:[0-9]+]] atomic_char = i8;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_short:[0-9]+]] atomic_short = i16;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_int:[0-9]+]] atomic_int = i32;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_long:[0-9]+]] atomic_long = i64;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_llong:[0-9]+]] atomic_llong = i64;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_char16_t:[0-9]+]] atomic_char16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_char32_t:[0-9]+]] atomic_char32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_wchar_t:[0-9]+]] atomic_wchar_t = i32;
// DEFAULT-NEXT:     global %[[VALUE_aba:[0-9]+]] aba: atomic bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_abt:[0-9]+]] abt: atomic bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_aca:[0-9]+]] aca: atomic i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_act:[0-9]+]] act: atomic i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ac16a:[0-9]+]] ac16a: atomic u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ac16t:[0-9]+]] ac16t: atomic u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ac32a:[0-9]+]] ac32a: atomic u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ac32t:[0-9]+]] ac32t: atomic u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_awca:[0-9]+]] awca: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_awct:[0-9]+]] awct: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_asa:[0-9]+]] asa: atomic i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ast:[0-9]+]] ast: atomic i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_aia:[0-9]+]] aia: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ait:[0-9]+]] ait: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ala:[0-9]+]] ala: atomic i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_alt:[0-9]+]] alt: atomic i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_alla:[0-9]+]] alla: atomic i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_allt:[0-9]+]] allt: atomic i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_apa:[0-9]+]] apa: atomic ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r1:[0-9]+]] r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_r2:[0-9]+]] r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %[[VALUE_r3:[0-9]+]] r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r1]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r1]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_r1]]), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r2]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r3]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r2]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r3]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r1_2:[0-9]+]] r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_r2_2:[0-9]+]] r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %[[VALUE_r3_2:[0-9]+]] r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r1_2]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r1_2]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_r1_2]]), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r2_2]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_2]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r3_2]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_2]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_2]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r2_2]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_2]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r3_2]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_2]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_2]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_2]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_2]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r1_3:[0-9]+]] r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_r2_3:[0-9]+]] r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %[[VALUE_r3_3:[0-9]+]] r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r1_3]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r1_3]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_r1_3]]), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r2_3]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_3]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r3_3]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_3]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_3]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r2_3]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_3]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r3_3]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_3]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_3]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_3]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_3]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r1_4:[0-9]+]] r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_r2_4:[0-9]+]] r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %[[VALUE_r3_4:[0-9]+]] r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r1_4]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r1_4]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_r1_4]]), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r2_4]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_4]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r3_4]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_4]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_4]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r2_4]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_4]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r3_4]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_4]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_4]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_4]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_4]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r1_5:[0-9]+]] r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_r2_5:[0-9]+]] r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %[[VALUE_r3_5:[0-9]+]] r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r1_5]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r1_5]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_r1_5]]), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r2_5]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_5]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r3_5]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_5]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_5]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r2_5]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_5]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r3_5]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_5]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_5]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_5]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_5]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r1_6:[0-9]+]] r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_r2_6:[0-9]+]] r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %[[VALUE_r3_6:[0-9]+]] r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r1_6]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r1_6]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_r1_6]]), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r2_6]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_6]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r3_6]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_6]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_6]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r2_6]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_6]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r3_6]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_6]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_6]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_6]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_6]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r1_7:[0-9]+]] r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_r2_7:[0-9]+]] r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %[[VALUE_r3_7:[0-9]+]] r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r1_7]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r1_7]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_r1_7]]), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r2_7]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_7]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r3_7]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_7]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_7]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r2_7]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_7]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r3_7]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_7]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_7]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_7]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_7]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r1_8:[0-9]+]] r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_r2_8:[0-9]+]] r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %[[VALUE_r3_8:[0-9]+]] r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r1_8]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r1_8]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_r1_8]]), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r2_8]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_8]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r3_8]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_8]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_8]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r2_8]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_8]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r3_8]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_8]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_8]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_8]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_8]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r1_9:[0-9]+]] r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_r2_9:[0-9]+]] r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %[[VALUE_r3_9:[0-9]+]] r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r1_9]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r1_9]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_r1_9]]), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r2_9]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_9]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r3_9]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_9]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_9]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r2_9]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_9]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r3_9]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_9]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_9]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_9]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_9]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r1_10:[0-9]+]] r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_r2_10:[0-9]+]] r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %[[VALUE_r3_10:[0-9]+]] r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r1_10]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r1_10]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_r1_10]]), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r2_10]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_10]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_r3_10]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_10]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_10]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r2_10]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_10]]), const<i32>(2)), ne<i32>(read<i32>(%[[VALUE_r3_10]]), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_10]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r2_10]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r1_10]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_r3_10]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
