#define func(vol, op1, op2, op3)                                               \
  _Bool op1##_##op2##_##op3##_##vol(int a, int b) {                            \
    vol _Bool x = op_##op1(a, b);                                              \
    vol _Bool y = op_##op2(a, b);                                              \
    return op_##op3(x, y);                                                     \
  }

#define op_lt(a, b)  ((a) < (b))
#define op_le(a, b)  ((a) <= (b))
#define op_eq(a, b)  ((a) == (b))
#define op_ne(a, b)  ((a) != (b))
#define op_gt(a, b)  ((a) > (b))
#define op_ge(a, b)  ((a) >= (b))
#define op_xor(a, b) ((a) ^ (b))

#define funcs(a)                                                               \
  a(lt, lt, ne) a(lt, lt, eq) a(lt, lt, xor) a(lt, le, ne) a(lt, le, eq) a(    \
      lt, le, xor) a(lt, gt, ne) a(lt, gt, eq) a(lt, gt, xor) a(lt, ge, ne)    \
      a(lt, ge, eq) a(lt, ge, xor) a(lt, eq, ne) a(lt, eq, eq) a(lt, eq, xor)  \
          a(lt, ne, ne) a(lt, ne, eq) a(lt, ne, xor)                           \
                                                                               \
              a(le, lt, ne) a(le, lt, eq) a(le, lt, xor) a(le, le, ne)         \
                  a(le, le, eq) a(le, le, xor) a(le, gt, ne) a(le, gt, eq) a(  \
                      le, gt, xor) a(le, ge, ne) a(le, ge, eq) a(le, ge, xor)  \
                      a(le, eq, ne) a(le, eq, eq) a(le, eq, xor) a(le, ne, ne) \
                          a(le, ne, eq) a(le, ne, xor)                         \
                                                                               \
                              a(gt, lt, ne) a(gt, lt, eq) a(gt, lt, xor)       \
                                  a(gt, le, ne) a(gt, le, eq) a(               \
                                      gt, le, xor) a(gt, gt, ne) a(gt, gt, eq) \
                                      a(gt, gt, xor) a(gt, ge, ne) a(          \
                                          gt, ge, eq) a(gt, ge, xor)           \
                                          a(gt, eq, ne) a(gt, eq, eq) a(       \
                                              gt, eq,                          \
                                              xor) a(gt, ne,                   \
                                                     ne) a(gt, ne,             \
                                                           eq) a(gt, ne, xor)  \
                                                                               \
                                              a(ge, lt, ne) a(ge, lt, eq) a(   \
                                                  ge, lt,                      \
                                                  xor) a(ge, le,               \
                                                         ne) a(ge, le, eq)     \
                                                  a(ge, le, xor) a(ge, gt, ne) \
                                                      a(ge, gt, eq) a(         \
                                                          ge, gt,              \
                                                          xor) a(ge, ge, ne)   \
                                                          a(ge, ge, eq) a(     \
                                                              ge, ge,          \
                                                              xor) a(ge, eq,   \
                                                                     ne) a(ge, \
                                                                           eq, \
                                                                           eq) \
                                                              a(ge, eq,        \
                                                                xor) a(ge, ne, \
                                                                       ne)     \
                                                                  a(ge, ne,    \
                                                                    eq) a(ge,  \
                                                                          ne,  \
                                                                          xor)

#define funcs1(a, b, c) func(, a, b, c) func(volatile, a, b, c)

funcs(funcs1)

