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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %1 @foo(%2 s: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 ss: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(compound_literal %6 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = read<i32>(field1(deref(read<ptr<@type0>>(%2)))), field1 = read<i32>(field0(deref(read<ptr<@type0>>(%2)))))));
// DEFAULT-NEXT:         write<@type0>(deref(read<ptr<@type0>>(%2)), copy<@type0, reason=assign>(read<@type0>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 s: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(12));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, addr_of<ptr<@type0>>(%5));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(%5)), const<i32>(12)), ne<i32>(read<i32>(field1(%5)), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
