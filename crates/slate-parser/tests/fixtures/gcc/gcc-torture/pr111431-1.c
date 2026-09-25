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
// DEFAULT-NEXT:     fn %0 @foo(%1 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 b: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32>(%1), const<i32>(0)));
// DEFAULT-NEXT:         return and<i32>(read<i32>(%1), read<i32>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @func_0_(%4 a: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 b: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32>(%4), const<i32>(0)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32>(%4), read<i32>(%5)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @func_0_volatile(%7 a: volatile i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 b: volatile i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32, volatile>(%7), const<i32>(0)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32, volatile>(%7), read<i32, volatile>(%8)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @func_1_(%10 a: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 b: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32>(%10), const<i32>(1)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32>(%10), read<i32>(%11)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @func_1_volatile(%13 a: volatile i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 b: volatile i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32, volatile>(%13), const<i32>(1)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32, volatile>(%13), read<i32, volatile>(%14)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @func_5_(%16 a: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 b: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32>(%16), const<i32>(5)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32>(%16), read<i32>(%17)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @func_5_volatile(%19 a: volatile i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 b: volatile i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32, volatile>(%19), const<i32>(5)));
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(read<i32, volatile>(%19), read<i32, volatile>(%20)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %22 a: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%22), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%22, read<i32>(%28));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     do %24
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%3, read<i32>(%22))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%6, read<i32>(%22))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     do %25
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%9, read<i32>(%22))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%12, read<i32>(%22))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     do %26
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%15, read<i32>(%22))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32) -> bool>(%18, read<i32>(%22))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
