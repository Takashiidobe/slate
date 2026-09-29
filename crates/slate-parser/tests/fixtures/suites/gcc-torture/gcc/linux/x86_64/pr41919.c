extern void abort(void);

#define assert(x)                                                              \
  if (!(x))                                                                    \
  abort()

struct S1 {
  signed char f0;
};

int g_23 = 0;

static struct S1 foo(void) {
  int      *l_100 = &g_23;
  int     **l_110 = &l_100;
  struct S1 l_128 = {1};
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  return l_128;
}

static signed char bar(signed char si1, signed char si2) {
  return (si1 <= 0) ? si1 : (si2 * 2);
}
int main(void) {
  struct S1 s = foo();
  if (bar(0x99 ^ (s.f0 && 1), 1) != -104)
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_S1:[0-9]+]] S1 = struct {
// DEFAULT-NEXT:         field0 f0: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_g_23:[0-9]+]] g_23: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> @type[[TYPE_S1]] [linkage=internal] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_l_100:[0-9]+]] l_100: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_g_23]]);
// DEFAULT-NEXT:         let %[[VALUE_l_110:[0-9]+]] l_110: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%[[VALUE_l_100]]);
// DEFAULT-NEXT:         let %[[VALUE_l_128:[0-9]+]] l_128: @type[[TYPE_S1]] [storage=automatic] = aggregate<@type[[TYPE_S1]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if not<bool>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_l_100]]), addr_of<ptr<i32>>(%[[VALUE_g_23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_l_100]]), addr_of<ptr<i32>>(%[[VALUE_g_23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_l_100]]), addr_of<ptr<i32>>(%[[VALUE_g_23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_l_100]]), addr_of<ptr<i32>>(%[[VALUE_g_23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_l_100]]), addr_of<ptr<i32>>(%[[VALUE_g_23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_l_100]]), addr_of<ptr<i32>>(%[[VALUE_g_23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_l_100]]), addr_of<ptr<i32>>(%[[VALUE_g_23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return copy<@type[[TYPE_S1]], reason=return>(read<@type[[TYPE_S1]]>(%[[VALUE_l_128]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_si1:[0-9]+]] si1: i8, %[[VALUE_si2:[0-9]+]] si2: i8) -> i8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(conditional<i32>(le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_si1]])), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_si1]])), mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_si2]])), const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S1]] [storage=automatic] = copy<@type[[TYPE_S1]], reason=assign>(call<@type[[TYPE_S1]], signature=fn() -> @type[[TYPE_S1]], abi=sysv64() -> native_c>(%[[VALUE_foo]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i8, i8) -> i8>(%[[VALUE_bar]], truncate<i8, reason=arg, fits=unknown>(xor<i32>(const<i32>(153), from_bool<i32, reason=promotion>(logical_and<bool>(ne<i8>(read<i8>(field0(%[[VALUE_s]])), const<i8>(0)), ne<i32>(const<i32>(1), const<i32>(0)))))), truncate<i8, reason=arg, fits=always>(const<i32>(1)))), neg<i32, overflow=ub>(const<i32>(104)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
