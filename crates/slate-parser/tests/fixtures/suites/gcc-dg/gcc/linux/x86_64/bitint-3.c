/* PR c/102989 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */

_BitInt(32) a, b = 2147483647wb;
_BitInt(64) c, d = 9223372036854775807wb;
struct S {
  _BitInt(2) a;
  _BitInt(6) b;
} s, t = { -1wb - 1wb, 31wb };

void
foo (void)
{
  a = b;
  c = d;
  s.a = t.a;
  s.b = t.b;
}

void
bar (void)
{
  a += b;
  c += d;
  s.a += t.a;
  s.b += t.b;
}

#if __BITINT_MAXWIDTH__ >= 128
unsigned _BitInt(128) e = 340282366920938463463374607431768211455uwb;
_BitInt(128) f = 20000000000000000000000000000000000000wb;
_BitInt(128) g = -20000000000000000000000000000000000000wb;
#endif

#if __BITINT_MAXWIDTH__ >= 575
unsigned _BitInt(575) h = 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb;
_BitInt(575) i = 2000000000000000000000000000000000000000000wb;
_BitInt(575) j = -2000000000000000000000000000000000000000000wb;
#endif

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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i2b;
// DEFAULT-NEXT:         field1 b: i6b;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     global %0 a: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32b [storage=static] = const<i32b>(2147483647) [linkage=external];
// DEFAULT-NEXT:     global %2 c: i64b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i64b [storage=static] = const<i64b>(9223372036854775807) [linkage=external];
// DEFAULT-NEXT:     global %5 s: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 t: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = sub<i2b, overflow=ub>(neg<i2b, overflow=ub>(const<i2b>(1)), const<i2b>(1)), field1 = const<i6b>(31)) [linkage=external];
// DEFAULT-NEXT:     global %9 e: u128b [storage=static] = const<u128b>(340282366920938463463374607431768211455) [linkage=external];
// DEFAULT-NEXT:     global %10 f: i128b [storage=static] = widen<i128b, reason=assign>(const<i125b>(20000000000000000000000000000000000000)) [linkage=external];
// DEFAULT-NEXT:     global %11 g: i128b [storage=static] = widen<i128b, reason=assign>(neg<i125b, overflow=ub>(const<i125b>(20000000000000000000000000000000000000))) [linkage=external];
// DEFAULT-NEXT:     global %12 h: u575b [storage=static] = const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567) [linkage=external];
// DEFAULT-NEXT:     global %13 i: i575b [storage=static] = widen<i575b, reason=assign>(const<i142b>(2000000000000000000000000000000000000000000)) [linkage=external];
// DEFAULT-NEXT:     global %14 j: i575b [storage=static] = widen<i575b, reason=assign>(neg<i142b, overflow=ub>(const<i142b>(2000000000000000000000000000000000000000000))) [linkage=external];
// DEFAULT-NEXT:     fn %7 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32b>(%0, read<i32b>(%1));
// DEFAULT-NEXT:         write<i64b>(%2, read<i64b>(%3));
// DEFAULT-NEXT:         write<i2b>(field0(%5), read<i2b>(field0(%6)));
// DEFAULT-NEXT:         write<i6b>(field1(%5), read<i6b>(field1(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15: i32b [synthetic] = read<i32b>(%0);
// DEFAULT-NEXT:         let %16: i32b [synthetic] = add<i32b, overflow=ub>(read<i32b>(%15), read<i32b>(%1));
// DEFAULT-NEXT:         write<i32b>(%0, read<i32b>(%16));
// DEFAULT-NEXT:         let %17: i64b [synthetic] = read<i64b>(%2);
// DEFAULT-NEXT:         let %18: i64b [synthetic] = add<i64b, overflow=ub>(read<i64b>(%17), read<i64b>(%3));
// DEFAULT-NEXT:         write<i64b>(%2, read<i64b>(%18));
// DEFAULT-NEXT:         let %19: i2b [synthetic] = read<i2b>(field0(%5));
// DEFAULT-NEXT:         let %20: i2b [synthetic] = add<i2b, overflow=ub>(read<i2b>(%19), read<i2b>(field0(%6)));
// DEFAULT-NEXT:         write<i2b>(field0(%5), read<i2b>(%20));
// DEFAULT-NEXT:         let %21: i6b [synthetic] = read<i6b>(field1(%5));
// DEFAULT-NEXT:         let %22: i6b [synthetic] = add<i6b, overflow=ub>(read<i6b>(%21), read<i6b>(field1(%6)));
// DEFAULT-NEXT:         write<i6b>(field1(%5), read<i6b>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
