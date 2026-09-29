#include <stdio.h>

static int sum_vla(int length, int (*values)[length]) {
  int total = 0;
  for (int index = 0; index < length; ++index) {
    total += (*values)[index];
  }
  return total;
}

int main(void) {
  int result;
  {
    int length = 4;
    int values[length];
    for (int index = 0; index < length; ++index) {
      values[index] = index + 3;
    }
    result = sum_vla(length, &values);
  }
  printf("%d\n", result + 1);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sum_vla:[0-9]+]] @sum_vla(%[[VALUE_length:[0-9]+]] length: i32, %[[VALUE_values:[0-9]+]] values: ptr<vla<i32, %[[VALUE0:[0-9]+]]>>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_length]])));
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_index:[0-9]+]] index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_index]]), read<i32>(%[[VALUE_length]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_index]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(read<ptr<vla<i32, %[[VALUE0]]>>>(%[[VALUE_values]]))), read<i32>(%[[VALUE_index]])))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_length_2:[0-9]+]] length: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_length_2]])));
// DEFAULT-NEXT:             let %[[VALUE_values_2:[0-9]+]] values: vla<i32, %[[VALUE6]]> [storage=automatic];
// DEFAULT-NEXT:             for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     let %[[VALUE_index_2:[0-9]+]] index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%[[VALUE_index_2]]), read<i32>(%[[VALUE_length_2]]))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index_2]]);
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_index_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%[[VALUE_values_2]]), read<i32>(%[[VALUE_index_2]]))), add<i32, overflow=ub>(read<i32>(%[[VALUE_index_2]]), const<i32>(3)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result]], call<i32, signature=fn(i32, ptr<vla<i32, *>>) -> i32>(%[[VALUE_sum_vla]], read<i32>(%[[VALUE_length_2]]), pointer_cast<ptr<vla<i32, *>>, reason=arg>(addr_of<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE_values_2]]))));
// DEFAULT-NEXT:             call<i32, signature=fn(i32, ptr<vla<i32, *>>) -> i32>(%[[VALUE_sum_vla]], read<i32>(%[[VALUE_length_2]]), pointer_cast<ptr<vla<i32, *>>, reason=arg>(addr_of<ptr<vla<i32, %[[VALUE6]]>>>(%[[VALUE_values_2]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), add<i32, overflow=ub>(read<i32>(%[[VALUE_result]]), const<i32>(1)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
