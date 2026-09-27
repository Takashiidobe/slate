/* { dg-do compile { target lra } } */

/* Verify output operands.  */

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
  register int y __asm__ (R0);

  /* Preserve status quo and don't error out.  */
  __asm__ ("" : "=r" (x), "=r" (x));

  /* Be more strict for hard register constraints and error out.  */
  __asm__ ("" : "={"R0"}" (x), "={"R1"}" (x)); /* { dg-error "multiple outputs to lvalue 'x'" } */

  /* Still error out in case of a mixture.  */
  __asm__ ("" : "=r" (x), "={"R1"}" (x)); /* { dg-error "multiple outputs to lvalue 'x'" } */

  return x + y;
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
// DEFAULT-NEXT:     fn %0 @test() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %1 x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %2 y: i32 [storage=automatic] [register="0"];
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%1);
// DEFAULT-NEXT:             lateout 1 "r" [reg] width 32 place<i32>(%1);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "{0}" [{ax}] width 32 place<i32>(%1);
// DEFAULT-NEXT:             lateout 1 "{1}" [{dx}] width 32 place<i32>(%1);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%1);
// DEFAULT-NEXT:             lateout 1 "{1}" [{dx}] width 32 place<i32>(%1);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%1), read<i32>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
