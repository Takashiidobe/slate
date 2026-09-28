#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((malloc, alloc_size(1), alloc_align(2))) static void *
allocate(size_t size, size_t alignment) {
  (void)alignment;
  return malloc(size);
}

__attribute__((malloc, alloc_size(1, 2))) static void *
allocate_array(size_t count, size_t size) {
  return calloc(count, size);
}

__attribute__((assume_aligned(16, 4))) static void *
offset_aligned(void *pointer) {
  return (char *)pointer + 4;
}

int main(void) {
  int *values = allocate(3 * sizeof(int), _Alignof(int));
  int *zeroed = allocate_array(2, sizeof(int));
  values[0]   = 7;
  values[1]   = 9;
  int *second = offset_aligned(values);
  printf("%d %d %d\n", values[0], *second, zeroed[1]);
  free(zeroed);
  free(values);
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
// DEFAULT-NEXT:     global %27 .str27: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%22 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%23 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @calloc(%24 __nmemb: u64, %25 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %9 @free(%26 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @allocate(%11 size: u64, %12 alignment: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<u64>(%12);
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, read<u64>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @allocate_array(%14 count: u64, %15 size: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%7, read<u64>(%14), read<u64>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @offset_aligned(%17 pointer: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%17)), const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %19 values: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%10, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(4)), const<u64>(4)));
// DEFAULT-NEXT:         let %20 zeroed: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%13, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%19), const<i32>(0))), const<i32>(7));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%19), const<i32>(1))), const<i32>(9));
// DEFAULT-NEXT:         let %21 second: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%16, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%19))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%27)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%19), const<i32>(0)))), read<i32>(deref(read<ptr<i32>>(%21))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%20), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%9, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%20)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%9, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%19)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
