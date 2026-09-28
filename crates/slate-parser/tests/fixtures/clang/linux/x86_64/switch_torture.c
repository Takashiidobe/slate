#include <stdio.h>

int f(int x) {
  int out = 0;
  switch (x) {
  case -500 ... - 1:
    out += 100;
  case 0:
  case 1:
    out += 1;
  case 2 ... 100:
    out += 2;
    if (out > 50) {
      break;
    }
    out += 3;
  case 101:
    out += 4;
    if (x % 2 == 0) {
      break;
    }
  default:
    out += 5;
  case 200 ... 500:
  case 600:
  case 700 ... 900:
    out += 6;
    break;
  case 999:
    out += 7;
  case 1000:
    out += 8;
  }
  return out;
}

int main(void) {
  for (int x = -600; x < 1001; x++)
    printf("%d,", f(x));
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
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 44, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%7 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%3 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 out: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %8 read<i32>(%3)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %8 const<i32>(-500) ... const<i32>(-1):
// DEFAULT-NEXT:                     let %12: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(100));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%13));
// DEFAULT-NEXT:                 case %8 const<i32>(0):
// DEFAULT-NEXT:                     case %8 const<i32>(1):
// DEFAULT-NEXT:                         let %14: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                         let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%4, read<i32>(%15));
// DEFAULT-NEXT:                 case %8 const<i32>(2) ... const<i32>(100):
// DEFAULT-NEXT:                     let %16: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(2));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%17));
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%4), const<i32>(50))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         break %8;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(3));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%19));
// DEFAULT-NEXT:                 case %8 const<i32>(101):
// DEFAULT-NEXT:                     let %20: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(4));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%21));
// DEFAULT-NEXT:                 if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%3), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         break %8;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 default %8:
// DEFAULT-NEXT:                     let %22: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(5));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%23));
// DEFAULT-NEXT:                 case %8 const<i32>(200) ... const<i32>(500):
// DEFAULT-NEXT:                     case %8 const<i32>(600):
// DEFAULT-NEXT:                         case %8 const<i32>(700) ... const<i32>(900):
// DEFAULT-NEXT:                             let %24: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                             let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(6));
// DEFAULT-NEXT:                             write<i32>(%4, read<i32>(%25));
// DEFAULT-NEXT:                 break %8;
// DEFAULT-NEXT:                 case %8 const<i32>(999):
// DEFAULT-NEXT:                     let %26: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(7));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%27));
// DEFAULT-NEXT:                 case %8 const<i32>(1000):
// DEFAULT-NEXT:                     let %28: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(8));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%29));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %6 x: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(600));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), const<i32>(1001))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%31));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%6)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%11)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
