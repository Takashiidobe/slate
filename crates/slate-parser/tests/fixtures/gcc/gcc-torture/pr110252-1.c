/* This is reduced from sel-sched.cc which was noticed was being miscompiled
 * too. */
int g(int min_need_stall) __attribute__((__noipa__));
int g(int min_need_stall) {
  return min_need_stall < 0 ? 1
                            : ((min_need_stall) < (1) ? (min_need_stall) : (1));
}
int main(void) {
  for (int i = -100; i <= 100; i++) {
    int t = g(i);
    if (t != (i != 0))
      __builtin_abort();
  }
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
// DEFAULT-NEXT:     fn %0 @g(%1 min_need_stall: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(lt<i32>(read<i32>(%1), const<i32>(0)), const<i32>(1), conditional<i32>(lt<i32>(read<i32>(%1), const<i32>(1)), read<i32>(%1), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %3 i: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(100));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%3), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %4 t: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%3));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%4), from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%3), const<i32>(0))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
