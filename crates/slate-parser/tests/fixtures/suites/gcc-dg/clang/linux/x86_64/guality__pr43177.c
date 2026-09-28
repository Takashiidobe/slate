/* { dg-do run } */
/* { dg-options "-g" } */

void __attribute__((noinline)) bar(long x) {
  asm volatile("" : : "r"(x) : "memory");
}

long __attribute__((noinline)) foo(long x) {
  long l = x + 3;
  bar(l); /* { dg-final { gdb-test .+1 "l" "10" } } */
  bar(l); /* { dg-final { gdb-test . "x" "7" } } */
  return l;
}

long __attribute__((noinline)) baz(int x) {
  long l = x + 3;
  bar(l); /* { dg-final { gdb-test .+1 "l" "10" } } */
  bar(l); /* { dg-final { gdb-test . "x" "7" } } */
  return l;
}

int
main(void) {
  int i;
  asm volatile("" : "=r"(i) : "0"(7));
  foo(i);
  baz(i);
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
// DEFAULT-NEXT:     fn %0 @bar(%1 x: i64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             in 0 "r" [reg] width 64 read<i64>(%1);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @foo(%3 x: i64) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 l: i64 [storage=automatic] = add<i64, overflow=ub>(read<i64>(%3), widen<i64, reason=usual_arith>(const<i32>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%0, read<i64>(%4));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%0, read<i64>(%4));
// DEFAULT-NEXT:         return read<i64>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @baz(%6 x: i32) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 l: i64 [storage=automatic] = widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%6), const<i32>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%0, read<i64>(%7));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%0, read<i64>(%7));
// DEFAULT-NEXT:         return read<i64>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%9) from const<i32>(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i64, signature=fn(i64) -> i64>(%2, widen<i64, reason=arg>(read<i32>(%9)));
// DEFAULT-NEXT:         call<i64, signature=fn(i32) -> i64>(%5, read<i32>(%9));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
