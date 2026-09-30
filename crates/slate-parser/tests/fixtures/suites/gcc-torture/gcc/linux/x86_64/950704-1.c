/* { dg-additional-options "-fwrapv" } */
void abort(void);
void exit(int);

int errflag;

long long f(long long x, long long y) {
  long long r;

  errflag = 0;
  r       = x + y;
  if (x >= 0) {
    if ((y < 0) || (r >= 0))
      return r;
  } else {
    if ((y > 0) || (r < 0))
      return r;
  }
  errflag = 1;
  return 0;
}

int main(void) {
  f(0, 0);
  if (errflag)
    abort();

  f(1, -1);
  if (errflag)
    abort();

  f(-1, 1);
  if (errflag)
    abort();

  f(0x8000000000000000LL, 0x8000000000000000LL);
  if (!errflag)
    abort();

  f(0x8000000000000000LL, -1LL);
  if (!errflag)
    abort();

  f(0x7fffffffffffffffLL, 0x7fffffffffffffffLL);
  if (!errflag)
    abort();

  f(0x7fffffffffffffffLL, 1LL);
  if (!errflag)
    abort();

  f(0x7fffffffffffffffLL, 0x8000000000000000LL);
  if (errflag)
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
// DEFAULT-NEXT:     global %[[VALUE_errflag:[0-9]+]] errflag: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: i64, %[[VALUE_y:[0-9]+]] y: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_errflag]], const<i32>(0));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_r]], add<i64, overflow=ub>(read<i64>(%[[VALUE_x]]), read<i64>(%[[VALUE_y]])));
// DEFAULT-NEXT:         if ge<i64>(read<i64>(%[[VALUE_x]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(lt<i64>(read<i64>(%[[VALUE_y]]), widen<i64, reason=usual_arith>(const<i32>(0))), ge<i64>(read<i64>(%[[VALUE_r]]), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                     return read<i64>(%[[VALUE_r]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(gt<i64>(read<i64>(%[[VALUE_y]]), widen<i64, reason=usual_arith>(const<i32>(0))), lt<i64>(read<i64>(%[[VALUE_r]]), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                     return read<i64>(%[[VALUE_r]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_errflag]], const<i32>(1));
// DEFAULT-NEXT:         return widen<i64, reason=return>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_f]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_errflag]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_f]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_errflag]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_f]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_errflag]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_f]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(9223372036854775808)), reinterpret<i64, reason=arg, fits=unknown>(const<u64>(9223372036854775808)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_errflag]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_f]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(9223372036854775808)), neg<i64, overflow=ub>(const<i64>(1)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_errflag]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_f]], const<i64>(9223372036854775807), const<i64>(9223372036854775807));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_errflag]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_f]], const<i64>(9223372036854775807), const<i64>(1));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_errflag]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_f]], const<i64>(9223372036854775807), reinterpret<i64, reason=arg, fits=unknown>(const<u64>(9223372036854775808)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_errflag]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
