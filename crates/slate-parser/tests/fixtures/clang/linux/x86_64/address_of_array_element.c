#include <stdio.h>

int read_pointer(int *pointer) { return *pointer; }

int read_array_element_through_call(void) {
  int  values[1] = {10};
  int *pointer   = &values[0];
  return read_pointer(pointer);
}

int main(void) {
  int  values[4] = {2, 4, 6, 8};
  int *p         = &values[1];
  int *q         = &values[3];
  printf("%d %ld %d\n", *p + *q, q - p, read_array_element_through_call());
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([37, 100, 32, 37, 108, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_read_pointer:[0-9]+]] @read_pointer(%[[VALUE_pointer:[0-9]+]] pointer: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%[[VALUE_pointer]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_read_array_element_through_call:[0-9]+]] @read_array_element_through_call() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_values:[0-9]+]] values: array<i32, 1> [storage=automatic] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(10));
// DEFAULT-NEXT:         let %[[VALUE_pointer_2:[0-9]+]] pointer: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_values]]), const<i32>(0))));
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_read_pointer]], read<ptr<i32>>(%[[VALUE_pointer_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_values_2:[0-9]+]] values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(4), index2 = const<i32>(6), index3 = const<i32>(8));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values_2]]), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values_2]]), const<i32>(3))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str]])), add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_p]]))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_q]])))), ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<i32>>(%[[VALUE_q]]), read<ptr<i32>>(%[[VALUE_p]])), call<i32, signature=fn() -> i32>(%[[VALUE_read_array_element_through_call]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
