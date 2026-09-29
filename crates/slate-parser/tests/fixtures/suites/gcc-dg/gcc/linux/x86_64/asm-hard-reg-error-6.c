/* { dg-do compile { target lra } } */

#if defined __hppa__
# define R0 "20"
# define R1 "21"
#elif defined __AVR__
# define R0 "20"
# define R1 "24"
#elif defined __PRU__
# define R0 "0"
# define R1 "4"
#else
# define R0 "0"
# define R1 "1"
#endif

int
test (void)
{
  int x;

  __asm__ ("" : "={"R0"}{"R1"}" (x));            /* { dg-error "multiple hard register constraints in one alternative are not supported" } */
  __asm__ ("" : "={"R0"}m{"R1"}" (x) : "r" (x)); /* { dg-error "multiple hard register constraints in one alternative are not supported" } */
  __asm__ ("" : "={"R0"}m" (x) : "r" (x));
  __asm__ ("" : "=r" (x) : "{"R0"}{"R1"}" (x));  /* { dg-error "multiple hard register constraints in one alternative are not supported" } */
  __asm__ ("" : "=r" (x) : "{"R0"}i{"R1"}" (x)); /* { dg-error "multiple hard register constraints in one alternative are not supported" } */
  __asm__ ("" : "=r" (x) : "{"R0"}i" (x));

  __asm__ ("" : "={"R0"}r" (x) : "r" (x)); /* { dg-error "hard register constraints and regular register constraints in one alternative are not supported" } */
  __asm__ ("" : "=r{"R0"}" (x) : "r" (x)); /* { dg-error "hard register constraints and regular register constraints in one alternative are not supported" } */
  __asm__ ("" : "=r" (x) : "{"R0"}r" (x)); /* { dg-error "hard register constraints and regular register constraints in one alternative are not supported" } */
  __asm__ ("" : "=r" (x) : "r{"R0"}" (x)); /* { dg-error "hard register constraints and regular register constraints in one alternative are not supported" } */

  return x;
}

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
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "{0}{1}" [{0}{1}] width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "{0}m{1}" [{0}m{1}] width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             lateout 0 "{0}m" [unresolved("{") | unresolved("0") | unresolved("}") | mem] -> mem width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 1 "{0}{1}" [{0}{1}] width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 1 "{0}i{1}" [{0}i{1}] width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] [alternative=none] {
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 1 "{0}i" [unresolved("{") | unresolved("0") | unresolved("}") | imm | sym] width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             rejected: 0 (operand 1: unresolved("{"));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "{0}r" [unresolved("{") | unresolved("0") | unresolved("}") | reg] -> reg width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "r{0}" [reg | unresolved("{") | unresolved("0") | unresolved("}")] -> reg width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 1 "{0}r" [unresolved("{") | unresolved("0") | unresolved("}") | reg] -> reg width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 1 "r{0}" [reg | unresolved("{") | unresolved("0") | unresolved("}")] -> reg width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
