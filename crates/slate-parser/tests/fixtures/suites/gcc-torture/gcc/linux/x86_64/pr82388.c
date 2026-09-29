/* PR tree-optimization/82388 */

struct A {
  int b;
  int c;
  int d;
} e;

struct A foo(void) {
  struct A h[30] = {{0, 0, 0}};
  return h[29];
}

int main() {
  e = foo();
  return e.b;
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
// DEFAULT-NEXT:         field1 c: i32;
// DEFAULT-NEXT:         field2 d: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: @type[[TYPE_A]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> @type[[TYPE_A]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: array<@type[[TYPE_A]], 30> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_A]], 30>, zero_fill=true>(index0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_A]], reason=return>(read<@type[[TYPE_A]]>(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(30)>(%[[VALUE_h]]), const<i32>(29)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<@type[[TYPE_A]]>(%[[VALUE_e]], copy<@type[[TYPE_A]], reason=assign>(call<@type[[TYPE_A]], signature=fn() -> @type[[TYPE_A]], abi=sysv64() -> native_c>(%[[VALUE_foo]])));
// DEFAULT-NEXT:         copy<@type[[TYPE_A]], reason=assign>(call<@type[[TYPE_A]], signature=fn() -> @type[[TYPE_A]], abi=sysv64() -> native_c>(%[[VALUE_foo]]));
// DEFAULT-NEXT:         return read<i32>(field0(%[[VALUE_e]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
