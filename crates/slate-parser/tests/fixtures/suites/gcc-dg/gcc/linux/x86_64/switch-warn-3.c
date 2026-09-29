/* { dg-do compile } */
/* { dg-options "-Wswitch-enum" } */

enum a { a0, a1, a2, a3 };

int error(enum a aa)
{
  switch ( aa )
  {
  case a0 ... a3:
    return 1;
  }
  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_a:[0-9]+]] a = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_a0:[0-9]+]] a0 = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_a1:[0-9]+]] a1 = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_a2:[0-9]+]] a2 = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_a3:[0-9]+]] a3 = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %[[VALUE_error:[0-9]+]] @error(%[[VALUE_aa:[0-9]+]] aa: @type[[TYPE_a]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] enum_to_int<u32, reason=promotion>(read<@type[[TYPE_a]]>(%[[VALUE_aa]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<u32>(0) ... const<u32>(3):
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
