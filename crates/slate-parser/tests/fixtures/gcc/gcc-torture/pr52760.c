/* PR tree-optimization/52760 */

struct T {
  unsigned short a, b, c, d;
};

__attribute__((noinline, noclone)) void foo(int x, struct T *y) {
  int i;

  for (i = 0; i < x; i++) {
    y[i].a = ((0x00ff & y[i].a >> 8) | (0xff00 & y[i].a << 8));
    y[i].b = ((0x00ff & y[i].b >> 8) | (0xff00 & y[i].b << 8));
    y[i].c = ((0x00ff & y[i].c >> 8) | (0xff00 & y[i].c << 8));
    y[i].d = ((0x00ff & y[i].d >> 8) | (0xff00 & y[i].d << 8));
  }
}

int main() {
  struct T t = {0x0001, 0x0203, 0x0405, 0x0607};
  foo(1, &t);
  if (t.a != 0x0100 || t.b != 0x0302 || t.c != 0x0504 || t.d != 0x0706)
    __builtin_abort();
  return 0;
}


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
// DEFAULT-NEXT:     type @type0 T = struct {
// DEFAULT-NEXT:         field0 a: u16;
// DEFAULT-NEXT:         field1 b: u16;
// DEFAULT-NEXT:         field2 c: u16;
// DEFAULT-NEXT:         field3 d: u16;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0, 2, 4, 6]];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: i32, %3 y: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), read<i32>(%2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%9));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u16>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4)))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(and<i32>(const<i32>(255), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4))))))), const<i32>(8))), and<i32>(const<i32>(65280), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4))))))), const<i32>(8)))))));
// DEFAULT-NEXT:                     write<u16>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4)))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(and<i32>(const<i32>(255), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4))))))), const<i32>(8))), and<i32>(const<i32>(65280), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4))))))), const<i32>(8)))))));
// DEFAULT-NEXT:                     write<u16>(field2(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4)))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(and<i32>(const<i32>(255), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field2(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4))))))), const<i32>(8))), and<i32>(const<i32>(65280), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field2(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4))))))), const<i32>(8)))))));
// DEFAULT-NEXT:                     write<u16>(field3(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4)))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(and<i32>(const<i32>(255), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4))))))), const<i32>(8))), and<i32>(const<i32>(65280), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%3), read<i32>(%4))))))), const<i32>(8)))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 t: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(1))), field1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(515))), field2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(1029))), field3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(1543))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<@type0>) -> void>(%1, const<i32>(1), addr_of<ptr<@type0>>(%6));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(%6)))), const<i32>(256)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(%6)))), const<i32>(770))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field2(%6)))), const<i32>(1284))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%6)))), const<i32>(1798)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
