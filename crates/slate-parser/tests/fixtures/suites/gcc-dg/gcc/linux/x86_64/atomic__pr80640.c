/* { dg-do run } */
/* { dg-options "-pthread" } */
/* { dg-require-effective-target pthread } */

#include <pthread.h>

static volatile int sem1;
static volatile int sem2;

static void *f(void *va)
{
  void **p = va;
  if (*p) return *p;
  sem1 = 1;
  while (!sem2)
    sched_yield ();
  __atomic_thread_fence(__ATOMIC_ACQUIRE);
  // GCC used to RTL-CSE this and the first load, causing 0 to be returned
  return *p;
}

int main()
{
  void *p = 0;
  pthread_t thr;
  if (pthread_create(&thr, 0, f, &p))
    return 2;
  while (!sem1)
    sched_yield ();
  __atomic_thread_fence(__ATOMIC_ACQUIRE);
  p = &p;
  __atomic_thread_fence(__ATOMIC_RELEASE);
  sem2 = 1;
  pthread_join(thr, &p);
  return !p;
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
// DEFAULT-NEXT:     type @type[[TYPE_pthread_t:[0-9]+]] pthread_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_pthread_attr_t:[0-9]+]] pthread_attr_t = union {
// DEFAULT-NEXT:         field0 __size: array<i8, 56>;
// DEFAULT-NEXT:         field1 __align: i64;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_pthread_attr_t_2:[0-9]+]] pthread_attr_t = @type[[TYPE_pthread_attr_t]];
// DEFAULT-NEXT:     global %[[VALUE_sem1:[0-9]+]] sem1: volatile i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_sem2:[0-9]+]] sem2: volatile i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_sched_yield:[0-9]+]] @sched_yield() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pthread_create:[0-9]+]] @pthread_create(%[[VALUE___newthread:[0-9]+]] __newthread: ptr<u64> [restrict], %[[VALUE___attr:[0-9]+]] __attr: ptr<const @type[[TYPE_pthread_attr_t]]> [restrict], %[[VALUE___start_routine:[0-9]+]] __start_routine: ptr<fn(ptr<void>) -> ptr<void>>, %[[VALUE___arg:[0-9]+]] __arg: ptr<void> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pthread_join:[0-9]+]] @pthread_join(%[[VALUE___th:[0-9]+]] __th: u64, %[[VALUE___thread_return:[0-9]+]] __thread_return: ptr<ptr<void>>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_va:[0-9]+]] va: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<ptr<void>> [storage=automatic] = pointer_cast<ptr<ptr<void>>, reason=assign>(read<ptr<void>>(%[[VALUE_va]]));
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p]]))), null<ptr<void>>)
// DEFAULT-NEXT:             return read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p]])));
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_sem1]], const<i32>(1));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_sem2]]), const<i32>(0)))
// DEFAULT-NEXT:             call<i32, signature=fn() -> i32>(%[[VALUE_sched_yield]]);
// DEFAULT-NEXT:         fence<scope=thread, order=acquire>;
// DEFAULT-NEXT:         return read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         let %[[VALUE_thr:[0-9]+]] thr: u64 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<u64>, ptr<const @type[[TYPE_pthread_attr_t]]>, ptr<fn(ptr<void>) -> ptr<void>>, ptr<void>) -> i32>(%[[VALUE_pthread_create]], addr_of<ptr<u64>>(%[[VALUE_thr]]), null<ptr<const @type[[TYPE_pthread_attr_t]]>>, function_decay<ptr<fn(ptr<void>) -> ptr<void>>>(%[[VALUE_f]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<void>>>(%[[VALUE_p_2]]))), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(2);
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_sem1]]), const<i32>(0)))
// DEFAULT-NEXT:             call<i32, signature=fn() -> i32>(%[[VALUE_sched_yield]]);
// DEFAULT-NEXT:         fence<scope=thread, order=acquire>;
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p_2]], pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<ptr<void>>>(%[[VALUE_p_2]])));
// DEFAULT-NEXT:         fence<scope=thread, order=release>;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_sem2]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(u64, ptr<ptr<void>>) -> i32>(%[[VALUE_pthread_join]], read<u64>(%[[VALUE_thr]]), addr_of<ptr<ptr<void>>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(not<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_p_2]]), null<ptr<void>>)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
