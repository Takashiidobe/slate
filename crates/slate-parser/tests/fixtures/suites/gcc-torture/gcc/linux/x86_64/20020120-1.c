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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: ptr<void>;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 12, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 e: ptr<@type[[TYPE_A]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_A]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE_A]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_inl:[0-9]+]] @inl(%[[VALUE_v:[0-9]+]] v: u32, %[[VALUE_w:[0-9]+]] w: u8, %[[VALUE_x:[0-9]+]] x: u8, %[[VALUE_y:[0-9]+]] y: u8, %[[VALUE_z:[0-9]+]] z: u8) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE2:[0-9]+]] read<u32>(%[[VALUE_v]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE2]] const<u32>(2):
// DEFAULT-NEXT:                     return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_w]]))), const<i32>(248)), const<i32>(8)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x]]))), const<i32>(252)), const<i32>(3))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_y]]))), const<i32>(248)), const<i32>(3))));
// DEFAULT-NEXT:                 case %[[VALUE2]] const<u32>(4):
// DEFAULT-NEXT:                     return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_z]]))), const<i32>(24)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_w]]))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x]]))), const<i32>(8))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_y]])))));
// DEFAULT-NEXT:                 default %[[VALUE2]]:
// DEFAULT-NEXT:                     return reinterpret<u32, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: ptr<@type[[TYPE_B]]>, %[[VALUE_y_2:[0-9]+]] y: i32, %[[VALUE_z_2:[0-9]+]] z: ptr<const f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_A]]> [storage=automatic] = read<ptr<@type[[TYPE_A]]>>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x_2]]))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_y_2]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(field0(deref(read<ptr<@type[[TYPE_A]]>>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x_2]]))))))), null<ptr<void>>)
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_bar]], read<ptr<@type[[TYPE_A]]>>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x_2]])))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_c:[0-9]+]] c: array<u8, 4> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b:[0-9]+]] b: u32 [storage=automatic];
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(0))), float_to_int<u8, reason=assign, out_of_range=ub, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<const f32>, subtract=false, element=f32, overflow=ub>(read<ptr<const f32>>(%[[VALUE_z_2]]), const<i32>(0))))));
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(1))), float_to_int<u8, reason=assign, out_of_range=ub, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<const f32>, subtract=false, element=f32, overflow=ub>(read<ptr<const f32>>(%[[VALUE_z_2]]), const<i32>(1))))));
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(2))), float_to_int<u8, reason=assign, out_of_range=ub, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<const f32>, subtract=false, element=f32, overflow=ub>(read<ptr<const f32>>(%[[VALUE_z_2]]), const<i32>(2))))));
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(3))), float_to_int<u8, reason=assign, out_of_range=ub, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<const f32>, subtract=false, element=f32, overflow=ub>(read<ptr<const f32>>(%[[VALUE_z_2]]), const<i32>(3))))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_b]], call<u32, signature=fn(u32, u8, u8, u8, u8) -> u32>(%[[VALUE_inl]], read<u32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]])))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(0)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(1)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(2)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(3))))));
// DEFAULT-NEXT:                 call<u32, signature=fn(u32, u8, u8, u8, u8) -> u32>(%[[VALUE_inl]], read<u32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]])))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(0)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(1)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(2)))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_c]]), const<i32>(3)))));
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]])))), null<ptr<void>>)
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_bar]], read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]]));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_baz]], read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]]));
// DEFAULT-NEXT:                 write<u32>(field2(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]]))), read<u32>(%[[VALUE_b]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
