/* { dg-do compile } */
/* { dg-options "-Wsign-compare -Werror=sign-compare -Werror=switch-enum" } */
/* { dg-message "warnings being treated as errors" "" {target "*-*-*"} 0 } */

int bar()
{
  unsigned x = 0;
  int y = 1;

  /* generates an error - ok */
  x += x < y ? 1 : 0; /* { dg-error "comparison" } */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
  /* generates no diagnostic - ok */
  x += x < y ? 1 : 0;
#pragma GCC diagnostic pop

  x += x < y ? 1 : 0; /* { dg-error "comparison" } */

  return x;
}

enum EE { ONE, TWO };

int f (enum EE e)
{
  int r = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

  switch (e)
    {
    case ONE:
      r = 1;
      break;
    }
#pragma GCC diagnostic pop

  switch (e) /* { dg-error "switch" } */
    {
    case ONE:
      r = 1;
      break;
    }
  return r;
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
// DEFAULT-NEXT:     type @type[[TYPE_EE:[0-9]+]] EE = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_ONE:[0-9]+]] ONE = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_TWO:[0-9]+]] TWO = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %[[VALUE_ONE]] @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_TWO]] x: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_TWO]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE0]]), reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(lt<u32>(read<u32>(%[[VALUE_TWO]]), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_y]]))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_TWO]], read<u32>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_TWO]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(lt<u32>(read<u32>(%[[VALUE_TWO]]), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_y]]))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_TWO]], read<u32>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_TWO]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(lt<u32>(read<u32>(%[[VALUE_TWO]]), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_y]]))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_TWO]], read<u32>(%[[VALUE5]]));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE_TWO]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_e:[0-9]+]] e: @type[[TYPE_EE]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %[[VALUE6:[0-9]+]] enum_to_int<u32, reason=promotion>(read<@type[[TYPE_EE]]>(%[[VALUE_e]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE6]] const<u32>(0):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_r]], const<i32>(1));
// DEFAULT-NEXT:                 break %[[VALUE6]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         switch %[[VALUE7:[0-9]+]] enum_to_int<u32, reason=promotion>(read<@type[[TYPE_EE]]>(%[[VALUE_e]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE7]] const<u32>(0):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_r]], const<i32>(1));
// DEFAULT-NEXT:                 break %[[VALUE7]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
