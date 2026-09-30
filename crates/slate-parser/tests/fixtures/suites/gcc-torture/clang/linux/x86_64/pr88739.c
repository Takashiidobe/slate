/* PR tree-optimization/88739 */
#if __SIZEOF_SHORT__ == 2 && __SIZEOF_INT__ == 4 && __CHAR_BIT__ == 8
struct A {
  unsigned int a, b, c;
  unsigned int d : 30;
  unsigned int e : 2;
};

union U {
  struct A       f;
  unsigned int   g[4];
  unsigned short h[8];
  unsigned char  i[16];
};
volatile union U v = {.f.d = 0x4089};

__attribute__((noipa)) void bar(int x) {
  static int i;
  switch (i++) {
  case 0:
    if (x != v.f.d)
      __builtin_abort();
    break;
  case 1:
    if (x != v.f.e)
      __builtin_abort();
    break;
  case 2:
    if (x != v.g[3])
      __builtin_abort();
    break;
  case 3:
    if (x != v.h[6])
      __builtin_abort();
    break;
  case 4:
    if (x != v.h[7])
      __builtin_abort();
    break;
  default:
    __builtin_abort();
    break;
  }
}

void foo(unsigned int x) {
  union U u;
  u.f.d = x >> 2;
  u.f.e = 0;
  bar(u.f.d);
  bar(u.f.e);
  bar(u.g[3]);
  bar(u.h[6]);
  bar(u.h[7]);
}

int main() {
  foo(0x10224);
  return 0;
}
#else
int main() { return 0; }
#endif


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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32 : 30;
// DEFAULT-NEXT:         field4 e: u32 : 2;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12, 15], bit_offsets=[None, None, None, Some(96), Some(126)], bit_units=[(12, 4)], field_units=[None, None, None, Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 f: @type[[TYPE_A]];
// DEFAULT-NEXT:         field1 g: array<u32, 4>;
// DEFAULT-NEXT:         field2 h: array<u16, 8>;
// DEFAULT-NEXT:         field3 i: array<u8, 16>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 0, 0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: volatile @type[[TYPE_U]] [storage=static] = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = aggregate<@type[[TYPE_A]], zero_fill=true>(field3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16521)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         switch %[[VALUE2:[0-9]+]] read<i32>(%[[VALUE0]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i32>(0):
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_x]]), reinterpret<i32, reason=promotion, fits=unknown>(read<u32, volatile>(bitfield3<unit=0, bytes=12..16, bits=0..30>(field0(%[[VALUE_v]])))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE2]];
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i32>(1):
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_x]]), reinterpret<i32, reason=promotion, fits=unknown>(read<u32, volatile>(bitfield4<unit=0, bytes=12..16, bits=30..32>(field0(%[[VALUE_v]])))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE2]];
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i32>(2):
// DEFAULT-NEXT:                     if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_x]])), read<u32, volatile>(deref(ptr_offset<ptr<volatile u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<volatile u32>, length=Some(4)>(field1(%[[VALUE_v]])), const<i32>(3)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE2]];
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i32>(3):
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_x]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile>(deref(ptr_offset<ptr<volatile u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<volatile u16>, length=Some(8)>(field2(%[[VALUE_v]])), const<i32>(6)))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE2]];
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i32>(4):
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_x]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile>(deref(ptr_offset<ptr<volatile u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<volatile u16>, length=Some(8)>(field2(%[[VALUE_v]])), const<i32>(7)))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE2]];
// DEFAULT-NEXT:                 default %[[VALUE2]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE2]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=12..16, bits=0..30>(field0(%[[VALUE_u]])), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_x_2]]), const<i32>(2)));
// DEFAULT-NEXT:         write<u32>(bitfield4<unit=0, bytes=12..16, bits=30..32>(field0(%[[VALUE_u]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=12..16, bits=0..30>(field0(%[[VALUE_u]])))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield4<unit=0, bytes=12..16, bits=30..32>(field0(%[[VALUE_u]])))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], reinterpret<i32, reason=arg, fits=unknown>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field1(%[[VALUE_u]])), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(8)>(field2(%[[VALUE_u]])), const<i32>(6)))))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(8)>(field2(%[[VALUE_u]])), const<i32>(7)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(66084)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
