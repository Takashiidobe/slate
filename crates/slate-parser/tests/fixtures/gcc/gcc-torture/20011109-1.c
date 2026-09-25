// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-require-effective-target indirect_calls } */

typedef struct { short x[4]; } S;
typedef struct { unsigned int a, b, c; S *d; } T;

S *(*foo) (T *, int, int, int, int);
unsigned short *(*bar)(const T *);
unsigned short baz(T *,const int);

T *die (void)
{
  typedef struct { unsigned int a, b, e; double f, g; } U;

  char h[8], i[2053], j[2053];
  double k, l, m;
  U n;
  T *o;
  unsigned short p;
  int q, r;
  long s;
  unsigned short *t;
  S *u;
  unsigned char *v, *w;
  unsigned int x;

  o = 0;
  for (x = 0; x < n.e; x++)
    {
      l = 1.0;
      if (n.g - n.f <= 1.0)
	l = ((1 << o->c) - 1) / (n.g - n.f);
      v = w;
      for (r = o->b - 1; r >= 0; r--)
	{
	  u = foo (o, 0, r, o->a, 1);
	  if (!u)
	    break;
	  t = bar (o);
	  for (q = 0; q < (int) o->a; q++)
	    {
	      h[0] = *v;
	      s = *v++;
	      k = (double) s;
	      m = l*k;
	      p = m < 0 ? 0 : m > (1 << o->c) - 1 ? (1 << o->c) - 1 : m + 0.5;
	      p = baz (o,p);
	      t[q] = p;
	      *u++ = o->d[p];
	    }
	}
    }
  return o;
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 x: array<i16, 4>;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type1 S = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: ptr<@type0>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 8, 16]];
// DEFAULT-NEXT:     type @type3 T = @type2;
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 e: u32;
// DEFAULT-NEXT:         field3 f: f64;
// DEFAULT-NEXT:         field4 g: f64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 4, 8, 16, 24]];
// DEFAULT-NEXT:     type @type5 U = @type4;
// DEFAULT-NEXT:     global %4 foo: ptr<fn(ptr<@type2>, i32, i32, i32, i32) -> ptr<@type0>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 bar: ptr<fn(ptr<const @type2>) -> ptr<u16>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @baz(%27 <unnamed>: ptr<@type2>, %28 <unnamed>: i32 [const]) -> u16 [linkage=external];
// DEFAULT-NEXT:     fn %7 @die() -> ptr<@type2> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 h: array<i8, 8> [storage=automatic];
// DEFAULT-NEXT:         let %11 i: array<i8, 2053> [storage=automatic];
// DEFAULT-NEXT:         let %12 j: array<i8, 2053> [storage=automatic];
// DEFAULT-NEXT:         let %13 k: f64 [storage=automatic];
// DEFAULT-NEXT:         let %14 l: f64 [storage=automatic];
// DEFAULT-NEXT:         let %15 m: f64 [storage=automatic];
// DEFAULT-NEXT:         let %16 n: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %17 o: ptr<@type2> [storage=automatic];
// DEFAULT-NEXT:         let %18 p: u16 [storage=automatic];
// DEFAULT-NEXT:         let %19 q: i32 [storage=automatic];
// DEFAULT-NEXT:         let %20 r: i32 [storage=automatic];
// DEFAULT-NEXT:         let %21 s: i64 [storage=automatic];
// DEFAULT-NEXT:         let %22 t: ptr<u16> [storage=automatic];
// DEFAULT-NEXT:         let %23 u: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %24 v: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %25 w: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %26 x: u32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type2>>(%17, null<ptr<@type2>>);
// DEFAULT-NEXT:         for %29
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%26, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%26), read<u32>(field2(%16)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %32: u32 [synthetic] = read<u32>(%26);
// DEFAULT-NEXT:                 let %33: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%32), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%26, read<u32>(%33));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(%14, const<f64>(1.0));
// DEFAULT-NEXT:                     if le<f64, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(field4(%16)), read<f64>(field3(%16))), const<f64>(1.0))
// DEFAULT-NEXT:                         write<f64>(%14, div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u32>(field2(deref(read<ptr<@type2>>(%17))))), const<i32>(1))), sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(field4(%16)), read<f64>(field3(%16)))));
// DEFAULT-NEXT:                     write<ptr<u8>>(%24, read<ptr<u8>>(%25));
// DEFAULT-NEXT:                     for %30
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%20, reinterpret<i32, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(read<u32>(field1(deref(read<ptr<@type2>>(%17)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:                         condition: ge<i32>(read<i32>(%20), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %34: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                             let %35: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%20, read<i32>(%35));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<ptr<@type0>>(%23, call<ptr<@type0>, signature=fn(ptr<@type2>, i32, i32, i32, i32) -> ptr<@type0>>(read<ptr<fn(ptr<@type2>, i32, i32, i32, i32) -> ptr<@type0>>>(%4), read<ptr<@type2>>(%17), const<i32>(0), read<i32>(%20), reinterpret<i32, reason=arg, fits=unknown>(read<u32>(field0(deref(read<ptr<@type2>>(%17))))), const<i32>(1)));
// DEFAULT-NEXT:                                 call<ptr<@type0>, signature=fn(ptr<@type2>, i32, i32, i32, i32) -> ptr<@type0>>(read<ptr<fn(ptr<@type2>, i32, i32, i32, i32) -> ptr<@type0>>>(%4), read<ptr<@type2>>(%17), const<i32>(0), read<i32>(%20), reinterpret<i32, reason=arg, fits=unknown>(read<u32>(field0(deref(read<ptr<@type2>>(%17))))), const<i32>(1));
// DEFAULT-NEXT:                                 if not<bool>(ne<ptr<@type0>>(read<ptr<@type0>>(%23), null<ptr<@type0>>))
// DEFAULT-NEXT:                                     break %30;
// DEFAULT-NEXT:                                 write<ptr<u16>>(%22, call<ptr<u16>, signature=fn(ptr<const @type2>) -> ptr<u16>>(read<ptr<fn(ptr<const @type2>) -> ptr<u16>>>(%5), pointer_cast<ptr<const @type2>, reason=arg>(read<ptr<@type2>>(%17))));
// DEFAULT-NEXT:                                 call<ptr<u16>, signature=fn(ptr<const @type2>) -> ptr<u16>>(read<ptr<fn(ptr<const @type2>) -> ptr<u16>>>(%5), pointer_cast<ptr<const @type2>, reason=arg>(read<ptr<@type2>>(%17)));
// DEFAULT-NEXT:                                 for %31
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%19), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(field0(deref(read<ptr<@type2>>(%17))))))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %36: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                                         let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%19, read<i32>(%37));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%10), const<i32>(0))), reinterpret<i8, reason=assign, fits=unknown>(read<u8>(deref(read<ptr<u8>>(%24)))));
// DEFAULT-NEXT:                                             let %38: ptr<u8> [synthetic] = read<ptr<u8>>(%24);
// DEFAULT-NEXT:                                             let %39: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%38), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%24, read<ptr<u8>>(%39));
// DEFAULT-NEXT:                                             write<i64>(%21, reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(read<u8>(deref(read<ptr<u8>>(%38))))));
// DEFAULT-NEXT:                                             write<f64>(%13, int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(%21)));
// DEFAULT-NEXT:                                             write<f64>(%15, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%14), read<f64>(%13)));
// DEFAULT-NEXT:                                             write<u16>(%18, float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(conditional<f64>(lt<f64, exceptions=ignore>(read<f64>(%15), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), conditional<f64>(gt<f64, exceptions=ignore>(read<f64>(%15), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u32>(field2(deref(read<ptr<@type2>>(%17))))), const<i32>(1)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u32>(field2(deref(read<ptr<@type2>>(%17))))), const<i32>(1))), add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%15), const<f64>(0.5))))));
// DEFAULT-NEXT:                                             write<u16>(%18, call<u16, signature=fn(ptr<@type2>, i32) -> u16>(%6, read<ptr<@type2>>(%17), reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(%18)))));
// DEFAULT-NEXT:                                             call<u16, signature=fn(ptr<@type2>, i32) -> u16>(%6, read<ptr<@type2>>(%17), reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(%18))));
// DEFAULT-NEXT:                                             write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%22), read<i32>(%19))), read<u16>(%18));
// DEFAULT-NEXT:                                             let %40: ptr<@type0> [synthetic] = read<ptr<@type0>>(%23);
// DEFAULT-NEXT:                                             let %41: ptr<@type0> [synthetic] = ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%40), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<@type0>>(%23, read<ptr<@type0>>(%41));
// DEFAULT-NEXT:                                             write<@type0>(deref(read<ptr<@type0>>(%40)), copy<@type0, reason=assign>(read<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(field3(deref(read<ptr<@type2>>(%17)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%18))))))));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type2>>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
