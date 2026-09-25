#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

static int once_total;

static int thread_worker(void *argument) { return *(int *)argument + 1; }

static void once_handler(void) { once_total += 1; }

static void tss_destructor(void *value) { once_total += value != NULL; }

static void quick_handler(void) { once_total += 100; }

int main(void) {
  thrd_t thread;
  tss_t  key;
  int    argument       = 40;
  int    thread_result  = 0;
  int    thread_created = thrd_create(&thread, thread_worker, &argument);
  int    thread_joined =
      thread_created == thrd_success ? thrd_join(thread, &thread_result) : -1;

  once_flag control = ONCE_FLAG_INIT;
  call_once(&control, once_handler);
  call_once(&control, once_handler);

  int key_created = tss_create(&key, tss_destructor);
  if (key_created == thrd_success) {
    tss_delete(key);
  }

  int quick_registered = at_quick_exit(quick_handler);
  printf("%d %d %d %d %d %d\n", thread_created, thread_joined, thread_result,
         once_total, key_created, quick_registered);
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
// DEFAULT-NEXT:     type @type0 __tss_t = u32;
// DEFAULT-NEXT:     type @type1 __thrd_t = u64;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 __data: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 __once_flag = @type2;
// DEFAULT-NEXT:     type @type4 once_flag = @type2;
// DEFAULT-NEXT:     type @type5 tss_t = u32;
// DEFAULT-NEXT:     type @type6 tss_dtor_t = ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:     type @type7 thrd_t = u64;
// DEFAULT-NEXT:     type @type8 thrd_start_t = ptr<fn(ptr<void>) -> i32>;
// DEFAULT-NEXT:     type @type9 = enum : u32 {
// DEFAULT-NEXT:         %0 thrd_success = const<i32>(0);
// DEFAULT-NEXT:         %1 thrd_busy = const<i32>(1);
// DEFAULT-NEXT:         %2 thrd_error = const<i32>(2);
// DEFAULT-NEXT:         %3 thrd_nomem = const<i32>(3);
// DEFAULT-NEXT:         %4 thrd_timedout = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %22 once_total: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%39 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @at_quick_exit(%40 __func: ptr<fn() -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @call_once(%41 __flag: ptr<@type2>, %42 __func: ptr<fn() -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %18 @thrd_create(%43 __thr: ptr<u64>, %44 __func: ptr<fn(ptr<void>) -> i32>, %45 __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %19 @thrd_join(%46 __thr: u64, %47 __res: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %20 @tss_create(%50 __tss_id: ptr<u32>, %51 __destructor: ptr<fn(ptr<void>) -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @tss_delete(%52 __tss_id: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %23 @thread_worker(%24 argument: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%24)))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @once_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %54: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:         let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%22, read<i32>(%55));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @tss_destructor(%27 value: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %56: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:         let %57: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%56), from_bool<i32, reason=promotion>(ne<ptr<void>>(read<ptr<void>>(%27), null<ptr<void>>)));
// DEFAULT-NEXT:         write<i32>(%22, read<i32>(%57));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @quick_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %58: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:         let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), const<i32>(100));
// DEFAULT-NEXT:         write<i32>(%22, read<i32>(%59));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %30 thread: u64 [storage=automatic];
// DEFAULT-NEXT:         let %31 key: u32 [storage=automatic];
// DEFAULT-NEXT:         let %32 argument: i32 [storage=automatic] = const<i32>(40);
// DEFAULT-NEXT:         let %33 thread_result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %34 thread_created: i32 [storage=automatic] = call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%18, addr_of<ptr<u64>>(%30), function_decay<ptr<fn(ptr<void>) -> i32>>(%23), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%32)));
// DEFAULT-NEXT:         let %35 thread_joined: i32 [storage=automatic];
// DEFAULT-NEXT:         let %60: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%34), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%60, call<i32, signature=fn(u64, ptr<i32>) -> i32>(%19, read<u64>(%30), addr_of<ptr<i32>>(%33)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%60, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%35, read<i32>(%60));
// DEFAULT-NEXT:         let %36 control: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, ptr<fn() -> void>) -> void>(%7, addr_of<ptr<@type2>>(%36), function_decay<ptr<fn() -> void>>(%25));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, ptr<fn() -> void>) -> void>(%7, addr_of<ptr<@type2>>(%36), function_decay<ptr<fn() -> void>>(%25));
// DEFAULT-NEXT:         let %37 key_created: i32 [storage=automatic] = call<i32, signature=fn(ptr<u32>, ptr<fn(ptr<void>) -> void>) -> i32>(%20, addr_of<ptr<u32>>(%31), function_decay<ptr<fn(ptr<void>) -> void>>(%26));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%37), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(u32) -> void>(%21, read<u32>(%31));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %38 quick_registered: i32 [storage=automatic] = call<i32, signature=fn(ptr<fn() -> void>) -> i32>(%5, function_decay<ptr<fn() -> void>>(%28));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%53)), read<i32>(%34), read<i32>(%35), read<i32>(%33), read<i32>(%22), read<i32>(%37), read<i32>(%38));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
