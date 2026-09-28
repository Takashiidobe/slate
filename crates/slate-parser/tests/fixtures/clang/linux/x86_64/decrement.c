#include <stdio.h>

int main(void) {
  int a    = 5;
  int post = a--;
  int pre  = --a;
  int sum  = a-- + --pre;
  printf("%d %d %d %d\n", a, post, pre, sum);

  unsigned char c = 0;
  c--;
  printf("%u\n", (unsigned)c);
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
// DEFAULT-NEXT:     global %8 .str8: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%7 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 a: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %3 post: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %11: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%11));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%10));
// DEFAULT-NEXT:         let %4 pre: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%13));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%13));
// DEFAULT-NEXT:         let %5 sum: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %15: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%15));
// DEFAULT-NEXT:         let %16: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %17: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%17));
// DEFAULT-NEXT:         write<i32>(%5, add<i32, overflow=ub>(read<i32>(%14), read<i32>(%17)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%8)), read<i32>(%2), read<i32>(%3), read<i32>(%4), read<i32>(%5));
// DEFAULT-NEXT:         let %6 c: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %18: u8 [synthetic] = read<u8>(%6);
// DEFAULT-NEXT:         let %19: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%18))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%6, read<u8>(%19));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), widen<u32, reason=explicit>(read<u8>(%6)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
