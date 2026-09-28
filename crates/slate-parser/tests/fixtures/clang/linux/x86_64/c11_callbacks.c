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
// DEFAULT-NEXT:     global %36 once_total: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %67 .str67: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%53 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @at_quick_exit(%54 __func: ptr<fn() -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @call_once(%55 __flag: ptr<@type2>, %56 __func: ptr<fn() -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %25 @thrd_create(%57 __thr: ptr<u64>, %58 __func: ptr<fn(ptr<void>) -> i32>, %59 __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %28 @thrd_join(%60 __thr: u64, %61 __res: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %33 @tss_create(%64 __tss_id: ptr<u32>, %65 __destructor: ptr<fn(ptr<void>) -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %35 @tss_delete(%66 __tss_id: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %37 @thread_worker(%38 argument: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%38)))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @once_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %68: i32 [synthetic] = read<i32>(%36);
// DEFAULT-NEXT:         let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%36, read<i32>(%69));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @tss_destructor(%41 value: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %70: i32 [synthetic] = read<i32>(%36);
// DEFAULT-NEXT:         let %71: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%70), from_bool<i32, reason=promotion>(ne<ptr<void>>(read<ptr<void>>(%41), null<ptr<void>>)));
// DEFAULT-NEXT:         write<i32>(%36, read<i32>(%71));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @quick_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %72: i32 [synthetic] = read<i32>(%36);
// DEFAULT-NEXT:         let %73: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%72), const<i32>(100));
// DEFAULT-NEXT:         write<i32>(%36, read<i32>(%73));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %44 thread: u64 [storage=automatic];
// DEFAULT-NEXT:         let %45 key: u32 [storage=automatic];
// DEFAULT-NEXT:         let %46 argument: i32 [storage=automatic] = const<i32>(40);
// DEFAULT-NEXT:         let %47 thread_result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %48 thread_created: i32 [storage=automatic] = call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%25, addr_of<ptr<u64>>(%44), function_decay<ptr<fn(ptr<void>) -> i32>>(%37), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%46)));
// DEFAULT-NEXT:         let %49 thread_joined: i32 [storage=automatic];
// DEFAULT-NEXT:         let %74: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%48), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%74, call<i32, signature=fn(u64, ptr<i32>) -> i32>(%28, read<u64>(%44), addr_of<ptr<i32>>(%47)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%74, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%49, read<i32>(%74));
// DEFAULT-NEXT:         let %50 control: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, ptr<fn() -> void>) -> void>(%11, addr_of<ptr<@type2>>(%50), function_decay<ptr<fn() -> void>>(%39));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, ptr<fn() -> void>) -> void>(%11, addr_of<ptr<@type2>>(%50), function_decay<ptr<fn() -> void>>(%39));
// DEFAULT-NEXT:         let %51 key_created: i32 [storage=automatic] = call<i32, signature=fn(ptr<u32>, ptr<fn(ptr<void>) -> void>) -> i32>(%33, addr_of<ptr<u32>>(%45), function_decay<ptr<fn(ptr<void>) -> void>>(%40));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%51), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(u32) -> void>(%35, read<u32>(%45));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %52 quick_registered: i32 [storage=automatic] = call<i32, signature=fn(ptr<fn() -> void>) -> i32>(%7, function_decay<ptr<fn() -> void>>(%42));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%67)), read<i32>(%48), read<i32>(%49), read<i32>(%47), read<i32>(%36), read<i32>(%51), read<i32>(%52));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
