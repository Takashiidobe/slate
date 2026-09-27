/* { dg-do compile { target aarch64*-*-* arm*-*-* i?86-*-* powerpc*-*-* pru*-*-* riscv*-*-* s390*-*-* x86_64-*-* } } */

#if defined (__aarch64__)
# define GPR1_RAW "x0"
# define GPR2 "{x1}"
# define GPR3 "{x2}"
# define INVALID_GPR_A "{x31}"
#elif defined (__arm__)
# define GPR1_RAW "r0"
# define GPR2 "{r1}"
# define GPR3 "{r2}"
# define INVALID_GPR_A "{r16}"
#elif defined (__i386__)
# define GPR1_RAW "%eax"
# define GPR2 "{%ebx}"
# define GPR3 "{%edx}"
# define INVALID_GPR_A "{%eex}"
#elif defined (__powerpc__) || defined (__POWERPC__)
# define GPR1_RAW "r4"
# define GPR2 "{r5}"
# define GPR3 "{r6}"
# define INVALID_GPR_A "{r33}"
#elif defined (__PRU__)
# define GPR1_RAW "r20"
# define GPR2 "{r21}"
# define GPR3 "{r22}"
# define INVALID_GPR_A "{r34}"
#elif defined (__riscv)
# define GPR1_RAW "t4"
# define GPR2 "{t5}"
# define GPR3 "{t6}"
# define INVALID_GPR_A "{t7}"
#elif defined (__s390__)
# define GPR1_RAW "r4"
# define GPR2 "{r5}"
# define GPR3 "{r6}"
# define INVALID_GPR_A "{r17}"
#elif defined (__x86_64__)
# define GPR1_RAW "rax"
# define GPR2 "{rbx}"
# define GPR3 "{rcx}"
# define INVALID_GPR_A "{rex}"
#endif

#define GPR1 "{"GPR1_RAW"}"
#define INVALID_GPR_B "{"GPR1_RAW

struct { int a[128]; } s = {0};

