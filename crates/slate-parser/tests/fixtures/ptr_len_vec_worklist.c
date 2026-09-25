#include <stdio.h>
#include <stdlib.h>

static int consume_values(int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i) {
    values[i] += 1;
    sum       += values[i];
  }
  free(values);
  return sum;
}

static int forward_consume(int *values, int len) {
  return consume_values(values, len);
}

int main(void) {
  int  len    = 4;
  int *values = malloc(len * sizeof(int));
  for (int i = 0; i < len; ++i)
    values[i] = i * 2;
  int sum = forward_consume(values, len);
  printf("%d\n", sum);
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
// DEFAULT-NEXT:     global %22 .str22: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%17 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @malloc(%18 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @free(%19 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @consume_values(%5 values: ptr<i32>, %6 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %8 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), read<i32>(%6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%24));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %25: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), read<i32>(%8));
// DEFAULT-NEXT:                     let %26: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%25)));
// DEFAULT-NEXT:                     let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%25)), read<i32>(%27));
// DEFAULT-NEXT:                     let %28: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                     let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), read<i32>(%8)))));
// DEFAULT-NEXT:                     write<i32>(%7, read<i32>(%29));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%5)));
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @forward_consume(%10 values: ptr<i32>, %11 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<i32>, i32) -> i32>(%4, read<ptr<i32>>(%10), read<i32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 len: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:         let %14 values: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%13))), const<u64>(4))));
// DEFAULT-NEXT:         for %21
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %15 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%15), read<i32>(%13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%31));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%14), read<i32>(%15))), mul<i32, overflow=ub>(read<i32>(%15), const<i32>(2)));
// DEFAULT-NEXT:         let %16 sum: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>, i32) -> i32>(%9, read<ptr<i32>>(%14), read<i32>(%13));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%22)), read<i32>(%16));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
