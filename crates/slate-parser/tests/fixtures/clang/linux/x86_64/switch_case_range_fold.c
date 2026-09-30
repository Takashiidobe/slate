#include <stdio.h>

int classify(int c) {
  switch (c) {
  case 48:
  case 49:
  case 50:
  case 51:
  case 52:
  case 53:
  case 54:
  case 55:
  case 56:
  case 57:
    return 1;
  case 'a':
  case 'b':
  case 'c':
    return 2;
  case 100:
  case 200:
    return 3;
  case -3:
  case -2:
  case -1:
  case 7:
    return 4;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d %d\n", classify('5'), classify('b'), classify(100),
         classify(200), classify(-2), classify(7), classify(9));
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_classify:[0-9]+]] @classify(%[[VALUE_c:[0-9]+]] c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_c]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(48):
// DEFAULT-NEXT:                     case %[[VALUE0]] const<i32>(49):
// DEFAULT-NEXT:                         case %[[VALUE0]] const<i32>(50):
// DEFAULT-NEXT:                             case %[[VALUE0]] const<i32>(51):
// DEFAULT-NEXT:                                 case %[[VALUE0]] const<i32>(52):
// DEFAULT-NEXT:                                     case %[[VALUE0]] const<i32>(53):
// DEFAULT-NEXT:                                         case %[[VALUE0]] const<i32>(54):
// DEFAULT-NEXT:                                             case %[[VALUE0]] const<i32>(55):
// DEFAULT-NEXT:                                                 case %[[VALUE0]] const<i32>(56):
// DEFAULT-NEXT:                                                     case %[[VALUE0]] const<i32>(57):
// DEFAULT-NEXT:                                                         return const<i32>(1);
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(97):
// DEFAULT-NEXT:                     case %[[VALUE0]] const<i32>(98):
// DEFAULT-NEXT:                         case %[[VALUE0]] const<i32>(99):
// DEFAULT-NEXT:                             return const<i32>(2);
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(100):
// DEFAULT-NEXT:                     case %[[VALUE0]] const<i32>(200):
// DEFAULT-NEXT:                         return const<i32>(3);
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(-3):
// DEFAULT-NEXT:                     case %[[VALUE0]] const<i32>(-2):
// DEFAULT-NEXT:                         case %[[VALUE0]] const<i32>(-1):
// DEFAULT-NEXT:                             case %[[VALUE0]] const<i32>(7):
// DEFAULT-NEXT:                                 return const<i32>(4);
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], const<i32>(53)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], const<i32>(98)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], const<i32>(100)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], const<i32>(200)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], neg<i32, overflow=ub>(const<i32>(2))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], const<i32>(7)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], const<i32>(9)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
