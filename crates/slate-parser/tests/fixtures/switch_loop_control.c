#include <stdio.h>

int run(void) {
  int out = 0;
  for (int i = 0; i <= 3; i++) {
    switch (i) {
    case 0:
      out += 1;
      break;
    case 1:
      continue;
    case 2:
      out += 20;
      break;
    default:
      out += 100;
      break;
    }
    out += 3;
  }
  return out;
}

int main(void) {
  printf("%d\n", run());
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
// DEFAULT-NEXT:     global %8 .str8: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%5 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @run() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 out: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %3 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%3), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     switch %7 read<i32>(%3)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %7 const<i32>(0):
// DEFAULT-NEXT:                                 let %11: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                                 let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%2, read<i32>(%12));
// DEFAULT-NEXT:                             break %7;
// DEFAULT-NEXT:                             case %7 const<i32>(1):
// DEFAULT-NEXT:                                 continue %6;
// DEFAULT-NEXT:                             case %7 const<i32>(2):
// DEFAULT-NEXT:                                 let %13: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(20));
// DEFAULT-NEXT:                                 write<i32>(%2, read<i32>(%14));
// DEFAULT-NEXT:                             break %7;
// DEFAULT-NEXT:                             default %7:
// DEFAULT-NEXT:                                 let %15: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(100));
// DEFAULT-NEXT:                                 write<i32>(%2, read<i32>(%16));
// DEFAULT-NEXT:                             break %7;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     let %17: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                     let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(3));
// DEFAULT-NEXT:                     write<i32>(%2, read<i32>(%18));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%8)), call<i32, signature=fn() -> i32>(%1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
