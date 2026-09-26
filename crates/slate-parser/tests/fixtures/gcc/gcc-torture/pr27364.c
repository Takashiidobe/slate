void exit(int);

int f(unsigned number_of_digits_to_use) {
  if (number_of_digits_to_use > 1294)
    return 0;
  return (number_of_digits_to_use * 3321928 / 1000000 + 1) / 16;
}

int main(void) {
  if (f(11) != 2)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %0 @exit(%4 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @f(%2 number_of_digits_to_use: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1294)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(div<u32, by_zero=ub>(add<u32, overflow=wrap>(div<u32, by_zero=ub>(mul<u32, overflow=wrap>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3321928))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1000000))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(11))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
