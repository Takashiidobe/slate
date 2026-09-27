/* PR debug/41353 */
/* { dg-do run } */
/* { dg-options "-g" } */

int vari __attribute__((used)) = 17, varj;

__attribute__((noinline)) int f1(void) {
  /* { dg-final { gdb-test .+7 "vari" "17" } } */
  int vari1 = 2 * vari; /* { dg-final { gdb-test .+6 "vari1" "2 * 17" } } */
  int vari2 = 3 * vari; /* { dg-final { gdb-test .+5 "vari2" "3 * 17" } } */
  int vari3 = 2 * vari; /* { dg-final { gdb-test .+4 "vari3" "2 * 17" } } */
  int vari4 = 3 * vari; /* { dg-final { gdb-test .+3 "vari4" "3 * 17" } } */
  int vari5 = 4 * vari; /* { dg-final { gdb-test .+2 "vari5" "4 * 17" } } */
  int vari6 = 5 * vari; /* { dg-final { gdb-test .+1 "vari6" "5 * 17" } } */
  return varj;
}

__attribute__((noinline)) int f2(int i, int j) {
  j      += i;
  /* { dg-final { gdb-test .+4 "i" "37" } } */
  /* { dg-final { gdb-test .+3 "j" "28 + 37" { xfail { { ! aarch64-*-* } && { no-opts "-O0" } } } } } */
  int i1  = 2 * i; /* { dg-final { gdb-test .+2 "i1" "2 * 37" } } */
  int i2  = 3 * i; /* { dg-final { gdb-test .+1 "i2" "3 * 37" } } */
  return j;
}

__attribute__((noinline)) int f3(int i) {
  asm volatile("" : "+r"(i));
  /* { dg-final { gdb-test .+4 "i" "12" } } */
  int i1 = 2 * i; /* { dg-final { gdb-test .+3 "i1" "2 * 12" } } */
  int i2 = 2 * i; /* { dg-final { gdb-test .+2 "i2" "2 * 12" } } */
  int i3 = 3 * i; /* { dg-final { gdb-test .+1 "i3" "3 * 12" } } */
  return i;
}

int (*volatile fnp1)(void)     = f1;
int (*volatile fnp2)(int, int) = f2;
int (*volatile fnp3)(int)      = f3;

int
main(int argc, char *argv[]) {
  asm volatile("" : : "r"(&fnp1) : "memory");
  asm volatile("" : : "r"(&fnp2) : "memory");
  asm volatile("" : : "r"(&fnp3) : "memory");
  fnp1();
  fnp2(37, 28);
  fnp3(12);
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
// DEFAULT-NEXT:     global %0 vari: i32 [storage=static] = const<i32>(17) [linkage=external] [used];
// DEFAULT-NEXT:     global %1 varj: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 fnp1: volatile ptr<fn() -> i32> [storage=static] = function_decay<ptr<fn() -> i32>>(%2) [linkage=external];
// DEFAULT-NEXT:     global %20 fnp2: volatile ptr<fn(i32, i32) -> i32> [storage=static] = function_decay<ptr<fn(i32, i32) -> i32>>(%9) [linkage=external];
// DEFAULT-NEXT:     global %21 fnp3: volatile ptr<fn(i32) -> i32> [storage=static] = function_decay<ptr<fn(i32) -> i32>>(%14) [linkage=external];
// DEFAULT-NEXT:     fn %2 @f1() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 vari1: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(2), read<i32>(%0));
// DEFAULT-NEXT:         let %4 vari2: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(3), read<i32>(%0));
// DEFAULT-NEXT:         let %5 vari3: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(2), read<i32>(%0));
// DEFAULT-NEXT:         let %6 vari4: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(3), read<i32>(%0));
// DEFAULT-NEXT:         let %7 vari5: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(4), read<i32>(%0));
// DEFAULT-NEXT:         let %8 vari6: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(5), read<i32>(%0));
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f2(%10 i: i32, %11 j: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %25: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), read<i32>(%10));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%26));
// DEFAULT-NEXT:         let %12 i1: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(2), read<i32>(%10));
// DEFAULT-NEXT:         let %13 i2: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(3), read<i32>(%10));
// DEFAULT-NEXT:         return read<i32>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f3(%15 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%15);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %16 i1: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(2), read<i32>(%15));
// DEFAULT-NEXT:         let %17 i2: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(2), read<i32>(%15));
// DEFAULT-NEXT:         let %18 i3: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(3), read<i32>(%15));
// DEFAULT-NEXT:         return read<i32>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @main(%23 argc: i32, %24 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "r" [reg] width 64 addr_of<ptr<volatile ptr<fn() -> i32>>>(%19);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "r" [reg] width 64 addr_of<ptr<volatile ptr<fn(i32, i32) -> i32>>>(%20);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "r" [reg] width 64 addr_of<ptr<volatile ptr<fn(i32) -> i32>>>(%21);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(read<ptr<fn() -> i32>, volatile>(%19));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>, volatile>(%20), const<i32>(37), const<i32>(28));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>, volatile>(%21), const<i32>(12));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
