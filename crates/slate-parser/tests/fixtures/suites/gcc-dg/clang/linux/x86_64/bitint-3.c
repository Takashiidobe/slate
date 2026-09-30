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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i2b;
// DEFAULT-NEXT:         field1 b: i6b;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32b [storage=static] = const<i32b>(2147483647) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i64b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i64b [storage=static] = const<i64b>(9223372036854775807) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: @type[[TYPE_S]] [storage=static] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = sub<i2b, overflow=ub>(neg<i2b, overflow=ub>(const<i2b>(1)), const<i2b>(1)), field1 = const<i6b>(31)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: u128b [storage=static] = const<u128b>(340282366920938463463374607431768211455) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i128b [storage=static] = widen<i128b, reason=assign>(const<i125b>(20000000000000000000000000000000000000)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i128b [storage=static] = widen<i128b, reason=assign>(neg<i125b, overflow=ub>(const<i125b>(20000000000000000000000000000000000000))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: u575b [storage=static] = const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i575b [storage=static] = widen<i575b, reason=assign>(const<i142b>(2000000000000000000000000000000000000000000)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i575b [storage=static] = widen<i575b, reason=assign>(neg<i142b, overflow=ub>(const<i142b>(2000000000000000000000000000000000000000000))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32b>(%[[VALUE_a]], read<i32b>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i64b>(%[[VALUE_c]], read<i64b>(%[[VALUE_d]]));
// DEFAULT-NEXT:         write<i2b>(field0(%[[VALUE_s]]), read<i2b>(field0(%[[VALUE_t]])));
// DEFAULT-NEXT:         write<i6b>(field1(%[[VALUE_s]]), read<i6b>(field1(%[[VALUE_t]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32b [synthetic] = read<i32b>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32b [synthetic] = add<i32b, overflow=ub>(read<i32b>(%[[VALUE0]]), read<i32b>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32b>(%[[VALUE_a]], read<i32b>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i64b [synthetic] = read<i64b>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i64b [synthetic] = add<i64b, overflow=ub>(read<i64b>(%[[VALUE2]]), read<i64b>(%[[VALUE_d]]));
// DEFAULT-NEXT:         write<i64b>(%[[VALUE_c]], read<i64b>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i2b [synthetic] = read<i2b>(field0(%[[VALUE_s]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i2b [synthetic] = add<i2b, overflow=ub>(read<i2b>(%[[VALUE4]]), read<i2b>(field0(%[[VALUE_t]])));
// DEFAULT-NEXT:         write<i2b>(field0(%[[VALUE_s]]), read<i2b>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i6b [synthetic] = read<i6b>(field1(%[[VALUE_s]]));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i6b [synthetic] = add<i6b, overflow=ub>(read<i6b>(%[[VALUE6]]), read<i6b>(field1(%[[VALUE_t]])));
// DEFAULT-NEXT:         write<i6b>(field1(%[[VALUE_s]]), read<i6b>(%[[VALUE7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
