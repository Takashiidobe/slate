#include <stdio.h>

#define CHECK_VALUE(expression)                                                \
  ({                                                                           \
    __label__ failed, done;                                                    \
    int result;                                                                \
    if (!(expression))                                                         \
      goto failed;                                                             \
    result = 17;                                                               \
    goto done;                                                                 \
  failed:                                                                      \
    result = -5;                                                               \
  done:                                                                        \
    result;                                                                    \
  })

int main(void) {
  int value  = 0;
  int first  = CHECK_VALUE(++value == 1);
  int second = CHECK_VALUE(++value == 9);
  printf("%d %d %d\n", first, second, value);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_first:[0-9]+]] first: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             if not<bool>(eq<i32>(read<i32>(%[[VALUE2]]), const<i32>(1)))
// DEFAULT-NEXT:                 goto %[[VALUE_failed:[0-9]+]];
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result]], const<i32>(17));
// DEFAULT-NEXT:             goto %[[VALUE_done:[0-9]+]];
// DEFAULT-NEXT:             label %[[VALUE_failed]] failed:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result]], neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:             label %[[VALUE_done]] done:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], read<i32>(%[[VALUE_result]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_first]], read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         let %[[VALUE_second:[0-9]+]] second: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_result_2:[0-9]+]] result: i32 [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:             if not<bool>(eq<i32>(read<i32>(%[[VALUE5]]), const<i32>(9)))
// DEFAULT-NEXT:                 goto %[[VALUE_failed_2:[0-9]+]];
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_2]], const<i32>(17));
// DEFAULT-NEXT:             goto %[[VALUE_done_2:[0-9]+]];
// DEFAULT-NEXT:             label %[[VALUE_failed_2]] failed:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_2]], neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:             label %[[VALUE_done_2]] done:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             write<i32>(%[[VALUE3]], read<i32>(%[[VALUE_result_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_second]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<i32>(%[[VALUE_first]]), read<i32>(%[[VALUE_second]]), read<i32>(%[[VALUE_value]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
