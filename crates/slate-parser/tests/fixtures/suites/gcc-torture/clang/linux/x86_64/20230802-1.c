/*  We used to simplify these incorrectly.  */
__attribute__((noipa)) long long foo(unsigned int x) {
  int y = x;
  y     = ~y;
  return ((long long)x) & y;
}

__attribute__((noipa)) long long foo_v(volatile unsigned int x) {
  volatile int y = x;
  y              = ~y;
  return ((long long)x) & y;
}

__attribute__((noipa)) long long bar(unsigned int x) {
  int y = x;
  y     = ~y;
  return ((long long)x) ^ y;
}

__attribute__((noipa)) long long bar_v(volatile unsigned int x) {
  volatile int y = x;
  y              = ~y;
  return ((long long)x) ^ y;
}

__attribute__((noipa)) long long baz(unsigned int x) {
  int y = x;
  y     = ~y;
  return y ^ ((long long)x);
}

__attribute__((noipa)) long long baz_v(volatile unsigned int x) {
  volatile int y = x;
  y              = ~y;
  return y ^ ((long long)x);
}

int main() {
  for (int t = -1; t <= 1; t++) {
    if (foo(t) != foo_v(t))
      __builtin_abort();
    if (bar(t) != bar_v(t))
      __builtin_abort();
    if (baz(t) != baz_v(t))
      __builtin_abort();
  }
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], not<i32>(read<i32>(%[[VALUE_y]])));
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32>(%[[VALUE_x]]))), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_y]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo_v:[0-9]+]] @foo_v(%[[VALUE_x_2:[0-9]+]] x: volatile u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: volatile i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32, volatile>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_y_2]], not<i32>(read<i32, volatile>(%[[VALUE_y_2]])));
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32, volatile>(%[[VALUE_x_2]]))), widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_y_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_3:[0-9]+]] x: u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y_3]], not<i32>(read<i32>(%[[VALUE_y_3]])));
// DEFAULT-NEXT:         return xor<i64>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32>(%[[VALUE_x_3]]))), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_y_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_v:[0-9]+]] @bar_v(%[[VALUE_x_4:[0-9]+]] x: volatile u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_4:[0-9]+]] y: volatile i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32, volatile>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_y_4]], not<i32>(read<i32, volatile>(%[[VALUE_y_4]])));
// DEFAULT-NEXT:         return xor<i64>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32, volatile>(%[[VALUE_x_4]]))), widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_y_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_5:[0-9]+]] x: u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_5:[0-9]+]] y: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(%[[VALUE_x_5]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y_5]], not<i32>(read<i32>(%[[VALUE_y_5]])));
// DEFAULT-NEXT:         return xor<i64>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_y_5]])), reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32>(%[[VALUE_x_5]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz_v:[0-9]+]] @baz_v(%[[VALUE_x_6:[0-9]+]] x: volatile u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_6:[0-9]+]] y: volatile i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32, volatile>(%[[VALUE_x_6]]));
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_y_6]], not<i32>(read<i32, volatile>(%[[VALUE_y_6]])));
// DEFAULT-NEXT:         return xor<i64>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_y_6]])), reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32, volatile>(%[[VALUE_x_6]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_t:[0-9]+]] t: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_t]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_t]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i64>(call<i64, signature=fn(u32) -> i64>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_t]]))), call<i64, signature=fn(u32) -> i64>(%[[VALUE_foo_v]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_t]]))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i64>(call<i64, signature=fn(u32) -> i64>(%[[VALUE_bar]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_t]]))), call<i64, signature=fn(u32) -> i64>(%[[VALUE_bar_v]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_t]]))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i64>(call<i64, signature=fn(u32) -> i64>(%[[VALUE_baz]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_t]]))), call<i64, signature=fn(u32) -> i64>(%[[VALUE_baz_v]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_t]]))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
