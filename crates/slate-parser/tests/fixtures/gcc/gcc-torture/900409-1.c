void abort(void);
void exit(int);

long f1(long a) { return a & 0xff000000L; }
long f2(long a) { return a & ~0xff000000L; }
long f3(long a) { return a & 0x000000ffL; }
long f4(long a) { return a & ~0x000000ffL; }
long f5(long a) { return a & 0x0000ffffL; }
long f6(long a) { return a & ~0x0000ffffL; }

int main(void) {
  long a = 0x89ABCDEF;

  if (f1(a) != 0x89000000L || f2(a) != 0x00ABCDEFL || f3(a) != 0x000000EFL ||
      f4(a) != 0x89ABCD00L || f5(a) != 0x0000CDEFL || f6(a) != 0x89AB0000L)
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
// DEFAULT-NEXT:     fn %1 @exit(%16 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f1(%3 a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%3), const<i64>(4278190080));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f2(%5 a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%5), not<i64>(const<i64>(4278190080)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f3(%7 a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%7), const<i64>(255));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f4(%9 a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%9), not<i64>(const<i64>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f5(%11 a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%11), const<i64>(65535));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f6(%13 a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%13), not<i64>(const<i64>(65535)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 a: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2309737967)));
// DEFAULT-NEXT:         let %17: bool [synthetic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%2, read<i64>(%15)), const<i64>(2298478592))
// DEFAULT-NEXT:             write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%17, ne<i64>(call<i64, signature=fn(i64) -> i64>(%4, read<i64>(%15)), const<i64>(11259375)));
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%17)
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, ne<i64>(call<i64, signature=fn(i64) -> i64>(%6, read<i64>(%15)), const<i64>(239)));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, ne<i64>(call<i64, signature=fn(i64) -> i64>(%8, read<i64>(%15)), const<i64>(2309737728)));
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, ne<i64>(call<i64, signature=fn(i64) -> i64>(%10, read<i64>(%15)), const<i64>(52719)));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%20)
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, ne<i64>(call<i64, signature=fn(i64) -> i64>(%12, read<i64>(%15)), const<i64>(2309685248)));
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
