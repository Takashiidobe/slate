/* PR tree-optimization/112941 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

#if __BITINT_MAXWIDTH__ >= 4096
void
f1 (_BitInt(4096) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] *= (unsigned _BitInt(2048)) r;
  p[1] *= (unsigned _BitInt(2048)) s;
  p[2] *= (unsigned _BitInt(2048)) t;
  p[3] *= (unsigned _BitInt(2048)) u;
}

void
f2 (_BitInt(4094) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] /= (unsigned _BitInt(2048)) r;
  p[1] /= (unsigned _BitInt(2048)) s;
  p[2] /= (unsigned _BitInt(2048)) t;
  p[3] /= (unsigned _BitInt(2048)) u;
}

void
f3 (_BitInt(4096) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] *= (unsigned _BitInt(2110)) r;
  p[1] *= (unsigned _BitInt(2110)) s;
  p[2] *= (unsigned _BitInt(2110)) t;
  p[3] *= (unsigned _BitInt(2110)) u;
}

void
f4 (_BitInt(4094) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] /= (unsigned _BitInt(2110)) r;
  p[1] /= (unsigned _BitInt(2110)) s;
  p[2] /= (unsigned _BitInt(2110)) t;
  p[3] /= (unsigned _BitInt(2110)) u;
}

void
f5 (unsigned _BitInt(4096) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] *= (unsigned _BitInt(2048)) r;
  p[1] *= (unsigned _BitInt(2048)) s;
  p[2] *= (unsigned _BitInt(2048)) t;
  p[3] *= (unsigned _BitInt(2048)) u;
}

void
f6 (unsigned _BitInt(4094) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] /= (unsigned _BitInt(2048)) r;
  p[1] /= (unsigned _BitInt(2048)) s;
  p[2] /= (unsigned _BitInt(2048)) t;
  p[3] /= (unsigned _BitInt(2048)) u;
}

void
f7 (unsigned _BitInt(4096) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] *= (unsigned _BitInt(2110)) r;
  p[1] *= (unsigned _BitInt(2110)) s;
  p[2] *= (unsigned _BitInt(2110)) t;
  p[3] *= (unsigned _BitInt(2110)) u;
}

void
f8 (unsigned _BitInt(4094) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] /= (unsigned _BitInt(2110)) r;
  p[1] /= (unsigned _BitInt(2110)) s;
  p[2] /= (unsigned _BitInt(2110)) t;
  p[3] /= (unsigned _BitInt(2110)) u;
}

#if __SIZEOF_INT128__
void
f9 (_BitInt(4096) *p, __int128 r)
{
  p[0] *= (unsigned _BitInt(2048)) r;
}

void
f10 (_BitInt(4094) *p, __int128 r)
{
  p[0] /= (unsigned _BitInt(2048)) r;
}

void
f11 (_BitInt(4096) *p, __int128 r)
{
  p[0] *= (unsigned _BitInt(2110)) r;
}

void
f12 (_BitInt(4094) *p, __int128 r)
{
  p[0] /= (unsigned _BitInt(2110)) r;
}

void
f13 (unsigned _BitInt(4096) *p, __int128 r)
{
  p[0] *= (unsigned _BitInt(2048)) r;
}

void
f14 (unsigned _BitInt(4094) *p, __int128 r)
{
  p[0] /= (unsigned _BitInt(2048)) r;
}

void
f15 (unsigned _BitInt(4096) *p, __int128 r)
{
  p[0] *= (unsigned _BitInt(2110)) r;
}

void
f16 (unsigned _BitInt(4094) *p, __int128 r)
{
  p[0] /= (unsigned _BitInt(2110)) r;
}
#endif
#else
int i;
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
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_p:[0-9]+]] p: ptr<i4096b>, %[[VALUE_r:[0-9]+]] r: i32, %[[VALUE_s:[0-9]+]] s: i115b, %[[VALUE_t:[0-9]+]] t: i128b, %[[VALUE_u:[0-9]+]] u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%[[VALUE_p]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE0]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i4096b [synthetic] = mul<i4096b, overflow=ub>(read<i4096b>(%[[VALUE1]]), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i32>(%[[VALUE_r]]))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE0]])), read<i4096b>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%[[VALUE_p]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE3]])));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i4096b [synthetic] = mul<i4096b, overflow=ub>(read<i4096b>(%[[VALUE4]]), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i115b>(%[[VALUE_s]]))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE3]])), read<i4096b>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%[[VALUE_p]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE6]])));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i4096b [synthetic] = mul<i4096b, overflow=ub>(read<i4096b>(%[[VALUE7]]), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128b>(%[[VALUE_t]]))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE6]])), read<i4096b>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%[[VALUE_p]]), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE9]])));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i4096b [synthetic] = mul<i4096b, overflow=ub>(read<i4096b>(%[[VALUE10]]), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i231b>(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE9]])), read<i4096b>(%[[VALUE11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_p_2:[0-9]+]] p: ptr<i4094b>, %[[VALUE_r_2:[0-9]+]] r: i32, %[[VALUE_s_2:[0-9]+]] s: i115b, %[[VALUE_t_2:[0-9]+]] t: i128b, %[[VALUE_u_2:[0-9]+]] u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%[[VALUE_p_2]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE12]])));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i4094b [synthetic] = div<i4094b, by_zero=ub, min_by_neg_one=ub>(read<i4094b>(%[[VALUE13]]), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i32>(%[[VALUE_r_2]]))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE12]])), read<i4094b>(%[[VALUE14]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%[[VALUE_p_2]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE15]])));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i4094b [synthetic] = div<i4094b, by_zero=ub, min_by_neg_one=ub>(read<i4094b>(%[[VALUE16]]), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i115b>(%[[VALUE_s_2]]))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE15]])), read<i4094b>(%[[VALUE17]]));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%[[VALUE_p_2]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE18]])));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i4094b [synthetic] = div<i4094b, by_zero=ub, min_by_neg_one=ub>(read<i4094b>(%[[VALUE19]]), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128b>(%[[VALUE_t_2]]))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE18]])), read<i4094b>(%[[VALUE20]]));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%[[VALUE_p_2]]), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE21]])));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i4094b [synthetic] = div<i4094b, by_zero=ub, min_by_neg_one=ub>(read<i4094b>(%[[VALUE22]]), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i231b>(%[[VALUE_u_2]]))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE21]])), read<i4094b>(%[[VALUE23]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_p_3:[0-9]+]] p: ptr<i4096b>, %[[VALUE_r_3:[0-9]+]] r: i32, %[[VALUE_s_3:[0-9]+]] s: i115b, %[[VALUE_t_3:[0-9]+]] t: i128b, %[[VALUE_u_3:[0-9]+]] u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%[[VALUE_p_3]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE24]])));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i4096b [synthetic] = mul<i4096b, overflow=ub>(read<i4096b>(%[[VALUE25]]), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i32>(%[[VALUE_r_3]]))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE24]])), read<i4096b>(%[[VALUE26]]));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%[[VALUE_p_3]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE27]])));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i4096b [synthetic] = mul<i4096b, overflow=ub>(read<i4096b>(%[[VALUE28]]), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i115b>(%[[VALUE_s_3]]))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE27]])), read<i4096b>(%[[VALUE29]]));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%[[VALUE_p_3]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE30]])));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i4096b [synthetic] = mul<i4096b, overflow=ub>(read<i4096b>(%[[VALUE31]]), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128b>(%[[VALUE_t_3]]))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE30]])), read<i4096b>(%[[VALUE32]]));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%[[VALUE_p_3]]), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE33]])));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i4096b [synthetic] = mul<i4096b, overflow=ub>(read<i4096b>(%[[VALUE34]]), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i231b>(%[[VALUE_u_3]]))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE33]])), read<i4096b>(%[[VALUE35]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_p_4:[0-9]+]] p: ptr<i4094b>, %[[VALUE_r_4:[0-9]+]] r: i32, %[[VALUE_s_4:[0-9]+]] s: i115b, %[[VALUE_t_4:[0-9]+]] t: i128b, %[[VALUE_u_4:[0-9]+]] u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%[[VALUE_p_4]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE36]])));
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i4094b [synthetic] = div<i4094b, by_zero=ub, min_by_neg_one=ub>(read<i4094b>(%[[VALUE37]]), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i32>(%[[VALUE_r_4]]))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE36]])), read<i4094b>(%[[VALUE38]]));
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%[[VALUE_p_4]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE39]])));
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i4094b [synthetic] = div<i4094b, by_zero=ub, min_by_neg_one=ub>(read<i4094b>(%[[VALUE40]]), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i115b>(%[[VALUE_s_4]]))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE39]])), read<i4094b>(%[[VALUE41]]));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%[[VALUE_p_4]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE42]])));
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i4094b [synthetic] = div<i4094b, by_zero=ub, min_by_neg_one=ub>(read<i4094b>(%[[VALUE43]]), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128b>(%[[VALUE_t_4]]))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE42]])), read<i4094b>(%[[VALUE44]]));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%[[VALUE_p_4]]), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE45]])));
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i4094b [synthetic] = div<i4094b, by_zero=ub, min_by_neg_one=ub>(read<i4094b>(%[[VALUE46]]), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i231b>(%[[VALUE_u_4]]))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE45]])), read<i4094b>(%[[VALUE47]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_p_5:[0-9]+]] p: ptr<u4096b>, %[[VALUE_r_5:[0-9]+]] r: i32, %[[VALUE_s_5:[0-9]+]] s: i115b, %[[VALUE_t_5:[0-9]+]] t: i128b, %[[VALUE_u_5:[0-9]+]] u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%[[VALUE_p_5]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE48]])));
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: u4096b [synthetic] = mul<u4096b, overflow=wrap>(read<u4096b>(%[[VALUE49]]), widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i32>(%[[VALUE_r_5]])))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE48]])), read<u4096b>(%[[VALUE50]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%[[VALUE_p_5]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE51]])));
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: u4096b [synthetic] = mul<u4096b, overflow=wrap>(read<u4096b>(%[[VALUE52]]), widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i115b>(%[[VALUE_s_5]])))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE51]])), read<u4096b>(%[[VALUE53]]));
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%[[VALUE_p_5]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE54]])));
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: u4096b [synthetic] = mul<u4096b, overflow=wrap>(read<u4096b>(%[[VALUE55]]), widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128b>(%[[VALUE_t_5]])))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE54]])), read<u4096b>(%[[VALUE56]]));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%[[VALUE_p_5]]), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE57]])));
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: u4096b [synthetic] = mul<u4096b, overflow=wrap>(read<u4096b>(%[[VALUE58]]), widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i231b>(%[[VALUE_u_5]])))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE57]])), read<u4096b>(%[[VALUE59]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_p_6:[0-9]+]] p: ptr<u4094b>, %[[VALUE_r_6:[0-9]+]] r: i32, %[[VALUE_s_6:[0-9]+]] s: i115b, %[[VALUE_t_6:[0-9]+]] t: i128b, %[[VALUE_u_6:[0-9]+]] u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%[[VALUE_p_6]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE60]])));
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: u4094b [synthetic] = div<u4094b, by_zero=ub>(read<u4094b>(%[[VALUE61]]), widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i32>(%[[VALUE_r_6]])))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE60]])), read<u4094b>(%[[VALUE62]]));
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%[[VALUE_p_6]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE63]])));
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: u4094b [synthetic] = div<u4094b, by_zero=ub>(read<u4094b>(%[[VALUE64]]), widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i115b>(%[[VALUE_s_6]])))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE63]])), read<u4094b>(%[[VALUE65]]));
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%[[VALUE_p_6]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE66]])));
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: u4094b [synthetic] = div<u4094b, by_zero=ub>(read<u4094b>(%[[VALUE67]]), widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128b>(%[[VALUE_t_6]])))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE66]])), read<u4094b>(%[[VALUE68]]));
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%[[VALUE_p_6]]), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE69]])));
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: u4094b [synthetic] = div<u4094b, by_zero=ub>(read<u4094b>(%[[VALUE70]]), widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i231b>(%[[VALUE_u_6]])))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE69]])), read<u4094b>(%[[VALUE71]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_p_7:[0-9]+]] p: ptr<u4096b>, %[[VALUE_r_7:[0-9]+]] r: i32, %[[VALUE_s_7:[0-9]+]] s: i115b, %[[VALUE_t_7:[0-9]+]] t: i128b, %[[VALUE_u_7:[0-9]+]] u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%[[VALUE_p_7]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE72]])));
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: u4096b [synthetic] = mul<u4096b, overflow=wrap>(read<u4096b>(%[[VALUE73]]), widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i32>(%[[VALUE_r_7]])))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE72]])), read<u4096b>(%[[VALUE74]]));
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%[[VALUE_p_7]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE75]])));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: u4096b [synthetic] = mul<u4096b, overflow=wrap>(read<u4096b>(%[[VALUE76]]), widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i115b>(%[[VALUE_s_7]])))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE75]])), read<u4096b>(%[[VALUE77]]));
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%[[VALUE_p_7]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE78]])));
// DEFAULT-NEXT:         let %[[VALUE80:[0-9]+]]: u4096b [synthetic] = mul<u4096b, overflow=wrap>(read<u4096b>(%[[VALUE79]]), widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128b>(%[[VALUE_t_7]])))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE78]])), read<u4096b>(%[[VALUE80]]));
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%[[VALUE_p_7]]), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE81]])));
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: u4096b [synthetic] = mul<u4096b, overflow=wrap>(read<u4096b>(%[[VALUE82]]), widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i231b>(%[[VALUE_u_7]])))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE81]])), read<u4096b>(%[[VALUE83]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_p_8:[0-9]+]] p: ptr<u4094b>, %[[VALUE_r_8:[0-9]+]] r: i32, %[[VALUE_s_8:[0-9]+]] s: i115b, %[[VALUE_t_8:[0-9]+]] t: i128b, %[[VALUE_u_8:[0-9]+]] u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%[[VALUE_p_8]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE84]])));
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: u4094b [synthetic] = div<u4094b, by_zero=ub>(read<u4094b>(%[[VALUE85]]), widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i32>(%[[VALUE_r_8]])))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE84]])), read<u4094b>(%[[VALUE86]]));
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%[[VALUE_p_8]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE88:[0-9]+]]: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE87]])));
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: u4094b [synthetic] = div<u4094b, by_zero=ub>(read<u4094b>(%[[VALUE88]]), widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i115b>(%[[VALUE_s_8]])))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE87]])), read<u4094b>(%[[VALUE89]]));
// DEFAULT-NEXT:         let %[[VALUE90:[0-9]+]]: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%[[VALUE_p_8]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE91:[0-9]+]]: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE90]])));
// DEFAULT-NEXT:         let %[[VALUE92:[0-9]+]]: u4094b [synthetic] = div<u4094b, by_zero=ub>(read<u4094b>(%[[VALUE91]]), widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128b>(%[[VALUE_t_8]])))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE90]])), read<u4094b>(%[[VALUE92]]));
// DEFAULT-NEXT:         let %[[VALUE93:[0-9]+]]: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%[[VALUE_p_8]]), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE94:[0-9]+]]: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE93]])));
// DEFAULT-NEXT:         let %[[VALUE95:[0-9]+]]: u4094b [synthetic] = div<u4094b, by_zero=ub>(read<u4094b>(%[[VALUE94]]), widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i231b>(%[[VALUE_u_8]])))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE93]])), read<u4094b>(%[[VALUE95]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE_p_9:[0-9]+]] p: ptr<i4096b>, %[[VALUE_r_9:[0-9]+]] r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE96:[0-9]+]]: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%[[VALUE_p_9]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE96]])));
// DEFAULT-NEXT:         let %[[VALUE98:[0-9]+]]: i4096b [synthetic] = mul<i4096b, overflow=ub>(read<i4096b>(%[[VALUE97]]), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128>(%[[VALUE_r_9]]))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE96]])), read<i4096b>(%[[VALUE98]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_p_10:[0-9]+]] p: ptr<i4094b>, %[[VALUE_r_10:[0-9]+]] r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%[[VALUE_p_10]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE99]])));
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: i4094b [synthetic] = div<i4094b, by_zero=ub, min_by_neg_one=ub>(read<i4094b>(%[[VALUE100]]), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128>(%[[VALUE_r_10]]))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE99]])), read<i4094b>(%[[VALUE101]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11(%[[VALUE_p_11:[0-9]+]] p: ptr<i4096b>, %[[VALUE_r_11:[0-9]+]] r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%[[VALUE_p_11]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE102]])));
// DEFAULT-NEXT:         let %[[VALUE104:[0-9]+]]: i4096b [synthetic] = mul<i4096b, overflow=ub>(read<i4096b>(%[[VALUE103]]), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128>(%[[VALUE_r_11]]))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%[[VALUE102]])), read<i4096b>(%[[VALUE104]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12(%[[VALUE_p_12:[0-9]+]] p: ptr<i4094b>, %[[VALUE_r_12:[0-9]+]] r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%[[VALUE_p_12]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE106:[0-9]+]]: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE105]])));
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: i4094b [synthetic] = div<i4094b, by_zero=ub, min_by_neg_one=ub>(read<i4094b>(%[[VALUE106]]), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128>(%[[VALUE_r_12]]))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%[[VALUE105]])), read<i4094b>(%[[VALUE107]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f13:[0-9]+]] @f13(%[[VALUE_p_13:[0-9]+]] p: ptr<u4096b>, %[[VALUE_r_13:[0-9]+]] r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE108:[0-9]+]]: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%[[VALUE_p_13]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE108]])));
// DEFAULT-NEXT:         let %[[VALUE110:[0-9]+]]: u4096b [synthetic] = mul<u4096b, overflow=wrap>(read<u4096b>(%[[VALUE109]]), widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128>(%[[VALUE_r_13]])))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE108]])), read<u4096b>(%[[VALUE110]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f14:[0-9]+]] @f14(%[[VALUE_p_14:[0-9]+]] p: ptr<u4094b>, %[[VALUE_r_14:[0-9]+]] r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%[[VALUE_p_14]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE112:[0-9]+]]: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE111]])));
// DEFAULT-NEXT:         let %[[VALUE113:[0-9]+]]: u4094b [synthetic] = div<u4094b, by_zero=ub>(read<u4094b>(%[[VALUE112]]), widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128>(%[[VALUE_r_14]])))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE111]])), read<u4094b>(%[[VALUE113]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f15:[0-9]+]] @f15(%[[VALUE_p_15:[0-9]+]] p: ptr<u4096b>, %[[VALUE_r_15:[0-9]+]] r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE114:[0-9]+]]: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%[[VALUE_p_15]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE115:[0-9]+]]: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE114]])));
// DEFAULT-NEXT:         let %[[VALUE116:[0-9]+]]: u4096b [synthetic] = mul<u4096b, overflow=wrap>(read<u4096b>(%[[VALUE115]]), widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128>(%[[VALUE_r_15]])))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%[[VALUE114]])), read<u4096b>(%[[VALUE116]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f16:[0-9]+]] @f16(%[[VALUE_p_16:[0-9]+]] p: ptr<u4094b>, %[[VALUE_r_16:[0-9]+]] r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE117:[0-9]+]]: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%[[VALUE_p_16]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE118:[0-9]+]]: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE117]])));
// DEFAULT-NEXT:         let %[[VALUE119:[0-9]+]]: u4094b [synthetic] = div<u4094b, by_zero=ub>(read<u4094b>(%[[VALUE118]]), widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128>(%[[VALUE_r_16]])))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%[[VALUE117]])), read<u4094b>(%[[VALUE119]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
