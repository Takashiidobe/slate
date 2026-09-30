void abort(void);
void exit(int);

long a = 1;

int foo(void) {
  switch (a % 2 % 2 % 2 % 2 % 2 % 2 % 2 % 2) {
  case 0:
    return 0;
  case 1:
    return 1;
  default:
    return -1;
  }
}

int main(void) {
  if (foo() != 1)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] rem<i64, by_zero=ub, min_by_neg_one=ub>(rem<i64, by_zero=ub, min_by_neg_one=ub>(rem<i64, by_zero=ub, min_by_neg_one=ub>(rem<i64, by_zero=ub, min_by_neg_one=ub>(rem<i64, by_zero=ub, min_by_neg_one=ub>(rem<i64, by_zero=ub, min_by_neg_one=ub>(rem<i64, by_zero=ub, min_by_neg_one=ub>(rem<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_a]]), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(2)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i64>(0):
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i64>(1):
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:                 default %[[VALUE1]]:
// DEFAULT-NEXT:                     return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_foo]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
