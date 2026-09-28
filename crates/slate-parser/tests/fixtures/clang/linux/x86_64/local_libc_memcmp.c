#include <stdio.h>
#include <string.h>

static int cmp_bytes(const unsigned char *a, int alen, const unsigned char *b,
                     int blen) {
  int sa = 0, sb = 0;
  for (int i = 0; i < alen; i++)
    sa += a[i];
  for (int i = 0; i < blen; i++)
    sb += b[i];
  int order = memcmp(a, b, 3);
  int sign  = (order > 0) - (order < 0);
  return sign * 1000 + (sa - sb);
}

int main(void) {
  unsigned char x[] = {1, 2, 3};
  unsigned char y[] = {1, 2, 4};
  unsigned char z[] = {1, 2, 3};
  printf("%d %d %d\n", cmp_bytes(x, 3, y, 3), cmp_bytes(y, 3, x, 3),
         cmp_bytes(x, 3, z, 3));
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
// DEFAULT-NEXT:     global %28 .str28: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%22 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @memcmp(%23 __s1: ptr<const void>, %24 __s2: ptr<const void>, %25 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %7 @cmp_bytes(%8 a: ptr<const u8>, %9 alen: i32, %10 b: ptr<const u8>, %11 blen: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 sa: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %13 sb: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %14 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), read<i32>(%9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(%8), read<i32>(%14)))))));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%32));
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %15 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%15), read<i32>(%11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%34));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(%10), read<i32>(%15)))))));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%36));
// DEFAULT-NEXT:         let %16 order: i32 [storage=automatic] = call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%6, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const u8>>(%8)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const u8>>(%10)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         let %17 sign: i32 [storage=automatic] = sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%16), const<i32>(0))), from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%16), const<i32>(0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%17), const<i32>(1000)), sub<i32, overflow=ub>(read<i32>(%12), read<i32>(%13)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %19 x: array<u8, 3> [storage=automatic] = aggregate<array<u8, 3>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         let %20 y: array<u8, 3> [storage=automatic] = aggregate<array<u8, 3>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         let %21 z: array<u8, 3> [storage=automatic] = aggregate<array<u8, 3>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%28)), call<i32, signature=fn(ptr<const u8>, i32, ptr<const u8>, i32) -> i32>(%7, pointer_cast<ptr<const u8>, reason=arg>(array_decay<ptr<u8>, length=Some(3)>(%19)), const<i32>(3), pointer_cast<ptr<const u8>, reason=arg>(array_decay<ptr<u8>, length=Some(3)>(%20)), const<i32>(3)), call<i32, signature=fn(ptr<const u8>, i32, ptr<const u8>, i32) -> i32>(%7, pointer_cast<ptr<const u8>, reason=arg>(array_decay<ptr<u8>, length=Some(3)>(%20)), const<i32>(3), pointer_cast<ptr<const u8>, reason=arg>(array_decay<ptr<u8>, length=Some(3)>(%19)), const<i32>(3)), call<i32, signature=fn(ptr<const u8>, i32, ptr<const u8>, i32) -> i32>(%7, pointer_cast<ptr<const u8>, reason=arg>(array_decay<ptr<u8>, length=Some(3)>(%19)), const<i32>(3), pointer_cast<ptr<const u8>, reason=arg>(array_decay<ptr<u8>, length=Some(3)>(%21)), const<i32>(3)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
