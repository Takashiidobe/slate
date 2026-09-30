int yf = 0;

void pl(int q5, int nd) {
  unsigned int hp = q5;
  int          zx = (q5 == 0) ? hp : (hp / q5);

  yf = ((nd < 2) * zx != 0) ? nd : 0;
}

int main(void) {
  pl(1, !yf);
  if (yf != 1)
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
// DEFAULT-NEXT:     global %[[VALUE_yf:[0-9]+]] yf: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pl:[0-9]+]] @pl(%[[VALUE_q5:[0-9]+]] q5: i32, %[[VALUE_nd:[0-9]+]] nd: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_hp:[0-9]+]] hp: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%[[VALUE_q5]]));
// DEFAULT-NEXT:         let %[[VALUE_zx:[0-9]+]] zx: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(conditional<u32>(eq<i32>(read<i32>(%[[VALUE_q5]]), const<i32>(0)), read<u32>(%[[VALUE_hp]]), div<u32, by_zero=ub>(read<u32>(%[[VALUE_hp]]), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_q5]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_yf]], conditional<i32>(ne<i32>(mul<i32, overflow=ub>(from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%[[VALUE_nd]]), const<i32>(2))), read<i32>(%[[VALUE_zx]])), const<i32>(0)), read<i32>(%[[VALUE_nd]]), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_pl]], const<i32>(1), from_bool<i32, reason=arg>(not<bool>(ne<i32>(read<i32>(%[[VALUE_yf]]), const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_yf]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
