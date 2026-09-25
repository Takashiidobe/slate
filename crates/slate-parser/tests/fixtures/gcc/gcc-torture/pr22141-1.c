/* PR middle-end/22141 */

extern void abort(void);

struct S {
  struct T {
    char a;
    char b;
    char c;
    char d;
  } t;
} u;

struct U {
  struct S s[4];
};

void __attribute__((noinline)) c1(struct T *p) {
  if (p->a != 1 || p->b != 2 || p->c != 3 || p->d != 4)
    abort();
  __builtin_memset(p, 0xaa, sizeof(*p));
}

void __attribute__((noinline)) c2(struct S *p) { c1(&p->t); }

void __attribute__((noinline)) c3(struct U *p) { c2(&p->s[2]); }

void __attribute__((noinline)) f1(void) { u = (struct S){{1, 2, 3, 4}}; }

void __attribute__((noinline)) f2(void) {
  u.t.a = 1;
  u.t.b = 2;
  u.t.c = 3;
  u.t.d = 4;
}

void __attribute__((noinline)) f3(void) {
  u.t.d = 4;
  u.t.b = 2;
  u.t.a = 1;
  u.t.c = 3;
}

void __attribute__((noinline)) f4(void) {
  struct S v;
  v.t.a = 1;
  v.t.b = 2;
  v.t.c = 3;
  v.t.d = 4;
  c2(&v);
}

void __attribute__((noinline)) f5(struct S *p) {
  p->t.a = 1;
  p->t.c = 3;
  p->t.d = 4;
  p->t.b = 2;
}

void __attribute__((noinline)) f6(void) {
  struct U v;
  v.s[2].t.a = 1;
  v.s[2].t.b = 2;
  v.s[2].t.c = 3;
  v.s[2].t.d = 4;
  c3(&v);
}

void __attribute__((noinline)) f7(struct U *p) {
  p->s[2].t.a = 1;
  p->s[2].t.c = 3;
  p->s[2].t.d = 4;
  p->s[2].t.b = 2;
}

int main(void) {
  struct U w;
  f1();
  c2(&u);
  f2();
  c1(&u.t);
  f3();
  c2(&u);
  f4();
  f5(&u);
  c2(&u);
  f6();
  f7(&w);
  c3(&w);
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
// DEFAULT-NEXT:         field0 t: @type1;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 T = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:         field3 d: i8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     type @type2 U = struct {
// DEFAULT-NEXT:         field0 s: array<@type0, 4>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %3 u: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @c1(%6 p: ptr<@type1>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type1>>(%6))))), const<i32>(1)), ne<i32>(widen<i32, reason=promotion>(read<i8>(field1(deref(read<ptr<@type1>>(%6))))), const<i32>(2))), ne<i32>(widen<i32, reason=promotion>(read<i8>(field2(deref(read<ptr<@type1>>(%6))))), const<i32>(3))), ne<i32>(widen<i32, reason=promotion>(read<i8>(field3(deref(read<ptr<@type1>>(%6))))), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%6)), const<i32>(170), const<u64>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @c2(%8 p: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%5, addr_of<ptr<@type1>>(field0(deref(read<ptr<@type0>>(%8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @c3(%10 p: ptr<@type2>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type2>>(%10)))), const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @f1() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type0>(%3, copy<@type0, reason=assign>(read<@type0>(compound_literal %24 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = aggregate<@type1, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), field3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f2() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(field0(field0(%3)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field1(field0(%3)), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(field2(field0(%3)), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(field3(field0(%3)), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @f3() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(field3(field0(%3)), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         write<i8>(field1(field0(%3)), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(field0(field0(%3)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field2(field0(%3)), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f4() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 v: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(field0(%15)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field1(field0(%15)), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(field2(field0(%15)), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(field3(field0(%15)), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @f5(%17 p: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(field0(field0(deref(read<ptr<@type0>>(%17)))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field2(field0(deref(read<ptr<@type0>>(%17)))), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(field3(field0(deref(read<ptr<@type0>>(%17)))), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         write<i8>(field1(field0(deref(read<ptr<@type0>>(%17)))), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f6() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 v: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(%19)), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field1(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(%19)), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(field2(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(%19)), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(field3(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(%19)), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>) -> void>(%9, addr_of<ptr<@type2>>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @f7(%21 p: ptr<@type2>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(field0(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type2>>(%21)))), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field2(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type2>>(%21)))), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(field3(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type2>>(%21)))), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         write<i8>(field1(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type2>>(%21)))), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %23 w: @type2 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(%3));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%5, addr_of<ptr<@type1>>(field0(%3)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(%3));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%16, addr_of<ptr<@type0>>(%3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(%3));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>) -> void>(%20, addr_of<ptr<@type2>>(%23));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>) -> void>(%9, addr_of<ptr<@type2>>(%23));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
