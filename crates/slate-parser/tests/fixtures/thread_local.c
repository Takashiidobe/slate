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
// DEFAULT-NEXT:     global %12 file_value: i32 [storage=thread] = const<i32>(5) [linkage=internal];
// DEFAULT-NEXT:     global %16 block_value: i32 [storage=thread] = const<i32>(7) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%29 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %10 @thrd_create(%30 __thr: ptr<u64>, %31 __func: ptr<fn(ptr<void>) -> i32>, %32 __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @thrd_join(%33 __thr: u64, %34 __res: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @update_values(%14 file_next: i32, %15 block_next: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 result: i32 [storage=automatic] = add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%12), const<i32>(100)), read<i32>(%16));
// DEFAULT-NEXT:         write<i32>(%12, read<i32>(%14));
// DEFAULT-NEXT:         write<i32>(%16, read<i32>(%15));
// DEFAULT-NEXT:         return read<i32>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @worker(%19 argument: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 values: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<void>>(%19));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%13, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%20), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%20), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %22 thread: u64 [storage=automatic];
// DEFAULT-NEXT:         let %23 values: array<i32, 2> [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(11), index1 = const<i32>(13));
// DEFAULT-NEXT:         let %24 main_before: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%13, const<i32>(17), const<i32>(19));
// DEFAULT-NEXT:         let %25 worker_result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %26 created: i32 [storage=automatic] = call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%10, addr_of<ptr<u64>>(%22), function_decay<ptr<fn(ptr<void>) -> i32>>(%18), pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         let %27 joined: i32 [storage=automatic];
// DEFAULT-NEXT:         let %36: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%26), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%36, call<i32, signature=fn(u64, ptr<i32>) -> i32>(%11, read<u64>(%22), addr_of<ptr<i32>>(%25)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%36, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%27, read<i32>(%36));
// DEFAULT-NEXT:         let %28 main_after: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%13, const<i32>(23), const<i32>(29));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%35)), read<i32>(%24), read<i32>(%25), read<i32>(%28), read<i32>(%26), read<i32>(%27));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
