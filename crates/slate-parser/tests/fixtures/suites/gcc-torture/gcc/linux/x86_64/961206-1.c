void abort(void);
void exit(int);

int sub1(unsigned long long i) {
  if (i < 0x80000000)
    return 1;
  else
    return 0;
}

int sub2(unsigned long long i) {
  if (i <= 0x7FFFFFFF)
    return 1;
  else
    return 0;
}

int sub3(unsigned long long i) {
  if (i >= 0x80000000)
    return 0;
  else
    return 1;
}

int sub4(unsigned long long i) {
  if (i > 0x7FFFFFFF)
    return 0;
  else
    return 1;
}

int main(void) {
  if (sub1(0x80000000ULL))
    abort();

  if (sub2(0x80000000ULL))
    abort();

  if (sub3(0x80000000ULL))
    abort();

  if (sub4(0x80000000ULL))
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
// DEFAULT-NEXT:     fn %[[VALUE_sub1:[0-9]+]] @sub1(%[[VALUE_i:[0-9]+]] i: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(read<u64>(%[[VALUE_i]]), widen<u64, reason=usual_arith>(const<u32>(2147483648)))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sub2:[0-9]+]] @sub2(%[[VALUE_i_2:[0-9]+]] i: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if le<u64>(read<u64>(%[[VALUE_i_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2147483647))))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sub3:[0-9]+]] @sub3(%[[VALUE_i_3:[0-9]+]] i: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ge<u64>(read<u64>(%[[VALUE_i_3]]), widen<u64, reason=usual_arith>(const<u32>(2147483648)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sub4:[0-9]+]] @sub4(%[[VALUE_i_4:[0-9]+]] i: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u64>(read<u64>(%[[VALUE_i_4]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2147483647))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE_sub1]], const<u64>(2147483648)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE_sub2]], const<u64>(2147483648)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE_sub3]], const<u64>(2147483648)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE_sub4]], const<u64>(2147483648)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
