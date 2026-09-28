#include <stdio.h>
#include <stdlib.h>

static int global_array[4];

static unsigned long runtime_alloc_size(int n) {
  void         *p    = malloc(n);
  unsigned long size = __builtin_dynamic_object_size(p, 0);
  free(p);
  return size;
}

int main(void) {
  int          local[6];
  int         *p = local;
  volatile int v = 3;
  (void)v;

  unsigned long local_whole     = __builtin_dynamic_object_size(local, 0);
  unsigned long local_remaining = __builtin_dynamic_object_size(&local[2], 1);
  unsigned long global_whole  = __builtin_dynamic_object_size(global_array, 0);
  unsigned long unknown       = __builtin_dynamic_object_size(p, 0);
  unsigned long unknown_upper = __builtin_dynamic_object_size(p, 2);
  unsigned long runtime_alloc = runtime_alloc_size(37);

  printf("%lu %lu %lu %lu %lu %lu\n", local_whole, local_remaining,
         global_whole, unknown, unknown_upper, runtime_alloc);
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %7 global_array: array<i32, 4> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([37, 108, 117, 32, 37, 108, 117, 32, 37, 108, 117, 32, 37, 108, 117, 32, 37, 108, 117, 32, 37, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%22 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%23 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @free(%24 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %27 @__builtin_dynamic_object_size(%25 <unnamed>: ptr<const void>, %26 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %8 @runtime_alloc_size(%9 n: i32) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%9))));
// DEFAULT-NEXT:         let %11 size: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%27, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%10)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, read<ptr<void>>(%10));
// DEFAULT-NEXT:         return read<u64>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 local: array<i32, 6> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %14 p: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(6)>(%13);
// DEFAULT-NEXT:         let %15 v: volatile i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         read<i32, volatile>(%15);
// DEFAULT-NEXT:         let %16 local_whole: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%27, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(6)>(%13)), const<i32>(0));
// DEFAULT-NEXT:         let %17 local_remaining: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%27, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(%13), const<i32>(2))))), const<i32>(1));
// DEFAULT-NEXT:         let %18 global_whole: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%27, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%7)), const<i32>(0));
// DEFAULT-NEXT:         let %19 unknown: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%27, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%14)), const<i32>(0));
// DEFAULT-NEXT:         let %20 unknown_upper: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%27, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%14)), const<i32>(2));
// DEFAULT-NEXT:         let %21 runtime_alloc: u64 [storage=automatic] = call<u64, signature=fn(i32) -> u64>(%8, const<i32>(37));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%28)), read<u64>(%16), read<u64>(%17), read<u64>(%18), read<u64>(%19), read<u64>(%20), read<u64>(%21));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
