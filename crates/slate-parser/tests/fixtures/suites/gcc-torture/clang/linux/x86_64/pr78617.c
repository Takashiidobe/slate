int a = 0;
int d = 1;
int f = 1;

int fn1() { return a || 1 >> a; }

int fn2(int p1, int p2) { return p2 >= 2 ? p1 : p1 >> 1; }

int fn3(int p1) { return d ^ p1; }

int fn4(int p1, int p2) { return fn3(!d > fn2((f = fn1() - 1000) || p2, p1)); }

int main() {
  if (fn4(0, 0) != 1)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(1), read<i32>(%[[VALUE_a]])), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE_p1:[0-9]+]] p1: i32, %[[VALUE_p2:[0-9]+]] p2: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ge<i32>(read<i32>(%[[VALUE_p2]]), const<i32>(2)), read<i32>(%[[VALUE_p1]]), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_p1]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3(%[[VALUE_p1_2:[0-9]+]] p1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return xor<i32>(read<i32>(%[[VALUE_d]]), read<i32>(%[[VALUE_p1_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4(%[[VALUE_p1_3:[0-9]+]] p1: i32, %[[VALUE_p2_2:[0-9]+]] p2: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_f]], sub<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_fn1]]), const<i32>(1000)));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_fn3]], from_bool<i32, reason=arg>(gt<i32>(from_bool<i32, reason=promotion>(not<bool>(ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0)))), call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_fn2]], from_bool<i32, reason=arg>(logical_or<bool>(ne<i32>(sub<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_fn1]]), const<i32>(1000)), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_p2_2]]), const<i32>(0)))), read<i32>(%[[VALUE_p1_3]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_fn4]], const<i32>(0), const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
