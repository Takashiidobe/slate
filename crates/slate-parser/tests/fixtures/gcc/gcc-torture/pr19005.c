/* PR target/19005 */
extern void abort(void);

int v, s;

void bar(int a, int b) {
  unsigned char x = v;

  if (!s) {
    if (a != x || b != (unsigned char)(x + 1))
      abort();
  } else if (a != (unsigned char)(x + 1) || b != x)
    abort();
  s ^= 1;
}

int foo(int x) {
  unsigned char a = x, b = x + 1;

  bar(a, b);
  a ^= b;
  b ^= a;
  a ^= b;
  bar(a, b);
  return 0;
}

int main(void) {
  for (v = -10; v < 266; v++)
    foo(v);
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
// DEFAULT-NEXT:     global %1 v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 s: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @bar(%4 a: i32, %5 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 x: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%1)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%2), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(read<i32>(%4), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6)))), ne<i32>(read<i32>(%5), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))), const<i32>(1))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_or<bool>(ne<i32>(read<i32>(%4), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))), const<i32>(1))))))), ne<i32>(read<i32>(%5), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6)))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %14: i32 [synthetic] = xor<i32>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @foo(%8 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 a: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%8)));
// DEFAULT-NEXT:         let %10 b: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%8), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%3, reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(%9))), reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(%10))));
// DEFAULT-NEXT:         let %15: u8 [synthetic] = read<u8>(%9);
// DEFAULT-NEXT:         let %16: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%15))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%10))))));
// DEFAULT-NEXT:         write<u8>(%9, read<u8>(%16));
// DEFAULT-NEXT:         let %17: u8 [synthetic] = read<u8>(%10);
// DEFAULT-NEXT:         let %18: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%17))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%9))))));
// DEFAULT-NEXT:         write<u8>(%10, read<u8>(%18));
// DEFAULT-NEXT:         let %19: u8 [synthetic] = read<u8>(%9);
// DEFAULT-NEXT:         let %20: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%19))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%10))))));
// DEFAULT-NEXT:         write<u8>(%9, read<u8>(%20));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%3, reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(%9))), reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(%10))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1, neg<i32, overflow=ub>(const<i32>(10)));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1), const<i32>(266))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(i32) -> i32>(%7, read<i32>(%1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
