/* { dg-do run } */
/* { dg-options "-O3" } */

/* This test does not work when the truth type does not match vector type.  */
/* { dg-additional-options "-march=armv8-a" { target aarch64_sve } } */


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

__attribute__ ((noinline, noipa, optimize("O1")))
void fun2(TYPE *x, int n)
{
    for (int i = 0; i < n; i++)
      x[i] = (-x[i]) >> 31;
}

int main ()
{
  TYPE a[N];
  TYPE b[N];

  a[0] = INT_MIN+1;
  b[0] = INT_MIN+1;

  for (int i = 1; i < N; ++i)
    {
      a[i] = BASE + i * 13;
      b[i] = BASE + i * 13;
      if (DEBUG)
        printf ("%d: 0x%x\n", i, a[i]);
    }

  fun1 (a, N);
  fun2 (b, N);

  for (int i = 0; i < N; ++i)
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
// DEFAULT-NEXT:     type @type0 __int32_t = i32;
// DEFAULT-NEXT:     type @type1 int32_t = i32;
// DEFAULT-NEXT:     global %21 .str21: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 58, 32, 48, 120, 37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 61, 32, 48, 120, 37, 120, 32, 61, 61, 32, 48, 120, 37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @printf(%17 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @fun1(%5 x: ptr<i32>, %6 n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %7 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), read<i32>(%6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%26));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), read<i32>(%7))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(neg<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), read<i32>(%7))))), const<i32>(31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fun2(%9 x: ptr<i32>, %10 n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %11 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), read<i32>(%10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%28));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%11))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(neg<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%11))))), const<i32>(31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 a: array<i32, 65> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %14 b: array<i32, 65> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%13), const<i32>(0))), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%14), const<i32>(0))), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %15 i: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%15), const<i32>(65))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%13), read<i32>(%15))), add<i32, overflow=ub>(conditional<i32>(lt<i32>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(126)), const<i32>(4)), mul<i32, overflow=ub>(read<i32>(%15), const<i32>(13))));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%14), read<i32>(%15))), add<i32, overflow=ub>(conditional<i32>(lt<i32>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(126)), const<i32>(4)), mul<i32, overflow=ub>(read<i32>(%15), const<i32>(13))));
// DEFAULT-NEXT:                     if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%21)), read<i32>(%15), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%13), read<i32>(%15)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%4, array_decay<ptr<i32>, length=Some(65)>(%13), const<i32>(65));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%8, array_decay<ptr<i32>, length=Some(65)>(%14), const<i32>(65));
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %16 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(65))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%32));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%23)), read<i32>(%16), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%13), read<i32>(%16)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%14), read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%13), read<i32>(%16)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65)>(%14), read<i32>(%16)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%24);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
