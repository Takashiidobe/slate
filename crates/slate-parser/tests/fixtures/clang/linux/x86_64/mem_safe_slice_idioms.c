#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int get_n(void) { return 3; }

int main(void) {
  unsigned char dst_a[8] = {0};
  unsigned char src_a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memcpy(dst_a, src_a, 4);

  unsigned char dst_b[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  unsigned char src_b[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(dst_b, src_b, 5);

  unsigned char self_c[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(self_c, self_c, 8);

  unsigned char dst_d[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memset(dst_d, 0x41, 5);

  unsigned char dst_e[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char src_e[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  memcpy(dst_e, src_e + 2, 4);

  unsigned char dst_f[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char src_f[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  int           n        = get_n();
  memcpy(dst_f, src_f, n);

  unsigned char  dst_g[4] = {0, 0, 0, 0};
  unsigned char *src_g    = malloc(4);
  memset(src_g, 5, 4);
  memcpy(dst_g, src_g, 4);
  free(src_g);

  for (int i = 0; i < 8; i++)
    printf("%d ", dst_a[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_b[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", self_c[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_d[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_e[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_f[i]);
  for (int i = 0; i < 4; i++)
    printf("%d ", dst_g[i]);
  printf("\n");
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
// DEFAULT-NEXT:     global %54 .str54: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %60 .str60: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %62 .str62: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %64 .str64: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %66 .str66: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %67 .str67: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%41 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%42 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @free(%43 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @memcpy(%44 __dest: ptr<void> [restrict], %45 __src: ptr<const void> [restrict], %46 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %14 @memmove(%47 __dest: ptr<void>, %48 __src: ptr<const void>, %49 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %18 @memset(%50 __s: ptr<void>, %51 __c: i32, %52 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %19 @get_n() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %21 dst_a: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=true>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %22 src_a: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%10, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%21)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%22)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         let %23 dst_b: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))));
// DEFAULT-NEXT:         let %24 src_b: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%23)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%24)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         let %25 self_c: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%25)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%25)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         let %26 dst_d: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%18, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%26)), const<i32>(65), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         let %27 dst_e: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         let %28 src_e: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%10, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%27)), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%28), const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         let %29 dst_f: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         let %30 src_f: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))));
// DEFAULT-NEXT:         let %31 n: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%19);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%10, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%29)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%30)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%31))));
// DEFAULT-NEXT:         let %32 dst_g: array<u8, 4> [storage=automatic] = aggregate<array<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %33 src_g: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%18, pointer_cast<ptr<void>, reason=arg>(read<ptr<u8>>(%33)), const<i32>(5), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%10, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(4)>(%32)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<u8>>(%33)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<u8>>(%33)));
// DEFAULT-NEXT:         for %53
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %34 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%34), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %68: i32 [synthetic] = read<i32>(%34);
// DEFAULT-NEXT:                 let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%34, read<i32>(%69));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%54)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%21), read<i32>(%34)))))));
// DEFAULT-NEXT:         for %55
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %35 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%35), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %70: i32 [synthetic] = read<i32>(%35);
// DEFAULT-NEXT:                 let %71: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%70), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%35, read<i32>(%71));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%56)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%23), read<i32>(%35)))))));
// DEFAULT-NEXT:         for %57
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %36 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%36), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %72: i32 [synthetic] = read<i32>(%36);
// DEFAULT-NEXT:                 let %73: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%72), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%36, read<i32>(%73));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%58)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%25), read<i32>(%36)))))));
// DEFAULT-NEXT:         for %59
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %37 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%37), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %74: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:                 let %75: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%74), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%37, read<i32>(%75));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%60)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%26), read<i32>(%37)))))));
// DEFAULT-NEXT:         for %61
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %38 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%38), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %76: i32 [synthetic] = read<i32>(%38);
// DEFAULT-NEXT:                 let %77: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%76), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%38, read<i32>(%77));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%62)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%27), read<i32>(%38)))))));
// DEFAULT-NEXT:         for %63
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %39 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%39), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %78: i32 [synthetic] = read<i32>(%39);
// DEFAULT-NEXT:                 let %79: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%78), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%39, read<i32>(%79));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%64)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%29), read<i32>(%39)))))));
// DEFAULT-NEXT:         for %65
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %40 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%40), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %80: i32 [synthetic] = read<i32>(%40);
// DEFAULT-NEXT:                 let %81: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%80), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%40, read<i32>(%81));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%66)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%32), read<i32>(%40)))))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%67)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
