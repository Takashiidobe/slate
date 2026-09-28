#define func(vol, op1, op2)                                                    \
  _Bool op1##_##op2##_##vol(int a, int b) {                                    \
    vol int x = op_##op1(a, b);                                                \
    return op_##op2(x, a);                                                     \
  }

#define op_lt(a, b)  ((a) < (b))
#define op_le(a, b)  ((a) <= (b))
#define op_eq(a, b)  ((a) == (b))
#define op_ne(a, b)  ((a) != (b))
#define op_gt(a, b)  ((a) > (b))
#define op_ge(a, b)  ((a) >= (b))
#define op_min(a, b) ((a) < (b) ? (a) : (b))
#define op_max(a, b) ((a) > (b) ? (a) : (b))

#define funcs(a)                                                               \
  a(min, lt) a(max, lt) a(min, gt) a(max, gt) a(min, le) a(max, le) a(min, ge) \
      a(max, ge) a(min, ne) a(max, ne) a(min, eq) a(max, eq)

#define funcs1(a, b) func(, a, b) func(volatile, a, b)

funcs(funcs1)

#define test(op1, op2)                                                         \
  do {                                                                         \
    if (op1##_##op2##_(x, y) != op1##_##op2##_volatile(x, y))                  \
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
// DEFAULT-NEXT:     fn %0 @min_lt_(%1 a: i32, %2 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%1), read<i32>(%2)), read<i32>(%1), read<i32>(%2));
// DEFAULT-NEXT:         return lt<i32>(read<i32>(%3), read<i32>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @min_lt_volatile(%5 a: i32, %6 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%5), read<i32>(%6)), read<i32>(%5), read<i32>(%6));
// DEFAULT-NEXT:         return lt<i32>(read<i32, volatile>(%7), read<i32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @max_lt_(%9 a: i32, %10 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%9), read<i32>(%10)), read<i32>(%9), read<i32>(%10));
// DEFAULT-NEXT:         return lt<i32>(read<i32>(%11), read<i32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @max_lt_volatile(%13 a: i32, %14 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%13), read<i32>(%14)), read<i32>(%13), read<i32>(%14));
// DEFAULT-NEXT:         return lt<i32>(read<i32, volatile>(%15), read<i32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @min_gt_(%17 a: i32, %18 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%17), read<i32>(%18)), read<i32>(%17), read<i32>(%18));
// DEFAULT-NEXT:         return gt<i32>(read<i32>(%19), read<i32>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @min_gt_volatile(%21 a: i32, %22 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%21), read<i32>(%22)), read<i32>(%21), read<i32>(%22));
// DEFAULT-NEXT:         return gt<i32>(read<i32, volatile>(%23), read<i32>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @max_gt_(%25 a: i32, %26 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%25), read<i32>(%26)), read<i32>(%25), read<i32>(%26));
// DEFAULT-NEXT:         return gt<i32>(read<i32>(%27), read<i32>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @max_gt_volatile(%29 a: i32, %30 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%29), read<i32>(%30)), read<i32>(%29), read<i32>(%30));
// DEFAULT-NEXT:         return gt<i32>(read<i32, volatile>(%31), read<i32>(%29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @min_le_(%33 a: i32, %34 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35 x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%33), read<i32>(%34)), read<i32>(%33), read<i32>(%34));
// DEFAULT-NEXT:         return le<i32>(read<i32>(%35), read<i32>(%33));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @min_le_volatile(%37 a: i32, %38 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %39 x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%37), read<i32>(%38)), read<i32>(%37), read<i32>(%38));
// DEFAULT-NEXT:         return le<i32>(read<i32, volatile>(%39), read<i32>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @max_le_(%41 a: i32, %42 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %43 x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%41), read<i32>(%42)), read<i32>(%41), read<i32>(%42));
// DEFAULT-NEXT:         return le<i32>(read<i32>(%43), read<i32>(%41));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @max_le_volatile(%45 a: i32, %46 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %47 x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%45), read<i32>(%46)), read<i32>(%45), read<i32>(%46));
// DEFAULT-NEXT:         return le<i32>(read<i32, volatile>(%47), read<i32>(%45));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @min_ge_(%49 a: i32, %50 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %51 x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%49), read<i32>(%50)), read<i32>(%49), read<i32>(%50));
// DEFAULT-NEXT:         return ge<i32>(read<i32>(%51), read<i32>(%49));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @min_ge_volatile(%53 a: i32, %54 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %55 x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%53), read<i32>(%54)), read<i32>(%53), read<i32>(%54));
// DEFAULT-NEXT:         return ge<i32>(read<i32, volatile>(%55), read<i32>(%53));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @max_ge_(%57 a: i32, %58 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %59 x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%57), read<i32>(%58)), read<i32>(%57), read<i32>(%58));
// DEFAULT-NEXT:         return ge<i32>(read<i32>(%59), read<i32>(%57));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @max_ge_volatile(%61 a: i32, %62 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %63 x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%61), read<i32>(%62)), read<i32>(%61), read<i32>(%62));
// DEFAULT-NEXT:         return ge<i32>(read<i32, volatile>(%63), read<i32>(%61));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @min_ne_(%65 a: i32, %66 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %67 x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%65), read<i32>(%66)), read<i32>(%65), read<i32>(%66));
// DEFAULT-NEXT:         return ne<i32>(read<i32>(%67), read<i32>(%65));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @min_ne_volatile(%69 a: i32, %70 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %71 x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%69), read<i32>(%70)), read<i32>(%69), read<i32>(%70));
// DEFAULT-NEXT:         return ne<i32>(read<i32, volatile>(%71), read<i32>(%69));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @max_ne_(%73 a: i32, %74 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %75 x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%73), read<i32>(%74)), read<i32>(%73), read<i32>(%74));
// DEFAULT-NEXT:         return ne<i32>(read<i32>(%75), read<i32>(%73));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %76 @max_ne_volatile(%77 a: i32, %78 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %79 x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%77), read<i32>(%78)), read<i32>(%77), read<i32>(%78));
// DEFAULT-NEXT:         return ne<i32>(read<i32, volatile>(%79), read<i32>(%77));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @min_eq_(%81 a: i32, %82 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %83 x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%81), read<i32>(%82)), read<i32>(%81), read<i32>(%82));
// DEFAULT-NEXT:         return eq<i32>(read<i32>(%83), read<i32>(%81));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %84 @min_eq_volatile(%85 a: i32, %86 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %87 x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%85), read<i32>(%86)), read<i32>(%85), read<i32>(%86));
// DEFAULT-NEXT:         return eq<i32>(read<i32, volatile>(%87), read<i32>(%85));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %88 @max_eq_(%89 a: i32, %90 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %91 x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%89), read<i32>(%90)), read<i32>(%89), read<i32>(%90));
// DEFAULT-NEXT:         return eq<i32>(read<i32>(%91), read<i32>(%89));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %92 @max_eq_volatile(%93 a: i32, %94 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %95 x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%93), read<i32>(%94)), read<i32>(%93), read<i32>(%94));
// DEFAULT-NEXT:         return eq<i32>(read<i32, volatile>(%95), read<i32>(%93));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %102 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %96 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %99
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %97 x: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %114: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %115: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%114), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%115));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %100
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %98 y: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%98), const<i32>(10))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %116: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                         let %117: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%116), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%98, read<i32>(%117));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             do %101
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%0, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%4, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %103
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%8, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%12, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %104
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%16, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%20, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %105
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%24, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%28, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %106
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%32, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%36, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %107
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%40, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%44, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %108
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%48, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%52, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %109
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%56, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%60, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %110
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%64, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%68, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %111
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%72, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%76, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %112
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%80, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%84, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %113
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%88, read<i32>(%97), read<i32>(%98))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%92, read<i32>(%97), read<i32>(%98))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
