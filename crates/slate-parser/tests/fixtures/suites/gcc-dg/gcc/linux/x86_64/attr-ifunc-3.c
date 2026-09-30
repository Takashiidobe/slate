/* { dg-do run }  */
/* { dg-require-ifunc "" } */
/* { dg-options "" } */

#include <stdio.h>

static int __attribute__((noinline))
     implementation (void *ptr)
{
  if (ptr)
    return ((int (*) (void *))ptr) (0);
  
  printf ("'ere I am JH\n");
  return 0;
}

static __typeof__ (implementation) *resolver (void)
{
  return (void *)implementation;
}

extern int magic (void *) __attribute__ ((ifunc ("resolver")));

int main ()
{
  return magic ((void *)magic);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([39, 101, 114, 101, 32, 73, 32, 97, 109, 32, 74, 72, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_implementation:[0-9]+]] @implementation(%[[VALUE_ptr:[0-9]+]] ptr: ptr<void>) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>>(%[[VALUE_ptr]]), null<ptr<void>>)
// DEFAULT-NEXT:             return call<i32, signature=fn(ptr<void>) -> i32>(pointer_cast<ptr<fn(ptr<void>) -> i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_ptr]])), null<ptr<void>>);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_resolver:[0-9]+]] @resolver() -> ptr<fn(ptr<void>) -> i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<fn(ptr<void>) -> i32>, reason=return>(pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn(ptr<void>) -> i32>>(%[[VALUE_implementation]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_magic:[0-9]+]] @magic(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external] [ifunc="resolver"];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_magic]], pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn(ptr<void>) -> i32>>(%[[VALUE_magic]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
