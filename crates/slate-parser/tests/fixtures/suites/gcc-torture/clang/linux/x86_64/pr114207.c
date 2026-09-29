#include <stdint.h>
#include <stdio.h>

struct S {
  int a, b;
};

__attribute__((noinline)) void foo(struct S *s) {
  struct S ss = (struct S){.a = s->b, .b = s->a};
  *s          = ss;
}

int main() {
  struct S s = {6, 12};
  foo(&s);
  if (s.a != 12 || s.b != 6)
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
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ss:[0-9]+]] ss: @type[[TYPE_S]] [storage=automatic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = read<i32>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])))), field1 = read<i32>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])))))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_ss]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(12));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s_2]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_s_2]])), const<i32>(12)), ne<i32>(read<i32>(field1(%[[VALUE_s_2]])), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
