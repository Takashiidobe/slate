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
// DEFAULT-NEXT:     type @type0 __thrd_t = u64;
// DEFAULT-NEXT:     type @type1 thrd_t = u64;
// DEFAULT-NEXT:     type @type2 thrd_start_t = ptr<fn(ptr<void>) -> i32>;
// DEFAULT-NEXT:     type @type3 = enum : u32 {
// DEFAULT-NEXT:         %0 thrd_success = const<i32>(0);
// DEFAULT-NEXT:         %1 thrd_busy = const<i32>(1);
// DEFAULT-NEXT:         %2 thrd_error = const<i32>(2);
// DEFAULT-NEXT:         %3 thrd_nomem = const<i32>(3);
// DEFAULT-NEXT:         %4 thrd_timedout = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %18 file_value: i32 [storage=thread] = const<i32>(5) [linkage=internal];
// DEFAULT-NEXT:     global %22 block_value: i32 [storage=thread] = const<i32>(7) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%35 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %14 @thrd_create(%36 __thr: ptr<u64>, %37 __func: ptr<fn(ptr<void>) -> i32>, %38 __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @thrd_join(%39 __thr: u64, %40 __res: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %19 @update_values(%20 file_next: i32, %21 block_next: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 result: i32 [storage=automatic] = add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%18), const<i32>(100)), read<i32>(%22));
// DEFAULT-NEXT:         write<i32>(%18, read<i32>(%20));
// DEFAULT-NEXT:         write<i32>(%22, read<i32>(%21));
// DEFAULT-NEXT:         return read<i32>(%23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @worker(%25 argument: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %26 values: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<void>>(%25));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%19, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%26), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%26), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %28 thread: u64 [storage=automatic];
// DEFAULT-NEXT:         let %29 values: array<i32, 2> [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(11), index1 = const<i32>(13));
// DEFAULT-NEXT:         let %30 main_before: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%19, const<i32>(17), const<i32>(19));
// DEFAULT-NEXT:         let %31 worker_result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %32 created: i32 [storage=automatic] = call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%14, addr_of<ptr<u64>>(%28), function_decay<ptr<fn(ptr<void>) -> i32>>(%24), pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(2)>(%29)));
// DEFAULT-NEXT:         let %33 joined: i32 [storage=automatic];
// DEFAULT-NEXT:         let %42: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%32), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%42, call<i32, signature=fn(u64, ptr<i32>) -> i32>(%17, read<u64>(%28), addr_of<ptr<i32>>(%31)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%42, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%33, read<i32>(%42));
// DEFAULT-NEXT:         let %34 main_after: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%19, const<i32>(23), const<i32>(29));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%41)), read<i32>(%30), read<i32>(%31), read<i32>(%34), read<i32>(%32), read<i32>(%33));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
