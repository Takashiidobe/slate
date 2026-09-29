/* Test C23 fallthrough attribute: mixtures with other attributes.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

/* Use of other standard attributes together with "fallthrough" goes
   through a different path to diagnosing ignored attributes from that
   used in attribute declarations without "fallthrough".  Verify that
   such ignored attributes result in a pedwarn (for use in a context
   not permitted in the constraints for those attributes) in this case
   as well.  */

int
f (int a)
{
  switch (a)
    {
    case 1:
      a++;
      [[fallthrough, deprecated]]; /* { dg-error "attribute ignored" } */
    case 2:
      a++;
      [[maybe_unused]] [[fallthrough]]; /* { dg-error "attribute ignored" } */
    case 3:
      a++;
      [[__nodiscard__, fallthrough]]; /* { dg-error "attribute ignored" } */
    case 4:
      a++;
    }
  return a;
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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_a:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_a]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(1):
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(2):
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(3):
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(4):
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
