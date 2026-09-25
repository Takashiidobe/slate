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
// DEFAULT-NEXT:     global %26 .str26: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([255, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%18 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @malloc(%19 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @free(%20 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @memcpy(%21 __dest: ptr<void> [restrict], %22 __src: ptr<const void> [restrict], %23 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @strlen(%24 __s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %6 @consume_bin(%7 buf: ptr<i8>, %8 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>) -> u64>(%5, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%7)));
// DEFAULT-NEXT:         let %9 score: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %10 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), read<i32>(%8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%29));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%7), read<i32>(%10))))))));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%31));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%7)));
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @forward_bin(%12 buf: ptr<i8>, %13 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<i8>, i32) -> i32>(%6, read<ptr<i8>>(%12), read<i32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 len: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %16 buf: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))), const<u64>(1))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%16)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%26)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%15))));
// DEFAULT-NEXT:         let %17 score: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, i32) -> i32>(%11, read<ptr<i8>>(%16), read<i32>(%15));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%27)), read<i32>(%17));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
