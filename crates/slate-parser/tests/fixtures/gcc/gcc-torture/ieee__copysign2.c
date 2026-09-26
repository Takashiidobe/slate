/* { dg-do run } */
#include <float.h>
#include <stdlib.h>
#include <string.h>

#define fpsizeoff sizeof(float)
#define fpsizeof  sizeof(double)
#define fpsizeofl sizeof(long double)

/* Work around the fact that with the Intel double-extended precision,
   we've got a 10 byte type stuffed into some amount of padding.  And
   the fact that -ffloat-store is going to stuff this value temporarily
   into some bit of stack frame that we've no control over and can't zero.  */
#if LDBL_MANT_DIG == 64
#if defined(__i386__) || defined(__x86_64__) || defined(__ia64__)
#undef fpsizeofl
#define fpsizeofl 10
#endif
#endif

/* Work around the fact that the sign of the second double in the IBM
   double-double format is not strictly specified when it contains a zero.
   For instance, -0.0L can be represented with either (-0.0, +0.0) or
   (-0.0, -0.0).  The former is what we'll get from the compiler when it
   builds constants; the later is what we'll get from the negation operator
   at runtime.  */
/* ??? This hack only works for big-endian, which is fortunately true for
   AIX and Darwin.  */
#if LDBL_MANT_DIG == 106
#undef fpsizeofl
#define fpsizeofl sizeof(double)
#endif

