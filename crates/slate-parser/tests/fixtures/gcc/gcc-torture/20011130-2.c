// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase caused infinite loop in life info computation
   after if conversion on IA-64.  Conditional register dead for
   pseudo holding sign-extended k was improperly computed,
   resulting in this pseudo being live at start of bb if it was
   dead at the end and vice versa; as it was a bb which had edge
   to itself, this resulted in alternative propagating this basic
   block forever.  */

typedef struct {
  unsigned char a;
  unsigned char b;
} S0;

typedef struct {
  S0 *c;
  int d;
  unsigned int e;
  unsigned char *f[3];
  void *g;
} S1;

int bar (int, void *);

int foo (S1 *x, float y)
{
  S0 *h;
  int i, j, k, l, m;
  float n, o, p;
  unsigned char *q, *r[3];

  h = x->c;
  m = h->a;
  l = h->b;
  n = y;
  o = 0.0;
  if (x->d == 8)
    for (j = 0; j < x->e; j++)
      for (k = 0; k < 3; k++)
	{
	  n = y;
	  o = 0.0;
	  if (m)
	    q = x->f[k] + x->e - 1 - j;
	  else
	    q = x->f[k] + j;
	  p = (*q - o) * y / (n - o);
	  p = 0.0 > p ? 0.0 : p;
	  p = y < p ? y : p;
	  if (l)
	    p = r[k][(int) p];
	  bar (p, x->g);
	}
  return 1;
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
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: u8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type1 S0 = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type0>;
// DEFAULT-NEXT:         field1 d: i32;
// DEFAULT-NEXT:         field2 e: u32;
// DEFAULT-NEXT:         field3 f: array<ptr<u8>, 3>;
// DEFAULT-NEXT:         field4 g: ptr<void>;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 8, 12, 16, 40]];
// DEFAULT-NEXT:     type @type3 S1 = @type2;
// DEFAULT-NEXT:     fn %4 @bar(%19 <unnamed>: i32, %20 <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo(%6 x: ptr<@type2>, %7 y: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 h: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 l: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 m: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 n: f32 [storage=automatic];
// DEFAULT-NEXT:         let %15 o: f32 [storage=automatic];
// DEFAULT-NEXT:         let %16 p: f32 [storage=automatic];
// DEFAULT-NEXT:         let %17 q: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %18 r: array<ptr<u8>, 3> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type0>>(%8, read<ptr<@type0>>(field0(deref(read<ptr<@type2>>(%6)))));
// DEFAULT-NEXT:         write<i32>(%13, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(field0(deref(read<ptr<@type0>>(%8)))))));
// DEFAULT-NEXT:         write<i32>(%12, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(field1(deref(read<ptr<@type0>>(%8)))))));
// DEFAULT-NEXT:         write<f32>(%14, read<f32>(%7));
// DEFAULT-NEXT:         write<f32>(%15, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field1(deref(read<ptr<@type2>>(%6)))), const<i32>(8))
// DEFAULT-NEXT:             for %21
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%10)), read<u32>(field2(deref(read<ptr<@type2>>(%6)))))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %23: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                     let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%10, read<i32>(%24));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     for %22
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%11), const<i32>(3))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %25: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                             let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%11, read<i32>(%26));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<f32>(%14, read<f32>(%7));
// DEFAULT-NEXT:                                 write<f32>(%15, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%13), const<i32>(0))
// DEFAULT-NEXT:                                     write<ptr<u8>>(%17, ptr_offset<ptr<u8>, subtract=true, element=u8, overflow=ub>(ptr_offset<ptr<u8>, subtract=true, element=u8, overflow=ub>(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(deref(ptr_offset<ptr<ptr<u8>>, subtract=false, element=ptr<u8>, overflow=ub>(array_decay<ptr<ptr<u8>>, length=Some(3)>(field3(deref(read<ptr<@type2>>(%6)))), read<i32>(%11)))), read<u32>(field2(deref(read<ptr<@type2>>(%6))))), const<i32>(1)), read<i32>(%10)));
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     write<ptr<u8>>(%17, ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(deref(ptr_offset<ptr<ptr<u8>>, subtract=false, element=ptr<u8>, overflow=ub>(array_decay<ptr<ptr<u8>>, length=Some(3)>(field3(deref(read<ptr<@type2>>(%6)))), read<i32>(%11)))), read<i32>(%10)));
// DEFAULT-NEXT:                                 write<f32>(%16, div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%17)))))), read<f32>(%15)), read<f32>(%7)), sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%14), read<f32>(%15))));
// DEFAULT-NEXT:                                 write<f32>(%16, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(conditional<f64>(gt<f64, exceptions=ignore>(const<f64>(0.0), float_widen<f64, reason=usual_arith>(read<f32>(%16))), const<f64>(0.0), float_widen<f64, reason=usual_arith>(read<f32>(%16)))));
// DEFAULT-NEXT:                                 write<f32>(%16, conditional<f32>(lt<f32, exceptions=ignore>(read<f32>(%7), read<f32>(%16)), read<f32>(%7), read<f32>(%16)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%12), const<i32>(0))
// DEFAULT-NEXT:                                     write<f32>(%16, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(deref(ptr_offset<ptr<ptr<u8>>, subtract=false, element=ptr<u8>, overflow=ub>(array_decay<ptr<ptr<u8>>, length=Some(3)>(%18), read<i32>(%11)))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(%16)))))));
// DEFAULT-NEXT:                                 call<i32, signature=fn(i32, ptr<void>) -> i32>(%4, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%16)), read<ptr<void>>(field4(deref(read<ptr<@type2>>(%6)))));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
