#define func(vol, op1, op2, op3)                                               \
  _Bool op1##_##op2##_##op3##_##vol(int a, int b) {                            \
    vol _Bool x = op_##op1(a, b);                                              \
    vol _Bool y = op_##op2(a, b);                                              \
    return op_##op3(x - y, 0);                                                 \
  }

#define op_lt(a, b) ((a) < (b))
#define op_le(a, b) ((a) <= (b))
#define op_gt(a, b) ((a) > (b))
#define op_ge(a, b) ((a) >= (b))

#define funcs(a)                                                               \
  a(gt, lt, lt) a(gt, lt, le) a(gt, lt, gt) a(gt, lt, ge)                      \
                                                                               \
      a(ge, le, lt) a(ge, le, le) a(ge, le, gt) a(ge, le, ge)                  \
                                                                               \
          a(lt, gt, lt) a(lt, gt, le) a(lt, gt, gt) a(lt, gt, ge)              \
                                                                               \
              a(le, ge, lt) a(le, ge, le) a(le, ge, gt) a(le, ge, ge)

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
// DEFAULT-NEXT:     fn %0 @gt_lt_lt_(%1 a: i32, %2 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 x: bool [storage=automatic] = gt<i32>(read<i32>(%1), read<i32>(%2));
// DEFAULT-NEXT:         let %4 y: bool [storage=automatic] = lt<i32>(read<i32>(%1), read<i32>(%2));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%3)), from_bool<i32, reason=promotion>(read<bool>(%4))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @gt_lt_lt_volatile(%6 a: i32, %7 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%6), read<i32>(%7));
// DEFAULT-NEXT:         let %9 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%6), read<i32>(%7));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%8)), from_bool<i32, reason=promotion>(read<bool, volatile>(%9))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @gt_lt_le_(%11 a: i32, %12 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 x: bool [storage=automatic] = gt<i32>(read<i32>(%11), read<i32>(%12));
// DEFAULT-NEXT:         let %14 y: bool [storage=automatic] = lt<i32>(read<i32>(%11), read<i32>(%12));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%13)), from_bool<i32, reason=promotion>(read<bool>(%14))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @gt_lt_le_volatile(%16 a: i32, %17 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%16), read<i32>(%17));
// DEFAULT-NEXT:         let %19 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%16), read<i32>(%17));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%18)), from_bool<i32, reason=promotion>(read<bool, volatile>(%19))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @gt_lt_gt_(%21 a: i32, %22 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 x: bool [storage=automatic] = gt<i32>(read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:         let %24 y: bool [storage=automatic] = lt<i32>(read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%23)), from_bool<i32, reason=promotion>(read<bool>(%24))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @gt_lt_gt_volatile(%26 a: i32, %27 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %28 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%26), read<i32>(%27));
// DEFAULT-NEXT:         let %29 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%26), read<i32>(%27));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%28)), from_bool<i32, reason=promotion>(read<bool, volatile>(%29))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @gt_lt_ge_(%31 a: i32, %32 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33 x: bool [storage=automatic] = gt<i32>(read<i32>(%31), read<i32>(%32));
// DEFAULT-NEXT:         let %34 y: bool [storage=automatic] = lt<i32>(read<i32>(%31), read<i32>(%32));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%33)), from_bool<i32, reason=promotion>(read<bool>(%34))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @gt_lt_ge_volatile(%36 a: i32, %37 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %38 x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%36), read<i32>(%37));
// DEFAULT-NEXT:         let %39 y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%36), read<i32>(%37));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%38)), from_bool<i32, reason=promotion>(read<bool, volatile>(%39))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @ge_le_lt_(%41 a: i32, %42 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %43 x: bool [storage=automatic] = ge<i32>(read<i32>(%41), read<i32>(%42));
// DEFAULT-NEXT:         let %44 y: bool [storage=automatic] = le<i32>(read<i32>(%41), read<i32>(%42));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%43)), from_bool<i32, reason=promotion>(read<bool>(%44))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @ge_le_lt_volatile(%46 a: i32, %47 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %48 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%46), read<i32>(%47));
// DEFAULT-NEXT:         let %49 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%46), read<i32>(%47));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%48)), from_bool<i32, reason=promotion>(read<bool, volatile>(%49))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @ge_le_le_(%51 a: i32, %52 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %53 x: bool [storage=automatic] = ge<i32>(read<i32>(%51), read<i32>(%52));
// DEFAULT-NEXT:         let %54 y: bool [storage=automatic] = le<i32>(read<i32>(%51), read<i32>(%52));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%53)), from_bool<i32, reason=promotion>(read<bool>(%54))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @ge_le_le_volatile(%56 a: i32, %57 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %58 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%56), read<i32>(%57));
// DEFAULT-NEXT:         let %59 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%56), read<i32>(%57));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%58)), from_bool<i32, reason=promotion>(read<bool, volatile>(%59))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @ge_le_gt_(%61 a: i32, %62 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %63 x: bool [storage=automatic] = ge<i32>(read<i32>(%61), read<i32>(%62));
// DEFAULT-NEXT:         let %64 y: bool [storage=automatic] = le<i32>(read<i32>(%61), read<i32>(%62));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%63)), from_bool<i32, reason=promotion>(read<bool>(%64))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %65 @ge_le_gt_volatile(%66 a: i32, %67 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %68 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%66), read<i32>(%67));
// DEFAULT-NEXT:         let %69 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%66), read<i32>(%67));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%68)), from_bool<i32, reason=promotion>(read<bool, volatile>(%69))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @ge_le_ge_(%71 a: i32, %72 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %73 x: bool [storage=automatic] = ge<i32>(read<i32>(%71), read<i32>(%72));
// DEFAULT-NEXT:         let %74 y: bool [storage=automatic] = le<i32>(read<i32>(%71), read<i32>(%72));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%73)), from_bool<i32, reason=promotion>(read<bool>(%74))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @ge_le_ge_volatile(%76 a: i32, %77 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %78 x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%76), read<i32>(%77));
// DEFAULT-NEXT:         let %79 y: volatile bool [storage=automatic] = le<i32>(read<i32>(%76), read<i32>(%77));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%78)), from_bool<i32, reason=promotion>(read<bool, volatile>(%79))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @lt_gt_lt_(%81 a: i32, %82 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %83 x: bool [storage=automatic] = lt<i32>(read<i32>(%81), read<i32>(%82));
// DEFAULT-NEXT:         let %84 y: bool [storage=automatic] = gt<i32>(read<i32>(%81), read<i32>(%82));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%83)), from_bool<i32, reason=promotion>(read<bool>(%84))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %85 @lt_gt_lt_volatile(%86 a: i32, %87 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %88 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%86), read<i32>(%87));
// DEFAULT-NEXT:         let %89 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%86), read<i32>(%87));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%88)), from_bool<i32, reason=promotion>(read<bool, volatile>(%89))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @lt_gt_le_(%91 a: i32, %92 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %93 x: bool [storage=automatic] = lt<i32>(read<i32>(%91), read<i32>(%92));
// DEFAULT-NEXT:         let %94 y: bool [storage=automatic] = gt<i32>(read<i32>(%91), read<i32>(%92));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%93)), from_bool<i32, reason=promotion>(read<bool>(%94))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %95 @lt_gt_le_volatile(%96 a: i32, %97 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %98 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%96), read<i32>(%97));
// DEFAULT-NEXT:         let %99 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%96), read<i32>(%97));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%98)), from_bool<i32, reason=promotion>(read<bool, volatile>(%99))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @lt_gt_gt_(%101 a: i32, %102 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %103 x: bool [storage=automatic] = lt<i32>(read<i32>(%101), read<i32>(%102));
// DEFAULT-NEXT:         let %104 y: bool [storage=automatic] = gt<i32>(read<i32>(%101), read<i32>(%102));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%103)), from_bool<i32, reason=promotion>(read<bool>(%104))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %105 @lt_gt_gt_volatile(%106 a: i32, %107 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %108 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%106), read<i32>(%107));
// DEFAULT-NEXT:         let %109 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%106), read<i32>(%107));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%108)), from_bool<i32, reason=promotion>(read<bool, volatile>(%109))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %110 @lt_gt_ge_(%111 a: i32, %112 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %113 x: bool [storage=automatic] = lt<i32>(read<i32>(%111), read<i32>(%112));
// DEFAULT-NEXT:         let %114 y: bool [storage=automatic] = gt<i32>(read<i32>(%111), read<i32>(%112));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%113)), from_bool<i32, reason=promotion>(read<bool>(%114))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %115 @lt_gt_ge_volatile(%116 a: i32, %117 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %118 x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%116), read<i32>(%117));
// DEFAULT-NEXT:         let %119 y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%116), read<i32>(%117));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%118)), from_bool<i32, reason=promotion>(read<bool, volatile>(%119))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %120 @le_ge_lt_(%121 a: i32, %122 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %123 x: bool [storage=automatic] = le<i32>(read<i32>(%121), read<i32>(%122));
// DEFAULT-NEXT:         let %124 y: bool [storage=automatic] = ge<i32>(read<i32>(%121), read<i32>(%122));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%123)), from_bool<i32, reason=promotion>(read<bool>(%124))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %125 @le_ge_lt_volatile(%126 a: i32, %127 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %128 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%126), read<i32>(%127));
// DEFAULT-NEXT:         let %129 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%126), read<i32>(%127));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%128)), from_bool<i32, reason=promotion>(read<bool, volatile>(%129))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %130 @le_ge_le_(%131 a: i32, %132 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %133 x: bool [storage=automatic] = le<i32>(read<i32>(%131), read<i32>(%132));
// DEFAULT-NEXT:         let %134 y: bool [storage=automatic] = ge<i32>(read<i32>(%131), read<i32>(%132));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%133)), from_bool<i32, reason=promotion>(read<bool>(%134))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %135 @le_ge_le_volatile(%136 a: i32, %137 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %138 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%136), read<i32>(%137));
// DEFAULT-NEXT:         let %139 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%136), read<i32>(%137));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%138)), from_bool<i32, reason=promotion>(read<bool, volatile>(%139))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %140 @le_ge_gt_(%141 a: i32, %142 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %143 x: bool [storage=automatic] = le<i32>(read<i32>(%141), read<i32>(%142));
// DEFAULT-NEXT:         let %144 y: bool [storage=automatic] = ge<i32>(read<i32>(%141), read<i32>(%142));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%143)), from_bool<i32, reason=promotion>(read<bool>(%144))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %145 @le_ge_gt_volatile(%146 a: i32, %147 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %148 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%146), read<i32>(%147));
// DEFAULT-NEXT:         let %149 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%146), read<i32>(%147));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%148)), from_bool<i32, reason=promotion>(read<bool, volatile>(%149))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @le_ge_ge_(%151 a: i32, %152 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %153 x: bool [storage=automatic] = le<i32>(read<i32>(%151), read<i32>(%152));
// DEFAULT-NEXT:         let %154 y: bool [storage=automatic] = ge<i32>(read<i32>(%151), read<i32>(%152));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%153)), from_bool<i32, reason=promotion>(read<bool>(%154))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %155 @le_ge_ge_volatile(%156 a: i32, %157 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %158 x: volatile bool [storage=automatic] = le<i32>(read<i32>(%156), read<i32>(%157));
// DEFAULT-NEXT:         let %159 y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%156), read<i32>(%157));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%158)), from_bool<i32, reason=promotion>(read<bool, volatile>(%159))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %160 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %163
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %161 x: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%161), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %181: i32 [synthetic] = read<i32>(%161);
// DEFAULT-NEXT:                 let %182: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%181), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%161, read<i32>(%182));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %164
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %162 y: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%162), const<i32>(10))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %183: i32 [synthetic] = read<i32>(%162);
// DEFAULT-NEXT:                         let %184: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%183), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%162, read<i32>(%184));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             do %165
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%0, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%5, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %166
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%10, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%15, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %167
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%20, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%25, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %168
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%30, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%35, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %169
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%40, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%45, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %170
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%50, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%55, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %171
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%60, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%65, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %172
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%70, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%75, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %173
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%80, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%85, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %174
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%90, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%95, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %175
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%100, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%105, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %176
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%110, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%115, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %177
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%120, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%125, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %178
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%130, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%135, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %179
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%140, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%145, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %180
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%150, read<i32>(%161), read<i32>(%162))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%155, read<i32>(%161), read<i32>(%162))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
