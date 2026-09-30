#include <stdio.h>

static int interpret(const unsigned char *code, int length) {
  static const int offsets[] = {&&add - &&dispatch, &&double_it - &&dispatch,
                                &&subtract - &&dispatch};
  int              index     = 0;
  int              value     = 1;

dispatch:
  if (index == length)
    return value;
  goto *(&&dispatch + offsets[code[index++]]);

add:
  value += 3;
  goto dispatch;
double_it:
  value *= 2;
  goto dispatch;
subtract:
  value -= 5;
  goto dispatch;
}

int main(void) {
  const unsigned char code[] = {0, 1, 2, 1};
  printf("%d\n", interpret(code, 4));
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
// DEFAULT-NEXT:     global %[[VALUE_offsets:[0-9]+]] offsets: array<i32, 3> [storage=static] [const] = aggregate<array<i32, 3>, zero_fill=false>(index0 = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=void, same_array=required, overflow=ub>(label_addr<ptr<void>>(%[[VALUE_add:[0-9]+]]), label_addr<ptr<void>>(%[[VALUE_dispatch:[0-9]+]]))), index1 = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=void, same_array=required, overflow=ub>(label_addr<ptr<void>>(%[[VALUE_double_it:[0-9]+]]), label_addr<ptr<void>>(%[[VALUE_dispatch]]))), index2 = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=void, same_array=required, overflow=ub>(label_addr<ptr<void>>(%[[VALUE_subtract:[0-9]+]]), label_addr<ptr<void>>(%[[VALUE_dispatch]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_interpret:[0-9]+]] @interpret(%[[VALUE_code:[0-9]+]] code: ptr<const u8>, %[[VALUE_length:[0-9]+]] length: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_index:[0-9]+]] index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         label %[[VALUE_dispatch]] dispatch:
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%[[VALUE_index]]), read<i32>(%[[VALUE_length]]))
// DEFAULT-NEXT:                 return read<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_index]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         goto *ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(label_addr<ptr<void>>(%[[VALUE_dispatch]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(3)>(%[[VALUE_offsets]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(%[[VALUE_code]]), read<i32>(%[[VALUE0]]))))))))));
// DEFAULT-NEXT:         label %[[VALUE_add]] add:
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(3));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         goto %[[VALUE_dispatch]];
// DEFAULT-NEXT:         label %[[VALUE_double_it]] double_it:
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(2));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         goto %[[VALUE_dispatch]];
// DEFAULT-NEXT:         label %[[VALUE_subtract]] subtract:
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(5));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         goto %[[VALUE_dispatch]];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_code_2:[0-9]+]] code: array<u8, 4> [storage=automatic] [const] = aggregate<array<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), call<i32, signature=fn(ptr<const u8>, i32) -> i32>(%[[VALUE_interpret]], array_decay<ptr<const u8>, length=Some(4)>(%[[VALUE_code_2]]), const<i32>(4)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