void
test (void)
{
  int x, y;
  register int gpr1 __asm__ (GPR1_RAW) = 0;

  __asm__ ("" :: "{}" (42)); /* { dg-error "invalid input constraint: \{\}" } */
  __asm__ ("" :: INVALID_GPR_A (42)); /* { dg-error "invalid input constraint" } */
  __asm__ ("" :: INVALID_GPR_B (42)); /* { dg-error "invalid input constraint" } */

  __asm__ ("" :: GPR1 (s)); /* { dg-error "data type isn't suitable for register .* of operand 0" } */

  __asm__ ("" :: "r" (gpr1), GPR1 (42)); /* { dg-error "multiple inputs to hard register" } */
  __asm__ ("" :: GPR1 (42), "r" (gpr1)); /* { dg-error "multiple inputs to hard register" } */
  __asm__ ("" :: GPR1 (42), GPR1 (42)); /* { dg-error "multiple inputs to hard register" } */
  __asm__ ("" :: GPR1","GPR2 (42), GPR2","GPR3 (42));
  __asm__ ("" :: GPR1","GPR2 (42), GPR3","GPR2 (42)); /* { dg-error "multiple inputs to hard register" } */
  __asm__ ("" :: GPR1","GPR2 (42), GPR1","GPR3 (42)); /* { dg-error "multiple inputs to hard register" } */
  __asm__ ("" : "+"GPR1 (x), "="GPR1 (y)); /* { dg-error "multiple outputs to hard register" } */
  __asm__ ("" : "="GPR1 (y) : GPR1 (42), "0" (42)); /* { dg-error "multiple inputs to hard register" } */
  __asm__ ("" : "+"GPR1 (x) : GPR1 (42)); /* { dg-error "multiple inputs to hard register" } */

  __asm__ ("" : "="GPR1 (gpr1));
  __asm__ ("" : "="GPR2 (gpr1)); /* { dg-error "constraint and register 'asm' for output operand 0 are unsatisfiable" } */
  __asm__ ("" :: GPR2 (gpr1)); /* { dg-error "constraint and register 'asm' for input operand 0 are unsatisfiable" } */
  __asm__ ("" : "="GPR1 (x) : "0" (gpr1));
  __asm__ ("" : "="GPR2 (x) : "0" (gpr1)); /* { dg-error "constraint and register 'asm' for input operand 0 are unsatisfiable" } */

  __asm__ ("" : "=&"GPR1 (x) : "0" (gpr1));
  __asm__ ("" : "=&"GPR1 (x) : "0" (42));
  __asm__ ("" : "=&"GPR2","GPR1 (x) : "r,"GPR1 (42));
  __asm__ ("" : "="GPR2",&"GPR1 (x) : "r,"GPR1 (42)); /* { dg-error "invalid hard register usage between earlyclobber operand and input operand" } */
  __asm__ ("" : "=&"GPR1 (x) : GPR1 (42)); /* { dg-error "invalid hard register usage between earlyclobber operand and input operand" } */
  __asm__ ("" : "=&"GPR2","GPR1 (x) : "r,r" (gpr1));
  __asm__ ("" : "="GPR2",&"GPR1 (x) : "r,r" (gpr1)); /* { dg-error "invalid hard register usage between earlyclobber operand and input operand" } */
  __asm__ ("" : "=&r" (gpr1) : GPR1 (42)); /* { dg-error "invalid hard register usage between earlyclobber operand and input operand" } */
  __asm__ ("" : "=&"GPR1 (x),  "=r" (y) : "1" (gpr1)); /* { dg-error "invalid hard register usage between earlyclobber operand and input operand" } */
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 128>;
// DEFAULT-NEXT:     } [size=512, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %1 s: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<i32, 128>, zero_fill=true>(index0 = const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %2 @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 y: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 gpr1: i32 [storage=automatic] [register="rax"] = const<i32>(0);
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             in 0 "{}" [{}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             in 0 "{rex}" [{rex}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             in 0 "{rax" [unresolved("{") | reg | {ax} | xmm_reg] -> reg width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             in 0 "{rax}" [{ax}] width 4096 read<@type0>(%1);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             in 0 "r" [reg] width 32 read<i32>(%5);
// DEFAULT-NEXT:             in 1 "{rax}" [{ax}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             in 0 "{rax}" [{ax}] width 32 const<i32>(42);
// DEFAULT-NEXT:             in 1 "r" [reg] width 32 read<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             in 0 "{rax}" [{ax}] width 32 const<i32>(42);
// DEFAULT-NEXT:             in 1 "{rax}" [{ax}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] [alternative=0] {
// DEFAULT-NEXT:             in 0 "{rax},{rbx}" [{ax}, {bx}] width 32 const<i32>(42);
// DEFAULT-NEXT:             in 1 "{rbx},{rcx}" [{bx}, {cx}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] [alternative=0] {
// DEFAULT-NEXT:             in 0 "{rax},{rbx}" [{ax}, {bx}] width 32 const<i32>(42);
// DEFAULT-NEXT:             in 1 "{rcx},{rbx}" [{cx}, {bx}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] [alternative=0] {
// DEFAULT-NEXT:             in 0 "{rax},{rbx}" [{ax}, {bx}] width 32 const<i32>(42);
// DEFAULT-NEXT:             in 1 "{rax},{rcx}" [{ax}, {cx}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "{rax}" [{ax}] width 32 place<i32>(%3);
// DEFAULT-NEXT:             lateout 1 "{rax}" [{ax}] width 32 place<i32>(%4);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "{rax}" [{ax}] width 32 place<i32>(%4) from const<i32>(42);
// DEFAULT-NEXT:             in 1 "{rax}" [{ax}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "{rax}" [{ax}] width 32 place<i32>(%3);
// DEFAULT-NEXT:             in 1 "{rax}" [{ax}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "{rax}" [{ax}] width 32 place<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "{rbx}" [{bx}] width 32 place<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             in 0 "{rbx}" [{bx}] width 32 read<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "{rax}" [{ax}] width 32 place<i32>(%3) from read<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "{rbx}" [{bx}] width 32 place<i32>(%3) from read<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inout 0 "{rax}" [{ax}] width 32 place<i32>(%3) from read<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inout 0 "{rax}" [{ax}] width 32 place<i32>(%3) from const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] [alternative=0] {
// DEFAULT-NEXT:             out 0 "{rbx},{rax}" [{bx}, {ax}] width 32 place<i32>(%3);
// DEFAULT-NEXT:             in 1 "r,{rax}" [reg, {ax}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] [alternative=0] {
// DEFAULT-NEXT:             out 0 "{rbx},{rax}" [{bx}, {ax}] width 32 place<i32>(%3);
// DEFAULT-NEXT:             in 1 "r,{rax}" [reg, {ax}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             out 0 "{rax}" [{ax}] width 32 place<i32>(%3);
// DEFAULT-NEXT:             in 1 "{rax}" [{ax}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] [alternative=0] {
// DEFAULT-NEXT:             out 0 "{rbx},{rax}" [{bx}, {ax}] width 32 place<i32>(%3);
// DEFAULT-NEXT:             in 1 "r,r" [reg, reg] width 32 read<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] [alternative=0] {
// DEFAULT-NEXT:             out 0 "{rbx},{rax}" [{bx}, {ax}] width 32 place<i32>(%3);
// DEFAULT-NEXT:             in 1 "r,r" [reg, reg] width 32 read<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             out 0 "r" [reg] width 32 place<i32>(%5);
// DEFAULT-NEXT:             in 1 "{rax}" [{ax}] width 32 const<i32>(42);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             out 0 "{rax}" [{ax}] width 32 place<i32>(%3);
// DEFAULT-NEXT:             inlateout 1 "r" [reg] width 32 place<i32>(%4) from read<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
