
unsigned char a = 6;
int           b, c;

static void fn1() {
  int           i = a > 1 ? 1 : a, j = 6 & (c = a && (b = a));
  int           d = 0, e = a, f = ~c, g = b || a;
  unsigned char h = ~a;
  if (a)
    f = j;
  if (h && g)
    d = a;
  i = -~(f * d * h) + c && (e || i) ^ f;
  if (i != 1)
    __builtin_abort();
}

int main() {
  fn1();
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
// DEFAULT-NEXT:     global %0 a: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @fn1() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic] = conditional<i32>(gt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))), const<i32>(1)), const<i32>(1), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))));
// DEFAULT-NEXT:         let %5 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12: bool [synthetic];
// DEFAULT-NEXT:         if ne<u8>(read<u8>(%0), const<u8>(0))
// DEFAULT-NEXT:             write<i32>(%1, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%0))));
// DEFAULT-NEXT:             write<bool>(%12, ne<i32>(reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%0))), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%12, const<bool>(false));
// DEFAULT-NEXT:         write<i32>(%2, from_bool<i32, reason=assign>(read<bool>(%12)));
// DEFAULT-NEXT:         write<i32>(%5, and<i32>(const<i32>(6), from_bool<i32, reason=assign>(read<bool>(%12))));
// DEFAULT-NEXT:         let %6 d: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %7 e: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%0)));
// DEFAULT-NEXT:         let %8 f: i32 [storage=automatic] = not<i32>(read<i32>(%2));
// DEFAULT-NEXT:         let %9 g: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%1), const<i32>(0)), ne<u8>(read<u8>(%0), const<u8>(0))));
// DEFAULT-NEXT:         let %10 h: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%0))))));
// DEFAULT-NEXT:         if ne<u8>(read<u8>(%0), const<u8>(0))
// DEFAULT-NEXT:             write<i32>(%8, read<i32>(%5));
// DEFAULT-NEXT:         if logical_and<bool>(ne<u8>(read<u8>(%10), const<u8>(0)), ne<i32>(read<i32>(%9), const<i32>(0)))
// DEFAULT-NEXT:             write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%0))));
// DEFAULT-NEXT:         write<i32>(%4, from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(add<i32, overflow=ub>(neg<i32, overflow=ub>(not<i32>(mul<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%8), read<i32>(%6)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%10)))))), read<i32>(%2)), const<i32>(0)), ne<i32>(xor<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(read<i32>(%7), const<i32>(0)), ne<i32>(read<i32>(%4), const<i32>(0)))), read<i32>(%8)), const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
