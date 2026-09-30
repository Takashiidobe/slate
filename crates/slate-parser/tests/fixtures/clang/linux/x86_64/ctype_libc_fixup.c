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
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] = const<i32>(97) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_tolower:[0-9]+]] @tolower(%[[VALUE___c:[0-9]+]] __c: i32) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_toupper:[0-9]+]] @toupper(%[[VALUE___c_2:[0-9]+]] __c: i32) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_next_lower:[0-9]+]] @next_lower() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE0]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_upper:[0-9]+]] upper: i32 [storage=automatic] = const<i32>(81);
// DEFAULT-NEXT:         let %[[VALUE_lower:[0-9]+]] lower: i32 [storage=automatic] = const<i32>(113);
// DEFAULT-NEXT:         let %[[VALUE_digit:[0-9]+]] digit: i32 [storage=automatic] = const<i32>(53);
// DEFAULT-NEXT:         let %[[VALUE_punct:[0-9]+]] punct: i32 [storage=automatic] = const<i32>(33);
// DEFAULT-NEXT:         let %[[VALUE_eof:[0-9]+]] eof: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_toupper]], read<i32>(%[[VALUE_lower]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_toupper]], read<i32>(%[[VALUE_digit]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_toupper]], read<i32>(%[[VALUE_punct]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_toupper]], read<i32>(%[[VALUE_upper]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_toupper]], read<i32>(%[[VALUE_eof]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_2]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_tolower]], read<i32>(%[[VALUE_upper]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_tolower]], read<i32>(%[[VALUE_digit]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_tolower]], read<i32>(%[[VALUE_punct]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_tolower]], read<i32>(%[[VALUE_lower]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_tolower]], read<i32>(%[[VALUE_eof]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_3]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_toupper]], call<i32, signature=fn() -> i32>(%[[VALUE_next_lower]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_tolower]], call<i32, signature=fn() -> i32>(%[[VALUE_next_lower]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
