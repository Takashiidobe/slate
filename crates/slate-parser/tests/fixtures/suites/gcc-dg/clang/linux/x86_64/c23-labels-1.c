/* Tests for labels before declarations and at ends of compound statements.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

int f(int x) {
  goto b;
a:
  int i = 2 * x;
aa:
  int u = 0;
  int v = 0;
  goto c;
b:
  goto a;
  {
    i *= 3;
  c:
  }
  return i + u + v;
d:
}

int main(void) {
  if (2 != f(1))
    __builtin_abort();

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
// DEFAULT-NEXT:     fn %0 @f(%6 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         goto %3;
// DEFAULT-NEXT:         label %1 a:
// DEFAULT-NEXT:             let %7 i: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(2), read<i32>(%6));
// DEFAULT-NEXT:         label %2 aa:
// DEFAULT-NEXT:             let %8 u: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %9 v: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         goto %4;
// DEFAULT-NEXT:         label %3 b:
// DEFAULT-NEXT:             goto %1;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %12: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:             let %13: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%12), const<i32>(3));
// DEFAULT-NEXT:             write<i32>(%7, read<i32>(%13));
// DEFAULT-NEXT:             label %4 c:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%7), read<i32>(%8)), read<i32>(%9));
// DEFAULT-NEXT:         label %5 d:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
