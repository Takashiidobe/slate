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
// DEFAULT-NEXT:     global %[[VALUE_values:[0-9]+]] values: array<i32, 8> [storage=static] [align=16] = aggregate<array<i32, 8>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = neg<i32, overflow=ub>(const<i32>(1)), index5 = neg<i32, overflow=ub>(const<i32>(2)), index6 = neg<i32, overflow=ub>(const<i32>(3)), index7 = const<i32>(65664)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_numvalues:[0-9]+]] numvalues: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(32), const<u64>(4)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_h0:[0-9]+]] @h0(%[[VALUE_A:[0-9]+]] A: i32, %[[VALUE_B:[0-9]+]] B: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(or<i32>(read<i32>(%[[VALUE_A]]), read<i32>(%[[VALUE_B]])), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_A]]), read<i32>(%[[VALUE_B]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_i0:[0-9]+]] @i0(%[[VALUE_A_2:[0-9]+]] A: i32, %[[VALUE_B_2:[0-9]+]] B: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(read<i32>(%[[VALUE_A_2]]), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_A_2]]), read<i32>(%[[VALUE_B_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_k0:[0-9]+]] @k0(%[[VALUE_A_3:[0-9]+]] A: i32, %[[VALUE_B_3:[0-9]+]] B: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(and<i32>(read<i32>(%[[VALUE_A_3]]), read<i32>(%[[VALUE_B_3]])), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_A_3]]), read<i32>(%[[VALUE_B_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h1:[0-9]+]] @h1(%[[VALUE_A_4:[0-9]+]] A: volatile i32, %[[VALUE_B_4:[0-9]+]] B: volatile i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(or<i32>(read<i32, volatile>(%[[VALUE_A_4]]), read<i32, volatile>(%[[VALUE_B_4]])), from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%[[VALUE_A_4]]), read<i32, volatile>(%[[VALUE_B_4]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_i1:[0-9]+]] @i1(%[[VALUE_A_5:[0-9]+]] A: volatile i32, %[[VALUE_B_5:[0-9]+]] B: volatile i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(read<i32, volatile>(%[[VALUE_A_5]]), from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%[[VALUE_A_5]]), read<i32, volatile>(%[[VALUE_B_5]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_k1:[0-9]+]] @k1(%[[VALUE_A_6:[0-9]+]] A: volatile i32, %[[VALUE_B_6:[0-9]+]] B: volatile i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(and<i32>(read<i32, volatile>(%[[VALUE_A_6]]), read<i32, volatile>(%[[VALUE_B_6]])), from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%[[VALUE_A_6]]), read<i32, volatile>(%[[VALUE_B_6]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_A_7:[0-9]+]] A: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_A_7]]), read<i32>(%[[VALUE_numvalues]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_A_7]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_A_7]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %[[VALUE_B_7:[0-9]+]] B: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_B_7]]), read<i32>(%[[VALUE_numvalues]]))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_B_7]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_B_7]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%[[VALUE_values]]), read<i32>(%[[VALUE_A_7]]))));
// DEFAULT-NEXT:                             let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%[[VALUE_values]]), read<i32>(%[[VALUE_B_7]]))));
// DEFAULT-NEXT:                             if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_h0]], read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_h1]], read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                             if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_i0]], read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_i1]], read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                             if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_k0]], read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_k1]], read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
