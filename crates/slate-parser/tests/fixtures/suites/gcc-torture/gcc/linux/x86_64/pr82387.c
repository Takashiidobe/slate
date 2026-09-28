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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 b: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %1 f: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> @type0 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 h: array<@type0, 70> [storage=automatic] [align=16] = aggregate<array<@type0, 70>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index2 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index3 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index4 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index5 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index6 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index7 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index8 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index9 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index10 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index11 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index12 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index13 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index14 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index15 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index16 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index17 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index18 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index19 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index20 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index21 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index22 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index23 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index24 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index25 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index26 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index27 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index28 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index29 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index30 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index31 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index32 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index33 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index34 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index35 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index36 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index37 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index38 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index39 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index40 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index41 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index42 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index43 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index44 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index45 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index46 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index47 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index48 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index49 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index50 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index51 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index52 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index53 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index54 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index55 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index56 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index57 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index58 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index59 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index60 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index61 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index62 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index63 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index64 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index65 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index66 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index67 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index68 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)), index69 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(70)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 i: @type0 [storage=automatic] = copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%2));
// DEFAULT-NEXT:         let %6 j: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(%5));
// DEFAULT-NEXT:         let %7: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%6)), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:             write<bool>(%7, ne<i32>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%7, const<bool>(false));
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
