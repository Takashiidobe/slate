/* PR rtl-optimization/63843 */

static inline __attribute__((always_inline)) unsigned short
foo(unsigned short v) {
  return (v << 8) | (v >> 8);
}

unsigned short __attribute__((noinline, noclone, hot)) bar(unsigned char *x) {
  unsigned int   a;
  unsigned short b;
  __builtin_memcpy(&a, &x[0], sizeof(a));
  a ^= 0x80808080U;
  __builtin_memcpy(&x[0], &a, sizeof(a));
  __builtin_memcpy(&b, &x[2], sizeof(b));
  return foo(b);
}

int main() {
  unsigned char x[8] = {0x01, 0x01, 0x01, 0x01};
  if (__CHAR_BIT__ == 8 && sizeof(short) == 2 && sizeof(int) == 4 &&
      bar(x) != 0x8181U)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %0 @foo(%1 v: u16) -> u16 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1))), const<i32>(8)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1))), const<i32>(8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar(%3 x: ptr<u8>) -> u16 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %5 b: u16 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u32>>(%4)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<u8>>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%3), const<i32>(0))))), const<u64>(4));
// DEFAULT-NEXT:         let %8: u32 [synthetic] = read<u32>(%4);
// DEFAULT-NEXT:         let %9: u32 [synthetic] = xor<u32>(read<u32>(%8), const<u32>(2155905152));
// DEFAULT-NEXT:         write<u32>(%4, read<u32>(%9));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u8>>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%3), const<i32>(0))))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<u32>>(%4)), const<u64>(4));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u16>>(%5)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<u8>>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%3), const<i32>(2))))), const<u64>(2));
// DEFAULT-NEXT:         return call<u16, signature=fn(u16) -> u16>(%0, read<u16>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 x: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=true>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(eq<i32>(const<i32>(8), const<i32>(8)), eq<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))), eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             write<bool>(%10, ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(ptr<u8>) -> u16>(%2, array_decay<ptr<u8>, length=Some(8)>(%7))))), const<u32>(33153)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
