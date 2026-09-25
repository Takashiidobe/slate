/* PR debug/43479 */
/* { dg-do run } */
/* { dg-options "-g" } */

__attribute__((noinline)) void foo(int k, int l, int m, int n) {
  l++;
  {
    int h = n;
    {
      int i = k;
      k++; /* { dg-final { gdb-test . "i" "6" } } */
    } /* { dg-final { gdb-test .-1 "h" "9" } } */
    /* { dg-final { gdb-test .-2 "n" "9" } } */
    {
      int j = m;
      m++; /* { dg-final { gdb-test . "j" "8" } } */
    } /* { dg-final { gdb-test .-1 "h" "9" } } */
    /* { dg-final { gdb-test 12 "n" "9" } } */
  }
  asm volatile("" : : "r"(k), "r"(l));
  asm volatile("" : : "r"(m), "r"(n));
}

int
main(void) {
  int q = 6;
  asm("" : "+r"(q));
  foo(q, q + 1, q + 2, q + 3);
  return 0;
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
// DEFAULT-NEXT:     fn %0 @foo(%1 k: i32, %2 l: i32, %3 m: i32, %4 n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%11));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %5 h: i32 [storage=automatic] = read<i32>(%4);
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %6 i: i32 [storage=automatic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%13));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %7 j: i32 [storage=automatic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%15));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" read<i32>(%1);
// DEFAULT-NEXT:             in 1 "r" read<i32>(%2);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" read<i32>(%3);
// DEFAULT-NEXT:             in 1 "r" read<i32>(%4);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 q: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:         asm "" {
// DEFAULT-NEXT:             out 0 "+r" place<i32>(%9);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32) -> void>(%0, read<i32>(%9), add<i32, overflow=ub>(read<i32>(%9), const<i32>(1)), add<i32, overflow=ub>(read<i32>(%9), const<i32>(2)), add<i32, overflow=ub>(read<i32>(%9), const<i32>(3)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
