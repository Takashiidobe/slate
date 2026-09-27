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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 EE = enum : u32 {
// DEFAULT-NEXT:         %0 ONE = const<i32>(0);
// DEFAULT-NEXT:         %1 TWO = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %0 @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %1 x: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %2 y: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %11: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:         let %12: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(lt<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%2))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%1, read<u32>(%12));
// DEFAULT-NEXT:         let %13: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:         let %14: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(lt<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%2))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%1, read<u32>(%14));
// DEFAULT-NEXT:         let %15: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:         let %16: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(lt<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%2))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%1, read<u32>(%16));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(read<u32>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f(%7 e: @type0) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %9 enum_to_int<u32, reason=promotion>(read<@type0>(%7))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %9 const<u32>(0):
// DEFAULT-NEXT:                     write<i32>(%8, const<i32>(1));
// DEFAULT-NEXT:                 break %9;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         switch %10 enum_to_int<u32, reason=promotion>(read<@type0>(%7))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %10 const<u32>(0):
// DEFAULT-NEXT:                     write<i32>(%8, const<i32>(1));
// DEFAULT-NEXT:                 break %10;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
