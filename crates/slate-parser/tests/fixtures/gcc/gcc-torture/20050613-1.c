/* PR tree-optimization/22043 */

extern void abort(void);

struct A {
  int i;
  int j;
  int k;
  int l;
};
struct B {
  struct A a;
  int      r[1];
};
struct C {
  struct A a;
  int      r[0];
};
struct D {
  struct A a;
  int      r[];
};

void foo(struct A *x) {
  if (x->i != 0 || x->j != 5 || x->k != 0 || x->l != 0)
    abort();
}

int main() {
  struct B b = {.a.j = 5};
  struct C c = {.a.j = 5};
  struct D d = {.a.j = 5};
  foo(&b.a);
  foo(&c.a);
  foo(&d.a);
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
// DEFAULT-NEXT:         field2 k: i32;
// DEFAULT-NEXT:         field3 l: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 a: @type0;
// DEFAULT-NEXT:         field1 r: array<i32, 1>;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type2 C = struct {
// DEFAULT-NEXT:         field0 a: @type0;
// DEFAULT-NEXT:         field1 r: array<i32, 0>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type3 D = struct {
// DEFAULT-NEXT:         field0 a: @type0;
// DEFAULT-NEXT:         field1 r: array<i32, incomplete>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 16]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo(%6 x: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%6)))), const<i32>(0)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%6)))), const<i32>(5))), ne<i32>(read<i32>(field2(deref(read<ptr<@type0>>(%6)))), const<i32>(0))), ne<i32>(read<i32>(field3(deref(read<ptr<@type0>>(%6)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 b: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>(field0 = aggregate<@type0, zero_fill=true>(field1 = const<i32>(5)));
// DEFAULT-NEXT:         let %9 c: @type2 [storage=automatic] = aggregate<@type2, zero_fill=true>(field0 = aggregate<@type0, zero_fill=true>(field1 = const<i32>(5)));
// DEFAULT-NEXT:         let %10 d: @type3 [storage=automatic] = aggregate<@type3, zero_fill=false>(field0 = aggregate<@type0, zero_fill=true>(field1 = const<i32>(5)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%5, addr_of<ptr<@type0>>(field0(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%5, addr_of<ptr<@type0>>(field0(%9)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%5, addr_of<ptr<@type0>>(field0(%10)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
