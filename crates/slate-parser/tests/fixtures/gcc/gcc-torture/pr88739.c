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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32 : 30;
// DEFAULT-NEXT:         field4 e: u32 : 2;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12, 15], bit_offsets=[None, None, None, Some(96), Some(126)], bit_units=[(12, 4)], field_units=[None, None, None, Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 U = union {
// DEFAULT-NEXT:         field0 f: @type0;
// DEFAULT-NEXT:         field1 g: array<u32, 4>;
// DEFAULT-NEXT:         field2 h: array<u16, 8>;
// DEFAULT-NEXT:         field3 i: array<u8, 16>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 0, 0, 0]];
// DEFAULT-NEXT:     global %2 v: volatile @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = aggregate<@type0, zero_fill=true>(field3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(16521)))) [linkage=external];
// DEFAULT-NEXT:     global %5 i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %11 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @bar(%4 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%5, read<i32>(%13));
// DEFAULT-NEXT:         switch %10 read<i32>(%12)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %10 const<i32>(0):
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%4), reinterpret<i32, reason=promotion, fits=unknown>(read<u32, volatile>(bitfield3<unit=0, bytes=12..16, bits=0..30>(field0(%2)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:                 break %10;
// DEFAULT-NEXT:                 case %10 const<i32>(1):
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%4), reinterpret<i32, reason=promotion, fits=unknown>(read<u32, volatile>(bitfield4<unit=0, bytes=12..16, bits=30..32>(field0(%2)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:                 break %10;
// DEFAULT-NEXT:                 case %10 const<i32>(2):
// DEFAULT-NEXT:                     if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%4)), read<u32, volatile>(deref(ptr_offset<ptr<volatile u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<volatile u32>, length=Some(4)>(field1(%2)), const<i32>(3)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:                 break %10;
// DEFAULT-NEXT:                 case %10 const<i32>(3):
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%4), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile>(deref(ptr_offset<ptr<volatile u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<volatile u16>, length=Some(8)>(field2(%2)), const<i32>(6)))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:                 break %10;
// DEFAULT-NEXT:                 case %10 const<i32>(4):
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%4), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile>(deref(ptr_offset<ptr<volatile u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<volatile u16>, length=Some(8)>(field2(%2)), const<i32>(7)))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:                 break %10;
// DEFAULT-NEXT:                 default %10:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:                 break %10;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo(%7 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 u: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=12..16, bits=0..30>(field0(%8)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%7), const<i32>(2)));
// DEFAULT-NEXT:         write<u32>(bitfield4<unit=0, bytes=12..16, bits=30..32>(field0(%8)), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=12..16, bits=0..30>(field0(%8)))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield4<unit=0, bytes=12..16, bits=30..32>(field0(%8)))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field1(%8)), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(8)>(field2(%8)), const<i32>(6)))))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(8)>(field2(%8)), const<i32>(7)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%6, reinterpret<u32, reason=arg, fits=always>(const<i32>(66084)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
