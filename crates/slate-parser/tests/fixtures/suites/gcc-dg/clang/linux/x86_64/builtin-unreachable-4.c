/* Check that this valid code doesn't ICE.  */
/* { dg-do compile } */
/* { dg-options "-O2" } */
void
g (int a, int b, int c, int d)
{
  if (d)
    {
      ((void)
       (!(a && b && c) ? __builtin_unreachable (), 0 : 0));
    }
  ((void)
   (!(a && b && c) ? __builtin_unreachable (), 0 : 0));
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_unreachable:[0-9]+]] @__builtin_unreachable() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32, %[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_d:[0-9]+]] d: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:                 if not<bool>(logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_unreachable]]);
// DEFAULT-NEXT:                     write<i32>(%[[VALUE0]], const<i32>(0));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<i32>(%[[VALUE0]], const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_unreachable]]);
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
