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
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h:
// DEFAULT-SAME: array<@type[[TYPE_A]], 70> [storage=automatic] [align=16] =
// DEFAULT-SAME: aggregate<array<@type[[TYPE_A]], 70>, zero_fill=false>(index0 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index1 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index2 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index3 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index4 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index5 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index6 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index7 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index8 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index9 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index10 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index11 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index12 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index13 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index14 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index15 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index16 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index17 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index18 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index19 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index20 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index21 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index22 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index23 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index24 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index25 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index26 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index27 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index28 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index29 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index30 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index31 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index32 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index33 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index34 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index35 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index36 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index37 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index38 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index39 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index40 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index41 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index42 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index43 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index44 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index45 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index46 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index47 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index48 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index49 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index50 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index51 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index52 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index53 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index54 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index55 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index56 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index57 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index58 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index59 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index60 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index61 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index62 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index63 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index64 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index65 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index66 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index67 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index68 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)), index69 =
// DEFAULT-SAME: aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(1)));
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
