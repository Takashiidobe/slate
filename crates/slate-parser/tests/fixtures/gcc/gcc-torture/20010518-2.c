// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-do compile } */
/* { dg-skip-if "Array too big" { "pdp11-*-*" } { "-mint32" } } */

/* Large static storage.  */

#include <limits.h>

static volatile char chars_1[INT_MAX / 2];
static volatile char chars_2[1];

int
foo (void)
{
  chars_1[10] = 'y';
  chars_2[0] = 'x';
}

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
// DEFAULT-NEXT:     global %0 chars_1: volatile array<i8, 1073741823> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %1 chars_2: volatile array<i8, 1> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %2 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8, volatile>(deref(ptr_offset<ptr<volatile i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<volatile i8>, length=Some(1073741823)>(%0), const<i32>(10))), truncate<i8, reason=assign, fits=always>(const<i32>(121)));
// DEFAULT-NEXT:         write<i8, volatile>(deref(ptr_offset<ptr<volatile i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<volatile i8>, length=Some(1)>(%1), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(120)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
