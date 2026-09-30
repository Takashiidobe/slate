/* PR c/102989 */
/* { dg-do compile { target { bitint && { float32 && int32 } } } } */
/* { dg-options "-std=c23 -Wconversion -Wfloat-conversion" } */
/* { dg-add-options float32 } */

void
foo (_Float32 x)
{
  _BitInt(57) a = 1.5F32;				/* { dg-warning "conversion from '_Float32' to '_BitInt\\\(57\\\)' changes value from '1.5e\\\+0f32' to '1'" } */
  _BitInt(27) b = 76117358uwb;				/* { dg-warning "signed conversion from 'unsigned _BitInt\\\(27\\\)' to '_BitInt\\\(27\\\)' changes value from '76117358' to '-58100370'" } */
  unsigned _BitInt(27) c = -15wb;			/* { dg-warning "unsigned conversion from '_BitInt\\\(5\\\)' to 'unsigned _BitInt\\\(27\\\)' changes value from '-15' to '134217713'" } */
  _BitInt(27) d = -390288573wb;				/* { dg-warning "overflow in conversion from '_BitInt\\\(30\\\)' to '_BitInt\\\(27\\\)' changes value from '-390288573' to '12364611'" } */
  unsigned _BitInt(27) e = 309641337uwb;		/* { dg-warning "conversion from 'unsigned _BitInt\\\(29\\\)' to 'unsigned _BitInt\\\(27\\\)' changes value from '309641337' to '41205881'" } */
  _BitInt(27) f = 76117358U;				/* { dg-warning "signed conversion from 'unsigned int' to '_BitInt\\\(27\\\)' changes value from '76117358' to '-58100370'" } */
  unsigned _BitInt(27) g = -15;				/* { dg-warning "unsigned conversion from 'int' to 'unsigned _BitInt\\\(27\\\)' changes value from '-15' to '134217713'" } */
  _BitInt(27) h = -390288573;				/* { dg-warning "overflow in conversion from 'int' to '_BitInt\\\(27\\\)' changes value from '-390288573' to '12364611'" } */
  unsigned _BitInt(27) i = 309641337U;			/* { dg-warning "conversion from 'unsigned int' to 'unsigned _BitInt\\\(27\\\)' changes value from '309641337' to '41205881'" } */
  int j = 2936216298uwb;				/* { dg-warning "signed conversion from 'unsigned _BitInt\\\(32\\\)' to 'int' changes value from '2936216298' to '-1358750998'" } */
  unsigned int k = -15wb;				/* { dg-warning "unsigned conversion from '_BitInt\\\(5\\\)' to 'unsigned int' changes value from '-15' to '4294967281'" } */
  int l = -8087431137529383656wb;			/* { dg-warning "overflow in conversion from '_BitInt\\\(64\\\)' to 'int' changes value from '-8087431137529383656' to '-1105152744'" } */
  unsigned int m = 1664073919553255778uwb;		/* { dg-warning "conversion from 'unsigned _BitInt\\\(61\\\)' to 'unsigned int' changes value from '1664073919553255778' to '3338058082'" } */
#if __BITINT_MAXWIDTH__ >= 575
  _Float32 n = 51441631083309184313435496923626431699697406185384986811300218556561965470218425783308778801748592322915101142266821623326688106425864884688172114173397118407357447763009120wb;	/* { dg-warning "conversion from '_BitInt\\\(575\\\)' to '_Float32' changes value from '0x353eab28b46b03ea99b84f9736cd8dbe5e986915a0383c3cb381c0da41e31b3621c75fd53262bfcb1b0e6251dbf00f3988784e29b08b65640c263e4d0959832a20e2ff5245be1e60' to '\\\+Inff32'" "" { target bitint575 } } */
#endif
  _BitInt(57) o = x;					/* { dg-warning "conversion from '_Float32' to '_BitInt\\\(57\\\)' may change value" } */
  unsigned _BitInt(15) p = 32767uwb;
  unsigned _BitInt(15) q = (_BitInt(42)) p;
  _BitInt(17) r = 0;
  _BitInt(17) s = ((_BitInt(42)) r) & 32767wb;
#if __BITINT_MAXWIDTH__ >= 575
  _BitInt(575) t = 0;
  _Float32 u = t;					/* { dg-warning "conversion from '_BitInt\\\(575\\\)' to '_Float32' may change value" "" { target bitint575 } } */
#endif
  _BitInt(42) v = 0;
  unsigned _BitInt(17) w = v;				/* { dg-warning "conversion from '_BitInt\\\(42\\\)' to 'unsigned _BitInt\\\(17\\\)' may change value" } */
  _BitInt(17) y = v;					/* { dg-warning "conversion from '_BitInt\\\(42\\\)' to '_BitInt\\\(17\\\)' may change value" } */
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i57b [storage=automatic] = float_to_int<i57b, reason=assign, out_of_range=ub, exceptions=observable>(const<f32>(1.5));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i27b [storage=automatic] = reinterpret<i27b, reason=assign, fits=unknown>(const<u27b>(76117358));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: u27b [storage=automatic] = reinterpret<u27b, reason=assign, fits=unknown>(widen<i27b, reason=assign>(neg<i5b, overflow=ub>(const<i5b>(15))));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i27b [storage=automatic] = truncate<i27b, reason=assign, fits=unknown>(neg<i30b, overflow=ub>(const<i30b>(390288573)));
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: u27b [storage=automatic] = truncate<u27b, reason=assign, fits=unknown>(const<u29b>(309641337));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i27b [storage=automatic] = reinterpret<i27b, reason=assign, fits=unknown>(truncate<u27b, reason=assign, fits=always>(const<u32>(76117358)));
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: u27b [storage=automatic] = reinterpret<u27b, reason=assign, fits=unknown>(truncate<i27b, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(15))));
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i27b [storage=automatic] = truncate<i27b, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(390288573)));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u27b [storage=automatic] = truncate<u27b, reason=assign, fits=unknown>(const<u32>(309641337));
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(const<u32b>(2936216298));
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=unknown>(widen<i32, reason=assign>(neg<i5b, overflow=ub>(const<i5b>(15))));
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i32 [storage=automatic] = truncate<i32, reason=assign, fits=unknown>(neg<i64b, overflow=ub>(const<i64b>(8087431137529383656)));
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(const<u61b>(1664073919553255778));
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i575b>(51441631083309184313435496923626431699697406185384986811300218556561965470218425783308778801748592322915101142266821623326688106425864884688172114173397118407357447763009120));
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: i57b [storage=automatic] = float_to_int<i57b, reason=assign, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: u15b [storage=automatic] = const<u15b>(32767);
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: u15b [storage=automatic] = reinterpret<u15b, reason=assign, fits=unknown>(truncate<i15b, reason=assign, fits=unknown>(reinterpret<i42b, reason=explicit, fits=unknown>(widen<u42b, reason=explicit>(read<u15b>(%[[VALUE_p]])))));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i17b [storage=automatic] = truncate<i17b, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: i17b [storage=automatic] = truncate<i17b, reason=assign, fits=unknown>(and<i42b>(widen<i42b, reason=explicit>(read<i17b>(%[[VALUE_r]])), widen<i42b, reason=usual_arith>(const<i16b>(32767))));
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: i575b [storage=automatic] = widen<i575b, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(read<i575b>(%[[VALUE_t]]));
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: i42b [storage=automatic] = widen<i42b, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: u17b [storage=automatic] = reinterpret<u17b, reason=assign, fits=unknown>(truncate<i17b, reason=assign, fits=unknown>(read<i42b>(%[[VALUE_v]])));
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i17b [storage=automatic] = truncate<i17b, reason=assign, fits=unknown>(read<i42b>(%[[VALUE_v]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
