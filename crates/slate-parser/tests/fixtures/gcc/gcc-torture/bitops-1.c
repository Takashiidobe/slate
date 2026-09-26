/* PR tree-optimization/110726 */

#define DECLS(n, VOL)                                                          \
  __attribute__((noinline, noclone)) int h##n(VOL int A, VOL int B) {          \
    return (A | B) & (A == B);                                                 \
  }                                                                            \
  __attribute__((noinline, noclone)) int i##n(VOL int A, VOL int B) {          \
    return A | (A == B);                                                       \
  }                                                                            \
  __attribute__((noinline, noclone)) int k##n(VOL int A, VOL int B) {          \
    return (A & B) | (A == B);                                                 \
  }

DECLS(0, )
DECLS(1, volatile)

int values[]  = {0, 1, 2, 3, -1, -2, -3, 0x10080};
int numvalues = sizeof(values) / sizeof(values[0]);

int main() {
  for (int A = 0; A < numvalues; A++)
    for (int B = 0; B < numvalues; B++) {
      int a = values[A];
      int b = values[B];
      if (h0(a, b) != h1(a, b))
        __builtin_abort();
      if (i0(a, b) != i1(a, b))
        __builtin_abort();
      if (k0(a, b) != k1(a, b))
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
// DEFAULT-NEXT:     global %18 values: array<i32, 8> [storage=static] [align=16] = aggregate<array<i32, 8>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = neg<i32, overflow=ub>(const<i32>(1)), index5 = neg<i32, overflow=ub>(const<i32>(2)), index6 = neg<i32, overflow=ub>(const<i32>(3)), index7 = const<i32>(65664)) [linkage=external];
// DEFAULT-NEXT:     global %19 numvalues: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(32), const<u64>(4)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @h0(%1 A: i32, %2 B: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(or<i32>(read<i32>(%1), read<i32>(%2)), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%1), read<i32>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @i0(%4 A: i32, %5 B: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(read<i32>(%4), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%4), read<i32>(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @k0(%7 A: i32, %8 B: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(and<i32>(read<i32>(%7), read<i32>(%8)), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%7), read<i32>(%8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @h1(%10 A: volatile i32, %11 B: volatile i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(or<i32>(read<i32, volatile>(%10), read<i32, volatile>(%11)), from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%10), read<i32, volatile>(%11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @i1(%13 A: volatile i32, %14 B: volatile i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(read<i32, volatile>(%13), from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%13), read<i32, volatile>(%14))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @k1(%16 A: volatile i32, %17 B: volatile i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(and<i32>(read<i32, volatile>(%16), read<i32, volatile>(%17)), from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%16), read<i32, volatile>(%17))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %21 A: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%21), read<i32>(%19))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%21, read<i32>(%28));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %26
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %22 B: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%22), read<i32>(%19))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %29: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                         let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%22, read<i32>(%30));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %23 a: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%18), read<i32>(%21))));
// DEFAULT-NEXT:                             let %24 b: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%18), read<i32>(%22))));
// DEFAULT-NEXT:                             if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%0, read<i32>(%23), read<i32>(%24)), call<i32, signature=fn(i32, i32) -> i32>(%9, read<i32>(%23), read<i32>(%24)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                             if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%3, read<i32>(%23), read<i32>(%24)), call<i32, signature=fn(i32, i32) -> i32>(%12, read<i32>(%23), read<i32>(%24)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                             if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%6, read<i32>(%23), read<i32>(%24)), call<i32, signature=fn(i32, i32) -> i32>(%15, read<i32>(%23), read<i32>(%24)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
