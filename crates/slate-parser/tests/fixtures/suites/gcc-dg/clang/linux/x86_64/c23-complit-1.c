/* Test storage duration of compound literals in parameter lists for C23.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

extern void abort (void);
extern void exit (int);

int x;

void f (int a[(int) { x }]);

int *q;

int
fp (int *p)
{
  q = p;
  return 1;
}

void
g (int a, int b[fp ((int [2]) { a, a + 2 })])
{
  if (q[0] != a || q[1] != a + 2)
    abort ();
}

int
main (void)
{
  int t[1] = { 0 };
  g (1, t);
  g (2, t);
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_a:[0-9]+]] a: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fp:[0-9]+]] @fp(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q]], read<ptr<i32>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: ptr<i32> [array=%[[VALUE1:[0-9]+]]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE1]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_fp]], array_decay<ptr<i32>, length=Some(2)>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = read<i32>(%[[VALUE_a_2]]), index1 = add<i32, overflow=ub>(read<i32>(%[[VALUE_a_2]]), const<i32>(2)))))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_q]]), const<i32>(0)))), read<i32>(%[[VALUE_a_2]])), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_q]]), const<i32>(1)))), add<i32, overflow=ub>(read<i32>(%[[VALUE_a_2]]), const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: array<i32, 1> [storage=automatic] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i32>) -> void>(%[[VALUE_g]], const<i32>(1), array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_t]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i32>) -> void>(%[[VALUE_g]], const<i32>(2), array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_t]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
