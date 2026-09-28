// SLATE-FILECHECK-DEFINES DEFAULT

/* This ICEed on IA-32 with -O2 -mcpu=i386, because reload was trying
   to reload into %sil register.  */

struct A
{
  void *a;
  unsigned int b, c, d;
};

struct B
{
  struct A *e;
};

void bar (struct A *);
void baz (struct A *);

static inline unsigned int
inl (unsigned int v, unsigned char w, unsigned char x, unsigned char y,
     unsigned char z)
{
  switch (v)
    {
    case 2:
      return ((w & 0xf8) << 8) | ((x & 0xfc) << 3) | ((y & 0xf8) >> 3);
    case 4:
      return (z << 24) | (w << 16) | (x << 8) | y;
    default:
      return 0;
    }
}

void foo (struct B *x, int y, const float *z)
{
  struct A *a = x->e;

  if (y)
    {
      if (x->e->a)
       bar (x->e);
    }
  else
    {
      unsigned char c[4];
      unsigned int b;

      c[0] = z[0]; c[1] = z[1]; c[2] = z[2]; c[3] = z[3];
      b = inl (a->b, c[0], c[1], c[2], c[3] );
      if (a->a)
       bar (a);
      else
       baz (a);
      a->c = b;
   }
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a: ptr<void>;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 12, 16]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 e: ptr<@type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %2 @bar(%17 <unnamed>: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @baz(%18 <unnamed>: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @inl(%5 v: u32, %6 w: u8, %7 x: u8, %8 y: u8, %9 z: u8) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %19 read<u32>(%5)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %19 const<u32>(2):
// DEFAULT-NEXT:                     return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))), const<i32>(248)), const<i32>(8)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%7))), const<i32>(252)), const<i32>(3))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8))), const<i32>(248)), const<i32>(3))));
// DEFAULT-NEXT:                 case %19 const<u32>(4):
// DEFAULT-NEXT:                     return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%9))), const<i32>(24)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%7))), const<i32>(8))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8)))));
// DEFAULT-NEXT:                 default %19:
// DEFAULT-NEXT:                     return reinterpret<u32, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @foo(%11 x: ptr<@type1>, %12 y: i32, %13 z: ptr<const f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14 a: ptr<@type0> [storage=automatic] = read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%12), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))))))), null<ptr<void>>)
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type0>) -> void>(%2, read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %15 c: array<u8, 4> [storage=automatic];
// DEFAULT-NEXT:                 let %16 b: u32 [storage=automatic];
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(0))), float_to_int<u8, reason=assign, out_of_range=ub, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<const f32>, subtract=false, element=f32, overflow=ub>(read<ptr<const f32>>(%13), const<i32>(0))))));
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(1))), float_to_int<u8, reason=assign, out_of_range=ub, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<const f32>, subtract=false, element=f32, overflow=ub>(read<ptr<const f32>>(%13), const<i32>(1))))));
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(2))), float_to_int<u8, reason=assign, out_of_range=ub, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<const f32>, subtract=false, element=f32, overflow=ub>(read<ptr<const f32>>(%13), const<i32>(2))))));
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(3))), float_to_int<u8, reason=assign, out_of_range=ub, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<const f32>, subtract=false, element=f32, overflow=ub>(read<ptr<const f32>>(%13), const<i32>(3))))));
// DEFAULT-NEXT:                 write<u32>(%16, call<u32, signature=fn(u32, u8, u8, u8, u8) -> u32>(%4, read<u32>(field1(deref(read<ptr<@type0>>(%14)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(0)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(1)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(2)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(3))))));
// DEFAULT-NEXT:                 call<u32, signature=fn(u32, u8, u8, u8, u8) -> u32>(%4, read<u32>(field1(deref(read<ptr<@type0>>(%14)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(0)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(1)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(2)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(3)))));
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(field0(deref(read<ptr<@type0>>(%14)))), null<ptr<void>>)
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type0>) -> void>(%2, read<ptr<@type0>>(%14));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type0>) -> void>(%3, read<ptr<@type0>>(%14));
// DEFAULT-NEXT:                 write<u32>(field2(deref(read<ptr<@type0>>(%14))), read<u32>(%16));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
