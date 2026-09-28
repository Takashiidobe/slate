#include <stdio.h>

static int classify(int value) {
  int result = 0;
  switch (value) {
  case 1 ... 4:
    result += 10;
  case 5 ... 8:
    result += 20;
    break;
  case 10 ... 12:
    result += 40;
    break;
  default:
    result = 90;
  }
  return result;
}

static int classify_direct(int value) {
  switch (value) {
  case -2 ... 2:
    return 7;
  default:
    return 9;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", classify(1), classify(2),
         classify(4), classify(5), classify(7), classify(8), classify(9),
         classify(10), classify(11), classify(12), classify_direct(-1),
         classify_direct(3));
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
// DEFAULT-NEXT:     global %11 .str11: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @classify(%3 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %9 read<i32>(%3)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %9 const<i32>(1) ... const<i32>(4):
// DEFAULT-NEXT:                     let %12: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(10));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%13));
// DEFAULT-NEXT:                 case %9 const<i32>(5) ... const<i32>(8):
// DEFAULT-NEXT:                     let %14: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(20));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%15));
// DEFAULT-NEXT:                 break %9;
// DEFAULT-NEXT:                 case %9 const<i32>(10) ... const<i32>(12):
// DEFAULT-NEXT:                     let %16: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(40));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%17));
// DEFAULT-NEXT:                 break %9;
// DEFAULT-NEXT:                 default %9:
// DEFAULT-NEXT:                     write<i32>(%4, const<i32>(90));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @classify_direct(%6 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %10 read<i32>(%6)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %10 const<i32>(-2) ... const<i32>(2):
// DEFAULT-NEXT:                     return const<i32>(7);
// DEFAULT-NEXT:                 default %10:
// DEFAULT-NEXT:                     return const<i32>(9);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(37)>(%11)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(4)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(5)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(7)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(8)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(9)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(10)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(11)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(12)), call<i32, signature=fn(i32) -> i32>(%5, neg<i32, overflow=ub>(const<i32>(1))), call<i32, signature=fn(i32) -> i32>(%5, const<i32>(3)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
