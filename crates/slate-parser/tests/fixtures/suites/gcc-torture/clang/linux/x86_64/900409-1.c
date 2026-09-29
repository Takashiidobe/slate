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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_a:[0-9]+]] a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%[[VALUE_a]]), const<i64>(4278190080));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_a_2:[0-9]+]] a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%[[VALUE_a_2]]), not<i64>(const<i64>(4278190080)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_a_3:[0-9]+]] a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%[[VALUE_a_3]]), const<i64>(255));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_a_4:[0-9]+]] a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%[[VALUE_a_4]]), not<i64>(const<i64>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_a_5:[0-9]+]] a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%[[VALUE_a_5]]), const<i64>(65535));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_a_6:[0-9]+]] a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(read<i64>(%[[VALUE_a_6]]), not<i64>(const<i64>(65535)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_7:[0-9]+]] a: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2309737967)));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_f1]], read<i64>(%[[VALUE_a_7]])), const<i64>(2298478592))
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_f2]], read<i64>(%[[VALUE_a_7]])), const<i64>(11259375)));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_f3]], read<i64>(%[[VALUE_a_7]])), const<i64>(239)));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_f4]], read<i64>(%[[VALUE_a_7]])), const<i64>(2309737728)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_f5]], read<i64>(%[[VALUE_a_7]])), const<i64>(52719)));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_f6]], read<i64>(%[[VALUE_a_7]])), const<i64>(2309685248)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
