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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 a: u16;
// DEFAULT-NEXT:         field1 b: u16;
// DEFAULT-NEXT:         field2 c: u16;
// DEFAULT-NEXT:         field3 d: u16;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0, 2, 4, 6]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: ptr<@type[[TYPE_T]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_x]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u16>(field0(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]])))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(and<i32>(const<i32>(255), shr<i32,
// DEFAULT-SAME: amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(field0(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]]))))))), const<i32>(8))), and<i32>(const<i32>(65280), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32,
// DEFAULT-SAME: reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]]))))))), const<i32>(8)))))));
// DEFAULT-NEXT:                     write<u16>(field1(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]])))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(and<i32>(const<i32>(255), shr<i32,
// DEFAULT-SAME: amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(field1(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]]))))))), const<i32>(8))), and<i32>(const<i32>(65280), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32,
// DEFAULT-SAME: reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]]))))))), const<i32>(8)))))));
// DEFAULT-NEXT:                     write<u16>(field2(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]])))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(and<i32>(const<i32>(255), shr<i32,
// DEFAULT-SAME: amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(field2(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]]))))))), const<i32>(8))), and<i32>(const<i32>(65280), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32,
// DEFAULT-SAME: reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field2(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]]))))))), const<i32>(8)))))));
// DEFAULT-NEXT:                     write<u16>(field3(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]])))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(and<i32>(const<i32>(255), shr<i32,
// DEFAULT-SAME: amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(field3(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]]))))))), const<i32>(8))), and<i32>(const<i32>(65280), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32,
// DEFAULT-SAME: reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(deref(ptr_offset<ptr<@type[[TYPE_T]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_T]],
// DEFAULT-SAME: overflow=ub>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_i]]))))))), const<i32>(8)))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_T]] [storage=automatic] = aggregate<@type[[TYPE_T]], zero_fill=false>(field0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(1))), field1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(515))), field2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(1029))), field3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(1543))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<@type[[TYPE_T]]>) -> void>(%[[VALUE_foo]], const<i32>(1), addr_of<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(%[[VALUE_t]])))), const<i32>(256)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(%[[VALUE_t]])))), const<i32>(770))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field2(%[[VALUE_t]])))), const<i32>(1284))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_t]])))), const<i32>(1798)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
