/* PR target/82524 */

struct S {
  unsigned char b, g, r, a;
};
union U {
  struct S c;
  unsigned v;
};

static inline unsigned char foo(unsigned char a, unsigned char b) {
  return ((a + 1) * b) >> 8;
}

__attribute__((noinline, noclone)) unsigned bar(union U *x, union U *y) {
  union U       z;
  unsigned char v = x->c.a;
  unsigned char w = foo(y->c.a, 255 - v);
  z.c.r           = foo(x->c.r, v) + foo(y->c.r, w);
  z.c.g           = foo(x->c.g, v) + foo(y->c.g, w);
  z.c.b           = foo(x->c.b, v) + foo(y->c.b, w);
  z.c.a           = 0;
  return z.v;
}

int main() {
  union U a, b, c;
  if ((unsigned char)~0 != 255 || sizeof(unsigned) != 4)
    return 0;
  a.c = (struct S){255, 255, 255, 0};
  b.c = (struct S){255, 255, 255, 255};
  c.v = bar(&a, &b);
  if (c.c.b != 255 || c.c.g != 255 || c.c.r != 255 || c.c.a != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 b: u8;
// DEFAULT-NEXT:         field1 g: u8;
// DEFAULT-NEXT:         field2 r: u8;
// DEFAULT-NEXT:         field3 a: u8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 c: @type[[TYPE_S]];
// DEFAULT-NEXT:         field1 v: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: u8, %[[VALUE_b:[0-9]+]] b: u8) -> u8 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(mul<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a]]))), const<i32>(1)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_b]])))), const<i32>(8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_U]]>, %[[VALUE_y:[0-9]+]] y: ptr<@type[[TYPE_U]]>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: u8 [storage=automatic] = read<u8>(field3(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_x]])))));
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: u8 [storage=automatic] = call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field3(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_y]]))))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(sub<i32, overflow=ub>(const<i32>(255), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_v]])))))));
// DEFAULT-NEXT:         write<u8>(field2(field0(%[[VALUE_z]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field2(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_x]]))))), read<u8>(%[[VALUE_v]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field2(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_y]]))))), read<u8>(%[[VALUE_w]]))))))));
// DEFAULT-NEXT:         reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field2(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_x]]))))), read<u8>(%[[VALUE_v]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field2(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_y]]))))), read<u8>(%[[VALUE_w]])))))));
// DEFAULT-NEXT:         write<u8>(field1(field0(%[[VALUE_z]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field1(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_x]]))))), read<u8>(%[[VALUE_v]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field1(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_y]]))))), read<u8>(%[[VALUE_w]]))))))));
// DEFAULT-NEXT:         reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field1(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_x]]))))), read<u8>(%[[VALUE_v]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field1(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_y]]))))), read<u8>(%[[VALUE_w]])))))));
// DEFAULT-NEXT:         write<u8>(field0(field0(%[[VALUE_z]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field0(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_x]]))))), read<u8>(%[[VALUE_v]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field0(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_y]]))))), read<u8>(%[[VALUE_w]]))))))));
// DEFAULT-NEXT:         reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field0(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_x]]))))), read<u8>(%[[VALUE_v]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%[[VALUE_foo]], read<u8>(field0(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_y]]))))), read<u8>(%[[VALUE_w]])))))));
// DEFAULT-NEXT:         write<u8>(field3(field0(%[[VALUE_z]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         return read<u32>(field1(%[[VALUE_z]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(255)), ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field0(%[[VALUE_a_2]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)))))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field0(%[[VALUE_b_2]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255)))))));
// DEFAULT-NEXT:         write<u32>(field1(%[[VALUE_c]]), call<u32, signature=fn(ptr<@type[[TYPE_U]]>, ptr<@type[[TYPE_U]]>) -> u32>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_a_2]]), addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:         call<u32, signature=fn(ptr<@type[[TYPE_U]]>, ptr<@type[[TYPE_U]]>) -> u32>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_a_2]]), addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(field0(%[[VALUE_c]]))))), const<i32>(255)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field1(field0(%[[VALUE_c]]))))), const<i32>(255))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field2(field0(%[[VALUE_c]]))))), const<i32>(255))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field3(field0(%[[VALUE_c]]))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
