/* PR tree-optimization/82387 */

struct A {
  int b;
};
int f = 1;

struct A foo(void) {
  struct A h[] = {
      {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
      {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
      {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
      {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
      {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
  };
  return h[24];
}

int main() {
  struct A i = foo(), j = i;
  j.b && (f = 0);
  return f;
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
// DEFAULT-NEXT:         field0 b: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> @type[[TYPE_A]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: array<@type[[TYPE_A]], 70> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_A]], 70>, zero_fill=false>(index0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index1 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index2 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index3 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index4 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index5 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index6 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index7 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index8 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index9 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index10 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index11 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index12 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index13 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index14 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index15 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index16 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index17 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index18 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index19 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index20 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index21 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index22 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index23 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index24 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index25 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index26 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index27 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index28 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index29 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index30 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index31 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index32 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index33 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index34 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index35 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index36 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index37 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index38 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index39 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index40 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index41 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index42 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index43 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index44 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index45 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index46 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index47 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index48 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index49 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index50 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index51 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index52 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index53 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index54 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index55 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index56 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index57 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index58 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index59 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index60 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index61 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index62 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index63 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index64 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index65 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index66 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index67 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index68 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index69 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_A]], reason=return>(read<@type[[TYPE_A]]>(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(70)>(%[[VALUE_h]]), const<i32>(24)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: @type[[TYPE_A]] [storage=automatic] = copy<@type[[TYPE_A]], reason=assign>(call<@type[[TYPE_A]], signature=fn() -> @type[[TYPE_A]], abi=sysv64() -> native_c>(%[[VALUE_foo]]));
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: @type[[TYPE_A]] [storage=automatic] = copy<@type[[TYPE_A]], reason=assign>(read<@type[[TYPE_A]]>(%[[VALUE_i]]));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%[[VALUE_j]])), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_f]], const<i32>(0));
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(false));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_f]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
