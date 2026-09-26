void abort(void);
void exit(int);

long long signed_poly(long long sum, long x) {
  sum += (long long)(long)sum * (long long)x;
  return sum;
}

unsigned long long unsigned_poly(unsigned long long sum, unsigned long x) {
  sum += (unsigned long long)(unsigned long)sum * (unsigned long long)x;
  return sum;
}

int main(void) {
  if (signed_poly(2LL, -3) != -4LL)
    abort();

  if (unsigned_poly(2ULL, 3) != 8ULL)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @signed_poly(%3 sum: i64, %4 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %11: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%10), mul<i64, overflow=ub>(read<i64>(%3), read<i64>(%4)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%11));
// DEFAULT-NEXT:         return read<i64>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @unsigned_poly(%6 sum: u64, %7 x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12: u64 [synthetic] = read<u64>(%6);
// DEFAULT-NEXT:         let %13: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%12), mul<u64, overflow=wrap>(read<u64>(%6), read<u64>(%7)));
// DEFAULT-NEXT:         write<u64>(%6, read<u64>(%13));
// DEFAULT-NEXT:         return read<u64>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%2, const<i64>(2), widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(3)))), neg<i64, overflow=ub>(const<i64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%5, const<u64>(2), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), const<u64>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
