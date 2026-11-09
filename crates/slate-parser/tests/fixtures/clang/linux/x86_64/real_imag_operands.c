short s;
int sizes[sizeof(__real__ s) + sizeof(__imag__ s)];
enum { K = __imag__ (short)3 + __real__ (char)4 };

int scalar_real_lvalue(int i) {
  __real__ i = 1;
  __real__ i += 2;
  __asm__("" : "+r"(__real__ i));
  return i;
}

int scalar_parts(void) {
  return __imag__ s + __real__ s;
}

_Complex int complex_part_outputs(_Complex int r) {
  __asm__("" : "=rm"(__imag__ r));
  __asm__("" : "=m"(__real__ r));
  return r;
}

_Complex int cast_operand(int c) {
  return __real__ (_Complex int)c + (_Complex int)c;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_K:[0-9]+]] K = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_K]] s: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sizes:[0-9]+]] sizes: array<i32, 4> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalar_real_lvalue:[0-9]+]] @scalar_real_lvalue(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_scalar_parts:[0-9]+]] @scalar_parts() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<i16>(%[[VALUE_K]]);
// DEFAULT-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(const<i16>(0)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_K]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_complex_part_outputs:[0-9]+]] @complex_part_outputs(%[[VALUE_r:[0-9]+]] r: complex<i32>) -> complex<i32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "rm" [reg | mem] -> reg width 32 place<i32>(imag(%[[VALUE_r]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             lateout 0 "m" [mem] width 32 place<i32>(real(%[[VALUE_r]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<complex<i32>>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_cast_operand:[0-9]+]] @cast_operand(%[[VALUE_c:[0-9]+]] c: i32) -> complex<i32> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<complex<i32>, complex=true, overflow=ub>(complex_to_real<i32, reason=explicit>(real_to_complex<complex<i32>, reason=explicit>(read<i32>(%[[VALUE_c]]))), real_to_complex<complex<i32>, reason=explicit>(read<i32>(%[[VALUE_c]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
