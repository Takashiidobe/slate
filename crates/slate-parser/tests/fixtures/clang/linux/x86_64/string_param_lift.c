#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_num(char *s) { return atoi(s); }

int forward_num(char *s) { return parse_num(s); }

int text_len(char *s) { return (int)strlen(s); }

int main(void) {
  char digits[] = "42";
  char word[]   = "hello";
  printf("%d %d\n", forward_num(digits), text_len(word));
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %19 .str19: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%16 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @atoi(%17 __nptr: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %6 @strlen(%18 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %7 @parse_num(%8 s: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @forward_num(%10 s: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<i8>) -> i32>(%7, read<ptr<i8>>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @text_len(%12 s: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%6, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%12)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 digits: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([52, 50, 0]);
// DEFAULT-NEXT:         let %15 word: array<i8, 6> [storage=automatic] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%19)), call<i32, signature=fn(ptr<i8>) -> i32>(%9, array_decay<ptr<i8>, length=Some(3)>(%14)), call<i32, signature=fn(ptr<i8>) -> i32>(%11, array_decay<ptr<i8>, length=Some(6)>(%15)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
