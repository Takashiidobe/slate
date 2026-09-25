/* PR debug/54693 */
/* { dg-do run } */
/* { dg-options "-g" } */

int v;

__attribute__((noinline, noclone)) void bar(int i) {
  v = i;
  asm volatile("" : : "r"(i) : "memory");
}

__attribute__((noinline, noclone)) void foo(int x, int y, int z) {
  int i = 0;
  while (x > 3 && y > 3 &&
         z > 3) { /* { dg-final { gdb-test .+2 "i" "v + 1" } } */
    /* { dg-final { gdb-test .+1 "x" "10 - i" { xfail { aarch64*-*-* && { any-opts "-fno-fat-lto-objects" } } } } } */
    bar(i); /* { dg-final { gdb-test . "y" "20 - 2 * i" { xfail { aarch64*-*-* && { any-opts "-fno-fat-lto-objects" "-Os" } } } } } */
    /* { dg-final { gdb-test .-1 "z" "30 - 3 * i" { xfail { aarch64*-*-* && { any-opts "-fno-fat-lto-objects" "-Os" } } } } } */
    i++, x--, y -= 2, z -= 3;
  }
}

int
main() {
  v = -1;
  foo(10, 20, 30);
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
// DEFAULT-NEXT:     global %0 v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar(%2 i: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%0, read<i32>(%2));
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" read<i32>(%2);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo(%4 x: i32, %5 y: i32, %6 z: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %9 logical_and<bool>(logical_and<bool>(gt<i32>(read<i32>(%4), const<i32>(3)), gt<i32>(read<i32>(%5), const<i32>(3))), gt<i32>(read<i32>(%6), const<i32>(3)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%1, read<i32>(%7));
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%11));
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%13));
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%14), const<i32>(2));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%15));
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%16), const<i32>(3));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%17));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%0, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32) -> void>(%3, const<i32>(10), const<i32>(20), const<i32>(30));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
