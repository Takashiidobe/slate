#include <stdio.h>

int score(int x) {
  int out = 0;
  switch (x) {
  default:
    out += 1;
  case 5:
    out += 10;
  case 6:
    out += 20;
    break;
  case 7:
    out += 40;
  }
  return out;
}

int main(void) {
  printf("%d %d %d %d\n", score(5), score(6), score(7), score(9));
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
// DEFAULT-NEXT:     global %7 .str7: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%5 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @score(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 out: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %6 read<i32>(%2)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 default %6:
// DEFAULT-NEXT:                     let %8: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%9));
// DEFAULT-NEXT:                 case %6 const<i32>(5):
// DEFAULT-NEXT:                     let %10: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(10));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%11));
// DEFAULT-NEXT:                 case %6 const<i32>(6):
// DEFAULT-NEXT:                     let %12: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(20));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%13));
// DEFAULT-NEXT:                 break %6;
// DEFAULT-NEXT:                 case %6 const<i32>(7):
// DEFAULT-NEXT:                     let %14: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(40));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%15));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%7)), call<i32, signature=fn(i32) -> i32>(%1, const<i32>(5)), call<i32, signature=fn(i32) -> i32>(%1, const<i32>(6)), call<i32, signature=fn(i32) -> i32>(%1, const<i32>(7)), call<i32, signature=fn(i32) -> i32>(%1, const<i32>(9)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
