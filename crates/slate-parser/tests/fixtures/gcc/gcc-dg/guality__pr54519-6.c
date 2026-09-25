/* PR debug/54519 */
/* { dg-do run } */
/* { dg-options "-g" } */

#if defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#define NOP "nop 0"
#elif defined(__MMIX__)
#define NOP "swym 0"
#elif defined(__or1k__)
#define NOP "l.nop"
#else
#define NOP "nop"
#endif

static inline void f1(int x, int y) {
  asm volatile(
      NOP); /* { dg-final { gdb-test .+1 "x" "2" { xfail { aarch64*-*-* && { any-opts "-Os" } } } } } */
  asm volatile(NOP); /* { dg-final { gdb-test . "y" "0" } } */
}

static inline void f2(int z) {
  f1(z, 0);
  f1(z, 1);
}

int
main() {
  f2(2);
  f2(3);
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
// DEFAULT-NEXT:     fn %0 @f1(%1 x: i32, %2 y: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "nop";
// DEFAULT-NEXT:         asm volatile "nop";
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f2(%4 z: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%0, read<i32>(%4), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%0, read<i32>(%4), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(3));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
