// SLATE-FILECHECK-DEFINES DEFAULT

short
inner_product (short *a, short *b)
{
  int i;
  short sum = 0;

  for (i = 9; i >= 0; i--)
    sum += (*a++) * (*b++);

  return sum;
}

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
// DEFAULT-NEXT:     fn %[[VALUE_inner_product:[0-9]+]] @inner_product(%[[VALUE_a:[0-9]+]] a: ptr<i16>, %[[VALUE_b:[0-9]+]] b: ptr<i16>) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(9));
// DEFAULT-NEXT:             condition: ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_sum]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<i16> [synthetic] = read<ptr<i16>>(%[[VALUE_a]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<i16> [synthetic] = ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i16>>(%[[VALUE_a]], read<ptr<i16>>(%[[VALUE5]]));
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<i16> [synthetic] = read<ptr<i16>>(%[[VALUE_b]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<i16> [synthetic] = ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i16>>(%[[VALUE_b]], read<ptr<i16>>(%[[VALUE7]]));
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE3]])), mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%[[VALUE4]])))), widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%[[VALUE6]])))))));
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_sum]], read<i16>(%[[VALUE8]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE_sum]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
