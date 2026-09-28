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
// DEFAULT-NEXT:     fn %0 @inner_product(%1 a: ptr<i16>, %2 b: ptr<i16>) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 sum: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         for %5
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(9));
// DEFAULT-NEXT:             condition: ge<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %6: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%7));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %8: i16 [synthetic] = read<i16>(%4);
// DEFAULT-NEXT:                 let %9: ptr<i16> [synthetic] = read<ptr<i16>>(%1);
// DEFAULT-NEXT:                 let %10: ptr<i16> [synthetic] = ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i16>>(%1, read<ptr<i16>>(%10));
// DEFAULT-NEXT:                 let %11: ptr<i16> [synthetic] = read<ptr<i16>>(%2);
// DEFAULT-NEXT:                 let %12: ptr<i16> [synthetic] = ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%11), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i16>>(%2, read<ptr<i16>>(%12));
// DEFAULT-NEXT:                 let %13: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%8)), mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%9)))), widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%11)))))));
// DEFAULT-NEXT:                 write<i16>(%4, read<i16>(%13));
// DEFAULT-NEXT:         return read<i16>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