#define test(op1, op2, op3)                                                    \
  do {                                                                         \
    if (op1##_##op2##_##op3##_(x, y) != op1##_##op2##_##op3##_volatile(x, y))  \
      __builtin_abort();                                                       \
  } while (0);

    int main() {
  for (int x = -10; x < 10; x++)
    for (int y = -10; y < 10; y++) {
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
// DEFAULT-NEXT:     fn %0 @lt_lt_ne_(%1 a: i32, %2 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 x: bool [storage=automatic] = lt<i32>(read<i32>(%1), read<i32>(%2));
// DEFAULT-NEXT:         let %4 y: bool [storage=automatic] = lt<i32>(read<i32>(%1), read<i32>(%2));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%3)), from_bool<i32, reason=promotion>(read<bool>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @lt_lt_ne_volatile(%6 a: i32, %7 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%6), read<i32>(%7));
// DEFAULT-NEXT:         let %9 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%6), read<i32>(%7));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%8)), from_bool<i32, reason=promotion>(read<bool, volatile>(%9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @lt_lt_eq_(%11 a: i32, %12 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 x: bool [storage=automatic] = lt<i32>(read<i32>(%11), read<i32>(%12));
// DEFAULT-NEXT:         let %14 y: bool [storage=automatic] = lt<i32>(read<i32>(%11), read<i32>(%12));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%13)), from_bool<i32, reason=promotion>(read<bool>(%14)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @lt_lt_eq_volatile(%16 a: i32, %17 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%16), read<i32>(%17));
// DEFAULT-NEXT:         let %19 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%16), read<i32>(%17));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%18)), from_bool<i32, reason=promotion>(read<bool, volatile>(%19)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @lt_lt_xor_(%21 a: i32, %22 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 x: bool [storage=automatic] = lt<i32>(read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:         let %24 y: bool [storage=automatic] = lt<i32>(read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%23)), from_bool<i32, reason=promotion>(read<bool>(%24))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @lt_lt_xor_volatile(%26 a: i32, %27 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %28 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%26), read<i32>(%27));
// DEFAULT-NEXT:         let %29 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%26), read<i32>(%27));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%28)), from_bool<i32, reason=promotion>(read<bool, volatile>(%29))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @lt_le_ne_(%31 a: i32, %32 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33 x: bool [storage=automatic] = lt<i32>(read<i32>(%31), read<i32>(%32));
// DEFAULT-NEXT:         let %34 y: bool [storage=automatic] = le<i32>(read<i32>(%31), read<i32>(%32));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%33)), from_bool<i32, reason=promotion>(read<bool>(%34)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @lt_le_ne_volatile(%36 a: i32, %37 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %38 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%36), read<i32>(%37));
// DEFAULT-NEXT:         let %39 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%36), read<i32>(%37));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%38)), from_bool<i32, reason=promotion>(read<bool, volatile>(%39)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @lt_le_eq_(%41 a: i32, %42 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %43 x: bool [storage=automatic] = lt<i32>(read<i32>(%41), read<i32>(%42));
// DEFAULT-NEXT:         let %44 y: bool [storage=automatic] = le<i32>(read<i32>(%41), read<i32>(%42));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%43)), from_bool<i32, reason=promotion>(read<bool>(%44)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @lt_le_eq_volatile(%46 a: i32, %47 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %48 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%46), read<i32>(%47));
// DEFAULT-NEXT:         let %49 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%46), read<i32>(%47));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%48)), from_bool<i32, reason=promotion>(read<bool, volatile>(%49)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @lt_le_xor_(%51 a: i32, %52 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %53 x: bool [storage=automatic] = lt<i32>(read<i32>(%51), read<i32>(%52));
// DEFAULT-NEXT:         let %54 y: bool [storage=automatic] = le<i32>(read<i32>(%51), read<i32>(%52));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%53)), from_bool<i32, reason=promotion>(read<bool>(%54))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @lt_le_xor_volatile(%56 a: i32, %57 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %58 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%56), read<i32>(%57));
// DEFAULT-NEXT:         let %59 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%56), read<i32>(%57));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%58)), from_bool<i32, reason=promotion>(read<bool, volatile>(%59))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @lt_gt_ne_(%61 a: i32, %62 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %63 x: bool [storage=automatic] = lt<i32>(read<i32>(%61), read<i32>(%62));
// DEFAULT-NEXT:         let %64 y: bool [storage=automatic] = gt<i32>(read<i32>(%61), read<i32>(%62));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%63)), from_bool<i32, reason=promotion>(read<bool>(%64)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %65 @lt_gt_ne_volatile(%66 a: i32, %67 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %68 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%66), read<i32>(%67));
// DEFAULT-NEXT:         let %69 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%66), read<i32>(%67));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%68)), from_bool<i32, reason=promotion>(read<bool, volatile>(%69)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @lt_gt_eq_(%71 a: i32, %72 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %73 x: bool [storage=automatic] = lt<i32>(read<i32>(%71), read<i32>(%72));
// DEFAULT-NEXT:         let %74 y: bool [storage=automatic] = gt<i32>(read<i32>(%71), read<i32>(%72));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%73)), from_bool<i32, reason=promotion>(read<bool>(%74)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @lt_gt_eq_volatile(%76 a: i32, %77 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %78 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%76), read<i32>(%77));
// DEFAULT-NEXT:         let %79 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%76), read<i32>(%77));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%78)), from_bool<i32, reason=promotion>(read<bool, volatile>(%79)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @lt_gt_xor_(%81 a: i32, %82 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %83 x: bool [storage=automatic] = lt<i32>(read<i32>(%81), read<i32>(%82));
// DEFAULT-NEXT:         let %84 y: bool [storage=automatic] = gt<i32>(read<i32>(%81), read<i32>(%82));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%83)), from_bool<i32, reason=promotion>(read<bool>(%84))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %85 @lt_gt_xor_volatile(%86 a: i32, %87 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %88 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%86), read<i32>(%87));
// DEFAULT-NEXT:         let %89 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%86), read<i32>(%87));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%88)), from_bool<i32, reason=promotion>(read<bool, volatile>(%89))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @lt_ge_ne_(%91 a: i32, %92 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %93 x: bool [storage=automatic] = lt<i32>(read<i32>(%91), read<i32>(%92));
// DEFAULT-NEXT:         let %94 y: bool [storage=automatic] = ge<i32>(read<i32>(%91), read<i32>(%92));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%93)), from_bool<i32, reason=promotion>(read<bool>(%94)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %95 @lt_ge_ne_volatile(%96 a: i32, %97 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %98 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%96), read<i32>(%97));
// DEFAULT-NEXT:         let %99 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%96), read<i32>(%97));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%98)), from_bool<i32, reason=promotion>(read<bool, volatile>(%99)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @lt_ge_eq_(%101 a: i32, %102 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %103 x: bool [storage=automatic] = lt<i32>(read<i32>(%101), read<i32>(%102));
// DEFAULT-NEXT:         let %104 y: bool [storage=automatic] = ge<i32>(read<i32>(%101), read<i32>(%102));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%103)), from_bool<i32, reason=promotion>(read<bool>(%104)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %105 @lt_ge_eq_volatile(%106 a: i32, %107 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %108 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%106), read<i32>(%107));
// DEFAULT-NEXT:         let %109 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%106), read<i32>(%107));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%108)), from_bool<i32, reason=promotion>(read<bool, volatile>(%109)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %110 @lt_ge_xor_(%111 a: i32, %112 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %113 x: bool [storage=automatic] = lt<i32>(read<i32>(%111), read<i32>(%112));
// DEFAULT-NEXT:         let %114 y: bool [storage=automatic] = ge<i32>(read<i32>(%111), read<i32>(%112));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%113)), from_bool<i32, reason=promotion>(read<bool>(%114))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %115 @lt_ge_xor_volatile(%116 a: i32, %117 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %118 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%116), read<i32>(%117));
// DEFAULT-NEXT:         let %119 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%116), read<i32>(%117));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%118)), from_bool<i32, reason=promotion>(read<bool, volatile>(%119))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %120 @lt_eq_ne_(%121 a: i32, %122 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %123 x: bool [storage=automatic] = lt<i32>(read<i32>(%121), read<i32>(%122));
// DEFAULT-NEXT:         let %124 y: bool [storage=automatic] = eq<i32>(read<i32>(%121), read<i32>(%122));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%123)), from_bool<i32, reason=promotion>(read<bool>(%124)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %125 @lt_eq_ne_volatile(%126 a: i32, %127 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %128 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%126), read<i32>(%127));
// DEFAULT-NEXT:         let %129 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%126), read<i32>(%127));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%128)), from_bool<i32, reason=promotion>(read<bool, volatile>(%129)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %130 @lt_eq_eq_(%131 a: i32, %132 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %133 x: bool [storage=automatic] = lt<i32>(read<i32>(%131), read<i32>(%132));
// DEFAULT-NEXT:         let %134 y: bool [storage=automatic] = eq<i32>(read<i32>(%131), read<i32>(%132));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%133)), from_bool<i32, reason=promotion>(read<bool>(%134)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %135 @lt_eq_eq_volatile(%136 a: i32, %137 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %138 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%136), read<i32>(%137));
// DEFAULT-NEXT:         let %139 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%136), read<i32>(%137));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%138)), from_bool<i32, reason=promotion>(read<bool, volatile>(%139)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %140 @lt_eq_xor_(%141 a: i32, %142 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %143 x: bool [storage=automatic] = lt<i32>(read<i32>(%141), read<i32>(%142));
// DEFAULT-NEXT:         let %144 y: bool [storage=automatic] = eq<i32>(read<i32>(%141), read<i32>(%142));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%143)), from_bool<i32, reason=promotion>(read<bool>(%144))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %145 @lt_eq_xor_volatile(%146 a: i32, %147 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %148 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%146), read<i32>(%147));
// DEFAULT-NEXT:         let %149 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%146), read<i32>(%147));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%148)), from_bool<i32, reason=promotion>(read<bool, volatile>(%149))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @lt_ne_ne_(%151 a: i32, %152 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %153 x: bool [storage=automatic] = lt<i32>(read<i32>(%151), read<i32>(%152));
// DEFAULT-NEXT:         let %154 y: bool [storage=automatic] = ne<i32>(read<i32>(%151), read<i32>(%152));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%153)), from_bool<i32, reason=promotion>(read<bool>(%154)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %155 @lt_ne_ne_volatile(%156 a: i32, %157 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %158 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%156), read<i32>(%157));
// DEFAULT-NEXT:         let %159 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%156), read<i32>(%157));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%158)), from_bool<i32, reason=promotion>(read<bool, volatile>(%159)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %160 @lt_ne_eq_(%161 a: i32, %162 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %163 x: bool [storage=automatic] = lt<i32>(read<i32>(%161), read<i32>(%162));
// DEFAULT-NEXT:         let %164 y: bool [storage=automatic] = ne<i32>(read<i32>(%161), read<i32>(%162));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%163)), from_bool<i32, reason=promotion>(read<bool>(%164)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %165 @lt_ne_eq_volatile(%166 a: i32, %167 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %168 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%166), read<i32>(%167));
// DEFAULT-NEXT:         let %169 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%166), read<i32>(%167));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%168)), from_bool<i32, reason=promotion>(read<bool, volatile>(%169)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %170 @lt_ne_xor_(%171 a: i32, %172 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %173 x: bool [storage=automatic] = lt<i32>(read<i32>(%171), read<i32>(%172));
// DEFAULT-NEXT:         let %174 y: bool [storage=automatic] = ne<i32>(read<i32>(%171), read<i32>(%172));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%173)), from_bool<i32, reason=promotion>(read<bool>(%174))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %175 @lt_ne_xor_volatile(%176 a: i32, %177 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %178 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%176), read<i32>(%177));
// DEFAULT-NEXT:         let %179 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%176), read<i32>(%177));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%178)), from_bool<i32, reason=promotion>(read<bool, volatile>(%179))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %180 @le_lt_ne_(%181 a: i32, %182 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %183 x: bool [storage=automatic] = le<i32>(read<i32>(%181), read<i32>(%182));
// DEFAULT-NEXT:         let %184 y: bool [storage=automatic] = lt<i32>(read<i32>(%181), read<i32>(%182));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%183)), from_bool<i32, reason=promotion>(read<bool>(%184)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %185 @le_lt_ne_volatile(%186 a: i32, %187 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %188 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%186), read<i32>(%187));
// DEFAULT-NEXT:         let %189 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%186), read<i32>(%187));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%188)), from_bool<i32, reason=promotion>(read<bool, volatile>(%189)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %190 @le_lt_eq_(%191 a: i32, %192 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %193 x: bool [storage=automatic] = le<i32>(read<i32>(%191), read<i32>(%192));
// DEFAULT-NEXT:         let %194 y: bool [storage=automatic] = lt<i32>(read<i32>(%191), read<i32>(%192));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%193)), from_bool<i32, reason=promotion>(read<bool>(%194)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %195 @le_lt_eq_volatile(%196 a: i32, %197 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %198 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%196), read<i32>(%197));
// DEFAULT-NEXT:         let %199 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%196), read<i32>(%197));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%198)), from_bool<i32, reason=promotion>(read<bool, volatile>(%199)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %200 @le_lt_xor_(%201 a: i32, %202 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %203 x: bool [storage=automatic] = le<i32>(read<i32>(%201), read<i32>(%202));
// DEFAULT-NEXT:         let %204 y: bool [storage=automatic] = lt<i32>(read<i32>(%201), read<i32>(%202));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%203)), from_bool<i32, reason=promotion>(read<bool>(%204))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %205 @le_lt_xor_volatile(%206 a: i32, %207 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %208 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%206), read<i32>(%207));
// DEFAULT-NEXT:         let %209 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%206), read<i32>(%207));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%208)), from_bool<i32, reason=promotion>(read<bool, volatile>(%209))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %210 @le_le_ne_(%211 a: i32, %212 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %213 x: bool [storage=automatic] = le<i32>(read<i32>(%211), read<i32>(%212));
// DEFAULT-NEXT:         let %214 y: bool [storage=automatic] = le<i32>(read<i32>(%211), read<i32>(%212));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%213)), from_bool<i32, reason=promotion>(read<bool>(%214)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %215 @le_le_ne_volatile(%216 a: i32, %217 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %218 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%216), read<i32>(%217));
// DEFAULT-NEXT:         let %219 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%216), read<i32>(%217));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%218)), from_bool<i32, reason=promotion>(read<bool, volatile>(%219)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %220 @le_le_eq_(%221 a: i32, %222 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %223 x: bool [storage=automatic] = le<i32>(read<i32>(%221), read<i32>(%222));
// DEFAULT-NEXT:         let %224 y: bool [storage=automatic] = le<i32>(read<i32>(%221), read<i32>(%222));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%223)), from_bool<i32, reason=promotion>(read<bool>(%224)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %225 @le_le_eq_volatile(%226 a: i32, %227 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %228 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%226), read<i32>(%227));
// DEFAULT-NEXT:         let %229 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%226), read<i32>(%227));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%228)), from_bool<i32, reason=promotion>(read<bool, volatile>(%229)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %230 @le_le_xor_(%231 a: i32, %232 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %233 x: bool [storage=automatic] = le<i32>(read<i32>(%231), read<i32>(%232));
// DEFAULT-NEXT:         let %234 y: bool [storage=automatic] = le<i32>(read<i32>(%231), read<i32>(%232));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%233)), from_bool<i32, reason=promotion>(read<bool>(%234))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %235 @le_le_xor_volatile(%236 a: i32, %237 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %238 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%236), read<i32>(%237));
// DEFAULT-NEXT:         let %239 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%236), read<i32>(%237));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%238)), from_bool<i32, reason=promotion>(read<bool, volatile>(%239))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %240 @le_gt_ne_(%241 a: i32, %242 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %243 x: bool [storage=automatic] = le<i32>(read<i32>(%241), read<i32>(%242));
// DEFAULT-NEXT:         let %244 y: bool [storage=automatic] = gt<i32>(read<i32>(%241), read<i32>(%242));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%243)), from_bool<i32, reason=promotion>(read<bool>(%244)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %245 @le_gt_ne_volatile(%246 a: i32, %247 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %248 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%246), read<i32>(%247));
// DEFAULT-NEXT:         let %249 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%246), read<i32>(%247));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%248)), from_bool<i32, reason=promotion>(read<bool, volatile>(%249)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %250 @le_gt_eq_(%251 a: i32, %252 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %253 x: bool [storage=automatic] = le<i32>(read<i32>(%251), read<i32>(%252));
// DEFAULT-NEXT:         let %254 y: bool [storage=automatic] = gt<i32>(read<i32>(%251), read<i32>(%252));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%253)), from_bool<i32, reason=promotion>(read<bool>(%254)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %255 @le_gt_eq_volatile(%256 a: i32, %257 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %258 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%256), read<i32>(%257));
// DEFAULT-NEXT:         let %259 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%256), read<i32>(%257));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%258)), from_bool<i32, reason=promotion>(read<bool, volatile>(%259)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %260 @le_gt_xor_(%261 a: i32, %262 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %263 x: bool [storage=automatic] = le<i32>(read<i32>(%261), read<i32>(%262));
// DEFAULT-NEXT:         let %264 y: bool [storage=automatic] = gt<i32>(read<i32>(%261), read<i32>(%262));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%263)), from_bool<i32, reason=promotion>(read<bool>(%264))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %265 @le_gt_xor_volatile(%266 a: i32, %267 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %268 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%266), read<i32>(%267));
// DEFAULT-NEXT:         let %269 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%266), read<i32>(%267));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%268)), from_bool<i32, reason=promotion>(read<bool, volatile>(%269))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %270 @le_ge_ne_(%271 a: i32, %272 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %273 x: bool [storage=automatic] = le<i32>(read<i32>(%271), read<i32>(%272));
// DEFAULT-NEXT:         let %274 y: bool [storage=automatic] = ge<i32>(read<i32>(%271), read<i32>(%272));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%273)), from_bool<i32, reason=promotion>(read<bool>(%274)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %275 @le_ge_ne_volatile(%276 a: i32, %277 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %278 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%276), read<i32>(%277));
// DEFAULT-NEXT:         let %279 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%276), read<i32>(%277));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%278)), from_bool<i32, reason=promotion>(read<bool, volatile>(%279)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %280 @le_ge_eq_(%281 a: i32, %282 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %283 x: bool [storage=automatic] = le<i32>(read<i32>(%281), read<i32>(%282));
// DEFAULT-NEXT:         let %284 y: bool [storage=automatic] = ge<i32>(read<i32>(%281), read<i32>(%282));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%283)), from_bool<i32, reason=promotion>(read<bool>(%284)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %285 @le_ge_eq_volatile(%286 a: i32, %287 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %288 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%286), read<i32>(%287));
// DEFAULT-NEXT:         let %289 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%286), read<i32>(%287));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%288)), from_bool<i32, reason=promotion>(read<bool, volatile>(%289)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %290 @le_ge_xor_(%291 a: i32, %292 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %293 x: bool [storage=automatic] = le<i32>(read<i32>(%291), read<i32>(%292));
// DEFAULT-NEXT:         let %294 y: bool [storage=automatic] = ge<i32>(read<i32>(%291), read<i32>(%292));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%293)), from_bool<i32, reason=promotion>(read<bool>(%294))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %295 @le_ge_xor_volatile(%296 a: i32, %297 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %298 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%296), read<i32>(%297));
// DEFAULT-NEXT:         let %299 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%296), read<i32>(%297));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%298)), from_bool<i32, reason=promotion>(read<bool, volatile>(%299))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %300 @le_eq_ne_(%301 a: i32, %302 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %303 x: bool [storage=automatic] = le<i32>(read<i32>(%301), read<i32>(%302));
// DEFAULT-NEXT:         let %304 y: bool [storage=automatic] = eq<i32>(read<i32>(%301), read<i32>(%302));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%303)), from_bool<i32, reason=promotion>(read<bool>(%304)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %305 @le_eq_ne_volatile(%306 a: i32, %307 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %308 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%306), read<i32>(%307));
// DEFAULT-NEXT:         let %309 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%306), read<i32>(%307));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%308)), from_bool<i32, reason=promotion>(read<bool, volatile>(%309)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %310 @le_eq_eq_(%311 a: i32, %312 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %313 x: bool [storage=automatic] = le<i32>(read<i32>(%311), read<i32>(%312));
// DEFAULT-NEXT:         let %314 y: bool [storage=automatic] = eq<i32>(read<i32>(%311), read<i32>(%312));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%313)), from_bool<i32, reason=promotion>(read<bool>(%314)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %315 @le_eq_eq_volatile(%316 a: i32, %317 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %318 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%316), read<i32>(%317));
// DEFAULT-NEXT:         let %319 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%316), read<i32>(%317));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%318)), from_bool<i32, reason=promotion>(read<bool, volatile>(%319)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %320 @le_eq_xor_(%321 a: i32, %322 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %323 x: bool [storage=automatic] = le<i32>(read<i32>(%321), read<i32>(%322));
// DEFAULT-NEXT:         let %324 y: bool [storage=automatic] = eq<i32>(read<i32>(%321), read<i32>(%322));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%323)), from_bool<i32, reason=promotion>(read<bool>(%324))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %325 @le_eq_xor_volatile(%326 a: i32, %327 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %328 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%326), read<i32>(%327));
// DEFAULT-NEXT:         let %329 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%326), read<i32>(%327));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%328)), from_bool<i32, reason=promotion>(read<bool, volatile>(%329))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %330 @le_ne_ne_(%331 a: i32, %332 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %333 x: bool [storage=automatic] = le<i32>(read<i32>(%331), read<i32>(%332));
// DEFAULT-NEXT:         let %334 y: bool [storage=automatic] = ne<i32>(read<i32>(%331), read<i32>(%332));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%333)), from_bool<i32, reason=promotion>(read<bool>(%334)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %335 @le_ne_ne_volatile(%336 a: i32, %337 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %338 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%336), read<i32>(%337));
// DEFAULT-NEXT:         let %339 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%336), read<i32>(%337));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%338)), from_bool<i32, reason=promotion>(read<bool, volatile>(%339)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %340 @le_ne_eq_(%341 a: i32, %342 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %343 x: bool [storage=automatic] = le<i32>(read<i32>(%341), read<i32>(%342));
// DEFAULT-NEXT:         let %344 y: bool [storage=automatic] = ne<i32>(read<i32>(%341), read<i32>(%342));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%343)), from_bool<i32, reason=promotion>(read<bool>(%344)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %345 @le_ne_eq_volatile(%346 a: i32, %347 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %348 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%346), read<i32>(%347));
// DEFAULT-NEXT:         let %349 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%346), read<i32>(%347));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%348)), from_bool<i32, reason=promotion>(read<bool, volatile>(%349)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %350 @le_ne_xor_(%351 a: i32, %352 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %353 x: bool [storage=automatic] = le<i32>(read<i32>(%351), read<i32>(%352));
// DEFAULT-NEXT:         let %354 y: bool [storage=automatic] = ne<i32>(read<i32>(%351), read<i32>(%352));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%353)), from_bool<i32, reason=promotion>(read<bool>(%354))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %355 @le_ne_xor_volatile(%356 a: i32, %357 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %358 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%356), read<i32>(%357));
// DEFAULT-NEXT:         let %359 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%356), read<i32>(%357));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%358)), from_bool<i32, reason=promotion>(read<bool, volatile>(%359))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %360 @gt_lt_ne_(%361 a: i32, %362 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %363 x: bool [storage=automatic] = gt<i32>(read<i32>(%361), read<i32>(%362));
// DEFAULT-NEXT:         let %364 y: bool [storage=automatic] = lt<i32>(read<i32>(%361), read<i32>(%362));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%363)), from_bool<i32, reason=promotion>(read<bool>(%364)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %365 @gt_lt_ne_volatile(%366 a: i32, %367 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %368 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%366), read<i32>(%367));
// DEFAULT-NEXT:         let %369 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%366), read<i32>(%367));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%368)), from_bool<i32, reason=promotion>(read<bool, volatile>(%369)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %370 @gt_lt_eq_(%371 a: i32, %372 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %373 x: bool [storage=automatic] = gt<i32>(read<i32>(%371), read<i32>(%372));
// DEFAULT-NEXT:         let %374 y: bool [storage=automatic] = lt<i32>(read<i32>(%371), read<i32>(%372));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%373)), from_bool<i32, reason=promotion>(read<bool>(%374)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %375 @gt_lt_eq_volatile(%376 a: i32, %377 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %378 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%376), read<i32>(%377));
// DEFAULT-NEXT:         let %379 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%376), read<i32>(%377));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%378)), from_bool<i32, reason=promotion>(read<bool, volatile>(%379)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %380 @gt_lt_xor_(%381 a: i32, %382 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %383 x: bool [storage=automatic] = gt<i32>(read<i32>(%381), read<i32>(%382));
// DEFAULT-NEXT:         let %384 y: bool [storage=automatic] = lt<i32>(read<i32>(%381), read<i32>(%382));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%383)), from_bool<i32, reason=promotion>(read<bool>(%384))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %385 @gt_lt_xor_volatile(%386 a: i32, %387 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %388 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%386), read<i32>(%387));
// DEFAULT-NEXT:         let %389 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%386), read<i32>(%387));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%388)), from_bool<i32, reason=promotion>(read<bool, volatile>(%389))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %390 @gt_le_ne_(%391 a: i32, %392 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %393 x: bool [storage=automatic] = gt<i32>(read<i32>(%391), read<i32>(%392));
// DEFAULT-NEXT:         let %394 y: bool [storage=automatic] = le<i32>(read<i32>(%391), read<i32>(%392));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%393)), from_bool<i32, reason=promotion>(read<bool>(%394)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %395 @gt_le_ne_volatile(%396 a: i32, %397 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %398 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%396), read<i32>(%397));
// DEFAULT-NEXT:         let %399 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%396), read<i32>(%397));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%398)), from_bool<i32, reason=promotion>(read<bool, volatile>(%399)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %400 @gt_le_eq_(%401 a: i32, %402 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %403 x: bool [storage=automatic] = gt<i32>(read<i32>(%401), read<i32>(%402));
// DEFAULT-NEXT:         let %404 y: bool [storage=automatic] = le<i32>(read<i32>(%401), read<i32>(%402));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%403)), from_bool<i32, reason=promotion>(read<bool>(%404)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %405 @gt_le_eq_volatile(%406 a: i32, %407 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %408 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%406), read<i32>(%407));
// DEFAULT-NEXT:         let %409 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%406), read<i32>(%407));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%408)), from_bool<i32, reason=promotion>(read<bool, volatile>(%409)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %410 @gt_le_xor_(%411 a: i32, %412 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %413 x: bool [storage=automatic] = gt<i32>(read<i32>(%411), read<i32>(%412));
// DEFAULT-NEXT:         let %414 y: bool [storage=automatic] = le<i32>(read<i32>(%411), read<i32>(%412));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%413)), from_bool<i32, reason=promotion>(read<bool>(%414))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %415 @gt_le_xor_volatile(%416 a: i32, %417 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %418 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%416), read<i32>(%417));
// DEFAULT-NEXT:         let %419 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%416), read<i32>(%417));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%418)), from_bool<i32, reason=promotion>(read<bool, volatile>(%419))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %420 @gt_gt_ne_(%421 a: i32, %422 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %423 x: bool [storage=automatic] = gt<i32>(read<i32>(%421), read<i32>(%422));
// DEFAULT-NEXT:         let %424 y: bool [storage=automatic] = gt<i32>(read<i32>(%421), read<i32>(%422));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%423)), from_bool<i32, reason=promotion>(read<bool>(%424)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %425 @gt_gt_ne_volatile(%426 a: i32, %427 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %428 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%426), read<i32>(%427));
// DEFAULT-NEXT:         let %429 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%426), read<i32>(%427));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%428)), from_bool<i32, reason=promotion>(read<bool, volatile>(%429)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %430 @gt_gt_eq_(%431 a: i32, %432 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %433 x: bool [storage=automatic] = gt<i32>(read<i32>(%431), read<i32>(%432));
// DEFAULT-NEXT:         let %434 y: bool [storage=automatic] = gt<i32>(read<i32>(%431), read<i32>(%432));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%433)), from_bool<i32, reason=promotion>(read<bool>(%434)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %435 @gt_gt_eq_volatile(%436 a: i32, %437 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %438 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%436), read<i32>(%437));
// DEFAULT-NEXT:         let %439 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%436), read<i32>(%437));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%438)), from_bool<i32, reason=promotion>(read<bool, volatile>(%439)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %440 @gt_gt_xor_(%441 a: i32, %442 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %443 x: bool [storage=automatic] = gt<i32>(read<i32>(%441), read<i32>(%442));
// DEFAULT-NEXT:         let %444 y: bool [storage=automatic] = gt<i32>(read<i32>(%441), read<i32>(%442));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%443)), from_bool<i32, reason=promotion>(read<bool>(%444))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %445 @gt_gt_xor_volatile(%446 a: i32, %447 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %448 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%446), read<i32>(%447));
// DEFAULT-NEXT:         let %449 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%446), read<i32>(%447));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%448)), from_bool<i32, reason=promotion>(read<bool, volatile>(%449))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %450 @gt_ge_ne_(%451 a: i32, %452 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %453 x: bool [storage=automatic] = gt<i32>(read<i32>(%451), read<i32>(%452));
// DEFAULT-NEXT:         let %454 y: bool [storage=automatic] = ge<i32>(read<i32>(%451), read<i32>(%452));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%453)), from_bool<i32, reason=promotion>(read<bool>(%454)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %455 @gt_ge_ne_volatile(%456 a: i32, %457 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %458 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%456), read<i32>(%457));
// DEFAULT-NEXT:         let %459 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%456), read<i32>(%457));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%458)), from_bool<i32, reason=promotion>(read<bool, volatile>(%459)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %460 @gt_ge_eq_(%461 a: i32, %462 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %463 x: bool [storage=automatic] = gt<i32>(read<i32>(%461), read<i32>(%462));
// DEFAULT-NEXT:         let %464 y: bool [storage=automatic] = ge<i32>(read<i32>(%461), read<i32>(%462));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%463)), from_bool<i32, reason=promotion>(read<bool>(%464)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %465 @gt_ge_eq_volatile(%466 a: i32, %467 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %468 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%466), read<i32>(%467));
// DEFAULT-NEXT:         let %469 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%466), read<i32>(%467));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%468)), from_bool<i32, reason=promotion>(read<bool, volatile>(%469)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %470 @gt_ge_xor_(%471 a: i32, %472 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %473 x: bool [storage=automatic] = gt<i32>(read<i32>(%471), read<i32>(%472));
// DEFAULT-NEXT:         let %474 y: bool [storage=automatic] = ge<i32>(read<i32>(%471), read<i32>(%472));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%473)), from_bool<i32, reason=promotion>(read<bool>(%474))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %475 @gt_ge_xor_volatile(%476 a: i32, %477 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %478 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%476), read<i32>(%477));
// DEFAULT-NEXT:         let %479 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%476), read<i32>(%477));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%478)), from_bool<i32, reason=promotion>(read<bool, volatile>(%479))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %480 @gt_eq_ne_(%481 a: i32, %482 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %483 x: bool [storage=automatic] = gt<i32>(read<i32>(%481), read<i32>(%482));
// DEFAULT-NEXT:         let %484 y: bool [storage=automatic] = eq<i32>(read<i32>(%481), read<i32>(%482));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%483)), from_bool<i32, reason=promotion>(read<bool>(%484)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %485 @gt_eq_ne_volatile(%486 a: i32, %487 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %488 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%486), read<i32>(%487));
// DEFAULT-NEXT:         let %489 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%486), read<i32>(%487));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%488)), from_bool<i32, reason=promotion>(read<bool, volatile>(%489)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %490 @gt_eq_eq_(%491 a: i32, %492 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %493 x: bool [storage=automatic] = gt<i32>(read<i32>(%491), read<i32>(%492));
// DEFAULT-NEXT:         let %494 y: bool [storage=automatic] = eq<i32>(read<i32>(%491), read<i32>(%492));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%493)), from_bool<i32, reason=promotion>(read<bool>(%494)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %495 @gt_eq_eq_volatile(%496 a: i32, %497 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %498 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%496), read<i32>(%497));
// DEFAULT-NEXT:         let %499 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%496), read<i32>(%497));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%498)), from_bool<i32, reason=promotion>(read<bool, volatile>(%499)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %500 @gt_eq_xor_(%501 a: i32, %502 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %503 x: bool [storage=automatic] = gt<i32>(read<i32>(%501), read<i32>(%502));
// DEFAULT-NEXT:         let %504 y: bool [storage=automatic] = eq<i32>(read<i32>(%501), read<i32>(%502));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%503)), from_bool<i32, reason=promotion>(read<bool>(%504))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %505 @gt_eq_xor_volatile(%506 a: i32, %507 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %508 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%506), read<i32>(%507));
// DEFAULT-NEXT:         let %509 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%506), read<i32>(%507));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%508)), from_bool<i32, reason=promotion>(read<bool, volatile>(%509))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %510 @gt_ne_ne_(%511 a: i32, %512 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %513 x: bool [storage=automatic] = gt<i32>(read<i32>(%511), read<i32>(%512));
// DEFAULT-NEXT:         let %514 y: bool [storage=automatic] = ne<i32>(read<i32>(%511), read<i32>(%512));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%513)), from_bool<i32, reason=promotion>(read<bool>(%514)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %515 @gt_ne_ne_volatile(%516 a: i32, %517 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %518 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%516), read<i32>(%517));
// DEFAULT-NEXT:         let %519 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%516), read<i32>(%517));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%518)), from_bool<i32, reason=promotion>(read<bool, volatile>(%519)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %520 @gt_ne_eq_(%521 a: i32, %522 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %523 x: bool [storage=automatic] = gt<i32>(read<i32>(%521), read<i32>(%522));
// DEFAULT-NEXT:         let %524 y: bool [storage=automatic] = ne<i32>(read<i32>(%521), read<i32>(%522));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%523)), from_bool<i32, reason=promotion>(read<bool>(%524)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %525 @gt_ne_eq_volatile(%526 a: i32, %527 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %528 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%526), read<i32>(%527));
// DEFAULT-NEXT:         let %529 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%526), read<i32>(%527));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%528)), from_bool<i32, reason=promotion>(read<bool, volatile>(%529)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %530 @gt_ne_xor_(%531 a: i32, %532 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %533 x: bool [storage=automatic] = gt<i32>(read<i32>(%531), read<i32>(%532));
// DEFAULT-NEXT:         let %534 y: bool [storage=automatic] = ne<i32>(read<i32>(%531), read<i32>(%532));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%533)), from_bool<i32, reason=promotion>(read<bool>(%534))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %535 @gt_ne_xor_volatile(%536 a: i32, %537 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %538 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%536), read<i32>(%537));
// DEFAULT-NEXT:         let %539 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%536), read<i32>(%537));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%538)), from_bool<i32, reason=promotion>(read<bool, volatile>(%539))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %540 @ge_lt_ne_(%541 a: i32, %542 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %543 x: bool [storage=automatic] = ge<i32>(read<i32>(%541), read<i32>(%542));
// DEFAULT-NEXT:         let %544 y: bool [storage=automatic] = lt<i32>(read<i32>(%541), read<i32>(%542));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%543)), from_bool<i32, reason=promotion>(read<bool>(%544)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %545 @ge_lt_ne_volatile(%546 a: i32, %547 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %548 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%546), read<i32>(%547));
// DEFAULT-NEXT:         let %549 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%546), read<i32>(%547));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%548)), from_bool<i32, reason=promotion>(read<bool, volatile>(%549)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %550 @ge_lt_eq_(%551 a: i32, %552 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %553 x: bool [storage=automatic] = ge<i32>(read<i32>(%551), read<i32>(%552));
// DEFAULT-NEXT:         let %554 y: bool [storage=automatic] = lt<i32>(read<i32>(%551), read<i32>(%552));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%553)), from_bool<i32, reason=promotion>(read<bool>(%554)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %555 @ge_lt_eq_volatile(%556 a: i32, %557 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %558 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%556), read<i32>(%557));
// DEFAULT-NEXT:         let %559 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%556), read<i32>(%557));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%558)), from_bool<i32, reason=promotion>(read<bool, volatile>(%559)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %560 @ge_lt_xor_(%561 a: i32, %562 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %563 x: bool [storage=automatic] = ge<i32>(read<i32>(%561), read<i32>(%562));
// DEFAULT-NEXT:         let %564 y: bool [storage=automatic] = lt<i32>(read<i32>(%561), read<i32>(%562));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%563)), from_bool<i32, reason=promotion>(read<bool>(%564))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %565 @ge_lt_xor_volatile(%566 a: i32, %567 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %568 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%566), read<i32>(%567));
// DEFAULT-NEXT:         let %569 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%566), read<i32>(%567));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%568)), from_bool<i32, reason=promotion>(read<bool, volatile>(%569))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %570 @ge_le_ne_(%571 a: i32, %572 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %573 x: bool [storage=automatic] = ge<i32>(read<i32>(%571), read<i32>(%572));
// DEFAULT-NEXT:         let %574 y: bool [storage=automatic] = le<i32>(read<i32>(%571), read<i32>(%572));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%573)), from_bool<i32, reason=promotion>(read<bool>(%574)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %575 @ge_le_ne_volatile(%576 a: i32, %577 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %578 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%576), read<i32>(%577));
// DEFAULT-NEXT:         let %579 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%576), read<i32>(%577));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%578)), from_bool<i32, reason=promotion>(read<bool, volatile>(%579)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %580 @ge_le_eq_(%581 a: i32, %582 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %583 x: bool [storage=automatic] = ge<i32>(read<i32>(%581), read<i32>(%582));
// DEFAULT-NEXT:         let %584 y: bool [storage=automatic] = le<i32>(read<i32>(%581), read<i32>(%582));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%583)), from_bool<i32, reason=promotion>(read<bool>(%584)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %585 @ge_le_eq_volatile(%586 a: i32, %587 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %588 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%586), read<i32>(%587));
// DEFAULT-NEXT:         let %589 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%586), read<i32>(%587));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%588)), from_bool<i32, reason=promotion>(read<bool, volatile>(%589)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %590 @ge_le_xor_(%591 a: i32, %592 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %593 x: bool [storage=automatic] = ge<i32>(read<i32>(%591), read<i32>(%592));
// DEFAULT-NEXT:         let %594 y: bool [storage=automatic] = le<i32>(read<i32>(%591), read<i32>(%592));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%593)), from_bool<i32, reason=promotion>(read<bool>(%594))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %595 @ge_le_xor_volatile(%596 a: i32, %597 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %598 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%596), read<i32>(%597));
// DEFAULT-NEXT:         let %599 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%596), read<i32>(%597));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%598)), from_bool<i32, reason=promotion>(read<bool, volatile>(%599))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %600 @ge_gt_ne_(%601 a: i32, %602 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %603 x: bool [storage=automatic] = ge<i32>(read<i32>(%601), read<i32>(%602));
// DEFAULT-NEXT:         let %604 y: bool [storage=automatic] = gt<i32>(read<i32>(%601), read<i32>(%602));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%603)), from_bool<i32, reason=promotion>(read<bool>(%604)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %605 @ge_gt_ne_volatile(%606 a: i32, %607 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %608 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%606), read<i32>(%607));
// DEFAULT-NEXT:         let %609 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%606), read<i32>(%607));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%608)), from_bool<i32, reason=promotion>(read<bool, volatile>(%609)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %610 @ge_gt_eq_(%611 a: i32, %612 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %613 x: bool [storage=automatic] = ge<i32>(read<i32>(%611), read<i32>(%612));
// DEFAULT-NEXT:         let %614 y: bool [storage=automatic] = gt<i32>(read<i32>(%611), read<i32>(%612));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%613)), from_bool<i32, reason=promotion>(read<bool>(%614)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %615 @ge_gt_eq_volatile(%616 a: i32, %617 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %618 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%616), read<i32>(%617));
// DEFAULT-NEXT:         let %619 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%616), read<i32>(%617));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%618)), from_bool<i32, reason=promotion>(read<bool, volatile>(%619)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %620 @ge_gt_xor_(%621 a: i32, %622 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %623 x: bool [storage=automatic] = ge<i32>(read<i32>(%621), read<i32>(%622));
// DEFAULT-NEXT:         let %624 y: bool [storage=automatic] = gt<i32>(read<i32>(%621), read<i32>(%622));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%623)), from_bool<i32, reason=promotion>(read<bool>(%624))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %625 @ge_gt_xor_volatile(%626 a: i32, %627 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %628 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%626), read<i32>(%627));
// DEFAULT-NEXT:         let %629 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%626), read<i32>(%627));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%628)), from_bool<i32, reason=promotion>(read<bool, volatile>(%629))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %630 @ge_ge_ne_(%631 a: i32, %632 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %633 x: bool [storage=automatic] = ge<i32>(read<i32>(%631), read<i32>(%632));
// DEFAULT-NEXT:         let %634 y: bool [storage=automatic] = ge<i32>(read<i32>(%631), read<i32>(%632));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%633)), from_bool<i32, reason=promotion>(read<bool>(%634)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %635 @ge_ge_ne_volatile(%636 a: i32, %637 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %638 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%636), read<i32>(%637));
// DEFAULT-NEXT:         let %639 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%636), read<i32>(%637));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%638)), from_bool<i32, reason=promotion>(read<bool, volatile>(%639)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %640 @ge_ge_eq_(%641 a: i32, %642 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %643 x: bool [storage=automatic] = ge<i32>(read<i32>(%641), read<i32>(%642));
// DEFAULT-NEXT:         let %644 y: bool [storage=automatic] = ge<i32>(read<i32>(%641), read<i32>(%642));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%643)), from_bool<i32, reason=promotion>(read<bool>(%644)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %645 @ge_ge_eq_volatile(%646 a: i32, %647 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %648 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%646), read<i32>(%647));
// DEFAULT-NEXT:         let %649 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%646), read<i32>(%647));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%648)), from_bool<i32, reason=promotion>(read<bool, volatile>(%649)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %650 @ge_ge_xor_(%651 a: i32, %652 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %653 x: bool [storage=automatic] = ge<i32>(read<i32>(%651), read<i32>(%652));
// DEFAULT-NEXT:         let %654 y: bool [storage=automatic] = ge<i32>(read<i32>(%651), read<i32>(%652));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%653)), from_bool<i32, reason=promotion>(read<bool>(%654))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %655 @ge_ge_xor_volatile(%656 a: i32, %657 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %658 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%656), read<i32>(%657));
// DEFAULT-NEXT:         let %659 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%656), read<i32>(%657));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%658)), from_bool<i32, reason=promotion>(read<bool, volatile>(%659))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %660 @ge_eq_ne_(%661 a: i32, %662 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %663 x: bool [storage=automatic] = ge<i32>(read<i32>(%661), read<i32>(%662));
// DEFAULT-NEXT:         let %664 y: bool [storage=automatic] = eq<i32>(read<i32>(%661), read<i32>(%662));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%663)), from_bool<i32, reason=promotion>(read<bool>(%664)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %665 @ge_eq_ne_volatile(%666 a: i32, %667 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %668 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%666), read<i32>(%667));
// DEFAULT-NEXT:         let %669 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%666), read<i32>(%667));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%668)), from_bool<i32, reason=promotion>(read<bool, volatile>(%669)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %670 @ge_eq_eq_(%671 a: i32, %672 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %673 x: bool [storage=automatic] = ge<i32>(read<i32>(%671), read<i32>(%672));
// DEFAULT-NEXT:         let %674 y: bool [storage=automatic] = eq<i32>(read<i32>(%671), read<i32>(%672));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%673)), from_bool<i32, reason=promotion>(read<bool>(%674)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %675 @ge_eq_eq_volatile(%676 a: i32, %677 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %678 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%676), read<i32>(%677));
// DEFAULT-NEXT:         let %679 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%676), read<i32>(%677));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%678)), from_bool<i32, reason=promotion>(read<bool, volatile>(%679)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %680 @ge_eq_xor_(%681 a: i32, %682 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %683 x: bool [storage=automatic] = ge<i32>(read<i32>(%681), read<i32>(%682));
// DEFAULT-NEXT:         let %684 y: bool [storage=automatic] = eq<i32>(read<i32>(%681), read<i32>(%682));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%683)), from_bool<i32, reason=promotion>(read<bool>(%684))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %685 @ge_eq_xor_volatile(%686 a: i32, %687 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %688 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%686), read<i32>(%687));
// DEFAULT-NEXT:         let %689 y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%686), read<i32>(%687));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%688)), from_bool<i32, reason=promotion>(read<bool, volatile>(%689))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %690 @ge_ne_ne_(%691 a: i32, %692 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %693 x: bool [storage=automatic] = ge<i32>(read<i32>(%691), read<i32>(%692));
// DEFAULT-NEXT:         let %694 y: bool [storage=automatic] = ne<i32>(read<i32>(%691), read<i32>(%692));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%693)), from_bool<i32, reason=promotion>(read<bool>(%694)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %695 @ge_ne_ne_volatile(%696 a: i32, %697 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %698 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%696), read<i32>(%697));
// DEFAULT-NEXT:         let %699 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%696), read<i32>(%697));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%698)), from_bool<i32, reason=promotion>(read<bool, volatile>(%699)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %700 @ge_ne_eq_(%701 a: i32, %702 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %703 x: bool [storage=automatic] = ge<i32>(read<i32>(%701), read<i32>(%702));
// DEFAULT-NEXT:         let %704 y: bool [storage=automatic] = ne<i32>(read<i32>(%701), read<i32>(%702));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%703)), from_bool<i32, reason=promotion>(read<bool>(%704)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %705 @ge_ne_eq_volatile(%706 a: i32, %707 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %708 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%706), read<i32>(%707));
// DEFAULT-NEXT:         let %709 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%706), read<i32>(%707));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%708)), from_bool<i32, reason=promotion>(read<bool, volatile>(%709)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %710 @ge_ne_xor_(%711 a: i32, %712 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %713 x: bool [storage=automatic] = ge<i32>(read<i32>(%711), read<i32>(%712));
// DEFAULT-NEXT:         let %714 y: bool [storage=automatic] = ne<i32>(read<i32>(%711), read<i32>(%712));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%713)), from_bool<i32, reason=promotion>(read<bool>(%714))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %715 @ge_ne_xor_volatile(%716 a: i32, %717 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %718 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%716), read<i32>(%717));
// DEFAULT-NEXT:         let %719 y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%716), read<i32>(%717));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%718)), from_bool<i32, reason=promotion>(read<bool, volatile>(%719))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %720 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %723
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %721 x: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%721), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %797: i32 [synthetic] = read<i32>(%721);
// DEFAULT-NEXT:                 let %798: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%797), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%721, read<i32>(%798));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %724
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %722 y: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%722), const<i32>(10))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %799: i32 [synthetic] = read<i32>(%722);
// DEFAULT-NEXT:                         let %800: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%799), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%722, read<i32>(%800));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             do %725
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%0, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%5, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %726
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%10, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%15, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %727
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%20, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%25, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %728
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%30, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%35, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %729
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%40, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%45, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %730
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%50, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%55, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %731
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%60, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%65, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %732
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%70, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%75, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %733
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%80, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%85, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %734
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%90, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%95, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %735
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%100, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%105, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %736
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%110, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%115, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %737
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%120, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%125, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %738
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%130, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%135, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %739
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%140, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%145, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %740
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%150, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%155, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %741
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%160, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%165, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %742
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%170, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%175, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %743
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%180, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%185, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %744
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%190, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%195, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %745
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%200, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%205, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %746
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%210, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%215, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %747
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%220, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%225, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %748
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%230, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%235, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %749
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%240, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%245, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %750
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%250, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%255, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %751
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%260, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%265, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %752
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%270, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%275, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %753
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%280, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%285, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %754
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%290, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%295, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %755
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%300, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%305, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %756
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%310, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%315, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %757
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%320, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%325, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %758
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%330, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%335, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %759
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%340, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%345, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %760
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%350, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%355, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %761
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%360, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%365, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %762
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%370, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%375, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %763
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%380, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%385, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %764
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%390, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%395, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %765
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%400, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%405, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %766
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%410, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%415, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %767
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%420, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%425, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %768
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%430, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%435, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %769
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%440, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%445, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %770
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%450, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%455, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %771
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%460, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%465, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %772
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%470, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%475, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %773
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%480, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%485, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %774
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%490, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%495, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %775
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%500, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%505, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %776
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%510, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%515, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %777
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%520, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%525, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %778
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%530, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%535, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %779
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%540, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%545, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %780
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%550, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%555, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %781
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%560, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%565, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %782
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%570, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%575, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %783
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%580, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%585, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %784
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%590, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%595, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %785
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%600, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%605, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %786
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%610, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%615, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %787
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%620, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%625, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %788
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%630, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%635, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %789
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%640, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%645, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %790
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%650, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%655, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %791
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%660, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%665, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %792
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%670, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%675, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %793
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%680, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%685, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %794
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%690, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%695, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %795
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%700, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%705, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %796
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%710, read<i32>(%721), read<i32>(%722))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%715, read<i32>(%721), read<i32>(%722))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
