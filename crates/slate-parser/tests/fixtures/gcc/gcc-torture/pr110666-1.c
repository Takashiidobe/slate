
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
// DEFAULT-NEXT:     fn %0 @eqeq_0(%1 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%1), const<i32>(0))), read<i32>(%1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @eqeq_0_v(%3 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%3), const<i32>(0))), read<i32, volatile>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @eqeq_1(%5 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%5), const<i32>(1))), read<i32>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @eqeq_1_v(%7 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%7), const<i32>(1))), read<i32, volatile>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @eqeq_2(%9 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%9), const<i32>(2))), read<i32>(%9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @eqeq_2_v(%11 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%11), const<i32>(2))), read<i32, volatile>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @eqne_0(%13 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%13), const<i32>(0))), read<i32>(%13)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @eqne_0_v(%15 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%15), const<i32>(0))), read<i32, volatile>(%15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @eqne_1(%17 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%17), const<i32>(1))), read<i32>(%17)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @eqne_1_v(%19 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%19), const<i32>(1))), read<i32, volatile>(%19)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @eqne_2(%21 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%21), const<i32>(2))), read<i32>(%21)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @eqne_2_v(%23 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%23), const<i32>(2))), read<i32, volatile>(%23)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @neeq_0(%25 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%25), const<i32>(0))), read<i32>(%25)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @neeq_0_v(%27 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%27), const<i32>(0))), read<i32, volatile>(%27)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @neeq_1(%29 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%29), const<i32>(1))), read<i32>(%29)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @neeq_1_v(%31 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%31), const<i32>(1))), read<i32, volatile>(%31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @neeq_2(%33 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%33), const<i32>(2))), read<i32>(%33)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @neeq_2_v(%35 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%35), const<i32>(2))), read<i32, volatile>(%35)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @nene_0(%37 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%37), const<i32>(0))), read<i32>(%37)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @nene_0_v(%39 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%39), const<i32>(0))), read<i32, volatile>(%39)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @nene_1(%41 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%41), const<i32>(1))), read<i32>(%41)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @nene_1_v(%43 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%43), const<i32>(1))), read<i32, volatile>(%43)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @nene_2(%45 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%45), const<i32>(2))), read<i32>(%45)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @nene_2_v(%47 a: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32, volatile>(%47), const<i32>(2))), read<i32, volatile>(%47)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %74
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %49 n: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%49), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %75: i32 [synthetic] = read<i32>(%49);
// DEFAULT-NEXT:                 let %76: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%75), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%49, read<i32>(%76));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%6, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%4, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%10, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%8, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%14, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%12, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%18, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%16, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%22, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%20, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%26, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%24, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%30, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%28, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%34, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%32, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%38, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%36, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%42, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%40, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%46, read<i32>(%49)), call<i32, signature=fn(i32) -> i32>(%44, read<i32>(%49)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
