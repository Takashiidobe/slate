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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_calloc:[0-9]+]] @calloc(%[[VALUE___nmemb:[0-9]+]] __nmemb: u64, %[[VALUE___size_2:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_allocate:[0-9]+]] @allocate(%[[VALUE_size:[0-9]+]] size: u64, %[[VALUE_alignment:[0-9]+]] alignment: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<u64>(%[[VALUE_alignment]]);
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], read<u64>(%[[VALUE_size]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_allocate_array:[0-9]+]] @allocate_array(%[[VALUE_count:[0-9]+]] count: u64, %[[VALUE_size_2:[0-9]+]] size: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], read<u64>(%[[VALUE_count]]), read<u64>(%[[VALUE_size_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_offset_aligned:[0-9]+]] @offset_aligned(%[[VALUE_pointer:[0-9]+]] pointer: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%[[VALUE_pointer]])), const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_values:[0-9]+]] values: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_allocate]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(4)), const<u64>(4)));
// DEFAULT-NEXT:         let %[[VALUE_zeroed:[0-9]+]] zeroed: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_allocate_array]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(0))), const<i32>(7));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(1))), const<i32>(9));
// DEFAULT-NEXT:         let %[[VALUE_second:[0-9]+]] second: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%[[VALUE_offset_aligned]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_values]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(0)))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_second]]))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_zeroed]]), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_zeroed]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_values]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
