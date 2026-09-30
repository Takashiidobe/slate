#include <stdio.h>
#include <threads.h>

static _Thread_local int file_value = 5;

static int update_values(int file_next, int block_next) {
  static _Thread_local int block_value = 7;
  int                      result      = file_value * 100 + block_value;
  file_value                           = file_next;
  block_value                          = block_next;
  return result;
}

static int worker(void *argument) {
  int *values = argument;
  return update_values(values[0], values[1]);
}

int main(void) {
  thrd_t thread;
  int    values[2]     = {11, 13};
  int    main_before   = update_values(17, 19);
  int    worker_result = 0;
  int    created       = thrd_create(&thread, worker, values);
  int joined = created == thrd_success ? thrd_join(thread, &worker_result) : -1;
  int main_after = update_values(23, 29);
  printf("%d %d %d %d %d\n", main_before, worker_result, main_after, created,
         joined);
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
// DEFAULT-NEXT:     type @type[[TYPE___thrd_t:[0-9]+]] __thrd_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_thrd_t:[0-9]+]] thrd_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_thrd_start_t:[0-9]+]] thrd_start_t = ptr<fn(ptr<void>) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_thrd_success:[0-9]+]] thrd_success = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_thrd_busy:[0-9]+]] thrd_busy = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_thrd_error:[0-9]+]] thrd_error = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_thrd_nomem:[0-9]+]] thrd_nomem = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_thrd_timedout:[0-9]+]] thrd_timedout = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_file_value:[0-9]+]] file_value: i32 [storage=thread] = const<i32>(5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_block_value:[0-9]+]] block_value: i32 [storage=thread] = const<i32>(7) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_thrd_busy]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_thrd_create:[0-9]+]] @thrd_create(%[[VALUE___thr:[0-9]+]] __thr: ptr<u64>, %[[VALUE___func:[0-9]+]] __func: ptr<fn(ptr<void>) -> i32>, %[[VALUE___arg:[0-9]+]] __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_thrd_join:[0-9]+]] @thrd_join(%[[VALUE___thr_2:[0-9]+]] __thr: u64, %[[VALUE___res:[0-9]+]] __res: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_update_values:[0-9]+]] @update_values(%[[VALUE_file_next:[0-9]+]] file_next: i32, %[[VALUE_block_next:[0-9]+]] block_next: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic] = add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_file_value]]), const<i32>(100)), read<i32>(%[[VALUE_block_value]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_file_value]], read<i32>(%[[VALUE_file_next]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_block_value]], read<i32>(%[[VALUE_block_next]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_worker:[0-9]+]] @worker(%[[VALUE_argument:[0-9]+]] argument: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_values:[0-9]+]] values: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<void>>(%[[VALUE_argument]]));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_update_values]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_thread:[0-9]+]] thread: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_values_2:[0-9]+]] values: array<i32, 2> [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(11), index1 = const<i32>(13));
// DEFAULT-NEXT:         let %[[VALUE_main_before:[0-9]+]] main_before: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_update_values]], const<i32>(17), const<i32>(19));
// DEFAULT-NEXT:         let %[[VALUE_worker_result:[0-9]+]] worker_result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_created:[0-9]+]] created: i32 [storage=automatic] = call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%[[VALUE_thrd_create]], addr_of<ptr<u64>>(%[[VALUE_thread]]), function_decay<ptr<fn(ptr<void>) -> i32>>(%[[VALUE_worker]]), pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_values_2]])));
// DEFAULT-NEXT:         let %[[VALUE_joined:[0-9]+]] joined: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_created]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], call<i32, signature=fn(u64, ptr<i32>) -> i32>(%[[VALUE_thrd_join]], read<u64>(%[[VALUE_thread]]), addr_of<ptr<i32>>(%[[VALUE_worker_result]])));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_joined]], read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         let %[[VALUE_main_after:[0-9]+]] main_after: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_update_values]], const<i32>(23), const<i32>(29));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_thrd_busy]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str]])), read<i32>(%[[VALUE_main_before]]), read<i32>(%[[VALUE_worker_result]]), read<i32>(%[[VALUE_main_after]]), read<i32>(%[[VALUE_created]]), read<i32>(%[[VALUE_joined]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
