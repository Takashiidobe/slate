/* Test atomic_is_lock_free for char8_t.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#include <stdatomic.h>
#include <stdint.h>

extern void abort(void);

_Atomic __CHAR8_TYPE__ ac8a;
atomic_char8_t         ac8t;

#define CHECK_TYPE(MACRO, V1, V2)                                              \
  do {                                                                         \
    int r1 = MACRO;                                                            \
    int r2 = atomic_is_lock_free(&V1);                                         \
    int r3 = atomic_is_lock_free(&V2);                                         \
    if (r1 != 0 && r1 != 1 && r1 != 2)                                         \
      abort();                                                                 \
    if (r2 != 0 && r2 != 1)                                                    \
      abort();                                                                 \
    if (r3 != 0 && r3 != 1)                                                    \
      abort();                                                                 \
    if (r1 == 2 && r2 != 1)                                                    \
      abort();                                                                 \
    if (r1 == 2 && r3 != 1)                                                    \
      abort();                                                                 \
    if (r1 == 0 && r2 != 0)                                                    \
      abort();                                                                 \
    if (r1 == 0 && r3 != 0)                                                    \
      abort();                                                                 \
  } while (0)

int main() {
  CHECK_TYPE(ATOMIC_CHAR8_T_LOCK_FREE, ac8a, ac8t);

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
// DEFAULT-NEXT:     type @type0 atomic_char8_t = u8;
// DEFAULT-NEXT:     global %2 ac8a: atomic u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 ac8t: atomic u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %8
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %5 r1: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %6 r2: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 let %7 r3: i32 [storage=automatic] = from_bool<i32, reason=assign>(const<bool>(true));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%5), const<i32>(0)), ne<i32>(read<i32>(%5), const<i32>(1))), ne<i32>(read<i32>(%5), const<i32>(2)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%6), const<i32>(0)), ne<i32>(read<i32>(%6), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(read<i32>(%7), const<i32>(0)), ne<i32>(read<i32>(%7), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%5), const<i32>(2)), ne<i32>(read<i32>(%6), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%5), const<i32>(2)), ne<i32>(read<i32>(%7), const<i32>(1)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%5), const<i32>(0)), ne<i32>(read<i32>(%6), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%5), const<i32>(0)), ne<i32>(read<i32>(%7), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
