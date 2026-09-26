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
// DEFAULT-NEXT:     global %13 .str13: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 44, 37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @score(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 out: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %10 read<i32>(%2)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %10 const<i32>(1):
// DEFAULT-NEXT:                     let %15: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%16));
// DEFAULT-NEXT:                 default %10:
// DEFAULT-NEXT:                     let %17: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(2));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%18));
// DEFAULT-NEXT:                 case %10 const<i32>(3):
// DEFAULT-NEXT:                     let %19: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(3));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%20));
// DEFAULT-NEXT:                 break %10;
// DEFAULT-NEXT:                 case %10 const<i32>(4):
// DEFAULT-NEXT:                     let %21: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(4));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%22));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @shared(%5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 out: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %11 read<i32>(%5)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %11 const<i32>(2):
// DEFAULT-NEXT:                     default %11:
// DEFAULT-NEXT:                         let %23: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                         let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(10));
// DEFAULT-NEXT:                         write<i32>(%6, read<i32>(%24));
// DEFAULT-NEXT:                 case %11 const<i32>(5):
// DEFAULT-NEXT:                     let %25: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                     let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(20));
// DEFAULT-NEXT:                     write<i32>(%6, read<i32>(%26));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %8 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%28));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%13)), call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%8)), call<i32, signature=fn(i32) -> i32>(%4, read<i32>(%8)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%14)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
