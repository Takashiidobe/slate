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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_Yf:[0-9]+]] Yf: array<f32, 8> [storage=static] [align=16] = aggregate<array<f32, 8>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), index4 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), index5 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index6 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), index7 = call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff:[0-9]+]])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_Zf:[0-9]+]] Zf: array<f32, 8> [storage=static] [const] [align=16] = aggregate<array<f32, 8>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0))), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0))), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), index4 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), index5 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), index6 = neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])), index7 = call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_Y:[0-9]+]] Y: array<f64, 8> [storage=static] [align=16] = aggregate<array<f64, 8>, zero_fill=false>(index0 = const<f64>(2.0), index1 = neg<f64>(const<f64>(2.0)), index2 = neg<f64>(const<f64>(2.0)), index3 = neg<f64>(const<f64>(2.0)), index4 = neg<f64>(const<f64>(2.0)), index5 = const<f64>(2.0), index6 = neg<f64>(const<f64>(0.0)), index7 = call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf:[0-9]+]])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_Z:[0-9]+]] Z: array<f64, 8> [storage=static] [const] [align=16] = aggregate<array<f64, 8>, zero_fill=false>(index0 = const<f64>(1.0), index1 = neg<f64>(const<f64>(1.0)), index2 = neg<f64>(const<f64>(1.0)), index3 = neg<f64>(const<f64>(0.0)), index4 = neg<f64>(const<f64>(0.0)), index5 = const<f64>(0.0), index6 = neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])), index7 = call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_Yl:[0-9]+]] Yl: array<f80, 8> [storage=static] = aggregate<array<f80, 8>, zero_fill=false>(index0 = float_widen<f80, reason=assign>(const<f64>(2.0)), index1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), index2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), index3 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), index4 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), index5 = float_widen<f80, reason=assign>(const<f64>(2.0)), index6 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), index7 = call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl:[0-9]+]])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_Zl:[0-9]+]] Zl: array<f80, 8> [storage=static] [const] = aggregate<array<f80, 8>, zero_fill=false>(index0 = float_widen<f80, reason=assign>(const<f64>(1.0)), index1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0))), index2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0))), index3 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), index4 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), index5 = float_widen<f80, reason=assign>(const<f64>(0.0)), index6 = neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])), index7 = call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_5]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const void>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const void>, %[[VALUE___n:[0-9]+]] __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf]] @__builtin_nanf(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysignf:[0-9]+]] @__builtin_copysignf(%[[VALUE1:[0-9]+]] <unnamed>: f32, %[[VALUE2:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_testf:[0-9]+]] @testf() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: array<f32, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_r]]), const<i32>(0))), call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_copysignf]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_Yf]]), const<i32>(0))))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_r]]), const<i32>(1))), call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_copysignf]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_Yf]]), const<i32>(1))))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_r]]), const<i32>(2))), call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_copysignf]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(neg<f64>(const<f64>(1.0))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_Yf]]), const<i32>(2))))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_r]]), const<i32>(3))), call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_copysignf]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(0.0)), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_Yf]]), const<i32>(3))))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_r]]), const<i32>(4))), call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_copysignf]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(neg<f64>(const<f64>(0.0))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_Yf]]), const<i32>(4))))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_r]]), const<i32>(5))), call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_copysignf]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(neg<f64>(const<f64>(0.0))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_Yf]]), const<i32>(5))))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_r]]), const<i32>(6))), call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_copysignf]], call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_Yf]]), const<i32>(6))))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_r]]), const<i32>(7))), call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_copysignf]], neg<f32>(call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_Yf]]), const<i32>(7))))));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(8)>(%[[VALUE_r]]), read<i32>(%[[VALUE_i]]))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<const f32>, length=Some(8)>(%[[VALUE_Zf]]), read<i32>(%[[VALUE_i]]))), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nan]] @__builtin_nan(%[[VALUE6:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysign:[0-9]+]] @__builtin_copysign(%[[VALUE7:[0-9]+]] <unnamed>: f64, %[[VALUE8:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: array<f64, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_r_2]]), const<i32>(0))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], const<f64>(1.0), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_Y]]), const<i32>(0))))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_r_2]]), const<i32>(1))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], const<f64>(1.0), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_Y]]), const<i32>(1))))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_r_2]]), const<i32>(2))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], neg<f64>(const<f64>(1.0)), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_Y]]), const<i32>(2))))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_r_2]]), const<i32>(3))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], const<f64>(0.0), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_Y]]), const<i32>(3))))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_r_2]]), const<i32>(4))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], neg<f64>(const<f64>(0.0)), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_Y]]), const<i32>(4))))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_r_2]]), const<i32>(5))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], neg<f64>(const<f64>(0.0)), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_Y]]), const<i32>(5))))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_r_2]]), const<i32>(6))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_Y]]), const<i32>(6))))));
// DEFAULT-NEXT:         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_r_2]]), const<i32>(7))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], neg<f64>(call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_4]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_Y]]), const<i32>(7))))));
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%[[VALUE_r_2]]), read<i32>(%[[VALUE_i_2]]))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<const f64>, length=Some(8)>(%[[VALUE_Z]]), read<i32>(%[[VALUE_i_2]]))), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infl]] @__builtin_infl() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanl]] @__builtin_nanl(%[[VALUE12:[0-9]+]] <unnamed>: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysignl:[0-9]+]] @__builtin_copysignl(%[[VALUE13:[0-9]+]] <unnamed>: f80, %[[VALUE14:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_testl:[0-9]+]] @testl() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: array<f80, 8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_r_3]]), const<i32>(0))), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE___builtin_copysignl]], float_widen<f80, reason=arg>(const<f64>(1.0)), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_Yl]]), const<i32>(0))))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_r_3]]), const<i32>(1))), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE___builtin_copysignl]], float_widen<f80, reason=arg>(const<f64>(1.0)), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_Yl]]), const<i32>(1))))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_r_3]]), const<i32>(2))), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE___builtin_copysignl]], float_widen<f80, reason=arg>(neg<f64>(const<f64>(1.0))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_Yl]]), const<i32>(2))))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_r_3]]), const<i32>(3))), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE___builtin_copysignl]], float_widen<f80, reason=arg>(const<f64>(0.0)), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_Yl]]), const<i32>(3))))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_r_3]]), const<i32>(4))), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE___builtin_copysignl]], float_widen<f80, reason=arg>(neg<f64>(const<f64>(0.0))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_Yl]]), const<i32>(4))))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_r_3]]), const<i32>(5))), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE___builtin_copysignl]], float_widen<f80, reason=arg>(neg<f64>(const<f64>(0.0))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_Yl]]), const<i32>(5))))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_r_3]]), const<i32>(6))), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE___builtin_copysignl]], call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_Yl]]), const<i32>(6))))));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_r_3]]), const<i32>(7))), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE___builtin_copysignl]], neg<f80>(call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_6]])))), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_Yl]]), const<i32>(7))))));
// DEFAULT-NEXT:         for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(8)>(%[[VALUE_r_3]]), read<i32>(%[[VALUE_i_3]]))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<const f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<const f80>, length=Some(8)>(%[[VALUE_Zl]]), read<i32>(%[[VALUE_i_3]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testf]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testl]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