#define TEST(TYPE, EXT)                                                        \
  static TYPE       Y##EXT[] = {2.0,  -2.0, -2.0, -2.0,                        \
                                -2.0, 2.0,  -0.0, __builtin_inf##EXT()};       \
  static const TYPE Z##EXT[] = {1.0,                                           \
                                -1.0,                                          \
                                -1.0,                                          \
                                -0.0,                                          \
                                -0.0,                                          \
                                0.0,                                           \
                                -__builtin_inf##EXT(),                         \
                                __builtin_nan##EXT("")};                       \
                                                                               \
  void test##EXT(void) {                                                       \
    TYPE r[8];                                                                 \
    int  i;                                                                    \
    r[0] = __builtin_copysign##EXT(1.0, Y##EXT[0]);                            \
    r[1] = __builtin_copysign##EXT(1.0, Y##EXT[1]);                            \
    r[2] = __builtin_copysign##EXT(-1.0, Y##EXT[2]);                           \
    r[3] = __builtin_copysign##EXT(0.0, Y##EXT[3]);                            \
    r[4] = __builtin_copysign##EXT(-0.0, Y##EXT[4]);                           \
    r[5] = __builtin_copysign##EXT(-0.0, Y##EXT[5]);                           \
    r[6] = __builtin_copysign##EXT(__builtin_inf##EXT(), Y##EXT[6]);           \
    r[7] = __builtin_copysign##EXT(-__builtin_nan##EXT(""), Y##EXT[7]);        \
    for (i = 0; i < 8; ++i)                                                    \
      if (memcmp(r + i, Z##EXT + i, fpsizeof##EXT) != 0)                       \
        abort();                                                               \
  }

TEST(float, f)
TEST(double, )
TEST(long double, l)

int main() {
  testf();
  test();
  testl();
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %3 Yf: array<f32, 8> [storage=static] [align=16] = aggregate<array<f32, 8>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), index4 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), index5 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index6 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), index7 = call<f32, signature=fn() -> f32>(__builtin_inff)) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %4 Zf: array<f32, 8> [storage=static] [const] [align=16] = aggregate<array<f32, 8>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0))), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0))), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), index4 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), index5 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), index6 = neg<f32>(call<f32, signature=fn() -> f32>(__builtin_inff)), index7 = call<f32, signature=fn(ptr<const i8>) -> f32>(__builtin_nanf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%22)))) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 Y: array<f64, 8> [storage=static] [align=16] = aggregate<array<f64, 8>, zero_fill=false>(index0 = const<f64>(2.0), index1 = neg<f64>(const<f64>(2.0)), index2 = neg<f64>(const<f64>(2.0)), index3 = neg<f64>(const<f64>(2.0)), index4 = neg<f64>(const<f64>(2.0)), index5 = const<f64>(2.0), index6 = neg<f64>(const<f64>(0.0)), index7 = call<f64, signature=fn() -> f64>(__builtin_inf)) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 Z: array<f64, 8> [storage=static] [const] [align=16] = aggregate<array<f64, 8>, zero_fill=false>(index0 = const<f64>(1.0), index1 = neg<f64>(const<f64>(1.0)), index2 = neg<f64>(const<f64>(1.0)), index3 = neg<f64>(const<f64>(0.0)), index4 = neg<f64>(const<f64>(0.0)), index5 = const<f64>(0.0), index6 = neg<f64>(call<f64, signature=fn() -> f64>(__builtin_inf)), index7 = call<f64, signature=fn(ptr<const i8>) -> f64>(__builtin_nan, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%25)))) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 Yl: array<f80, 8> [storage=static] = aggregate<array<f80, 8>, zero_fill=false>(index0 = float_widen<f80, reason=assign>(const<f64>(2.0)), index1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), index2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), index3 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), index4 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), index5 = float_widen<f80, reason=assign>(const<f64>(2.0)), index6 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), index7 = call<f80, signature=fn() -> f80>(__builtin_infl)) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 Zl: array<f80, 8> [storage=static] [const] = aggregate<array<f80, 8>, zero_fill=false>(index0 = float_widen<f80, reason=assign>(const<f64>(1.0)), index1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0))), index2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0))), index3 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), index4 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), index5 = float_widen<f80, reason=assign>(const<f64>(0.0)), index6 = neg<f80>(call<f80, signature=fn() -> f80>(__builtin_infl)), index7 = call<f80, signature=fn(ptr<const i8>) -> f80>(__builtin_nanl, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%28)))) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @memcmp(%19 __s1: ptr<const void>, %20 __s2: ptr<const void>, %21 __n: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @testf() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 r: array<f32, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%6), const<i32>(0))), call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(0))))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(0)))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%6), const<i32>(1))), call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(1))))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%6), const<i32>(2))), call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(2))))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%6), const<i32>(3))), call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(3))))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%6), const<i32>(4))), call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(4))))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%6), const<i32>(5))), call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(5))))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%6), const<i32>(6))), call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, call<f32, signature=fn() -> f32>(__builtin_inff), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(6))))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, call<f32, signature=fn() -> f32>(__builtin_inff), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%6), const<i32>(7))), call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, neg<f32>(call<f32, signature=fn(ptr<const i8>) -> f32>(__builtin_nanf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%23)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(7))))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, neg<f32>(call<f32, signature=fn(ptr<const i8>) -> f32>(__builtin_nanf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%23)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%32));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%6), read<i32>(%7))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<const f32>, length=Some(8)>(%4), read<i32>(%7))), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 r: array<f64, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %12 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%11), const<i32>(0))), call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, const<f64>(1.0), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(0))))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, const<f64>(1.0), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(0)))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%11), const<i32>(1))), call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, const<f64>(1.0), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(1))))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, const<f64>(1.0), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(1)))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%11), const<i32>(2))), call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, neg<f64>(const<f64>(1.0)), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(2))))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, neg<f64>(const<f64>(1.0)), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(2)))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%11), const<i32>(3))), call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, const<f64>(0.0), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(3))))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, const<f64>(0.0), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(3)))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%11), const<i32>(4))), call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, neg<f64>(const<f64>(0.0)), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(4))))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, neg<f64>(const<f64>(0.0)), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(4)))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%11), const<i32>(5))), call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, neg<f64>(const<f64>(0.0)), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(5))))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, neg<f64>(const<f64>(0.0)), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(5)))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%11), const<i32>(6))), call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, call<f64, signature=fn() -> f64>(__builtin_inf), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(6))))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, call<f64, signature=fn() -> f64>(__builtin_inf), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(6)))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%11), const<i32>(7))), call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, neg<f64>(call<f64, signature=fn(ptr<const i8>) -> f64>(__builtin_nan, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%26)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(7))))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, neg<f64>(call<f64, signature=fn(ptr<const i8>) -> f64>(__builtin_nan, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%26)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%8), const<i32>(7)))));
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%34));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%11), read<i32>(%12))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<const f64>, length=Some(8)>(%9), read<i32>(%12))), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @testl() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 r: array<f80, 8> [storage=automatic];
// DEFAULT-NEXT:         let %17 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%16), const<i32>(0))), call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(const<f64>(1.0)), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(0))))));
// DEFAULT-NEXT:         call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(const<f64>(1.0)), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(0)))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%16), const<i32>(1))), call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(const<f64>(1.0)), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(1))))));
// DEFAULT-NEXT:         call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(const<f64>(1.0)), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(1)))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%16), const<i32>(2))), call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(neg<f64>(const<f64>(1.0))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(2))))));
// DEFAULT-NEXT:         call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(neg<f64>(const<f64>(1.0))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(2)))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%16), const<i32>(3))), call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(const<f64>(0.0)), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(3))))));
// DEFAULT-NEXT:         call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(const<f64>(0.0)), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(3)))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%16), const<i32>(4))), call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(neg<f64>(const<f64>(0.0))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(4))))));
// DEFAULT-NEXT:         call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(neg<f64>(const<f64>(0.0))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(4)))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%16), const<i32>(5))), call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(neg<f64>(const<f64>(0.0))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(5))))));
// DEFAULT-NEXT:         call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, float_widen<f80, reason=arg>(neg<f64>(const<f64>(0.0))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(5)))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%16), const<i32>(6))), call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, call<f80, signature=fn() -> f80>(__builtin_infl), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(6))))));
// DEFAULT-NEXT:         call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, call<f80, signature=fn() -> f80>(__builtin_infl), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(6)))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%16), const<i32>(7))), call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, neg<f80>(call<f80, signature=fn(ptr<const i8>) -> f80>(__builtin_nanl, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%29)))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(7))))));
// DEFAULT-NEXT:         call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, neg<f80>(call<f80, signature=fn(ptr<const i8>) -> f80>(__builtin_nanl, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%29)))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%13), const<i32>(7)))));
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%36));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%16), read<i32>(%17))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<const f80>, length=Some(8)>(%14), read<i32>(%17))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
