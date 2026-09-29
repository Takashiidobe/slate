/* PR tree-optimization/88800 - Spurious -Werror=array-bounds for non-taken
   branch
   Verify that out-of-bounds memcpy calls are not folded when warnings are
   enabled (builtin-memcpy-2.c verifies they're not folded with warnings
   disabled).
   { dg-do compile }
   { dg-options "-O2 -Wall -fdump-tree-optimized" } */

extern void* memcpy (void*, const void*, __SIZE_TYPE__);

char a1[1], a2[2], a4[4], a8[8], a16[16], a32[32];

void f1 (const void *p)
{
  memcpy (a1, p, sizeof a1 * 2);    /* { dg-warning "\\\[-Warray-bounds" } */
}

void f2 (const void *p)
{
  memcpy (a2, p, sizeof a2 * 2);    /* { dg-warning "\\\[-Warray-bounds" } */
}

void f4 (const void *p)
{
  memcpy (a4, p, sizeof a4 * 2);    /* { dg-warning "\\\[-Warray-bounds" } */
}

void f8 (const void *p)
{
  memcpy (a8, p, sizeof a8 * 2);    /* { dg-warning "\\\[-Warray-bounds" } */
}

void f16 (const void *p)
{
  memcpy (a16, p, sizeof a16 * 2);  /* { dg-warning "\\\[-Warray-bounds" } */
}

void f32 (const void *p)
{
  memcpy (a32, p, sizeof a32 * 2);  /* { dg-warning "\\\[-Warray-bounds" } */
}

/* { dg-final { scan-tree-dump-times "memcpy" 6 "optimized" } } */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %[[VALUE_a1:[0-9]+]] a1: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a2:[0-9]+]] a2: array<i8, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a4:[0-9]+]] a4: array<i8, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a8:[0-9]+]] a8: array<i8, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a16:[0-9]+]] a16: array<i8, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a32:[0-9]+]] a32: array<i8, 32> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_p:[0-9]+]] p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_a1]])), read<ptr<const void>>(%[[VALUE_p]]), mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_p_2:[0-9]+]] p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]])), read<ptr<const void>>(%[[VALUE_p_2]]), mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_p_3:[0-9]+]] p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_a4]])), read<ptr<const void>>(%[[VALUE_p_3]]), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_p_4:[0-9]+]] p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a8]])), read<ptr<const void>>(%[[VALUE_p_4]]), mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f16:[0-9]+]] @f16(%[[VALUE_p_5:[0-9]+]] p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_a16]])), read<ptr<const void>>(%[[VALUE_p_5]]), mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f32:[0-9]+]] @f32(%[[VALUE_p_6:[0-9]+]] p: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%[[VALUE_a32]])), read<ptr<const void>>(%[[VALUE_p_6]]), mul<u64, overflow=wrap>(const<u64>(32), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
