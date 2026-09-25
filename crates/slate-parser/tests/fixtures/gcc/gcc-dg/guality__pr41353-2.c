/* PR debug/41353 */
/* { dg-do run } */
/* { dg-options "-g" } */

int varh;
int vari __attribute__((used)) = 17, varj;

__attribute__((noinline)) int f1(void) {
  int vari1 = 2 * vari; /* { dg-final { gdb-test .+2 "vari1" "2 * 17" } } */
  int vari2 = 3 * vari; /* { dg-final { gdb-test .+1 "vari2" "3 * 17" } } */
  return varj;
}

int (*volatile fnp1)(void) = f1;

int
main(int argc, char *argv[]) {
  asm volatile("" : : "r"(&fnp1) : "memory");
  fnp1();
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
// DEFAULT-NEXT:     global %0 varh: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 vari: i32 [storage=static] = const<i32>(17) [linkage=external] [used];
// DEFAULT-NEXT:     global %2 varj: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 fnp1: volatile ptr<fn() -> i32> [storage=static] = function_decay<ptr<fn() -> i32>>(%3) [linkage=external];
// DEFAULT-NEXT:     fn %3 @f1() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 vari1: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(2), read<i32>(%1));
// DEFAULT-NEXT:         let %5 vari2: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(3), read<i32>(%1));
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main(%8 argc: i32, %9 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" addr_of<ptr<volatile ptr<fn() -> i32>>>(%6);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(read<ptr<fn() -> i32>, volatile>(%6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
