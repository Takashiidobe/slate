/* PR tree-optimization/104604 */

unsigned char g;

__attribute__((noipa)) unsigned char foo(_Complex unsigned c) {
  unsigned char     v  = g;
  _Complex unsigned t  = 3;
  t                   /= c;
  return v + t;
}

__attribute__((noipa)) unsigned char bar(_Complex unsigned c) {
  unsigned char     v  = g;
  _Complex unsigned t  = 42;
  t                   /= c;
  return v + t;
}

int main() {
  unsigned char x = foo(7);
  if (x)
    __builtin_abort();
  if (bar(7) != 6)
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
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_c:[0-9]+]] c: complex<u32>) -> u8 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: u8 [storage=automatic] = read<u8>(%[[VALUE_g]]);
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: complex<u32> [storage=automatic] = real_to_complex<complex<u32>, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: complex<u32> [synthetic] = read<complex<u32>>(%[[VALUE_t]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: complex<u32> [synthetic] = div<complex<u32>, complex=true, overflow=wrap, by_zero=ub>(read<complex<u32>>(%[[VALUE0]]), read<complex<u32>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         write<complex<u32>>(%[[VALUE_t]], read<complex<u32>>(%[[VALUE1]]));
// DEFAULT-NEXT:         return truncate<u8, reason=return, fits=unknown>(complex_to_real<u32, reason=return>(add<complex<u32>, complex=true, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_v]])))), read<complex<u32>>(%[[VALUE_t]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_c_2:[0-9]+]] c: complex<u32>) -> u8 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: u8 [storage=automatic] = read<u8>(%[[VALUE_g]]);
// DEFAULT-NEXT:         let %[[VALUE_t_2:[0-9]+]] t: complex<u32> [storage=automatic] = real_to_complex<complex<u32>, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: complex<u32> [synthetic] = read<complex<u32>>(%[[VALUE_t_2]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: complex<u32> [synthetic] = div<complex<u32>, complex=true, overflow=wrap, by_zero=ub>(read<complex<u32>>(%[[VALUE2]]), read<complex<u32>>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:         write<complex<u32>>(%[[VALUE_t_2]], read<complex<u32>>(%[[VALUE3]]));
// DEFAULT-NEXT:         return truncate<u8, reason=return, fits=unknown>(complex_to_real<u32, reason=return>(add<complex<u32>, complex=true, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_v_2]])))), read<complex<u32>>(%[[VALUE_t_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u8 [storage=automatic] = call<u8, signature=fn(complex<u32>) -> u8, abi=sysv64(native_c) -> scalar>(%[[VALUE_foo]], real_to_complex<complex<u32>, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         if ne<u8>(read<u8>(%[[VALUE_x]]), const<u8>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(complex<u32>) -> u8, abi=sysv64(native_c) -> scalar>(%[[VALUE_bar]], real_to_complex<complex<u32>, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(7)))))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
