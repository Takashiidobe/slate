/* { dg-do run } */
/* Check that find_scan_insn properly handles swapped FP comparisons.  */
static double x;
static int    exit_code;

void __attribute__((noinline)) check_int(int a, int b) {
  exit_code += (a != b);
}

int
main(void) {
  x = 0.0;
  asm("" : "+m"(x));
  check_int(__builtin_isgreater(x, 1.0), 0);
  check_int(__builtin_isgreaterequal(x, 1.0), 0);
  check_int(__builtin_isless(x, 1.0), 1);
  check_int(__builtin_islessequal(x, 1.0), 1);
  check_int(__builtin_islessgreater(x, 1.0), 1);
  return exit_code;
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
// DEFAULT-NEXT:     global %0 x: f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %1 exit_code: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %2 @check_int(%3 a: i32, %4 b: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%3), read<i32>(%4))));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<f64>(%0, const<f64>(0.0));
// DEFAULT-NEXT:         asm "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "m" [mem] place<f64>(%0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%2, from_bool<i32, reason=arg>(gt<f64, exceptions=ignore>(read<f64>(%0), const<f64>(1.0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%2, from_bool<i32, reason=arg>(ge<f64, exceptions=ignore>(read<f64>(%0), const<f64>(1.0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%2, from_bool<i32, reason=arg>(lt<f64, exceptions=ignore>(read<f64>(%0), const<f64>(1.0))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%2, from_bool<i32, reason=arg>(le<f64, exceptions=ignore>(read<f64>(%0), const<f64>(1.0))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%2, or<i32>(from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(read<f64>(%0), const<f64>(1.0))), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(read<f64>(%0), const<f64>(1.0)))), const<i32>(1));
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
