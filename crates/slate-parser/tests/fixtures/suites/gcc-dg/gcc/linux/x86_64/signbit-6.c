/* { dg-do run } */
/* { dg-options "-O1" } */

#include <stdint.h>
#include <limits.h>
#include <stdio.h>

#ifndef N
#define N 65
#endif

#ifndef TYPE
#define TYPE int32_t
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

#define BASE ((TYPE) -1 < 0 ? -126 : 4)

__attribute__ ((noinline, noipa))
void fun1(TYPE *x, int n)
{
    for (int i = 0; i < n; i++)
      x[i] = (-x[i]) >> 31;
}

__attribute__ ((noinline, noipa, optimize("O0")))
void fun2(TYPE *x, int n)
{
    for (int i = 0; i < n; i++)
      x[i] = (-x[i]) >> 31;
}

int main ()
{
  TYPE a[N];
  TYPE b[N];

  /* This will invoke UB due to -INT32_MIN.  The test is supposed to pass
     because GCC is supposed to handle this UB case in a predictable way.  */
  a[0] = INT32_MIN;
  b[0] = INT32_MIN;

  for (int i = 1; i < N; ++i)
    {
      a[i] = BASE + i * 13;
      b[i] = BASE + i * 13;
      if (DEBUG)
        printf ("%d: 0x%x\n", i, a[i]);
    }

  fun1 (a, N);
  fun2 (b, N);

  if (DEBUG)
    printf ("%d = 0x%x == 0x%x\n", 0, a[0], b[0]);

  if (a[0] != 0x0 || b[0] != -1)
        __builtin_abort ();


  for (int i = 1; i < N; ++i)
    {
      if (DEBUG)
        printf ("%d = 0x%x == 0x%x\n", i, a[i], b[i]);

      if (a[i] != b[i])
        __builtin_abort ();
    }
  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE___int32_t:[0-9]+]] __int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 58, 32, 48, 120, 37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 61, 32, 48, 120, 37, 120, 32, 61, 61, 32, 48, 120, 37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 61, 32, 48, 120, 37, 120, 32, 61, 61, 32, 48, 120, 37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fun1:[0-9]+]] @fun1(%[[VALUE_x:[0-9]+]] x: ptr<i32>, %[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
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
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x]]), read<i32>(%[[VALUE_i]]))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(neg<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x]]), read<i32>(%[[VALUE_i]]))))), const<i32>(31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fun2:[0-9]+]] @fun2(%[[VALUE_x_2:[0-9]+]] x: ptr<i32>, %[[VALUE_n_2:[0-9]+]] n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_n_2]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_i_2]]))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(neg<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_i_2]]))))), const<i32>(31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i32, 65> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<i32, 65> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_a]]), const<i32>(0))), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_b]]), const<i32>(0))), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(65))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i_3]]))), add<i32, overflow=ub>(conditional<i32>(lt<i32>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(126)), const<i32>(4)), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(13))));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i_3]]))), add<i32, overflow=ub>(conditional<i32>(lt<i32>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(126)), const<i32>(4)), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(13))));
// DEFAULT-NEXT:                     if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<i32>(%[[VALUE_i_3]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i_3]])))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%[[VALUE_fun1]], array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_a]]), const<i32>(65));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%[[VALUE_fun2]], array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_b]]), const<i32>(65));
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_2]])), const<i32>(0), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_a]]), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_b]]), const<i32>(0)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_b]]), const<i32>(0)))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_4]]), const<i32>(65))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_3]])), read<i32>(%[[VALUE_i_4]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i_4]])))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i_4]])))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i_4]])))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i_4]])))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
