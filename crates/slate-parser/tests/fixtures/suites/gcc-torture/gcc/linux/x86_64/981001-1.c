void abort(void);
void exit(int);

#define NG 0x100L

unsigned long flg = 0;

long sub(int n) {
  int a, b;

  if (n >= 2) {
    if (n % 2 == 0) {
      a = sub(n / 2);

      return (a + 2 * sub(n / 2 - 1)) * a;
    } else {
      a = sub(n / 2 + 1);
      b = sub(n / 2);

      return a * a + b * b;
    }
  } else
    return (long)n;
}

int main(void) {
  if (sub(30) != 832040L)
    flg |= NG;

  if (flg)
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
// DEFAULT-NEXT:     global %[[VALUE_flg:[0-9]+]] flg: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_sub:[0-9]+]] @sub(%[[VALUE_n:[0-9]+]] n: i32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic];
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_n]]), const<i32>(2))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_n]]), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_a]], truncate<i32, reason=assign, fits=unknown>(call<i64, signature=fn(i32) -> i64>(%[[VALUE_sub]], div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_n]]), const<i32>(2)))));
// DEFAULT-NEXT:                         return mul<i64, overflow=ub>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_a]])), mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(const<i32>(2)), call<i64, signature=fn(i32) -> i64>(%[[VALUE_sub]], sub<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_n]]), const<i32>(2)), const<i32>(1))))), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_a]])));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_a]], truncate<i32, reason=assign, fits=unknown>(call<i64, signature=fn(i32) -> i64>(%[[VALUE_sub]], add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_n]]), const<i32>(2)), const<i32>(1)))));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_b]], truncate<i32, reason=assign, fits=unknown>(call<i64, signature=fn(i32) -> i64>(%[[VALUE_sub]], div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_n]]), const<i32>(2)))));
// DEFAULT-NEXT:                         return widen<i64, reason=return>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_a]])), mul<i32, overflow=ub>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_b]]))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return widen<i64, reason=explicit>(read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32) -> i64>(%[[VALUE_sub]], const<i32>(30)), const<i64>(832040))
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_flg]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE1]]), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(256)));
// DEFAULT-NEXT:             write<u64>(%[[VALUE_flg]], read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_flg]]), const<u64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
