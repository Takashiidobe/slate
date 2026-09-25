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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 b: u8;
// DEFAULT-NEXT:         field1 g: u8;
// DEFAULT-NEXT:         field2 r: u8;
// DEFAULT-NEXT:         field3 a: u8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     type @type1 U = union {
// DEFAULT-NEXT:         field0 c: @type0;
// DEFAULT-NEXT:         field1 v: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %2 @foo(%3 a: u8, %4 b: u8) -> u8 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(mul<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%3))), const<i32>(1)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%4)))), const<i32>(8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 x: ptr<@type1>, %7 y: ptr<@type1>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 z: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %9 v: u8 [storage=automatic] = read<u8>(field3(field0(deref(read<ptr<@type1>>(%6)))));
// DEFAULT-NEXT:         let %10 w: u8 [storage=automatic] = call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field3(field0(deref(read<ptr<@type1>>(%7))))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(sub<i32, overflow=ub>(const<i32>(255), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%9)))))));
// DEFAULT-NEXT:         write<u8>(field2(field0(%8)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field2(field0(deref(read<ptr<@type1>>(%6))))), read<u8>(%9)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field2(field0(deref(read<ptr<@type1>>(%7))))), read<u8>(%10))))))));
// DEFAULT-NEXT:         reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field2(field0(deref(read<ptr<@type1>>(%6))))), read<u8>(%9)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field2(field0(deref(read<ptr<@type1>>(%7))))), read<u8>(%10)))))));
// DEFAULT-NEXT:         write<u8>(field1(field0(%8)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field1(field0(deref(read<ptr<@type1>>(%6))))), read<u8>(%9)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field1(field0(deref(read<ptr<@type1>>(%7))))), read<u8>(%10))))))));
// DEFAULT-NEXT:         reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field1(field0(deref(read<ptr<@type1>>(%6))))), read<u8>(%9)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field1(field0(deref(read<ptr<@type1>>(%7))))), read<u8>(%10)))))));
// DEFAULT-NEXT:         write<u8>(field0(field0(%8)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field0(field0(deref(read<ptr<@type1>>(%6))))), read<u8>(%9)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field0(field0(deref(read<ptr<@type1>>(%7))))), read<u8>(%10))))))));
// DEFAULT-NEXT:         reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field0(field0(deref(read<ptr<@type1>>(%6))))), read<u8>(%9)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8) -> u8>(%2, read<u8>(field0(field0(deref(read<ptr<@type1>>(%7))))), read<u8>(%10)))))));
// DEFAULT-NEXT:         write<u8>(field3(field0(%8)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         return read<u32>(field1(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 a: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %13 b: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %14 c: @type1 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(255)), ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<@type0>(field0(%12), copy<@type0, reason=assign>(read<@type0>(compound_literal %15 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)))))));
// DEFAULT-NEXT:         write<@type0>(field0(%13), copy<@type0, reason=assign>(read<@type0>(compound_literal %16 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))), field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255)))))));
// DEFAULT-NEXT:         write<u32>(field1(%14), call<u32, signature=fn(ptr<@type1>, ptr<@type1>) -> u32>(%5, addr_of<ptr<@type1>>(%12), addr_of<ptr<@type1>>(%13)));
// DEFAULT-NEXT:         call<u32, signature=fn(ptr<@type1>, ptr<@type1>) -> u32>(%5, addr_of<ptr<@type1>>(%12), addr_of<ptr<@type1>>(%13));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(field0(%14))))), const<i32>(255)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field1(field0(%14))))), const<i32>(255))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field2(field0(%14))))), const<i32>(255))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field3(field0(%14))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
