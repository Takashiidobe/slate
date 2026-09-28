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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 b: i32;
// DEFAULT-NEXT:         field1 c: i32;
// DEFAULT-NEXT:         field2 d: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %1 e: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> @type0 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 h: array<@type0, 30> [storage=automatic] [align=16] = aggregate<array<@type0, 30>, zero_fill=true>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0)));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(30)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<@type0>(%1, copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%2)));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%2));
// DEFAULT-NEXT:         return read<i32>(field0(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
