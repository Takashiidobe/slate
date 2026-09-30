extern void abort(void);

int          a, c, d;
volatile int b;

static int foo(int p1, short p2) { return p1 / p2; }

int main() {
  char e;
  d = foo(a == 0, (0, 35536));
  e = d % 14;
  b = e && c;
  if (b != 0)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p1:[0-9]+]] p1: i32, %[[VALUE_p2:[0-9]+]] p2: i16) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_p1]]), widen<i32, reason=promotion>(read<i16>(%[[VALUE_p2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: i8 [storage=automatic];
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], call<i32, signature=fn(i32, i16) -> i32>(%[[VALUE_foo]], from_bool<i32, reason=arg>(eq<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))), truncate<i16, reason=arg, fits=unknown>(const<i32>(35536))));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_e]], truncate<i8, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_d]]), const<i32>(14))));
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_b]], from_bool<i32, reason=assign>(logical_and<bool>(ne<i8>(read<i8>(%[[VALUE_e]]), const<i8>(0)), ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
