/* PR tree-optimization/91597 */

enum E { A, B, C };
struct __attribute__((aligned(4))) S {
  enum E e;
};

enum E foo(struct S *o) {
  if (((__UINTPTR_TYPE__)(o) & 1) == 0)
    return o->e;
  else
    return A;
}

int bar(struct S *o) { return foo(o) == B || foo(o) == C; }

static inline void baz(struct S *o, int d) {
  if (__builtin_expect(!bar(o), 0))
    __builtin_abort();
  if (d > 2)
    return;
  baz(o, d + 1);
}

void qux(struct S *o) {
  switch (o->e) {
  case A:
    return;
  case B:
    baz(o, 0);
    break;
  case C:
    baz(o, 0);
    break;
  }
}

struct S s = {C};

int main() {
  qux(&s);
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
// DEFAULT-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_B:[0-9]+]] B = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_C:[0-9]+]] C = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 e: @type[[TYPE_E]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = int_to_enum<@type[[TYPE_E]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(2)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_o:[0-9]+]] o: ptr<@type[[TYPE_S]]>) -> @type[[TYPE_E]] [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<u64>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_o]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             return read<@type[[TYPE_E]]>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_o]]))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return int_to_enum<@type[[TYPE_E]], reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_o_2:[0-9]+]] o: ptr<@type[[TYPE_S]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if eq<u32>(enum_to_int<u32, reason=promotion>(call<@type[[TYPE_E]], signature=fn(ptr<@type[[TYPE_S]]>) -> @type[[TYPE_E]]>(%[[VALUE_foo]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_o_2]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], eq<u32>(enum_to_int<u32, reason=promotion>(call<@type[[TYPE_E]], signature=fn(ptr<@type[[TYPE_S]]>) -> @type[[TYPE_E]]>(%[[VALUE_foo]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_o_2]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(read<bool>(%[[VALUE0]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expect:[0-9]+]] @__builtin_expect(%[[VALUE1:[0-9]+]] <unnamed>: i64, %[[VALUE2:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_o_3:[0-9]+]] o: ptr<@type[[TYPE_S]]>, %[[VALUE_d:[0-9]+]] d: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE___builtin_expect]], from_bool<i64, reason=arg>(not<bool>(ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_S]]>) -> i32>(%[[VALUE_bar]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_o_3]])), const<i32>(0)))), widen<i64, reason=arg>(const<i32>(0))), const<i64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_d]]), const<i32>(2))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_baz]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_o_3]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_d]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(%[[VALUE_o_4:[0-9]+]] o: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %[[VALUE3:[0-9]+]] enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_o_4]])))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE3]] const<u32>(0):
// DEFAULT-NEXT:                     return;
// DEFAULT-NEXT:                 case %[[VALUE3]] const<u32>(1):
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_baz]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_o_4]]), const<i32>(0));
// DEFAULT-NEXT:                 break %[[VALUE3]];
// DEFAULT-NEXT:                 case %[[VALUE3]] const<u32>(2):
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_baz]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_o_4]]), const<i32>(0));
// DEFAULT-NEXT:                 break %[[VALUE3]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_qux]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
