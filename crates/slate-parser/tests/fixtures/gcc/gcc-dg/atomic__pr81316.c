/* { dg-do run } */
/* { dg-options "-pthread" } */
/* { dg-require-effective-target pthread } */

#include <pthread.h>
#include <stdlib.h>

static _Atomic int sem1;

static void *f(void *va)
{
  void **p = va;
  while (!__atomic_load_n(&sem1, __ATOMIC_ACQUIRE))
    sched_yield ();
  exit(!*p);
}

int main(int argc)
{
  void *p = 0;
  pthread_t thr;
  if (pthread_create(&thr, 0, f, &p))
    return 2;
  // GCC used to RTL-DSE this store
  p = &p;
  __atomic_store_n(&sem1, 1, __ATOMIC_RELEASE);
  int r = -1;
  while (r < 0)
    {
      sched_yield ();
      asm("":"+r"(r));
    }
  return r;
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
// DEFAULT-NEXT:     type @type0 pthread_t = u64;
// DEFAULT-NEXT:     type @type1 pthread_attr_t = union {
// DEFAULT-NEXT:         field0 __size: array<i8, 56>;
// DEFAULT-NEXT:         field1 __align: i64;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 pthread_attr_t = @type1;
// DEFAULT-NEXT:     global %6 sem1: atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @sched_yield() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @pthread_create(%15 __newthread: ptr<u64> [restrict], %16 __attr: ptr<const @type1> [restrict], %17 __start_routine: ptr<fn(ptr<void>) -> ptr<void>>, %18 __arg: ptr<void> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @exit(%19 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @f(%8 va: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 p: ptr<ptr<void>> [storage=automatic] = pointer_cast<ptr<ptr<void>>, reason=assign>(read<ptr<void>>(%8));
// DEFAULT-NEXT:         while %20 not<bool>(ne<i32>(read<i32, atomic=acquire>(deref(addr_of<ptr<atomic i32>>(%6))), const<i32>(0)))
// DEFAULT-NEXT:             call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%5, from_bool<i32, reason=arg>(not<bool>(ne<ptr<void>>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%9))), null<ptr<void>>))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main(%11 argc: i32) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 p: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         let %13 thr: u64 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<u64>, ptr<const @type1>, ptr<fn(ptr<void>) -> ptr<void>>, ptr<void>) -> i32>(%4, addr_of<ptr<u64>>(%13), null<ptr<const @type1>>, function_decay<ptr<fn(ptr<void>) -> ptr<void>>>(%7), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<void>>>(%12))), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(2);
// DEFAULT-NEXT:         write<ptr<void>>(%12, pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<ptr<void>>>(%12)));
// DEFAULT-NEXT:         write<i32, atomic=release>(deref(addr_of<ptr<atomic i32>>(%6)), const<i32>(1));
// DEFAULT-NEXT:         let %14 r: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         while %21 lt<i32>(read<i32>(%14), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:                 asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:                     inlateout 0 "r" [reg] width 32 place<i32>(%14);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
