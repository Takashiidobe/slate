int foo(int a) {
  int b = a == 0;
  return (a & b);
}

#define function(vol, cst)                                                     \
  __attribute__((noipa)) _Bool func_##cst##_##vol(vol int a) {                 \
    vol int b = a == cst;                                                      \
    return (a & b);                                                            \
  }

#define funcdefs(cst) function(, cst) function(volatile, cst)

#define funcs(f) f(0) f(1) f(5)

funcs(funcdefs)

#define test(cst)                                                              \
  do {                                                                         \
    if (func_##cst##_(a) != func_##cst##_volatile(a))                          \
      __builtin_abort();                                                       \
  } while (0);
    int main(void) {
  for (int a = -10; a <= 10; a++) {
    funcs(test)
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)));
// DEFAULT-NEXT:         return and<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func_0_:[0-9]+]] @func_0_(%[[VALUE_a_2:[0-9]+]] a: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32>(%[[VALUE_a_2]]), const<i32>(0)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]])), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func_0_volatile:[0-9]+]] @func_0_volatile(%[[VALUE_a_3:[0-9]+]] a: volatile i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_3:[0-9]+]] b: volatile i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32, volatile>(%[[VALUE_a_3]]), const<i32>(0)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32, volatile>(%[[VALUE_a_3]]), read<i32, volatile>(%[[VALUE_b_3]])), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func_1_:[0-9]+]] @func_1_(%[[VALUE_a_4:[0-9]+]] a: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_4:[0-9]+]] b: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32>(%[[VALUE_a_4]]), const<i32>(1)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_4]])), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func_1_volatile:[0-9]+]] @func_1_volatile(%[[VALUE_a_5:[0-9]+]] a: volatile i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_5:[0-9]+]] b: volatile i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32, volatile>(%[[VALUE_a_5]]), const<i32>(1)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32, volatile>(%[[VALUE_a_5]]), read<i32, volatile>(%[[VALUE_b_5]])), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func_5_:[0-9]+]] @func_5_(%[[VALUE_a_6:[0-9]+]] a: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_6:[0-9]+]] b: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32>(%[[VALUE_a_6]]), const<i32>(5)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32>(%[[VALUE_a_6]]), read<i32>(%[[VALUE_b_6]])), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func_5_volatile:[0-9]+]] @func_5_volatile(%[[VALUE_a_7:[0-9]+]] a: volatile i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_7:[0-9]+]] b: volatile i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32, volatile>(%[[VALUE_a_7]]), const<i32>(5)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32, volatile>(%[[VALUE_a_7]]), read<i32, volatile>(%[[VALUE_b_7]])), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_a_8:[0-9]+]] a: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_a_8]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_8]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_a_8]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%[[VALUE_func_0_]], read<i32>(%[[VALUE_a_8]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%[[VALUE_func_0_volatile]], read<i32>(%[[VALUE_a_8]]))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%[[VALUE_func_1_]], read<i32>(%[[VALUE_a_8]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%[[VALUE_func_1_volatile]], read<i32>(%[[VALUE_a_8]]))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%[[VALUE_func_5_]], read<i32>(%[[VALUE_a_8]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%[[VALUE_func_5_volatile]], read<i32>(%[[VALUE_a_8]]))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
