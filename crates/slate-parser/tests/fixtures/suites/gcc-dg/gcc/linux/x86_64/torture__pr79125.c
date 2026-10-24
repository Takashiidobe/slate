/* { dg-additional-options "-fpermissive" } */
int za;

void
hl (void)
{
  short int o8 = 0;
  short int m6 = 1;
  short int *ni = &m6;

  for (;;)
    {
      int af;
      short int *fd = (short int *)&ni;

      if (ni != 0)
        {
          if (m6 != 0)
            *ni = 0;
          else
            za = 0;
          af = (o8 * o8) || o8;
          if (af == 0)
            m6 /= 0; /* { dg-warning "division" } */
          while (za != 0)
            {
            }
        }
      *fd = &o8; /* { dg-warning "without a cast" } */
      for (af = 0; af < 2; ++af)
        af = za;
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
// DEFAULT-NEXT:     global %[[VALUE_za:[0-9]+]] za: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_hl:[0-9]+]] @hl() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_o8:[0-9]+]] o8: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_m6:[0-9]+]] m6: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_ni:[0-9]+]] ni: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%[[VALUE_m6]]);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_af:[0-9]+]] af: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE_fd:[0-9]+]] fd: ptr<i16> [storage=automatic] = pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<ptr<i16>>>(%[[VALUE_ni]]));
// DEFAULT-NEXT:                     if ne<ptr<i16>>(read<ptr<i16>>(%[[VALUE_ni]]), null<ptr<i16>>)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_m6]])), const<i32>(0))
// DEFAULT-NEXT:                                 write<i16>(deref(read<ptr<i16>>(%[[VALUE_ni]])), truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_za]], const<i32>(0));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_af]], from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_o8]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_o8]]))), const<i32>(0)), ne<i16>(read<i16>(%[[VALUE_o8]]), const<i16>(0)))));
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_af]]), const<i32>(0))
// DEFAULT-NEXT:                                 let %[[VALUE1:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_m6]]);
// DEFAULT-NEXT:                                 let %[[VALUE2:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE1]])), const<i32>(0)));
// DEFAULT-NEXT:                                 write<i16>(%[[VALUE_m6]], read<i16>(%[[VALUE2]]));
// DEFAULT-NEXT:                             while %[[VALUE3:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_za]]), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<i16>(deref(read<ptr<i16>>(%[[VALUE_fd]])), ptr_to_int<i16, reason=assign>(addr_of<ptr<i16>>(%[[VALUE_o8]])));
// DEFAULT-NEXT:                     for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_af]], const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_af]]), const<i32>(2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_af]]);
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_af]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_af]], read<i32>(%[[VALUE_za]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
