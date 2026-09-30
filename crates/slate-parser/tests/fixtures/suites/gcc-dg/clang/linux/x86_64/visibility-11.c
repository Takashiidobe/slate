/* PR middle-end/20297 */
/* The memcpy FUNCTION_DECL built in the middle-end for block moves got
   hidden visibility from the first push, so the call didn't use the PLT.  */

/* { dg-do compile { target i?86-*-* x86_64-*-* } } */
/* { dg-skip-if "" { *-*-darwin* } } */
/* { dg-require-visibility "" } */
/* { dg-require-effective-target fpic } */
/* { dg-options "-Os -fpic -mstringop-strategy=libcall" } */
/* { dg-final { scan-assembler "memcpy@PLT" } } */

#pragma GCC visibility push(hidden)
#pragma GCC visibility push(default)
extern void* memcpy (void *, const void *, __SIZE_TYPE__);
#pragma GCC visibility pop

struct a { int a[4096]; };

extern void *bar (struct a *, struct a *, int);

void *
foo (struct a *a, struct a *b, int c)
{
  struct a cc = *b;
  return bar (a, &cc, 4 * c);
}

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
// DEFAULT-NEXT:     type @type[[TYPE_a:[0-9]+]] a = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 4096>;
// DEFAULT-NEXT:     } [size=16384, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [visibility=hidden];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE3:[0-9]+]] <unnamed>: ptr<@type[[TYPE_a]]>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<@type[[TYPE_a]]>, %[[VALUE5:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_a]]>, %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_a]]>, %[[VALUE_c:[0-9]+]] c: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_cc:[0-9]+]] cc: @type[[TYPE_a]] [storage=automatic] = copy<@type[[TYPE_a]], reason=assign>(read<@type[[TYPE_a]]>(deref(read<ptr<@type[[TYPE_a]]>>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(ptr<@type[[TYPE_a]]>, ptr<@type[[TYPE_a]]>, i32) -> ptr<void>>(%[[VALUE_bar]], read<ptr<@type[[TYPE_a]]>>(%[[VALUE_a]]), addr_of<ptr<@type[[TYPE_a]]>>(%[[VALUE_cc]]), mul<i32, overflow=ub>(const<i32>(4), read<i32>(%[[VALUE_c]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
