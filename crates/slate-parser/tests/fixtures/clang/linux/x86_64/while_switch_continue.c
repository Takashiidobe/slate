#include <stdio.h>

int main(void) {
  int i     = 0;
  int steps = 0;
  while (i < 6) {
    int x = i % 3;
    switch (x) {
    case 0:
    case 1:
    case 2:
      i++;
      steps++;
      continue;
    default:
      break;
    }
    printf("unreachable %d\n", i);
    i++;
    steps++;
  }
  printf("steps=%d\n", steps);
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
// DEFAULT-NEXT:     global %9 .str9: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([117, 110, 114, 101, 97, 99, 104, 97, 98, 108, 101, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([115, 116, 101, 112, 115, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%6 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %4 steps: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %7 lt<i32>(read<i32>(%3), const<i32>(6))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %5 x: i32 [storage=automatic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%3), const<i32>(3));
// DEFAULT-NEXT:                 switch %8 read<i32>(%5)
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %8 const<i32>(0):
// DEFAULT-NEXT:                             case %8 const<i32>(1):
// DEFAULT-NEXT:                                 case %8 const<i32>(2):
// DEFAULT-NEXT:                                     let %11: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                                     let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%3, read<i32>(%12));
// DEFAULT-NEXT:                         let %13: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                         let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%4, read<i32>(%14));
// DEFAULT-NEXT:                         continue %7;
// DEFAULT-NEXT:                         default %8:
// DEFAULT-NEXT:                             break %8;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%9)), read<i32>(%3));
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%16));
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%18));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%10)), read<i32>(%4));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
