#include <stdio.h>

int main(void) {
  unsigned char  buf[4] = {10, 20, 30, 40};
  unsigned char *hit    = (unsigned char *)__builtin_memchr(buf, 30, 4);
  unsigned char *miss   = (unsigned char *)__builtin_memchr(buf, 99, 4);
  printf("%ld %d\n", hit - buf, miss == 0);
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
// DEFAULT-NEXT:     global %6 .str6: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([37, 108, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%5 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 buf: array<u8, 4> [storage=automatic] = aggregate<array<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(10))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(20))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(30))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(40))));
// DEFAULT-NEXT:         let %3 hit: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(__builtin_memchr, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(4)>(%2)), const<i32>(30), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         let %4 miss: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(__builtin_memchr, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(4)>(%2)), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%6)), ptr_diff<i64, element=u8, same_array=required, overflow=ub>(read<ptr<u8>>(%3), array_decay<ptr<u8>, length=Some(4)>(%2)), from_bool<i32, reason=vararg>(eq<ptr<u8>>(read<ptr<u8>>(%4), null<ptr<u8>>)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
