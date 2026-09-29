/* PR middle-end/24109 */

extern void abort(void);

struct A {
  int i;
  int j;
};
struct B {
  struct A *a;
  struct A *b;
};
struct C {
  struct B *c;
  struct A *d;
};
struct C e = {&(struct B){&(struct A){1, 2}, &(struct A){3, 4}},
              &(struct A){5, 6}};

int main(void) {
  if (e.c->a->i != 1 || e.c->a->j != 2)
    abort();
  if (e.c->b->i != 3 || e.c->b->j != 4)
    abort();
  if (e.d->i != 5 || e.d->j != 6)
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 a: ptr<@type[[TYPE_A]]>;
// DEFAULT-NEXT:         field1 b: ptr<@type[[TYPE_A]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE_B]]>;
// DEFAULT-NEXT:         field1 d: ptr<@type[[TYPE_A]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: @type[[TYPE_C]] [storage=static] = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_B]]>>(compound_literal %[[VALUE0:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_A]]>>(compound_literal %[[VALUE1:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2))), field1 = addr_of<ptr<@type[[TYPE_A]]>>(compound_literal %[[VALUE2:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4))))), field1 = addr_of<ptr<@type[[TYPE_A]]>>(compound_literal %[[VALUE3:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(5), field1 = const<i32>(6)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_A]]>>(field0(deref(read<ptr<@type[[TYPE_B]]>>(field0(%[[VALUE_e]])))))))), const<i32>(1)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(field0(deref(read<ptr<@type[[TYPE_B]]>>(field0(%[[VALUE_e]])))))))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_A]]>>(field1(deref(read<ptr<@type[[TYPE_B]]>>(field0(%[[VALUE_e]])))))))), const<i32>(3)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(field1(deref(read<ptr<@type[[TYPE_B]]>>(field0(%[[VALUE_e]])))))))), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_A]]>>(field1(%[[VALUE_e]]))))), const<i32>(5)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(field1(%[[VALUE_e]]))))), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
