/* PR target/98853 */

#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8 &&                        \
    __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
__attribute__((__noipa__)) unsigned long long
foo(unsigned x, unsigned long long y, unsigned long long z) {
  __builtin_memcpy(2 + (char *)&x, 2 + (char *)&y, 2);
  return x + z;
}
#endif

int main() {
#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8 &&                        \
    __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  if (foo(0x44444444U, 0x1111111111111111ULL, 0x2222222222222222ULL) !=
      0x2222222233336666ULL)
    __builtin_abort();
#endif
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: u32, %2 y: u64, %3 z: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<u32>>(%1)), const<i32>(2))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<u64>>(%2)), const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%1)), read<u64>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u32, u64, u64) -> u64>(%0, const<u32>(1145324612), const<u64>(1229782938247303441), const<u64>(2459565876494606882)), const<u64>(2459565876780951142))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
