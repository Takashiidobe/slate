/* PR target/119428 */

__attribute__((noipa)) void foo(unsigned int x, unsigned char *y) {
  y  += x >> 3;
  *y &= (unsigned char)~(1 << (x & 0x07));
}

int main() {
  unsigned char buf[8];
  __builtin_memset(buf, 0xff, 8);
  foo(8, buf);
  if (buf[1] != 0xfe)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: u32, %2 y: ptr<u8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5: ptr<u8> [synthetic] = read<ptr<u8>>(%2);
// DEFAULT-NEXT:         let %6: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%5), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%1), const<i32>(3)));
// DEFAULT-NEXT:         write<ptr<u8>>(%2, read<ptr<u8>>(%6));
// DEFAULT-NEXT:         let %7: ptr<u8> [synthetic] = read<ptr<u8>>(%2);
// DEFAULT-NEXT:         let %8: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%7)));
// DEFAULT-NEXT:         let %9: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), and<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7))))))))))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%7)), read<u8>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 buf: array<u8, 8> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%4)), const<i32>(255), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(u32, ptr<u8>) -> void>(%0, reinterpret<u32, reason=arg, fits=always>(const<i32>(8)), array_decay<ptr<u8>, length=Some(8)>(%4));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%4), const<i32>(1)))))), const<i32>(254))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
