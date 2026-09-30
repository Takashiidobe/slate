/* PR sanitizer/81281 */

void foo(unsigned p, unsigned a, unsigned b) {
  unsigned q = p + 7;
  if (a - (1U + __INT_MAX__) >= 2)
    __builtin_unreachable();
  int d = p + b;
  int c = p + a;
  if (c - d != __INT_MAX__)
    __builtin_abort();
}

void bar(unsigned p, unsigned a) {
  unsigned q = p + 7;
  if (a - (1U + __INT_MAX__) >= 2)
    __builtin_unreachable();
  int c = p;
  int d = p + a;
  if (c - d != -__INT_MAX__ - 1)
    __builtin_abort();
}

int main() {
  foo(-1U, 1U + __INT_MAX__, 1U);
  bar(-1U, 1U + __INT_MAX__);
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_unreachable:[0-9]+]] @__builtin_unreachable() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: u32, %[[VALUE_a:[0-9]+]] a: u32, %[[VALUE_b:[0-9]+]] b: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE_p]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         if ge<u32>(sub<u32, overflow=wrap>(read<u32>(%[[VALUE_a]]), add<u32, overflow=wrap>(const<u32>(1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_unreachable]]);
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_p]]), read<u32>(%[[VALUE_b]])));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_p]]), read<u32>(%[[VALUE_a]])));
// DEFAULT-NEXT:         if ne<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_c]]), read<i32>(%[[VALUE_d]])), const<i32>(2147483647))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p_2:[0-9]+]] p: u32, %[[VALUE_a_2:[0-9]+]] a: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q_2:[0-9]+]] q: u32 [storage=automatic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE_p_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         if ge<u32>(sub<u32, overflow=wrap>(read<u32>(%[[VALUE_a_2]]), add<u32, overflow=wrap>(const<u32>(1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_unreachable]]);
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:         let %[[VALUE_d_2:[0-9]+]] d: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_p_2]]), read<u32>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:         if ne<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_c_2]]), read<i32>(%[[VALUE_d_2]])), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32, u32) -> void>(%[[VALUE_foo]], neg<u32, overflow=wrap>(const<u32>(1)), add<u32, overflow=wrap>(const<u32>(1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))), const<u32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%[[VALUE_bar]], neg<u32, overflow=wrap>(const<u32>(1)), add<u32, overflow=wrap>(const<u32>(1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
