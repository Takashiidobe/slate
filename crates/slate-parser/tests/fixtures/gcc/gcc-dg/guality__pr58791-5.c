/* PR tree-optimization/58791 */
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

__attribute__((noinline, noclone)) unsigned int
foo(unsigned int a0, unsigned int a1, unsigned int a2, unsigned int a3,
    unsigned int a4) {
  unsigned int b0, b1, b2, b3, b4, e;
  /* this can be optimized to four additions... */
  b4 = a4 + a3 + a2 + a1 +
       a0; /* { dg-final { gdb-test pr58791-5.c:20 "b4" "4681" } } */
  b3 = a3 + a2 + a1 +
       a0;           /* { dg-final { gdb-test pr58791-5.c:20 "b3" "585" } } */
  b2 = a2 + a1 + a0; /* { dg-final { gdb-test pr58791-5.c:20 "b2" "73" } } */
  b1 = a1 + a0;      /* { dg-final { gdb-test pr58791-5.c:20 "b1" "9" } } */
  /* This is actually 0 */
  e  = b4 - b3 + b2 - b1 - a4 -
       a2; /* { dg-final { gdb-test pr58791-5.c:20 "e" "0" } } */
  asm volatile(NOP : : : "memory");
  asm volatile(NOP : : : "memory");
  return e;
}

int
main() {
  foo(1, 8, 64, 512, 4096);
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
// DEFAULT-NEXT:     fn %0 @foo(%1 a0: u32, %2 a1: u32, %3 a2: u32, %4 a3: u32, %5 a4: u32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 b0: u32 [storage=automatic];
// DEFAULT-NEXT:         let %7 b1: u32 [storage=automatic];
// DEFAULT-NEXT:         let %8 b2: u32 [storage=automatic];
// DEFAULT-NEXT:         let %9 b3: u32 [storage=automatic];
// DEFAULT-NEXT:         let %10 b4: u32 [storage=automatic];
// DEFAULT-NEXT:         let %11 e: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%10, add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(%5), read<u32>(%4)), read<u32>(%3)), read<u32>(%2)), read<u32>(%1)));
// DEFAULT-NEXT:         write<u32>(%9, add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(%4), read<u32>(%3)), read<u32>(%2)), read<u32>(%1)));
// DEFAULT-NEXT:         write<u32>(%8, add<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(%3), read<u32>(%2)), read<u32>(%1)));
// DEFAULT-NEXT:         write<u32>(%7, add<u32, overflow=wrap>(read<u32>(%2), read<u32>(%1)));
// DEFAULT-NEXT:         write<u32>(%11, sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(sub<u32, overflow=wrap>(read<u32>(%10), read<u32>(%9)), read<u32>(%8)), read<u32>(%7)), read<u32>(%5)), read<u32>(%3)));
// DEFAULT-NEXT:         asm volatile "nop" {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "nop" {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<u32>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<u32, signature=fn(u32, u32, u32, u32, u32) -> u32>(%0, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(8)), reinterpret<u32, reason=arg, fits=always>(const<i32>(64)), reinterpret<u32, reason=arg, fits=always>(const<i32>(512)), reinterpret<u32, reason=arg, fits=always>(const<i32>(4096)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
