/* C2Y N3370 - Case range expressions.  */
/* { dg-do run } */
/* { dg-options "-std=c2y -pedantic-errors" } */

extern void abort ();

void
foo (int x)
{
  switch (x)
    {
    case -42 ... 42:
      if (x < -42 || x > 42)
        abort ();
      break;
    case 43 ... 43:
      if (x != 43)
        abort ();
      break;
    case 44:
      if (x != 44)
        abort ();
      break;
    case 45 ... 46:
      if (x < 45 || x > 46)
        abort ();
      break;
    default:
      if (x >= -42 && x <= 46)
        abort ();
      break;
    }
}

int
main ()
{
  for (int i = -44; i <= 48; ++i)
    foo (i);
}

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_x]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(-42) ... const<i32>(42):
// DEFAULT-NEXT:                     if logical_or<bool>(lt<i32>(read<i32>(%[[VALUE_x]]), neg<i32, overflow=ub>(const<i32>(42))), gt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(42)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(43) ... const<i32>(43):
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(43))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(44):
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(44))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(45) ... const<i32>(46):
// DEFAULT-NEXT:                     if logical_or<bool>(lt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(45)), gt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(46)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     if logical_and<bool>(ge<i32>(read<i32>(%[[VALUE_x]]), neg<i32, overflow=ub>(const<i32>(42))), le<i32>(read<i32>(%[[VALUE_x]]), const<i32>(46)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(44));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(48))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
