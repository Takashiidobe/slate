#include <stdio.h>

int score(int x) {
  int out = 0;
  switch (x) {
  case 1:
    out += 1;
  default:
    out += 2;
  case 3:
    out += 3;
    break;
  case 4:
    out += 4;
  }
  return out;
}

int shared(int x) {
  int out = 0;
  switch (x) {
  case 2:
  default:
    out += 10;
  case 5:
    out += 20;
  }
  return out;
}

int main(void) {
  for (int i = 0; i < 7; i++)
    printf("%d,%d ", score(i), shared(i));
  printf("\n");
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 44, 37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_score:[0-9]+]] @score(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_out:[0-9]+]] out: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_x]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(1):
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_out]]);
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_out]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_out]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(2));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_out]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(3):
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_out]]);
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(3));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_out]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(4):
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_out]]);
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(4));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_out]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_out]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_shared:[0-9]+]] @shared(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_out_2:[0-9]+]] out: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %[[VALUE9:[0-9]+]] read<i32>(%[[VALUE_x_2]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE9]] const<i32>(2):
// DEFAULT-NEXT:                     default %[[VALUE9]]:
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_out_2]]);
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(10));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_out_2]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 case %[[VALUE9]] const<i32>(5):
// DEFAULT-NEXT:                     let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_out_2]]);
// DEFAULT-NEXT:                     let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(20));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_out_2]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_out_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_score]], read<i32>(%[[VALUE_i]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_shared]], read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
