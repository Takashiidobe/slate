/* { dg-do compile } */

#include <stdarg.h>

va_list v;
volatile int i;

void foo()
{
  i = va_arg(v, char); /* { dg-warning "is promoted to|so you should|abort" "char" } */
  i = va_arg(v, short); /* { dg-warning "is promoted to|abort" "short" } */
  i = va_arg(v, float); /* { dg-warning "is promoted to|abort" "float" } */
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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: va_list [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_i]], widen<i32, reason=assign>(va_arg<i8>(%[[VALUE_v]])));
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_i]], widen<i32, reason=assign>(va_arg<i16>(%[[VALUE_v]])));
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_i]], float_to_int<i32, reason=assign, out_of_range=ub, exceptions=observable>(va_arg<f32>(%[[VALUE_v]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
