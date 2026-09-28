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

__attribute__((noinline, noclone)) unsigned
foo(unsigned a, unsigned b, unsigned c, unsigned d, unsigned e) {
  unsigned f = b + c; /* { dg-final { gdb-test pr58791-3.c:19 "f" "5" } } */
  unsigned g = a - f; /* { dg-final { gdb-test pr58791-3.c:19 "g" "24" } } */
  unsigned h = d + e; /* { dg-final { gdb-test pr58791-3.c:19 "h" "9" } } */
  unsigned i = g - h; /* { dg-final { gdb-test pr58791-3.c:19 "i" "15" } } */
  unsigned j = f + 1; /* { dg-final { gdb-test pr58791-3.c:19 "j" "6" } } */
  unsigned k = g + 1; /* { dg-final { gdb-test pr58791-3.c:19 "k" "25" } } */
  unsigned l = h + 1; /* { dg-final { gdb-test pr58791-3.c:19 "l" "10" } } */
  unsigned m = i + 1; /* { dg-final { gdb-test pr58791-3.c:19 "m" "16" } } */
  asm volatile(NOP : : : "memory");
  asm volatile(NOP : : : "memory");
  return i;
}

int
main() {
  foo(29, 2, 3, 4, 5);
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
// DEFAULT-NEXT:     fn %0 @foo(%1 a: u32, %2 b: u32, %3 c: u32, %4 d: u32, %5 e: u32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 f: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%2), read<u32>(%3));
// DEFAULT-NEXT:         let %7 g: u32 [storage=automatic] = sub<u32, overflow=wrap>(read<u32>(%1), read<u32>(%6));
// DEFAULT-NEXT:         let %8 h: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%4), read<u32>(%5));
// DEFAULT-NEXT:         let %9 i: u32 [storage=automatic] = sub<u32, overflow=wrap>(read<u32>(%7), read<u32>(%8));
// DEFAULT-NEXT:         let %10 j: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %11 k: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %12 l: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %13 m: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%9), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<u32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<u32, signature=fn(u32, u32, u32, u32, u32) -> u32>(%0, reinterpret<u32, reason=arg, fits=always>(const<i32>(29)), reinterpret<u32, reason=arg, fits=always>(const<i32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(3)), reinterpret<u32, reason=arg, fits=always>(const<i32>(4)), reinterpret<u32, reason=arg, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
