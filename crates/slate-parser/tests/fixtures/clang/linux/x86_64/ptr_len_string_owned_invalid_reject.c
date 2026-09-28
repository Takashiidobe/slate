#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int consume_bin(char *buf, int len) {
  (void)strlen(buf);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += (unsigned char)buf[i];
  free(buf);
  return score;
}

static int forward_bin(char *buf, int len) { return consume_bin(buf, len); }

int main(void) {
  int   len = 2;
  char *buf = malloc(len * sizeof(char));
  memcpy(buf, "\xff", len);
  int score = forward_bin(buf, len);
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
// DEFAULT-NEXT:     global %33 .str33: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([255, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%25 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%26 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @free(%27 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @memcpy(%28 __dest: ptr<void> [restrict], %29 __src: ptr<const void> [restrict], %30 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %12 @strlen(%31 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %13 @consume_bin(%14 buf: ptr<i8>, %15 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>) -> u64>(%12, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%14)));
// DEFAULT-NEXT:         let %16 score: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %32
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %17 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), read<i32>(%15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%36));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %37: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%37), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%14), read<i32>(%17))))))));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%38));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%14)));
// DEFAULT-NEXT:         return read<i32>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @forward_bin(%19 buf: ptr<i8>, %20 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<i8>, i32) -> i32>(%13, read<ptr<i8>>(%19), read<i32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %22 len: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %23 buf: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%22))), const<u64>(1))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%10, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%23)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%33)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%22))));
// DEFAULT-NEXT:         let %24 score: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, i32) -> i32>(%18, read<ptr<i8>>(%23), read<i32>(%22));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%34)), read<i32>(%24));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
