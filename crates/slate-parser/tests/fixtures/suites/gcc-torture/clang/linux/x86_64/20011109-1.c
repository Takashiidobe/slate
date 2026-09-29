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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i16, 4>;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: ptr<@type[[TYPE0]]>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 e: u32;
// DEFAULT-NEXT:         field3 f: f64;
// DEFAULT-NEXT:         field4 g: f64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 4, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = @type[[TYPE2]];
// DEFAULT-NEXT:     global %[[VALUE_foo:[0-9]+]] foo: ptr<fn(ptr<@type[[TYPE1]]>, i32, i32, i32, i32) -> ptr<@type[[TYPE0]]>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bar:[0-9]+]] bar: ptr<fn(ptr<const @type[[TYPE1]]>) -> ptr<u16>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE1]]>, %[[VALUE1:[0-9]+]] <unnamed>: i32 [const]) -> u16 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_die:[0-9]+]] @die() -> ptr<@type[[TYPE1]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: array<i8, 8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: array<i8, 2053> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: array<i8, 2053> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: @type[[TYPE2]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: ptr<@type[[TYPE1]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: u16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: ptr<u16> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: ptr<@type[[TYPE0]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE1]]>>(%[[VALUE_o]], null<ptr<@type[[TYPE1]]>>);
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_x]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_x]]), read<u32>(field2(%[[VALUE_n]])))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_x]], read<u32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_l]], const<f64>(1.0));
// DEFAULT-NEXT:                     if le<f64, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field4(%[[VALUE_n]])), read<f64>(field3(%[[VALUE_n]]))), const<f64>(1.0))
// DEFAULT-NEXT:                         write<f64>(%[[VALUE_l]], div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u32>(field2(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]))))), const<i32>(1))), sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field4(%[[VALUE_n]])), read<f64>(field3(%[[VALUE_n]])))));
// DEFAULT-NEXT:                     write<ptr<u8>>(%[[VALUE_v]], read<ptr<u8>>(%[[VALUE_w]]));
// DEFAULT-NEXT:                     for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_r]], reinterpret<i32, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(read<u32>(field1(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:                         condition: ge<i32>(read<i32>(%[[VALUE_r]]), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r]]);
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_r]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE0]]>>(%[[VALUE_u]], call<ptr<@type[[TYPE0]]>, signature=fn(ptr<@type[[TYPE1]]>, i32, i32, i32, i32) -> ptr<@type[[TYPE0]]>>(read<ptr<fn(ptr<@type[[TYPE1]]>, i32, i32, i32, i32) -> ptr<@type[[TYPE0]]>>>(%[[VALUE_foo]]), read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]), const<i32>(0), read<i32>(%[[VALUE_r]]), reinterpret<i32, reason=arg, fits=unknown>(read<u32>(field0(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]))))), const<i32>(1)));
// DEFAULT-NEXT:                                 call<ptr<@type[[TYPE0]]>, signature=fn(ptr<@type[[TYPE1]]>, i32, i32, i32, i32) -> ptr<@type[[TYPE0]]>>(read<ptr<fn(ptr<@type[[TYPE1]]>, i32, i32, i32, i32) -> ptr<@type[[TYPE0]]>>>(%[[VALUE_foo]]), read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]), const<i32>(0), read<i32>(%[[VALUE_r]]), reinterpret<i32, reason=arg, fits=unknown>(read<u32>(field0(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]))))), const<i32>(1));
// DEFAULT-NEXT:                                 if not<bool>(ne<ptr<@type[[TYPE0]]>>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_u]]), null<ptr<@type[[TYPE0]]>>))
// DEFAULT-NEXT:                                     break %[[VALUE5]];
// DEFAULT-NEXT:                                 write<ptr<u16>>(%[[VALUE_t]], call<ptr<u16>, signature=fn(ptr<const @type[[TYPE1]]>) -> ptr<u16>>(read<ptr<fn(ptr<const @type[[TYPE1]]>) -> ptr<u16>>>(%[[VALUE_bar]]), pointer_cast<ptr<const @type[[TYPE1]]>, reason=arg>(read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]))));
// DEFAULT-NEXT:                                 call<ptr<u16>, signature=fn(ptr<const @type[[TYPE1]]>) -> ptr<u16>>(read<ptr<fn(ptr<const @type[[TYPE1]]>) -> ptr<u16>>>(%[[VALUE_bar]]), pointer_cast<ptr<const @type[[TYPE1]]>, reason=arg>(read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]])));
// DEFAULT-NEXT:                                 for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_q]], const<i32>(0));
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%[[VALUE_q]]), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(field0(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]))))))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_q]]);
// DEFAULT-NEXT:                                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_q]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_h]]), const<i32>(0))), reinterpret<i8, reason=assign, fits=unknown>(read<u8>(deref(read<ptr<u8>>(%[[VALUE_v]])))));
// DEFAULT-NEXT:                                             let %[[VALUE11:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_v]]);
// DEFAULT-NEXT:                                             let %[[VALUE12:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%[[VALUE_v]], read<ptr<u8>>(%[[VALUE12]]));
// DEFAULT-NEXT:                                             write<i64>(%[[VALUE_s]], reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(read<u8>(deref(read<ptr<u8>>(%[[VALUE11]]))))));
// DEFAULT-NEXT:                                             write<f64>(%[[VALUE_k]], int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(%[[VALUE_s]])));
// DEFAULT-NEXT:                                             write<f64>(%[[VALUE_m]], mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_l]]), read<f64>(%[[VALUE_k]])));
// DEFAULT-NEXT:                                             write<u16>(%[[VALUE_p]], float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(conditional<f64>(lt<f64,
// DEFAULT-SAME: exceptions=ignore>(read<f64>(%[[VALUE_m]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))),
// DEFAULT-SAME: int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), conditional<f64>(gt<f64,
// DEFAULT-SAME: exceptions=ignore>(read<f64>(%[[VALUE_m]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32,
// DEFAULT-SAME: overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u32>(field2(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]))))),
// DEFAULT-SAME: const<i32>(1)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1),
// DEFAULT-SAME: read<u32>(field2(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]))))), const<i32>(1))), add<f64, rounding=nearest_even, exceptions=ignore,
// DEFAULT-SAME: contract=on>(read<f64>(%[[VALUE_m]]), const<f64>(0.5))))));
// DEFAULT-NEXT:                                             write<u16>(%[[VALUE_p]], call<u16, signature=fn(ptr<@type[[TYPE1]]>, i32) -> u16>(%[[VALUE_baz]], read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]), reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(%[[VALUE_p]])))));
// DEFAULT-NEXT:                                             call<u16, signature=fn(ptr<@type[[TYPE1]]>, i32) -> u16>(%[[VALUE_baz]], read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]), reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(%[[VALUE_p]]))));
// DEFAULT-NEXT:                                             write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_t]]), read<i32>(%[[VALUE_q]]))), read<u16>(%[[VALUE_p]]));
// DEFAULT-NEXT:                                             let %[[VALUE13:[0-9]+]]: ptr<@type[[TYPE0]]> [synthetic] = read<ptr<@type[[TYPE0]]>>(%[[VALUE_u]]);
// DEFAULT-NEXT:                                             let %[[VALUE14:[0-9]+]]: ptr<@type[[TYPE0]]> [synthetic] = ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(read<ptr<@type[[TYPE0]]>>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<@type[[TYPE0]]>>(%[[VALUE_u]], read<ptr<@type[[TYPE0]]>>(%[[VALUE14]]));
// DEFAULT-NEXT:                                             write<@type[[TYPE0]]>(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE13]])), copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(read<ptr<@type[[TYPE0]]>>(field3(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_p]]))))))));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE1]]>>(%[[VALUE_o]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
