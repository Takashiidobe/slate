#include <stdio.h>
#include <string.h>

static int score_text(const unsigned char *bytes, int len) {
  (void)strlen((const char *)bytes);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += bytes[i];
  return score;
}

static int forward_text(const unsigned char *bytes, int len) {
  return score_text(bytes, len);
}

int main(void) {
  const unsigned char bytes[] = "abc";
  int                 score   = forward_text(bytes, 3);
  printf("%d\n", score);
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
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%14 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @strlen(%15 __s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @score_text(%4 bytes: ptr<const u8>, %5 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>) -> u64>(%2, pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const u8>>(%4)));
// DEFAULT-NEXT:         let %6 score: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %7 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), read<i32>(%5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(%4), read<i32>(%7)))))));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%21));
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @forward_text(%9 bytes: ptr<const u8>, %10 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const u8>, i32) -> i32>(%3, read<ptr<const u8>>(%9), read<i32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 bytes: array<u8, 4> [storage=automatic] [const] = code_units<array<u8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %13 score: i32 [storage=automatic] = call<i32, signature=fn(ptr<const u8>, i32) -> i32>(%8, array_decay<ptr<const u8>, length=Some(4)>(%12), const<i32>(3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), read<i32>(%13));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
