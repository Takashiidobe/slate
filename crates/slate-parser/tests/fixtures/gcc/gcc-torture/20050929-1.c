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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 a: ptr<@type0>;
// DEFAULT-NEXT:         field1 b: ptr<@type0>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 C = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type1>;
// DEFAULT-NEXT:         field1 d: ptr<@type0>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %4 e: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = addr_of<ptr<@type1>>(compound_literal %8 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = addr_of<ptr<@type0>>(compound_literal %6 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2))), field1 = addr_of<ptr<@type0>>(compound_literal %7 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4))))), field1 = addr_of<ptr<@type0>>(compound_literal %9 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(5), field1 = const<i32>(6)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(field0(%4)))))))), const<i32>(1)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(field0(%4)))))))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(field1(deref(read<ptr<@type1>>(field0(%4)))))))), const<i32>(3)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(field1(deref(read<ptr<@type1>>(field0(%4)))))))), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(field1(%4))))), const<i32>(5)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(field1(%4))))), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
