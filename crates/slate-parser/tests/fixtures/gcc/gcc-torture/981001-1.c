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
// DEFAULT-NEXT:     global %2 flg: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @sub(%4 n: i32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 b: i32 [storage=automatic];
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%4), const<i32>(2))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%5, truncate<i32, reason=assign, fits=unknown>(call<i64, signature=fn(i32) -> i64>(%3, div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), const<i32>(2)))));
// DEFAULT-NEXT:                         truncate<i32, reason=assign, fits=unknown>(call<i64, signature=fn(i32) -> i64>(%3, div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), const<i32>(2))));
// DEFAULT-NEXT:                         return mul<i64, overflow=ub>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%5)), mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(const<i32>(2)), call<i64, signature=fn(i32) -> i64>(%3, sub<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), const<i32>(2)), const<i32>(1))))), widen<i64, reason=usual_arith>(read<i32>(%5)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%5, truncate<i32, reason=assign, fits=unknown>(call<i64, signature=fn(i32) -> i64>(%3, add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), const<i32>(2)), const<i32>(1)))));
// DEFAULT-NEXT:                         truncate<i32, reason=assign, fits=unknown>(call<i64, signature=fn(i32) -> i64>(%3, add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:                         write<i32>(%6, truncate<i32, reason=assign, fits=unknown>(call<i64, signature=fn(i32) -> i64>(%3, div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), const<i32>(2)))));
// DEFAULT-NEXT:                         truncate<i32, reason=assign, fits=unknown>(call<i64, signature=fn(i32) -> i64>(%3, div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), const<i32>(2))));
// DEFAULT-NEXT:                         return widen<i64, reason=return>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%5), read<i32>(%5)), mul<i32, overflow=ub>(read<i32>(%6), read<i32>(%6))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return widen<i64, reason=explicit>(read<i32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32) -> i64>(%3, const<i32>(30)), const<i64>(832040))
// DEFAULT-NEXT:             let %9: u64 [synthetic] = read<u64>(%2);
// DEFAULT-NEXT:             let %10: u64 [synthetic] = or<u64>(read<u64>(%9), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(256)));
// DEFAULT-NEXT:             write<u64>(%2, read<u64>(%10));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%2), const<u64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
