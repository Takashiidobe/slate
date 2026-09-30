
#define func_name(outer, inner, cst)   outer##inner##_##cst
#define func_name_v(outer, inner, cst) outer##inner##_##cst##_v

#define func_decl(outer, inner, cst)                                           \
  int outer##inner##_##cst(int) __attribute__((noipa));                        \
  int outer##inner##_##cst(int a) { return (a op_##inner cst)op_##outer a; }   \
  int outer##inner##_##cst##_v(int) __attribute__((noipa));                    \
  int outer##inner##_##cst##_v(volatile int a) {                               \
    return (a op_##inner cst)op_##outer a;                                     \
  }

#define functions_n(outer, inner)                                              \
  func_decl(outer, inner, 0) func_decl(outer, inner, 1)                        \
      func_decl(outer, inner, 2)

#define functions()                                                            \
  functions_n(eq, eq) functions_n(eq, ne) functions_n(ne, eq)                  \
      functions_n(ne, ne)

#define op_ne !=
#define op_eq ==

#define test(inner, outer, cst, arg)                                           \
  func_name_v(inner, outer, cst)(arg) != func_name(inner, outer, cst)(arg)

functions()

#define tests_n(inner, outer, arg)                                             \
  if (test(inner, outer, 0, arg))                                              \
    __builtin_abort();                                                         \
  if (test(inner, outer, 1, arg))                                              \
    __builtin_abort();                                                         \
  if (test(inner, outer, 2, arg))                                              \
    __builtin_abort();

#define tests(arg)                                                             \
  tests_n(eq, eq, arg) tests_n(eq, ne, arg) tests_n(ne, eq, arg)               \
      tests_n(ne, ne, arg)

    int main() {
  for (int n = -1; n <= 2; n++) {
    tests(n)
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
// DEFAULT-NEXT:     fn %[[VALUE_eqeq_0:[0-9]+]] @eqeq_0(%[[VALUE_a:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))), read<i32>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqeq_0_v:[0-9]+]] @eqeq_0_v(%[[VALUE_a_2:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%[[VALUE_a_2]]), const<i32>(0))), read<i32, volatile>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqeq_1:[0-9]+]] @eqeq_1(%[[VALUE_a_3:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_a_3]]), const<i32>(1))), read<i32>(%[[VALUE_a_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqeq_1_v:[0-9]+]] @eqeq_1_v(%[[VALUE_a_4:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%[[VALUE_a_4]]), const<i32>(1))), read<i32, volatile>(%[[VALUE_a_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqeq_2:[0-9]+]] @eqeq_2(%[[VALUE_a_5:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_a_5]]), const<i32>(2))), read<i32>(%[[VALUE_a_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqeq_2_v:[0-9]+]] @eqeq_2_v(%[[VALUE_a_6:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%[[VALUE_a_6]]), const<i32>(2))), read<i32, volatile>(%[[VALUE_a_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqne_0:[0-9]+]] @eqne_0(%[[VALUE_a_7:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%[[VALUE_a_7]]), const<i32>(0))), read<i32>(%[[VALUE_a_7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqne_0_v:[0-9]+]] @eqne_0_v(%[[VALUE_a_8:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%[[VALUE_a_8]]), const<i32>(0))), read<i32, volatile>(%[[VALUE_a_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqne_1:[0-9]+]] @eqne_1(%[[VALUE_a_9:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%[[VALUE_a_9]]), const<i32>(1))), read<i32>(%[[VALUE_a_9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqne_1_v:[0-9]+]] @eqne_1_v(%[[VALUE_a_10:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%[[VALUE_a_10]]), const<i32>(1))), read<i32, volatile>(%[[VALUE_a_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqne_2:[0-9]+]] @eqne_2(%[[VALUE_a_11:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%[[VALUE_a_11]]), const<i32>(2))), read<i32>(%[[VALUE_a_11]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqne_2_v:[0-9]+]] @eqne_2_v(%[[VALUE_a_12:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%[[VALUE_a_12]]), const<i32>(2))), read<i32, volatile>(%[[VALUE_a_12]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_neeq_0:[0-9]+]] @neeq_0(%[[VALUE_a_13:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_a_13]]), const<i32>(0))), read<i32>(%[[VALUE_a_13]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_neeq_0_v:[0-9]+]] @neeq_0_v(%[[VALUE_a_14:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%[[VALUE_a_14]]), const<i32>(0))), read<i32, volatile>(%[[VALUE_a_14]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_neeq_1:[0-9]+]] @neeq_1(%[[VALUE_a_15:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_a_15]]), const<i32>(1))), read<i32>(%[[VALUE_a_15]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_neeq_1_v:[0-9]+]] @neeq_1_v(%[[VALUE_a_16:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%[[VALUE_a_16]]), const<i32>(1))), read<i32, volatile>(%[[VALUE_a_16]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_neeq_2:[0-9]+]] @neeq_2(%[[VALUE_a_17:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_a_17]]), const<i32>(2))), read<i32>(%[[VALUE_a_17]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_neeq_2_v:[0-9]+]] @neeq_2_v(%[[VALUE_a_18:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%[[VALUE_a_18]]), const<i32>(2))), read<i32, volatile>(%[[VALUE_a_18]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nene_0:[0-9]+]] @nene_0(%[[VALUE_a_19:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%[[VALUE_a_19]]), const<i32>(0))), read<i32>(%[[VALUE_a_19]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nene_0_v:[0-9]+]] @nene_0_v(%[[VALUE_a_20:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%[[VALUE_a_20]]), const<i32>(0))), read<i32, volatile>(%[[VALUE_a_20]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nene_1:[0-9]+]] @nene_1(%[[VALUE_a_21:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%[[VALUE_a_21]]), const<i32>(1))), read<i32>(%[[VALUE_a_21]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nene_1_v:[0-9]+]] @nene_1_v(%[[VALUE_a_22:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%[[VALUE_a_22]]), const<i32>(1))), read<i32, volatile>(%[[VALUE_a_22]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nene_2:[0-9]+]] @nene_2(%[[VALUE_a_23:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%[[VALUE_a_23]]), const<i32>(2))), read<i32>(%[[VALUE_a_23]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nene_2_v:[0-9]+]] @nene_2_v(%[[VALUE_a_24:[0-9]+]] a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%[[VALUE_a_24]]), const<i32>(2))), read<i32, volatile>(%[[VALUE_a_24]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_n]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqeq_0_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqeq_0]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqeq_1_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqeq_1]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqeq_2_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqeq_2]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqne_0_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqne_0]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqne_1_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqne_1]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqne_2_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_eqne_2]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_neeq_0_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_neeq_0]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_neeq_1_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_neeq_1]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_neeq_2_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_neeq_2]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_nene_0_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_nene_0]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_nene_1_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_nene_1]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_nene_2_v]], read<i32>(%[[VALUE_n]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_nene_2]], read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
