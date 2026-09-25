/* PR middle-end/71626 */

typedef __INTPTR_TYPE__ V
    __attribute__((__vector_size__(sizeof(__INTPTR_TYPE__))));

__attribute__((noinline, noclone)) V foo() {
  V v = {(__INTPTR_TYPE__)foo};
  return v;
}

int main() {
  V v = foo();
  if (v[0] != (__INTPTR_TYPE__)foo)
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
// DEFAULT-NEXT:     type @type0 V = vector<i64, 1>;
// DEFAULT-NEXT:     fn %1 @foo() -> vector<i64, 1> [linkage=external] [inline=never] [definition=emitted] [abi=sysv64() -> coerce<f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 v: vector<i64, 1> [storage=automatic] = aggregate<vector<i64, 1>, zero_fill=false>(index0 = ptr_to_int<i64, reason=explicit>(function_decay<ptr<fn() -> vector<i64, 1>>>(%1)));
// DEFAULT-NEXT:         return read<vector<i64, 1>>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 v: vector<i64, 1> [storage=automatic] = call<vector<i64, 1>, signature=fn() -> vector<i64, 1>, abi=sysv64() -> coerce<f64>>(%1);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(lane(%4, const<i32>(0))), ptr_to_int<i64, reason=explicit>(function_decay<ptr<fn() -> vector<i64, 1>>>(%1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
