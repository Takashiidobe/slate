/* PR tree-optimization/78726 */
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

unsigned char b = 36, c = 173;
unsigned int  d;

__attribute__((noinline, noclone)) void foo(void) {
  unsigned a  = ~b;
  unsigned d1 = a * c; /* { dg-final { gdb-test 21 "d1" "~36U * 173" } } */
  unsigned d2 =
      d1 * c; /* { dg-final { gdb-test 21 "d2" "~36U * 173 * 173" } } */
  unsigned d3 = 1023094746 *
                a; /* { dg-final { gdb-test 21 "d3" "~36U * 1023094746" } } */
  d           = d2 + d3;
  unsigned d4 =
      d1 * 2; /* { dg-final { gdb-test .+3 "d4" "~36U * 173 * 2" } } */
  unsigned d5 =
      d2 * 2; /* { dg-final { gdb-test .+2 "d5" "~36U * 173 * 173 * 2" } } */
  unsigned d6 =
      d3 * 2; /* { dg-final { gdb-test .+1 "d6" "~36U * 1023094746 * 2" } } */
  asm(NOP : : : "memory");
}

int
main() {
  asm volatile("" : : "g"(&b), "g"(&c) : "memory");
  foo();
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
// DEFAULT-NEXT:     global %0 b: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(36))) [linkage=external];
// DEFAULT-NEXT:     global %1 c: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(173))) [linkage=external];
// DEFAULT-NEXT:     global %2 d: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 a: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0)))));
// DEFAULT-NEXT:         let %5 d1: u32 [storage=automatic] = mul<u32, overflow=wrap>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1)))));
// DEFAULT-NEXT:         let %6 d2: u32 [storage=automatic] = mul<u32, overflow=wrap>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1)))));
// DEFAULT-NEXT:         let %7 d3: u32 [storage=automatic] = mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1023094746)), read<u32>(%4));
// DEFAULT-NEXT:         write<u32>(%2, add<u32, overflow=wrap>(read<u32>(%6), read<u32>(%7)));
// DEFAULT-NEXT:         let %8 d4: u32 [storage=automatic] = mul<u32, overflow=wrap>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         let %9 d5: u32 [storage=automatic] = mul<u32, overflow=wrap>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         let %10 d6: u32 [storage=automatic] = mul<u32, overflow=wrap>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         asm "nop" [dialect=att] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm] width 64 addr_of<ptr<u8>>(%0);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm] width 64 addr_of<ptr<u8>>(%1);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
