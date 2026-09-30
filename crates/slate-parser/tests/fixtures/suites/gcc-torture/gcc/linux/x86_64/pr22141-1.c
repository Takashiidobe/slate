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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 t: @type[[TYPE_T:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_T]] T = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:         field3 d: i8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = struct {
// DEFAULT-NEXT:         field0 s: array<@type[[TYPE_S]], 4>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_c1:[0-9]+]] @c1(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_T]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p]]))))), const<i32>(1)), ne<i32>(widen<i32, reason=promotion>(read<i8>(field1(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p]]))))), const<i32>(2))), ne<i32>(widen<i32, reason=promotion>(read<i8>(field2(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p]]))))), const<i32>(3))), ne<i32>(widen<i32, reason=promotion>(read<i8>(field3(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p]]))))), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p]])), const<i32>(170), const<u64>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c2:[0-9]+]] @c2(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_T]]>) -> void>(%[[VALUE_c1]], addr_of<ptr<@type[[TYPE_T]]>>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c3:[0-9]+]] @c3(%[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_U]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_c2]], addr_of<ptr<@type[[TYPE_S]]>>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_p_3]])))), const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_u]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = aggregate<@type[[TYPE_T]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), field3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(field0(field0(%[[VALUE_u]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field1(field0(%[[VALUE_u]])), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(field2(field0(%[[VALUE_u]])), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(field3(field0(%[[VALUE_u]])), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(field3(field0(%[[VALUE_u]])), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         write<i8>(field1(field0(%[[VALUE_u]])), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(field0(field0(%[[VALUE_u]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field2(field0(%[[VALUE_u]])), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(field0(%[[VALUE_v]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field1(field0(%[[VALUE_v]])), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(field2(field0(%[[VALUE_v]])), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(field3(field0(%[[VALUE_v]])), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_c2]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_v]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_p_4:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(field0(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_4]])))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field2(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_4]])))), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(field3(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_4]])))), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         write<i8>(field1(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_4]])))), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(%[[VALUE_v_2]])), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field1(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(%[[VALUE_v_2]])), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(field2(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(%[[VALUE_v_2]])), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(field3(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(%[[VALUE_v_2]])), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_U]]>) -> void>(%[[VALUE_c3]], addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_p_5:[0-9]+]] p: ptr<@type[[TYPE_U]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(field0(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_p_5]])))), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field2(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_p_5]])))), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(field3(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_p_5]])))), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         write<i8>(field1(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_p_5]])))), const<i32>(2))))), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f1]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_c2]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_u]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_T]]>) -> void>(%[[VALUE_c1]], addr_of<ptr<@type[[TYPE_T]]>>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f3]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_c2]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_u]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f4]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_f5]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_u]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_c2]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_u]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f6]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_U]]>) -> void>(%[[VALUE_f7]], addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_w]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_U]]>) -> void>(%[[VALUE_c3]], addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_w]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
