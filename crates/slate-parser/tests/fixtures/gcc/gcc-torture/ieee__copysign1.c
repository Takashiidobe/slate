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
// DEFAULT-NEXT:     global %34 .str34: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 Tf: array<@type1, 8> [storage=static] [const] = aggregate<array<@type1, 8>, zero_fill=false>(index0 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0))), index1 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0)))), index2 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0))), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0)))), index3 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0)))), index4 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0)))), index5 = aggregate<@type1, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0))), index6 = aggregate<@type1, zero_fill=false>(field0 = call<f32, signature=fn() -> f32>(__builtin_inff), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), field2 = neg<f32>(call<f32, signature=fn() -> f32>(__builtin_inff))), index7 = aggregate<@type1, zero_fill=false>(field0 = neg<f32>(call<f32, signature=fn(ptr<const i8>) -> f32>(__builtin_nanf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%34)))), field1 = call<f32, signature=fn() -> f32>(__builtin_inff), field2 = call<f32, signature=fn(ptr<const i8>) -> f32>(__builtin_nanf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%35))))) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 T: array<@type2, 8> [storage=static] [const] = aggregate<array<@type2, 8>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field0 = const<f64>(1.0), field1 = const<f64>(2.0), field2 = const<f64>(1.0)), index1 = aggregate<@type2, zero_fill=false>(field0 = const<f64>(1.0), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(1.0))), index2 = aggregate<@type2, zero_fill=false>(field0 = neg<f64>(const<f64>(1.0)), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(1.0))), index3 = aggregate<@type2, zero_fill=false>(field0 = const<f64>(0.0), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(0.0))), index4 = aggregate<@type2, zero_fill=false>(field0 = neg<f64>(const<f64>(0.0)), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(0.0))), index5 = aggregate<@type2, zero_fill=false>(field0 = neg<f64>(const<f64>(0.0)), field1 = const<f64>(2.0), field2 = const<f64>(0.0)), index6 = aggregate<@type2, zero_fill=false>(field0 = call<f64, signature=fn() -> f64>(__builtin_inf), field1 = neg<f64>(const<f64>(0.0)), field2 = neg<f64>(call<f64, signature=fn() -> f64>(__builtin_inf))), index7 = aggregate<@type2, zero_fill=false>(field0 = neg<f64>(call<f64, signature=fn(ptr<const i8>) -> f64>(__builtin_nan, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%37)))), field1 = call<f64, signature=fn() -> f64>(__builtin_inf), field2 = call<f64, signature=fn(ptr<const i8>) -> f64>(__builtin_nan, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%38))))) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 Tl: array<@type3, 8> [storage=static] [const] = aggregate<array<@type3, 8>, zero_fill=false>(index0 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(const<f64>(1.0)), field1 = float_widen<f80, reason=assign>(const<f64>(2.0)), field2 = float_widen<f80, reason=assign>(const<f64>(1.0))), index1 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(const<f64>(1.0)), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0)))), index2 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0))), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0)))), index3 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(const<f64>(0.0)), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0)))), index4 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0)))), index5 = aggregate<@type3, zero_fill=false>(field0 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), field1 = float_widen<f80, reason=assign>(const<f64>(2.0)), field2 = float_widen<f80, reason=assign>(const<f64>(0.0))), index6 = aggregate<@type3, zero_fill=false>(field0 = call<f80, signature=fn() -> f80>(__builtin_infl), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), field2 = neg<f80>(call<f80, signature=fn() -> f80>(__builtin_infl))), index7 = aggregate<@type3, zero_fill=false>(field0 = neg<f80>(call<f80, signature=fn(ptr<const i8>) -> f80>(__builtin_nanl, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%40)))), field1 = call<f80, signature=fn() -> f80>(__builtin_infl), field2 = call<f80, signature=fn(ptr<const i8>) -> f80>(__builtin_nanl, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%41))))) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @memcmp(%31 __s1: ptr<const void>, %32 __s2: ptr<const void>, %33 __n: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @cf(%4 x: f32, %5 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, read<f32>(%4), read<f32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @testf() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(96), const<u64>(12))));
// DEFAULT-NEXT:         let %11 r: f32 [storage=automatic];
// DEFAULT-NEXT:         for %36
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), read<i32>(%10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%44));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32>(%11, call<f32, signature=fn(f32, f32) -> f32>(%3, read<f32>(field0(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(8)>(%7), read<i32>(%9))))), read<f32>(field1(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(8)>(%7), read<i32>(%9)))))));
// DEFAULT-NEXT:                     call<f32, signature=fn(f32, f32) -> f32>(%3, read<f32>(field0(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(8)>(%7), read<i32>(%9))))), read<f32>(field1(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(8)>(%7), read<i32>(%9))))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f32>>(%11)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const f32>>(field2(deref(ptr_offset<ptr<const @type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<const @type1>, length=Some(8)>(%7), read<i32>(%9)))))), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @c(%13 x: f64, %14 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(__builtin_copysign, read<f64>(%13), read<f64>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %18 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %19 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(192), const<u64>(24))));
// DEFAULT-NEXT:         let %20 r: f64 [storage=automatic];
// DEFAULT-NEXT:         for %39
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%18, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%18), read<i32>(%19))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %45: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                 let %46: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%45), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%18, read<i32>(%46));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(%20, call<f64, signature=fn(f64, f64) -> f64>(%12, read<f64>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(8)>(%16), read<i32>(%18))))), read<f64>(field1(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(8)>(%16), read<i32>(%18)))))));
// DEFAULT-NEXT:                     call<f64, signature=fn(f64, f64) -> f64>(%12, read<f64>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(8)>(%16), read<i32>(%18))))), read<f64>(field1(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(8)>(%16), read<i32>(%18))))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f64>>(%20)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const f64>>(field2(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(8)>(%16), read<i32>(%18)))))), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @cl(%22 x: f80, %23 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(__builtin_copysignl, read<f80>(%22), read<f80>(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @testl() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %28 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(384), const<u64>(48))));
// DEFAULT-NEXT:         let %29 r: f80 [storage=automatic];
// DEFAULT-NEXT:         for %42
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%27, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%27), read<i32>(%28))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %47: i32 [synthetic] = read<i32>(%27);
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%47), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%27, read<i32>(%48));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f80>(%29, call<f80, signature=fn(f80, f80) -> f80>(%21, read<f80>(field0(deref(ptr_offset<ptr<const @type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<const @type3>, length=Some(8)>(%25), read<i32>(%27))))), read<f80>(field1(deref(ptr_offset<ptr<const @type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<const @type3>, length=Some(8)>(%25), read<i32>(%27)))))));
// DEFAULT-NEXT:                     call<f80, signature=fn(f80, f80) -> f80>(%21, read<f80>(field0(deref(ptr_offset<ptr<const @type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<const @type3>, length=Some(8)>(%25), read<i32>(%27))))), read<f80>(field1(deref(ptr_offset<ptr<const @type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<const @type3>, length=Some(8)>(%25), read<i32>(%27))))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f80>>(%29)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const f80>>(field2(deref(ptr_offset<ptr<const @type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<const @type3>, length=Some(8)>(%25), read<i32>(%27)))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
