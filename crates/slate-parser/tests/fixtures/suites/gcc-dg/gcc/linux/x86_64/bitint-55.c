/* PR tree-optimization/112941 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

#if __BITINT_MAXWIDTH__ >= 4096
void
f1 (_BitInt(4096) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] += (unsigned _BitInt(2048)) r;
  p[1] += (unsigned _BitInt(2048)) s;
  p[2] += (unsigned _BitInt(2048)) t;
  p[3] += (unsigned _BitInt(2048)) u;
}

void
f2 (_BitInt(4094) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] -= (unsigned _BitInt(2048)) r;
  p[1] -= (unsigned _BitInt(2048)) s;
  p[2] -= (unsigned _BitInt(2048)) t;
  p[3] -= (unsigned _BitInt(2048)) u;
}

void
f3 (_BitInt(4096) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] += (unsigned _BitInt(2110)) r;
  p[1] += (unsigned _BitInt(2110)) s;
  p[2] += (unsigned _BitInt(2110)) t;
  p[3] += (unsigned _BitInt(2110)) u;
}

void
f4 (_BitInt(4094) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] -= (unsigned _BitInt(2110)) r;
  p[1] -= (unsigned _BitInt(2110)) s;
  p[2] -= (unsigned _BitInt(2110)) t;
  p[3] -= (unsigned _BitInt(2110)) u;
}

void
f5 (unsigned _BitInt(4096) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] += (unsigned _BitInt(2048)) r;
  p[1] += (unsigned _BitInt(2048)) s;
  p[2] += (unsigned _BitInt(2048)) t;
  p[3] += (unsigned _BitInt(2048)) u;
}

void
f6 (unsigned _BitInt(4094) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] -= (unsigned _BitInt(2048)) r;
  p[1] -= (unsigned _BitInt(2048)) s;
  p[2] -= (unsigned _BitInt(2048)) t;
  p[3] -= (unsigned _BitInt(2048)) u;
}

void
f7 (unsigned _BitInt(4096) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] += (unsigned _BitInt(2110)) r;
  p[1] += (unsigned _BitInt(2110)) s;
  p[2] += (unsigned _BitInt(2110)) t;
  p[3] += (unsigned _BitInt(2110)) u;
}

void
f8 (unsigned _BitInt(4094) *p, int r, _BitInt(115) s, _BitInt(128) t, _BitInt(231) u)
{
  p[0] -= (unsigned _BitInt(2110)) r;
  p[1] -= (unsigned _BitInt(2110)) s;
  p[2] -= (unsigned _BitInt(2110)) t;
  p[3] -= (unsigned _BitInt(2110)) u;
}

#if __SIZEOF_INT128__
void
f9 (_BitInt(4096) *p, __int128 r)
{
  p[0] += (unsigned _BitInt(2048)) r;
}

void
f10 (_BitInt(4094) *p, __int128 r)
{
  p[0] -= (unsigned _BitInt(2048)) r;
}

void
f11 (_BitInt(4096) *p, __int128 r)
{
  p[0] += (unsigned _BitInt(2110)) r;
}

void
f12 (_BitInt(4094) *p, __int128 r)
{
  p[0] -= (unsigned _BitInt(2110)) r;
}

void
f13 (unsigned _BitInt(4096) *p, __int128 r)
{
  p[0] += (unsigned _BitInt(2048)) r;
}

void
f14 (unsigned _BitInt(4094) *p, __int128 r)
{
  p[0] -= (unsigned _BitInt(2048)) r;
}

void
f15 (unsigned _BitInt(4096) *p, __int128 r)
{
  p[0] += (unsigned _BitInt(2110)) r;
}

void
f16 (unsigned _BitInt(4094) *p, __int128 r)
{
  p[0] -= (unsigned _BitInt(2110)) r;
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
// DEFAULT-NEXT:     fn %0 @f1(%1 p: ptr<i4096b>, %2 r: i32, %3 s: i115b, %4 t: i128b, %5 u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %72: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%1), const<i32>(0));
// DEFAULT-NEXT:         let %73: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%72)));
// DEFAULT-NEXT:         let %74: i4096b [synthetic] = add<i4096b, overflow=ub>(read<i4096b>(%73), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i32>(%2))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%72)), read<i4096b>(%74));
// DEFAULT-NEXT:         let %75: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%1), const<i32>(1));
// DEFAULT-NEXT:         let %76: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%75)));
// DEFAULT-NEXT:         let %77: i4096b [synthetic] = add<i4096b, overflow=ub>(read<i4096b>(%76), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i115b>(%3))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%75)), read<i4096b>(%77));
// DEFAULT-NEXT:         let %78: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%1), const<i32>(2));
// DEFAULT-NEXT:         let %79: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%78)));
// DEFAULT-NEXT:         let %80: i4096b [synthetic] = add<i4096b, overflow=ub>(read<i4096b>(%79), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128b>(%4))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%78)), read<i4096b>(%80));
// DEFAULT-NEXT:         let %81: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%1), const<i32>(3));
// DEFAULT-NEXT:         let %82: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%81)));
// DEFAULT-NEXT:         let %83: i4096b [synthetic] = add<i4096b, overflow=ub>(read<i4096b>(%82), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i231b>(%5))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%81)), read<i4096b>(%83));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f2(%7 p: ptr<i4094b>, %8 r: i32, %9 s: i115b, %10 t: i128b, %11 u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %84: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%7), const<i32>(0));
// DEFAULT-NEXT:         let %85: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%84)));
// DEFAULT-NEXT:         let %86: i4094b [synthetic] = sub<i4094b, overflow=ub>(read<i4094b>(%85), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i32>(%8))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%84)), read<i4094b>(%86));
// DEFAULT-NEXT:         let %87: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%7), const<i32>(1));
// DEFAULT-NEXT:         let %88: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%87)));
// DEFAULT-NEXT:         let %89: i4094b [synthetic] = sub<i4094b, overflow=ub>(read<i4094b>(%88), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i115b>(%9))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%87)), read<i4094b>(%89));
// DEFAULT-NEXT:         let %90: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%7), const<i32>(2));
// DEFAULT-NEXT:         let %91: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%90)));
// DEFAULT-NEXT:         let %92: i4094b [synthetic] = sub<i4094b, overflow=ub>(read<i4094b>(%91), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128b>(%10))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%90)), read<i4094b>(%92));
// DEFAULT-NEXT:         let %93: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%7), const<i32>(3));
// DEFAULT-NEXT:         let %94: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%93)));
// DEFAULT-NEXT:         let %95: i4094b [synthetic] = sub<i4094b, overflow=ub>(read<i4094b>(%94), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i231b>(%11))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%93)), read<i4094b>(%95));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f3(%13 p: ptr<i4096b>, %14 r: i32, %15 s: i115b, %16 t: i128b, %17 u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %96: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%13), const<i32>(0));
// DEFAULT-NEXT:         let %97: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%96)));
// DEFAULT-NEXT:         let %98: i4096b [synthetic] = add<i4096b, overflow=ub>(read<i4096b>(%97), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i32>(%14))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%96)), read<i4096b>(%98));
// DEFAULT-NEXT:         let %99: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%13), const<i32>(1));
// DEFAULT-NEXT:         let %100: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%99)));
// DEFAULT-NEXT:         let %101: i4096b [synthetic] = add<i4096b, overflow=ub>(read<i4096b>(%100), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i115b>(%15))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%99)), read<i4096b>(%101));
// DEFAULT-NEXT:         let %102: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%13), const<i32>(2));
// DEFAULT-NEXT:         let %103: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%102)));
// DEFAULT-NEXT:         let %104: i4096b [synthetic] = add<i4096b, overflow=ub>(read<i4096b>(%103), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128b>(%16))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%102)), read<i4096b>(%104));
// DEFAULT-NEXT:         let %105: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%13), const<i32>(3));
// DEFAULT-NEXT:         let %106: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%105)));
// DEFAULT-NEXT:         let %107: i4096b [synthetic] = add<i4096b, overflow=ub>(read<i4096b>(%106), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i231b>(%17))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%105)), read<i4096b>(%107));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f4(%19 p: ptr<i4094b>, %20 r: i32, %21 s: i115b, %22 t: i128b, %23 u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %108: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%19), const<i32>(0));
// DEFAULT-NEXT:         let %109: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%108)));
// DEFAULT-NEXT:         let %110: i4094b [synthetic] = sub<i4094b, overflow=ub>(read<i4094b>(%109), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i32>(%20))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%108)), read<i4094b>(%110));
// DEFAULT-NEXT:         let %111: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%19), const<i32>(1));
// DEFAULT-NEXT:         let %112: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%111)));
// DEFAULT-NEXT:         let %113: i4094b [synthetic] = sub<i4094b, overflow=ub>(read<i4094b>(%112), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i115b>(%21))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%111)), read<i4094b>(%113));
// DEFAULT-NEXT:         let %114: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%19), const<i32>(2));
// DEFAULT-NEXT:         let %115: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%114)));
// DEFAULT-NEXT:         let %116: i4094b [synthetic] = sub<i4094b, overflow=ub>(read<i4094b>(%115), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128b>(%22))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%114)), read<i4094b>(%116));
// DEFAULT-NEXT:         let %117: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%19), const<i32>(3));
// DEFAULT-NEXT:         let %118: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%117)));
// DEFAULT-NEXT:         let %119: i4094b [synthetic] = sub<i4094b, overflow=ub>(read<i4094b>(%118), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i231b>(%23))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%117)), read<i4094b>(%119));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @f5(%25 p: ptr<u4096b>, %26 r: i32, %27 s: i115b, %28 t: i128b, %29 u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %120: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%25), const<i32>(0));
// DEFAULT-NEXT:         let %121: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%120)));
// DEFAULT-NEXT:         let %122: u4096b [synthetic] = add<u4096b, overflow=wrap>(read<u4096b>(%121), widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i32>(%26)))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%120)), read<u4096b>(%122));
// DEFAULT-NEXT:         let %123: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%25), const<i32>(1));
// DEFAULT-NEXT:         let %124: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%123)));
// DEFAULT-NEXT:         let %125: u4096b [synthetic] = add<u4096b, overflow=wrap>(read<u4096b>(%124), widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i115b>(%27)))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%123)), read<u4096b>(%125));
// DEFAULT-NEXT:         let %126: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%25), const<i32>(2));
// DEFAULT-NEXT:         let %127: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%126)));
// DEFAULT-NEXT:         let %128: u4096b [synthetic] = add<u4096b, overflow=wrap>(read<u4096b>(%127), widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128b>(%28)))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%126)), read<u4096b>(%128));
// DEFAULT-NEXT:         let %129: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%25), const<i32>(3));
// DEFAULT-NEXT:         let %130: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%129)));
// DEFAULT-NEXT:         let %131: u4096b [synthetic] = add<u4096b, overflow=wrap>(read<u4096b>(%130), widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i231b>(%29)))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%129)), read<u4096b>(%131));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @f6(%31 p: ptr<u4094b>, %32 r: i32, %33 s: i115b, %34 t: i128b, %35 u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %132: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%31), const<i32>(0));
// DEFAULT-NEXT:         let %133: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%132)));
// DEFAULT-NEXT:         let %134: u4094b [synthetic] = sub<u4094b, overflow=wrap>(read<u4094b>(%133), widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i32>(%32)))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%132)), read<u4094b>(%134));
// DEFAULT-NEXT:         let %135: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%31), const<i32>(1));
// DEFAULT-NEXT:         let %136: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%135)));
// DEFAULT-NEXT:         let %137: u4094b [synthetic] = sub<u4094b, overflow=wrap>(read<u4094b>(%136), widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i115b>(%33)))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%135)), read<u4094b>(%137));
// DEFAULT-NEXT:         let %138: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%31), const<i32>(2));
// DEFAULT-NEXT:         let %139: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%138)));
// DEFAULT-NEXT:         let %140: u4094b [synthetic] = sub<u4094b, overflow=wrap>(read<u4094b>(%139), widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128b>(%34)))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%138)), read<u4094b>(%140));
// DEFAULT-NEXT:         let %141: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%31), const<i32>(3));
// DEFAULT-NEXT:         let %142: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%141)));
// DEFAULT-NEXT:         let %143: u4094b [synthetic] = sub<u4094b, overflow=wrap>(read<u4094b>(%142), widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i231b>(%35)))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%141)), read<u4094b>(%143));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @f7(%37 p: ptr<u4096b>, %38 r: i32, %39 s: i115b, %40 t: i128b, %41 u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %144: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%37), const<i32>(0));
// DEFAULT-NEXT:         let %145: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%144)));
// DEFAULT-NEXT:         let %146: u4096b [synthetic] = add<u4096b, overflow=wrap>(read<u4096b>(%145), widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i32>(%38)))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%144)), read<u4096b>(%146));
// DEFAULT-NEXT:         let %147: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%37), const<i32>(1));
// DEFAULT-NEXT:         let %148: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%147)));
// DEFAULT-NEXT:         let %149: u4096b [synthetic] = add<u4096b, overflow=wrap>(read<u4096b>(%148), widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i115b>(%39)))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%147)), read<u4096b>(%149));
// DEFAULT-NEXT:         let %150: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%37), const<i32>(2));
// DEFAULT-NEXT:         let %151: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%150)));
// DEFAULT-NEXT:         let %152: u4096b [synthetic] = add<u4096b, overflow=wrap>(read<u4096b>(%151), widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128b>(%40)))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%150)), read<u4096b>(%152));
// DEFAULT-NEXT:         let %153: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%37), const<i32>(3));
// DEFAULT-NEXT:         let %154: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%153)));
// DEFAULT-NEXT:         let %155: u4096b [synthetic] = add<u4096b, overflow=wrap>(read<u4096b>(%154), widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i231b>(%41)))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%153)), read<u4096b>(%155));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @f8(%43 p: ptr<u4094b>, %44 r: i32, %45 s: i115b, %46 t: i128b, %47 u: i231b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %156: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%43), const<i32>(0));
// DEFAULT-NEXT:         let %157: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%156)));
// DEFAULT-NEXT:         let %158: u4094b [synthetic] = sub<u4094b, overflow=wrap>(read<u4094b>(%157), widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i32>(%44)))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%156)), read<u4094b>(%158));
// DEFAULT-NEXT:         let %159: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%43), const<i32>(1));
// DEFAULT-NEXT:         let %160: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%159)));
// DEFAULT-NEXT:         let %161: u4094b [synthetic] = sub<u4094b, overflow=wrap>(read<u4094b>(%160), widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i115b>(%45)))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%159)), read<u4094b>(%161));
// DEFAULT-NEXT:         let %162: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%43), const<i32>(2));
// DEFAULT-NEXT:         let %163: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%162)));
// DEFAULT-NEXT:         let %164: u4094b [synthetic] = sub<u4094b, overflow=wrap>(read<u4094b>(%163), widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128b>(%46)))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%162)), read<u4094b>(%164));
// DEFAULT-NEXT:         let %165: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%43), const<i32>(3));
// DEFAULT-NEXT:         let %166: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%165)));
// DEFAULT-NEXT:         let %167: u4094b [synthetic] = sub<u4094b, overflow=wrap>(read<u4094b>(%166), widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i231b>(%47)))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%165)), read<u4094b>(%167));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @f9(%49 p: ptr<i4096b>, %50 r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %168: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%49), const<i32>(0));
// DEFAULT-NEXT:         let %169: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%168)));
// DEFAULT-NEXT:         let %170: i4096b [synthetic] = add<i4096b, overflow=ub>(read<i4096b>(%169), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128>(%50))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%168)), read<i4096b>(%170));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @f10(%52 p: ptr<i4094b>, %53 r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %171: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%52), const<i32>(0));
// DEFAULT-NEXT:         let %172: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%171)));
// DEFAULT-NEXT:         let %173: i4094b [synthetic] = sub<i4094b, overflow=ub>(read<i4094b>(%172), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128>(%53))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%171)), read<i4094b>(%173));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @f11(%55 p: ptr<i4096b>, %56 r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %174: ptr<i4096b> [synthetic] = ptr_offset<ptr<i4096b>, subtract=false, element=i4096b, overflow=ub>(read<ptr<i4096b>>(%55), const<i32>(0));
// DEFAULT-NEXT:         let %175: i4096b [synthetic] = read<i4096b>(deref(read<ptr<i4096b>>(%174)));
// DEFAULT-NEXT:         let %176: i4096b [synthetic] = add<i4096b, overflow=ub>(read<i4096b>(%175), reinterpret<i4096b, reason=usual_arith, fits=unknown>(widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128>(%56))))));
// DEFAULT-NEXT:         write<i4096b>(deref(read<ptr<i4096b>>(%174)), read<i4096b>(%176));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @f12(%58 p: ptr<i4094b>, %59 r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %177: ptr<i4094b> [synthetic] = ptr_offset<ptr<i4094b>, subtract=false, element=i4094b, overflow=ub>(read<ptr<i4094b>>(%58), const<i32>(0));
// DEFAULT-NEXT:         let %178: i4094b [synthetic] = read<i4094b>(deref(read<ptr<i4094b>>(%177)));
// DEFAULT-NEXT:         let %179: i4094b [synthetic] = sub<i4094b, overflow=ub>(read<i4094b>(%178), reinterpret<i4094b, reason=usual_arith, fits=unknown>(widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128>(%59))))));
// DEFAULT-NEXT:         write<i4094b>(deref(read<ptr<i4094b>>(%177)), read<i4094b>(%179));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @f13(%61 p: ptr<u4096b>, %62 r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %180: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%61), const<i32>(0));
// DEFAULT-NEXT:         let %181: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%180)));
// DEFAULT-NEXT:         let %182: u4096b [synthetic] = add<u4096b, overflow=wrap>(read<u4096b>(%181), widen<u4096b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128>(%62)))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%180)), read<u4096b>(%182));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @f14(%64 p: ptr<u4094b>, %65 r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %183: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%64), const<i32>(0));
// DEFAULT-NEXT:         let %184: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%183)));
// DEFAULT-NEXT:         let %185: u4094b [synthetic] = sub<u4094b, overflow=wrap>(read<u4094b>(%184), widen<u4094b, reason=usual_arith>(reinterpret<u2048b, reason=explicit, fits=unknown>(widen<i2048b, reason=explicit>(read<i128>(%65)))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%183)), read<u4094b>(%185));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @f15(%67 p: ptr<u4096b>, %68 r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %186: ptr<u4096b> [synthetic] = ptr_offset<ptr<u4096b>, subtract=false, element=u4096b, overflow=ub>(read<ptr<u4096b>>(%67), const<i32>(0));
// DEFAULT-NEXT:         let %187: u4096b [synthetic] = read<u4096b>(deref(read<ptr<u4096b>>(%186)));
// DEFAULT-NEXT:         let %188: u4096b [synthetic] = add<u4096b, overflow=wrap>(read<u4096b>(%187), widen<u4096b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128>(%68)))));
// DEFAULT-NEXT:         write<u4096b>(deref(read<ptr<u4096b>>(%186)), read<u4096b>(%188));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @f16(%70 p: ptr<u4094b>, %71 r: i128) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %189: ptr<u4094b> [synthetic] = ptr_offset<ptr<u4094b>, subtract=false, element=u4094b, overflow=ub>(read<ptr<u4094b>>(%70), const<i32>(0));
// DEFAULT-NEXT:         let %190: u4094b [synthetic] = read<u4094b>(deref(read<ptr<u4094b>>(%189)));
// DEFAULT-NEXT:         let %191: u4094b [synthetic] = sub<u4094b, overflow=wrap>(read<u4094b>(%190), widen<u4094b, reason=usual_arith>(reinterpret<u2110b, reason=explicit, fits=unknown>(widen<i2110b, reason=explicit>(read<i128>(%71)))));
// DEFAULT-NEXT:         write<u4094b>(deref(read<ptr<u4094b>>(%189)), read<u4094b>(%191));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
