/* PR target/78796 */
/* { dg-do run } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-mcmodel=large -fno-pie -no-pie" { target aarch64-*-* } } */
/* { dg-require-effective-target tls_runtime } */
/* { dg-add-options tls } */

struct S { int a, b, c, d, e; };
struct S t;
__thread struct S s;

__attribute__((used, noinline, noclone)) void
foo (int *x, int *y)
{
  asm volatile ("" : : "g" (x), "g" (y) : "memory");
  if (*x != 1 || *y != 2)
    __builtin_abort ();
}

__attribute__((used, noinline, noclone)) void
bar (void)
{
  foo (&t.c, &s.c);
}

int
main ()
{
  t.c = 1;
  s.c = 2;
  bar ();
  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:         field4 e: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     global %1 t: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 s: @type0 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %8 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @foo(%4 x: ptr<i32>, %5 y: ptr<i32>) -> void [linkage=external] [used] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm | sym] -> reg width 64 read<ptr<i32>>(%4);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm | sym] -> reg width 64 read<ptr<i32>>(%5);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(read<ptr<i32>>(%4))), const<i32>(1)), ne<i32>(read<i32>(deref(read<ptr<i32>>(%5))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar() -> void [linkage=external] [used] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%3, addr_of<ptr<i32>>(field2(%1)), addr_of<ptr<i32>>(field2(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field2(%1), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field2(%2), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
