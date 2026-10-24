/* { dg-do compile { target powerpc*-*-* ia64-*-* i?86-*-* x86_64-*-* } } */
/* { dg-options "-fpermissive -O2 -fnon-call-exceptions -fsel-sched-pipelining -fsel-sched-pipelining-outer-loops -fselective-scheduling -fno-if-conversion -fno-tree-dce -w" } */

int kn;

void
gd (short int sk)
{
  char *as;

  while (sk < 1)
    {
      char *ci;

      if (*ci == 0)
        *as += ci;

      for (kn = 0; kn < 18; ++kn)
        {
        }

      ++sk;
    }
}

// SLATE-FILECHECK-ARGS -fpermissive
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
// DEFAULT-NEXT:     global %[[VALUE_kn:[0-9]+]] kn: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gd:[0-9]+]] @gd(%[[VALUE_sk:[0-9]+]] sk: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_as:[0-9]+]] as: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] lt<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_sk]])), const<i32>(1))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_ci:[0-9]+]] ci: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_ci]])))), const<i32>(0))
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_as]]);
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%[[VALUE1]])));
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i8 [synthetic] = ptr_to_int<i8, reason=assign>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_ci]]), widen<i32, reason=promotion>(read<i8>(%[[VALUE2]]))));
// DEFAULT-NEXT:                     write<i8>(deref(read<ptr<i8>>(%[[VALUE1]])), read<i8>(%[[VALUE3]]));
// DEFAULT-NEXT:                 for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_kn]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_kn]]), const<i32>(18))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_kn]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_kn]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_sk]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE7]])), const<i32>(1)));
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_sk]], read<i16>(%[[VALUE8]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
