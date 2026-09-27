/* PR debug/43329 */
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

static inline void foo(int argx) {
  int varx = argx;
  __asm__ volatile(NOP); /* { dg-final { gdb-test .+1 "argx" "25" } } */
  __asm__ volatile(NOP
                   :
                   : "g"(varx)); /* { dg-final { gdb-test . "varx" "25" } } */
}

int i;

__attribute__((noinline)) void baz(int x) {
  asm volatile("" : : "r"(x) : "memory");
}

static inline void bar(void) {
  foo(25);
  i = i + 2;
  i = i * 2;
  i = i - 4;
  baz(i);
  i = i * 2;
  i = i >> 1;
  i = i << 6;
  baz(i);
  i = i + 2;
  i = i * 2;
  i = i - 4;
  baz(i);
  i = i * 2;
  i = i >> 6;
  i = i << 1;
  baz(i);
}

int
main(void) {
  __asm__ volatile("" : "=r"(i) : "0"(0));
  bar();
  bar();
  return i;
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
// DEFAULT-NEXT:     global %3 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo(%1 argx: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 varx: i32 [storage=automatic] = read<i32>(%1);
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att];
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm] -> reg width 32 read<i32>(%2);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @baz(%5 x: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "r" [reg] width 32 read<i32>(%5);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar() -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(25));
// DEFAULT-NEXT:         write<i32>(%3, add<i32, overflow=ub>(read<i32>(%3), const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(%3, mul<i32, overflow=ub>(read<i32>(%3), const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(%3, sub<i32, overflow=ub>(read<i32>(%3), const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, mul<i32, overflow=ub>(read<i32>(%3), const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(%3, shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%3), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%3, shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%3), const<i32>(6)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, add<i32, overflow=ub>(read<i32>(%3), const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(%3, mul<i32, overflow=ub>(read<i32>(%3), const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(%3, sub<i32, overflow=ub>(read<i32>(%3), const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, mul<i32, overflow=ub>(read<i32>(%3), const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(%3, shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%3), const<i32>(6)));
// DEFAULT-NEXT:         write<i32>(%3, shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%3), const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%3) from const<i32>(0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
