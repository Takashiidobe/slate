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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: u8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_S0:[0-9]+]] S0 = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE0]]>;
// DEFAULT-NEXT:         field1 d: i32;
// DEFAULT-NEXT:         field2 e: u32;
// DEFAULT-NEXT:         field3 f: array<ptr<u8>, 3>;
// DEFAULT-NEXT:         field4 g: ptr<void>;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 8, 12, 16, 40]];
// DEFAULT-NEXT:     type @type[[TYPE_S1:[0-9]+]] S1 = @type[[TYPE1]];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE1:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE1]]>, %[[VALUE_y:[0-9]+]] y: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: ptr<@type[[TYPE0]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: array<ptr<u8>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE0]]>>(%[[VALUE_h]], read<ptr<@type[[TYPE0]]>>(field0(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_x]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_m]], reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_h]])))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_l]], reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_h]])))))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_n]], read<f32>(%[[VALUE_y]]));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_o]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(0.0)));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_x]])))), const<i32>(8))
// DEFAULT-NEXT:             for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_j]])), read<u32>(field2(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_x]])))))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_k]], const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_k]]), const<i32>(3))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<f32>(%[[VALUE_n]], read<f32>(%[[VALUE_y]]));
// DEFAULT-NEXT:                                 write<f32>(%[[VALUE_o]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(0.0)));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(0))
// DEFAULT-NEXT:                                     write<ptr<u8>>(%[[VALUE_q]], ptr_offset<ptr<u8>, subtract=true, element=u8, overflow=ub>(ptr_offset<ptr<u8>, subtract=true, element=u8, overflow=ub>(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(deref(ptr_offset<ptr<ptr<u8>>, subtract=false, element=ptr<u8>, overflow=ub>(array_decay<ptr<ptr<u8>>, length=Some(3)>(field3(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_x]])))), read<i32>(%[[VALUE_k]])))), read<u32>(field2(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_x]]))))), const<i32>(1)), read<i32>(%[[VALUE_j]])));
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     write<ptr<u8>>(%[[VALUE_q]], ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(deref(ptr_offset<ptr<ptr<u8>>, subtract=false, element=ptr<u8>, overflow=ub>(array_decay<ptr<ptr<u8>>, length=Some(3)>(field3(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_x]])))), read<i32>(%[[VALUE_k]])))), read<i32>(%[[VALUE_j]])));
// DEFAULT-NEXT:                                 write<f32>(%[[VALUE_p]], div<f32, rounding=nearest_even, exceptions=observable, contract=fast>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(sub<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%[[VALUE_q]])))))), read<f32>(%[[VALUE_o]])), read<f32>(%[[VALUE_y]])), sub<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_n]]), read<f32>(%[[VALUE_o]]))));
// DEFAULT-NEXT:                                 write<f32>(%[[VALUE_p]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(conditional<f64>(gt<f64, exceptions=observable>(const<f64>(0.0), float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE_p]]))), const<f64>(0.0), float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE_p]])))));
// DEFAULT-NEXT:                                 write<f32>(%[[VALUE_p]], conditional<f32>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_y]]), read<f32>(%[[VALUE_p]])), read<f32>(%[[VALUE_y]]), read<f32>(%[[VALUE_p]])));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE_l]]), const<i32>(0))
// DEFAULT-NEXT:                                     write<f32>(%[[VALUE_p]], int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(deref(ptr_offset<ptr<ptr<u8>>, subtract=false, element=ptr<u8>, overflow=ub>(array_decay<ptr<ptr<u8>>, length=Some(3)>(%[[VALUE_r]]), read<i32>(%[[VALUE_k]])))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE_p]])))))));
// DEFAULT-NEXT:                                 call<i32, signature=fn(i32, ptr<void>) -> i32>(%[[VALUE_bar]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE_p]])), read<ptr<void>>(field4(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_x]])))));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
