/* { dg-do compile } */

unsigned long ok = 0UL;
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wtraditional"
unsigned long bad = 1UL; /* { dg-warning "suffix" } */
/* Note the extra space before the pragma on this next line: */
 #pragma GCC diagnostic pop
unsigned long ok_again = 2UL; /* { dg-bogus "suffix" } */

/* Redundant with the previous pop, but just shows that it fails to stop the
 * following warning with an unpatched GCC: */
#pragma GCC diagnostic ignored "-Wtraditional"

/* { dg-bogus "would be stringified" "" { target *-*-* } .+1 } */
#define UNW_DEC_PROLOGUE(fmt, body, rlen, arg) \
  do {									\
      unw_rlen = rlen;							\
      *(int *)arg = body;						\
      printf("    %s:%s(rlen=%lu)\n",					\
             fmt, (body ? "body" : "prologue"), (unsigned long)rlen);	\
  } while (0)

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
// DEFAULT-NEXT:     global %0 ok: u64 [storage=static] = const<u64>(0) [linkage=external];
// DEFAULT-NEXT:     global %1 bad: u64 [storage=static] = const<u64>(1) [linkage=external];
// DEFAULT-NEXT:     global %2 ok_again: u64 [storage=static] = const<u64>(2) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
