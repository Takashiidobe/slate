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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([114, 101, 115, 32, 61, 61, 32, 40, 73, 78, 84, 95, 77, 73, 78, 32, 43, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, {{[0-9]+}}> [storage=static] = code_units<array<i8, {{[0-9]+}}>>({{\[[0-9, ]+\]}}) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([109, 97, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___assert_fail:[0-9]+]] @__assert_fail(%[[VALUE___assertion:[0-9]+]] __assertion: ptr<const i8>, %[[VALUE___file:[0-9]+]] __file: ptr<const i8>, %[[VALUE___line:[0-9]+]] __line: u32, %[[VALUE___function:[0-9]+]] __function: ptr<const i8>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: ptr<i32>, %[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_min:[0-9]+]] min: i32 [storage=automatic] = const<i32>(999999);
// DEFAULT-NEXT:         let %[[VALUE_bla:[0-9]+]] bla: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_min]]))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_min]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_bla]], const<i32>(1));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_bla]]), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_min]]);
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_min]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_min]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: array<i32, 6> [storage=automatic] [align=16] = aggregate<array<i32, 6>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(1), index2 = neg<i32, overflow=ub>(const<i32>(13)), index3 = const<i32>(2147483647), index4 = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), index5 = const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_res:[0-9]+]] res: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[VALUE_foo]], array_decay<ptr<i32>, length=Some(6)>(%[[VALUE_a_2]]), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(div<u64, by_zero=ub>(const<u64>(24), const<u64>(4)))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_res]]), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(1)))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(ptr<const i8>, ptr<const i8>, u32, ptr<const i8>) -> void>(%[[VALUE___assert_fail]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some({{[0-9]+}})>(%[[VALUE_str_2]])), reinterpret<u32, reason=arg, fits=always>(const<i32>(36)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
