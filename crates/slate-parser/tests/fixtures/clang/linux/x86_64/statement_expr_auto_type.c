#include <stdio.h>

#define EXCHANGE(pointer, replacement)                                         \
  ({                                                                           \
    __auto_type exchange_pointer = (pointer);                                  \
    __auto_type exchange_value   = *exchange_pointer;                          \
    *exchange_pointer            = (replacement);                              \
    exchange_value;                                                            \
  })

int main(void) {
  long values[] = {4, 9, 16};
  int  index    = 0;
  long old      = EXCHANGE(&values[index++], 25L);
  printf("%ld %ld %d\n", old, values[0], index);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([37, 108, 100, 32, 37, 108, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_values:[0-9]+]] values: array<i64, 3> [storage=automatic] [align=16] = aggregate<array<i64, 3>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(4)), index1 = widen<i64, reason=assign>(const<i32>(9)), index2 = widen<i64, reason=assign>(const<i32>(16)));
// DEFAULT-NEXT:         let %[[VALUE_index:[0-9]+]] index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_old:[0-9]+]] old: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i64 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_exchange_pointer:[0-9]+]] exchange_pointer: ptr<i64> [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_index]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             write<ptr<i64>>(%[[VALUE_exchange_pointer]], addr_of<ptr<i64>>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%[[VALUE_values]]), read<i32>(%[[VALUE1]])))));
// DEFAULT-NEXT:             let %[[VALUE_exchange_value:[0-9]+]] exchange_value: i64 [storage=automatic] = read<i64>(deref(read<ptr<i64>>(%[[VALUE_exchange_pointer]])));
// DEFAULT-NEXT:             write<i64>(deref(read<ptr<i64>>(%[[VALUE_exchange_pointer]])), const<i64>(25));
// DEFAULT-NEXT:             write<i64>(%[[VALUE0]], read<i64>(%[[VALUE_exchange_value]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i64>(%[[VALUE_old]], read<i64>(%[[VALUE0]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str]])), read<i64>(%[[VALUE_old]]), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%[[VALUE_values]]), const<i32>(0)))), read<i32>(%[[VALUE_index]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
