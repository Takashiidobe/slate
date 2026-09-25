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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 y: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(%1));
// DEFAULT-NEXT:         write<i32>(%2, not<i32>(read<i32>(%2)));
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32>(%1))), widen<i64, reason=usual_arith>(read<i32>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo_v(%4 x: volatile u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 y: volatile i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32, volatile>(%4));
// DEFAULT-NEXT:         write<i32, volatile>(%5, not<i32>(read<i32, volatile>(%5)));
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32, volatile>(%4))), widen<i64, reason=usual_arith>(read<i32, volatile>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar(%7 x: u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 y: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(%7));
// DEFAULT-NEXT:         write<i32>(%8, not<i32>(read<i32>(%8)));
// DEFAULT-NEXT:         return xor<i64>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32>(%7))), widen<i64, reason=usual_arith>(read<i32>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @bar_v(%10 x: volatile u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 y: volatile i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32, volatile>(%10));
// DEFAULT-NEXT:         write<i32, volatile>(%11, not<i32>(read<i32, volatile>(%11)));
// DEFAULT-NEXT:         return xor<i64>(reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32, volatile>(%10))), widen<i64, reason=usual_arith>(read<i32, volatile>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @baz(%13 x: u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 y: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(%13));
// DEFAULT-NEXT:         write<i32>(%14, not<i32>(read<i32>(%14)));
// DEFAULT-NEXT:         return xor<i64>(widen<i64, reason=usual_arith>(read<i32>(%14)), reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32>(%13))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @baz_v(%16 x: volatile u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 y: volatile i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32, volatile>(%16));
// DEFAULT-NEXT:         write<i32, volatile>(%17, not<i32>(read<i32, volatile>(%17)));
// DEFAULT-NEXT:         return xor<i64>(widen<i64, reason=usual_arith>(read<i32, volatile>(%17)), reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(read<u32, volatile>(%16))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %19 t: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%19), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i64>(call<i64, signature=fn(u32) -> i64>(%0, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%19))), call<i64, signature=fn(u32) -> i64>(%3, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%19))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i64>(call<i64, signature=fn(u32) -> i64>(%6, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%19))), call<i64, signature=fn(u32) -> i64>(%9, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%19))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i64>(call<i64, signature=fn(u32) -> i64>(%12, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%19))), call<i64, signature=fn(u32) -> i64>(%15, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%19))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
