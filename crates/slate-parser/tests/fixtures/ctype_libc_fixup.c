#include <ctype.h>
#include <stdio.h>

static int next_lower(void) {
  static int c = 'a';
  return c++;
}

int main(void) {
  int upper = 'Q';
  int lower = 'q';
  int digit = '5';
  int punct = '!';
  int eof   = EOF;

  printf("%d %d %d %d %d\n", toupper(lower), toupper(digit), toupper(punct),
         toupper(upper), toupper(eof));
  printf("%d %d %d %d %d\n", tolower(upper), tolower(digit), tolower(punct),
         tolower(lower), tolower(eof));
  printf("%d %d\n", toupper(next_lower()), tolower(next_lower()));
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
// DEFAULT-NEXT:     global %4 c: i32 [storage=static] = const<i32>(97) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @tolower(%11 __c: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @toupper(%12 __c: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @printf(%13 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @next_lower() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%18));
// DEFAULT-NEXT:         return read<i32>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 upper: i32 [storage=automatic] = const<i32>(81);
// DEFAULT-NEXT:         let %7 lower: i32 [storage=automatic] = const<i32>(113);
// DEFAULT-NEXT:         let %8 digit: i32 [storage=automatic] = const<i32>(53);
// DEFAULT-NEXT:         let %9 punct: i32 [storage=automatic] = const<i32>(33);
// DEFAULT-NEXT:         let %10 eof: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%14)), call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%7)), call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%8)), call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%9)), call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%6)), call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%10)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%15)), call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%6)), call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%8)), call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%9)), call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%7)), call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%10)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%16)), call<i32, signature=fn(i32) -> i32>(%1, call<i32, signature=fn() -> i32>(%3)), call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn() -> i32>(%3)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
