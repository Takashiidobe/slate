/* Make sure if conversion for two instructions does not break
   anything (if it runs).  */

/* { dg-do run } */
/* { dg-options "-O2 -std=c99" } */
/* { dg-require-effective-target int32plus } */

#include <limits.h>
#include <assert.h>

__attribute__ ((noinline))
int foo (int *a, int n)
{
  int min = 999999;
  int bla = 0;
  for (int i = 0; i < n; i++)
    {
      if (a[i] < min)
	{
	  min = a[i];
	  bla = 1;
	}
    }

  if (bla)
    min += 1;
  return min;
}

int main()
{
  int a[] = {2, 1, -13, INT_MAX, INT_MIN, 0};

  int res = foo (a, sizeof (a) / sizeof (a[0]));

  assert (res == (INT_MIN + 1));
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     global %15 .str15: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([114, 101, 115, 32, 61, 61, 32, 40, 73, 78, 84, 95, 77, 73, 78, 32, 43, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, {{[0-9]+}}> [storage=static] = code_units<array<i8, {{[0-9]+}}>>({{\[[0-9, ]+\]}}) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([109, 97, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @__assert_fail(%10 __assertion: ptr<const i8>, %11 __file: ptr<const i8>, %12 __line: u32, %13 __function: ptr<const i8>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @foo(%2 a: ptr<i32>, %3 n: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 min: i32 [storage=automatic] = const<i32>(999999);
// DEFAULT-NEXT:         let %5 bla: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %6 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), read<i32>(%3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%2), read<i32>(%6)))), read<i32>(%4))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%4, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%2), read<i32>(%6)))));
// DEFAULT-NEXT:                             write<i32>(%5, const<i32>(1));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             let %20: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:             let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%4, read<i32>(%21));
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 a: array<i32, 6> [storage=automatic] [align=16] = aggregate<array<i32, 6>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(1), index2 = neg<i32, overflow=ub>(const<i32>(13)), index3 = const<i32>(2147483647), index4 = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), index5 = const<i32>(0));
// DEFAULT-NEXT:         let %9 res: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>, i32) -> i32>(%1, array_decay<ptr<i32>, length=Some(6)>(%8), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(div<u64, by_zero=ub>(const<u64>(24), const<u64>(4)))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%9), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(ptr<const i8>, ptr<const i8>, u32, ptr<const i8>) -> void>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%15)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some({{[0-9]+}})>(%16)), reinterpret<u32, reason=arg, fits=always>(const<i32>(36)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%17)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
