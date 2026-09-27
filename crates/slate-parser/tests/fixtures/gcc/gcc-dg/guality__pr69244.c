/* PR debug/69244 */
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

union U {
  float f;
  int   i;
};
float a, b;

__attribute__((noinline, noclone)) void foo(void) {
  asm volatile("" : : "g"(&a), "g"(&b) : "memory");
}

int
main() {
  float e = a;
  foo();
  float   d = e;
  union U p;
  p.f = d += 2;
  int c    = p.i - 4;
  asm(NOP : : : "memory");
  b = c;
  return 0;
}

/* { dg-final { gdb-test 25 "c" "p.i-4" } } */



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
// DEFAULT-NEXT:     type @type0 U = union {
// DEFAULT-NEXT:         field0 f: f32;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     global %1 a: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<f32>>(%1);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<f32>>(%2);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 e: f32 [storage=automatic] = read<f32>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         let %6 d: f32 [storage=automatic] = read<f32>(%5);
// DEFAULT-NEXT:         let %7 p: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %9: f32 [synthetic] = read<f32>(%6);
// DEFAULT-NEXT:         let %10: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%9), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)));
// DEFAULT-NEXT:         write<f32>(%6, read<f32>(%10));
// DEFAULT-NEXT:         write<f32>(field0(%7), read<f32>(%10));
// DEFAULT-NEXT:         let %8 c: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(field1(%7)), const<i32>(4));
// DEFAULT-NEXT:         asm "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<f32>(%2, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%8)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
