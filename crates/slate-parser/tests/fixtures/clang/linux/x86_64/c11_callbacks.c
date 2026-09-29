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
// DEFAULT-NEXT:     type @type[[TYPE___tss_t:[0-9]+]] __tss_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___thrd_t:[0-9]+]] __thrd_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __data: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE___once_flag:[0-9]+]] __once_flag = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_once_flag:[0-9]+]] once_flag = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_tss_t:[0-9]+]] tss_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_tss_dtor_t:[0-9]+]] tss_dtor_t = ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_thrd_t:[0-9]+]] thrd_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_thrd_start_t:[0-9]+]] thrd_start_t = ptr<fn(ptr<void>) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_thrd_success:[0-9]+]] thrd_success = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_thrd_busy:[0-9]+]] thrd_busy = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_thrd_error:[0-9]+]] thrd_error = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_thrd_nomem:[0-9]+]] thrd_nomem = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_thrd_timedout:[0-9]+]] thrd_timedout = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_once_total:[0-9]+]] once_total: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_thrd_busy]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_at_quick_exit:[0-9]+]] @at_quick_exit(%[[VALUE___func:[0-9]+]] __func: ptr<fn() -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_call_once:[0-9]+]] @call_once(%[[VALUE___flag:[0-9]+]] __flag: ptr<@type[[TYPE0]]>, %[[VALUE___func_2:[0-9]+]] __func: ptr<fn() -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_thrd_create:[0-9]+]] @thrd_create(%[[VALUE___thr:[0-9]+]] __thr: ptr<u64>, %[[VALUE___func_3:[0-9]+]] __func: ptr<fn(ptr<void>) -> i32>, %[[VALUE___arg:[0-9]+]] __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_thrd_join:[0-9]+]] @thrd_join(%[[VALUE___thr_2:[0-9]+]] __thr: u64, %[[VALUE___res:[0-9]+]] __res: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tss_create:[0-9]+]] @tss_create(%[[VALUE___tss_id:[0-9]+]] __tss_id: ptr<u32>, %[[VALUE___destructor:[0-9]+]] __destructor: ptr<fn(ptr<void>) -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tss_delete:[0-9]+]] @tss_delete(%[[VALUE___tss_id_2:[0-9]+]] __tss_id: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_thread_worker:[0-9]+]] @thread_worker(%[[VALUE_argument:[0-9]+]] argument: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_argument]])))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_once_handler:[0-9]+]] @once_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_once_total]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_once_total]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tss_destructor:[0-9]+]] @tss_destructor(%[[VALUE_value:[0-9]+]] value: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_once_total]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), from_bool<i32, reason=promotion>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_value]]), null<ptr<void>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_once_total]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_quick_handler:[0-9]+]] @quick_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_once_total]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(100));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_once_total]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_thread:[0-9]+]] thread: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_key:[0-9]+]] key: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_argument_2:[0-9]+]] argument: i32 [storage=automatic] = const<i32>(40);
// DEFAULT-NEXT:         let %[[VALUE_thread_result:[0-9]+]] thread_result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_thread_created:[0-9]+]] thread_created: i32 [storage=automatic] = call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%[[VALUE_thrd_create]], addr_of<ptr<u64>>(%[[VALUE_thread]]), function_decay<ptr<fn(ptr<void>) -> i32>>(%[[VALUE_thread_worker]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_argument_2]])));
// DEFAULT-NEXT:         let %[[VALUE_thread_joined:[0-9]+]] thread_joined: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_thread_created]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE6]], call<i32, signature=fn(u64, ptr<i32>) -> i32>(%[[VALUE_thrd_join]], read<u64>(%[[VALUE_thread]]), addr_of<ptr<i32>>(%[[VALUE_thread_result]])));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE6]], neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_thread_joined]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:         let %[[VALUE_control:[0-9]+]] control: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE0]]>, ptr<fn() -> void>) -> void>(%[[VALUE_call_once]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_control]]), function_decay<ptr<fn() -> void>>(%[[VALUE_once_handler]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE0]]>, ptr<fn() -> void>) -> void>(%[[VALUE_call_once]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_control]]), function_decay<ptr<fn() -> void>>(%[[VALUE_once_handler]]));
// DEFAULT-NEXT:         let %[[VALUE_key_created:[0-9]+]] key_created: i32 [storage=automatic] = call<i32, signature=fn(ptr<u32>, ptr<fn(ptr<void>) -> void>) -> i32>(%[[VALUE_tss_create]], addr_of<ptr<u32>>(%[[VALUE_key]]), function_decay<ptr<fn(ptr<void>) -> void>>(%[[VALUE_tss_destructor]]));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_key_created]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(u32) -> void>(%[[VALUE_tss_delete]], read<u32>(%[[VALUE_key]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE_quick_registered:[0-9]+]] quick_registered: i32 [storage=automatic] = call<i32, signature=fn(ptr<fn() -> void>) -> i32>(%[[VALUE_at_quick_exit]], function_decay<ptr<fn() -> void>>(%[[VALUE_quick_handler]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_thrd_busy]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str]])), read<i32>(%[[VALUE_thread_created]]), read<i32>(%[[VALUE_thread_joined]]), read<i32>(%[[VALUE_thread_result]]), read<i32>(%[[VALUE_once_total]]), read<i32>(%[[VALUE_key_created]]), read<i32>(%[[VALUE_quick_registered]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
