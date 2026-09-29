/* Test evolved from source from Simona Perri <perri@mat.unical.it>
   and Gerald Pfeifer<pfeifer@dbai.tuwien.ac.at>.

   Copyright (C) 2003 Free Software Foundation  */

/* { dg-do run } */

#include <stdlib.h>

int main ()
{
  div_t d = div (20, 5);
  if ((d.quot != 4) || (d.rem))
    abort ();
  exit (0);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 quot: i32;
// DEFAULT-NEXT:         field1 rem: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_div_t:[0-9]+]] div_t = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_div:[0-9]+]] @div(%[[VALUE___numer:[0-9]+]] __numer: i32, %[[VALUE___denom:[0-9]+]] __denom: i32) -> @type[[TYPE0]] [linkage=external] [memory=none] [abi=sysv64(scalar, scalar) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: @type[[TYPE0]] [storage=automatic] = copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(i32, i32) -> @type[[TYPE0]], abi=sysv64(scalar, scalar) -> native_c>(%[[VALUE_div]], const<i32>(20), const<i32>(5)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_d]])), const<i32>(4)), ne<i32>(read<i32>(field1(%[[VALUE_d]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
