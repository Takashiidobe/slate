/* PR c/102989 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#include <stdarg.h>
#include <stdlib.h>

#if __BITINT_MAXWIDTH__ >= 128
unsigned _BitInt(128) b, c;
#endif
#if __BITINT_MAXWIDTH__ >= 575
signed _BitInt(575) d, e;
#endif

void
foo (int a, ...)
{
  va_list ap;
  va_start (ap, a);
  if (a == 1)
    {
      if (va_arg (ap, _BitInt(2)) != 1wb)
	abort ();
      if (va_arg (ap, _BitInt(3)) != 3wb)
	abort ();
      if (va_arg (ap, _BitInt(15)) != 16383wb)
	abort ();
      if (va_arg (ap, unsigned _BitInt(32)) != 4294967295uwb)
	abort ();
      if (va_arg (ap, _BitInt(64)) != 0x7fffffffffffffffwb)
	abort ();
    }
#if __BITINT_MAXWIDTH__ >= 128
  b = va_arg (ap, unsigned _BitInt(128));
#endif
#if __BITINT_MAXWIDTH__ >= 575
  d = va_arg (ap, _BitInt(575));
#endif
  if (va_arg (ap, int) != 42)
    abort ();
  va_end (ap);
}

void
bar (void)
{
  foo (1, 1wb, 3wb, 16383wb, 4294967295uwb, 9223372036854775807wb,
#if __BITINT_MAXWIDTH__ >= 128
       c,
#endif
#if __BITINT_MAXWIDTH__ >= 575
       e,
#endif
       42);
  foo (2,
#if __BITINT_MAXWIDTH__ >= 128
       c,
#endif
#if __BITINT_MAXWIDTH__ >= 575
       e,
#endif
       42);
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
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: u128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_a]]), const<i32>(1))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i2b>(va_arg<i2b>(%[[VALUE_ap]]), const<i2b>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<i3b>(va_arg<i3b>(%[[VALUE_ap]]), const<i3b>(3))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<i15b>(va_arg<i15b>(%[[VALUE_ap]]), const<i15b>(16383))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<u32b>(va_arg<u32b>(%[[VALUE_ap]]), const<u32b>(4294967295))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if ne<i64b>(va_arg<i64b>(%[[VALUE_ap]]), const<i64b>(9223372036854775807))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u128b>(%[[VALUE_b]], va_arg<u128b>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         write<i575b>(%[[VALUE_d]], va_arg<i575b>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%[[VALUE_ap]]), const<i32>(42))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_foo]], const<i32>(1), const<i2b>(1), const<i3b>(3), const<i15b>(16383), const<u32b>(4294967295), const<i64b>(9223372036854775807), read<u128b>(%[[VALUE_c]]), read<i575b>(%[[VALUE_e]]), const<i32>(42));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_foo]], const<i32>(2), read<u128b>(%[[VALUE_c]]), read<i575b>(%[[VALUE_e]]), const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
