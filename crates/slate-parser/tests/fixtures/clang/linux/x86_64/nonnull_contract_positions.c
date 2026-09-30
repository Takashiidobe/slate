#include <stdio.h>

__attribute__((nonnull(1, 3))) static int combine(int *left, int scale,
                                                  int *right, int *optional) {
  return *left + scale * *right + (optional ? *optional : 0);
}

__attribute__((nonnull)) static int difference(int *left, int scale,
                                               int *right) {
  return *left - scale * *right;
}

int main(void) {
  int left  = 11;
  int right = 3;
  printf("%d %d\n", combine(&left, 2, &right, NULL),
         difference(&left, 2, &right));
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_combine:[0-9]+]] @combine(%[[VALUE_left:[0-9]+]] left: ptr<i32>, %[[VALUE_scale:[0-9]+]] scale: i32, %[[VALUE_right:[0-9]+]] right: ptr<i32>, %[[VALUE_optional:[0-9]+]] optional: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_left]]))), mul<i32, overflow=ub>(read<i32>(%[[VALUE_scale]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_right]]))))), conditional<i32>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_optional]]), null<ptr<i32>>), read<i32>(deref(read<ptr<i32>>(%[[VALUE_optional]]))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_difference:[0-9]+]] @difference(%[[VALUE_left_2:[0-9]+]] left: ptr<i32>, %[[VALUE_scale_2:[0-9]+]] scale: i32, %[[VALUE_right_2:[0-9]+]] right: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_left_2]]))), mul<i32, overflow=ub>(read<i32>(%[[VALUE_scale_2]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_right_2]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_left_3:[0-9]+]] left: i32 [storage=automatic] = const<i32>(11);
// DEFAULT-NEXT:         let %[[VALUE_right_3:[0-9]+]] right: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), call<i32, signature=fn(ptr<i32>, i32, ptr<i32>, ptr<i32>) -> i32>(%[[VALUE_combine]], addr_of<ptr<i32>>(%[[VALUE_left_3]]), const<i32>(2), addr_of<ptr<i32>>(%[[VALUE_right_3]]), null<ptr<i32>>), call<i32, signature=fn(ptr<i32>, i32, ptr<i32>) -> i32>(%[[VALUE_difference]], addr_of<ptr<i32>>(%[[VALUE_left_3]]), const<i32>(2), addr_of<ptr<i32>>(%[[VALUE_right_3]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
