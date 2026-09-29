#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char a[32];
  char b[32];
  char c[32];
  strcpy(a, "  -42abc");
  strcpy(b, "+1000000000000zzz");
  strcpy(c, "9000000000000");
  printf("%d\n", atoi(a));
  printf("%ld\n", atol(b));
  printf("%lld\n", atoll(c));
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([32, 32, 45, 52, 50, 97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([43, 49, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 122, 122, 122, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([57, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atoi:[0-9]+]] @atoi(%[[VALUE___nptr:[0-9]+]] __nptr: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_atol:[0-9]+]] @atol(%[[VALUE___nptr_2:[0-9]+]] __nptr: ptr<const i8>) -> i64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_atoll:[0-9]+]] @atoll(%[[VALUE___nptr_3:[0-9]+]] __nptr: ptr<const i8>) -> i64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strcpy:[0-9]+]] @strcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<i8> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i8, 32> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<i8, 32> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: array<i8, 32> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(32)>(%[[VALUE_a]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(32)>(%[[VALUE_b]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(32)>(%[[VALUE_c]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_5]])), call<i64, signature=fn(ptr<const i8>) -> i64>(%[[VALUE_atol]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_6]])), call<i64, signature=fn(ptr<const i8>) -> i64>(%[[VALUE_atoll]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%[[VALUE_c]]))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
