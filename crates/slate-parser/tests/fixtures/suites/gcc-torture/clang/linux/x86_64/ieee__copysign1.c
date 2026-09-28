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
   AIX and, Darwin.  */
#if LDBL_MANT_DIG == 106
#undef fpsizeofl
#define fpsizeofl sizeof(double)
#endif

#define TEST(TYPE, EXT)                                                        \
  TYPE c##EXT(TYPE x, TYPE y) { return __builtin_copysign##EXT(x, y); }        \
                                                                               \
  struct D##EXT {                                                              \
    TYPE x, y, z;                                                              \
  };                                                                           \
                                                                               \
  static const struct D##EXT T##EXT[] = {                                      \
      {1.0, 2.0, 1.0},                                                         \
      {1.0, -2.0, -1.0},                                                       \
      {-1.0, -2.0, -1.0},                                                      \
      {0.0, -2.0, -0.0},                                                       \
      {-0.0, -2.0, -0.0},                                                      \
      {-0.0, 2.0, 0.0},                                                        \
      {__builtin_inf##EXT(), -0.0, -__builtin_inf##EXT()},                     \
      {-__builtin_nan##EXT(""), __builtin_inf##EXT(),                          \
       __builtin_nan##EXT("")}};                                               \
                                                                               \
  void test##EXT(void) {                                                       \
    int  i, n = sizeof(T##EXT) / sizeof(T##EXT[0]);                            \
    TYPE r;                                                                    \
    for (i = 0; i < n; ++i) {                                                  \
      r = c##EXT(T##EXT[i].x, T##EXT[i].y);                                    \
      if (memcmp(&r, &T##EXT[i].z, fpsizeof##EXT) != 0)                        \
        abort();                                                               \
    }                                                                          \
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
// DEFAULT-NEXT:     type @type1 Df = struct {
// DEFAULT-NEXT:         field0 x: f32;
// DEFAULT-NEXT:         field1 y: f32;
// DEFAULT-NEXT:         field2 z: f32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type2 D = struct {
// DEFAULT-NEXT:         field0 x: f64;
// DEFAULT-NEXT:         field1 y: f64;
// DEFAULT-NEXT:         field2 z: f64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type3 Dl = struct {
// DEFAULT-NEXT:         field0 x: f80;
// DEFAULT-NEXT:         field1 y: f80;
// DEFAULT-NEXT:         field2 z: f80;
// DEFAULT-NEXT:     } [size=48, align=16, offsets=[0, 16, 32]];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 Tf: array<@type1, 8> [storage=static] [const] [align=16] = aggregate<array<@type1, 8>, zero_fill=false>(index0 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0))), index1 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0)))), index2 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0))), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0)))), index3 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0)))), index4 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0)))), index5 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0))), index6 = aggregate<@type1, zero_fill=false>(field0 = call<f32, signature=fn() -> f32>(%40), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), field2 = neg<f32>(call<f32, signature=fn() -> f32>(%40))), index7 = aggregate<@type1, zero_fill=false>(field0 = neg<f32>(call<f32, signature=fn(ptr<const i8>) -> f32>(%42, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%43)))), field1 = call<f32, signature=fn() -> f32>(%40), field2 = call<f32, signature=fn(ptr<const i8>) -> f32>(%42, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%44))))) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 T: array<@type2, 8> [storage=static] [const] [align=16] = aggregate<array<@type2, 8>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field0 = const<f64>(1.0), field1 = const<f64>(2.0), field2 = const<f64>(1.0)), index1 = aggregate<@type2, zero_fill=false>(field0 = const<f64>(1.0), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(1.0))), index2 = aggregate<@type2, zero_fill=false>(field0 = neg<f64>(const<f64>(1.0)), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(1.0))), index3 = aggregate<@type2, zero_fill=false>(field0 = const<f64>(0.0), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(0.0))), index4 = aggregate<@type2, zero_fill=false>(field0 = neg<f64>(const<f64>(0.0)), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(0.0))), index5 = aggregate<@type2, zero_fill=false>(field0 = neg<f64>(const<f64>(0.0)), field1 = const<f64>(2.0), field2 = const<f64>(0.0)), index6 = aggregate<@type2, zero_fill=false>(field0 = call<f64, signature=fn() -> f64>(%49), field1 = neg<f64>(const<f64>(0.0)), field2 = neg<f64>(call<f64, signature=fn() -> f64>(%49))), index7 = aggregate<@type2, zero_fill=false>(field0 = neg<f64>(call<f64, signature=fn(ptr<const i8>) -> f64>(%51, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%52)))), field1 = call<f64, signature=fn() -> f64>(%49), field2 = call<f64, signature=fn(ptr<const i8>) -> f64>(%51, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%53))))) [linkage=internal];
// DEFAULT-NEXT:     global %61 .str61: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %62 .str62: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 Tl: array<@type3, 8> [storage=static] [const] = aggregate<array<@type3, 8>, zero_fill=false>(index0 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(const<f64>(1.0)), field1 = float_widen<f80, reason=assign>(const<f64>(2.0)), field2 = float_widen<f80, reason=assign>(const<f64>(1.0))), index1 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(const<f64>(1.0)), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0)))), index2 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0))), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0)))), index3 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(const<f64>(0.0)), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0)))), index4 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0)))), index5 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), field1 = float_widen<f80, reason=assign>(const<f64>(2.0)), field2 = float_widen<f80, reason=assign>(const<f64>(0.0))), index6 = aggregate<@type3, zero_fill=false>(field0 = call<f80, signature=fn() -> f80>(%58), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), field2 = neg<f80>(call<f80, signature=fn() -> f80>(%58))), index7 = aggregate<@type3, zero_fill=false>(field0 = neg<f80>(call<f80, signature=fn(ptr<const i8>) -> f80>(%60, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%61)))), field1 = call<f80, signature=fn() -> f80>(%58), field2 = call<f80, signature=fn(ptr<const i8>) -> f80>(%60, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%62))))) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @memcmp(%34 __s1: ptr<const void>, %35 __s2: ptr<const void>, %36 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %39 @__builtin_copysignf(%37 <unnamed>: f32, %38 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %6 @cf(%7 x: f32, %8 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%39, read<f32>(%7), read<f32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %42 @__builtin_nanf(%41 <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %11 @testf() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(96), const<u64>(12))));
// DEFAULT-NEXT:         let %14 r: f32 [storage=automatic];
// DEFAULT-NEXT:         for %45
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), read<i32>(%13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %64: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %65: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%64), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%65));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32>(%14, call<f32, signature=fn(f32, f32) -> f32>(%6, read<f32>(field0(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(8)>(%10), read<i32>(%12))))), read<f32>(field1(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(8)>(%10), read<i32>(%12)))))));
// DEFAULT-NEXT:                     call<f32, signature=fn(f32, f32) -> f32>(%6, read<f32>(field0(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(8)>(%10), read<i32>(%12))))), read<f32>(field1(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(8)>(%10), read<i32>(%12))))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%5, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f32>>(%14)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const f32>>(field2(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(8)>(%10), read<i32>(%12)))))), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @__builtin_copysign(%46 <unnamed>: f64, %47 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %15 @c(%16 x: f64, %17 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%48, read<f64>(%16), read<f64>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %51 @__builtin_nan(%50 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %20 @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %21 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %22 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(192), const<u64>(24))));
// DEFAULT-NEXT:         let %23 r: f64 [storage=automatic];
// DEFAULT-NEXT:         for %54
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%21, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%21), read<i32>(%22))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %66: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                 let %67: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%66), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%21, read<i32>(%67));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(%23, call<f64, signature=fn(f64, f64) -> f64>(%15, read<f64>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(8)>(%19), read<i32>(%21))))), read<f64>(field1(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(8)>(%19), read<i32>(%21)))))));
// DEFAULT-NEXT:                     call<f64, signature=fn(f64, f64) -> f64>(%15, read<f64>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(8)>(%19), read<i32>(%21))))), read<f64>(field1(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(8)>(%19), read<i32>(%21))))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%5, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f64>>(%23)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const f64>>(field2(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(8)>(%19), read<i32>(%21)))))), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @__builtin_copysignl(%55 <unnamed>: f80, %56 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %24 @cl(%25 x: f80, %26 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%57, read<f80>(%25), read<f80>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @__builtin_infl() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %60 @__builtin_nanl(%59 <unnamed>: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %29 @testl() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %30 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %31 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(384), const<u64>(48))));
// DEFAULT-NEXT:         let %32 r: f80 [storage=automatic];
// DEFAULT-NEXT:         for %63
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%30, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%30), read<i32>(%31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %68: i32 [synthetic] = read<i32>(%30);
// DEFAULT-NEXT:                 let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%30, read<i32>(%69));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f80>(%32, call<f80, signature=fn(f80, f80) -> f80>(%24, read<f80>(field0(deref(ptr_offset<ptr<const @type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<const @type3>, length=Some(8)>(%28), read<i32>(%30))))), read<f80>(field1(deref(ptr_offset<ptr<const @type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<const @type3>, length=Some(8)>(%28), read<i32>(%30)))))));
// DEFAULT-NEXT:                     call<f80, signature=fn(f80, f80) -> f80>(%24, read<f80>(field0(deref(ptr_offset<ptr<const @type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<const @type3>, length=Some(8)>(%28), read<i32>(%30))))), read<f80>(field1(deref(ptr_offset<ptr<const @type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<const @type3>, length=Some(8)>(%28), read<i32>(%30))))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%5, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f80>>(%32)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const f80>>(field2(deref(ptr_offset<ptr<const @type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<const @type3>, length=Some(8)>(%28), read<i32>(%30)))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%29);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
