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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_Df:[0-9]+]] Df = struct {
// DEFAULT-NEXT:         field0 x: f32;
// DEFAULT-NEXT:         field1 y: f32;
// DEFAULT-NEXT:         field2 z: f32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_D:[0-9]+]] D = struct {
// DEFAULT-NEXT:         field0 x: f64;
// DEFAULT-NEXT:         field1 y: f64;
// DEFAULT-NEXT:         field2 z: f64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_Dl:[0-9]+]] Dl = struct {
// DEFAULT-NEXT:         field0 x: f80;
// DEFAULT-NEXT:         field1 y: f80;
// DEFAULT-NEXT:         field2 z: f80;
// DEFAULT-NEXT:     } [size=48, align=16, offsets=[0, 16, 32]];
// DEFAULT-NEXT:     global %[[VALUE_Tf:[0-9]+]] Tf: array<@type[[TYPE_Df]], 8> [storage=static] [const] [align=16] = aggregate<array<@type[[TYPE_Df]], 8>, zero_fill=false>(index0 = aggregate<@type[[TYPE_Df]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0))), index1 = aggregate<@type[[TYPE_Df]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0)))), index2 = aggregate<@type[[TYPE_Df]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0))), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(1.0)))), index3 = aggregate<@type[[TYPE_Df]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0)))), index4 = aggregate<@type[[TYPE_Df]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(2.0))), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0)))), index5 = aggregate<@type[[TYPE_Df]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), field2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0))), index6 = aggregate<@type[[TYPE_Df]], zero_fill=false>(field0 = const<f32>(inf), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))), field2 = neg<f32>(const<f32>(inf))), index7 = aggregate<@type[[TYPE_Df]], zero_fill=false>(field0 = neg<f32>(const<f32>(bits=0x7fc00000)), field1 = const<f32>(inf), field2 = const<f32>(bits=0x7fc00000))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_T:[0-9]+]] T: array<@type[[TYPE_D]], 8> [storage=static] [const] [align=16] = aggregate<array<@type[[TYPE_D]], 8>, zero_fill=false>(index0 = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = const<f64>(1.0), field1 = const<f64>(2.0), field2 = const<f64>(1.0)), index1 = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = const<f64>(1.0), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(1.0))), index2 = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = neg<f64>(const<f64>(1.0)), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(1.0))), index3 = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = const<f64>(0.0), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(0.0))), index4 = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = neg<f64>(const<f64>(0.0)), field1 = neg<f64>(const<f64>(2.0)), field2 = neg<f64>(const<f64>(0.0))), index5 = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = neg<f64>(const<f64>(0.0)), field1 = const<f64>(2.0), field2 = const<f64>(0.0)), index6 = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = const<f64>(inf), field1 = neg<f64>(const<f64>(0.0)), field2 = neg<f64>(const<f64>(inf))), index7 = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = neg<f64>(const<f64>(bits=0x7ff8000000000000)), field1 = const<f64>(inf), field2 = const<f64>(bits=0x7ff8000000000000))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_Tl:[0-9]+]] Tl: array<@type[[TYPE_Dl]], 8> [storage=static] [const] = aggregate<array<@type[[TYPE_Dl]], 8>, zero_fill=false>(index0 = aggregate<@type[[TYPE_Dl]], zero_fill=false>(field0 = float_widen<f80, reason=assign>(const<f64>(1.0)), field1 = float_widen<f80, reason=assign>(const<f64>(2.0)), field2 = float_widen<f80, reason=assign>(const<f64>(1.0))), index1 = aggregate<@type[[TYPE_Dl]], zero_fill=false>(field0 = float_widen<f80, reason=assign>(const<f64>(1.0)), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0)))), index2 = aggregate<@type[[TYPE_Dl]], zero_fill=false>(field0 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0))), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(1.0)))), index3 = aggregate<@type[[TYPE_Dl]], zero_fill=false>(field0 = float_widen<f80, reason=assign>(const<f64>(0.0)), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0)))), index4 = aggregate<@type[[TYPE_Dl]], zero_fill=false>(field0 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(2.0))), field2 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0)))), index5 = aggregate<@type[[TYPE_Dl]], zero_fill=false>(field0 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), field1 = float_widen<f80, reason=assign>(const<f64>(2.0)), field2 = float_widen<f80, reason=assign>(const<f64>(0.0))), index6 = aggregate<@type[[TYPE_Dl]], zero_fill=false>(field0 = const<f80>(+Inf), field1 = float_widen<f80, reason=assign>(neg<f64>(const<f64>(0.0))), field2 = neg<f80>(const<f80>(+Inf))), index7 = aggregate<@type[[TYPE_Dl]], zero_fill=false>(field0 = neg<f80>(const<f80>(bits=0x7fffc000000000000000)), field1 = const<f80>(+Inf), field2 = const<f80>(bits=0x7fffc000000000000000))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const void>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const void>, %[[VALUE___n:[0-9]+]] __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysignf:[0-9]+]] @__builtin_copysignf(%[[VALUE0:[0-9]+]] <unnamed>: f32, %[[VALUE1:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_cf:[0-9]+]] @cf(%[[VALUE_x:[0-9]+]] x: f32, %[[VALUE_y:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_copysignf]], read<f32>(%[[VALUE_x]]), read<f32>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testf:[0-9]+]] @testf() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(96), const<u64>(12))));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: f32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32>(%[[VALUE_r]], call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_cf]], read<f32>(field0(deref(ptr_offset<ptr<const @type[[TYPE_Df]]>, subtract=false, element=@type[[TYPE_Df]], overflow=ub>(array_decay<ptr<const @type[[TYPE_Df]]>, length=Some(8)>(%[[VALUE_Tf]]), read<i32>(%[[VALUE_i]]))))), read<f32>(field1(deref(ptr_offset<ptr<const @type[[TYPE_Df]]>, subtract=false, element=@type[[TYPE_Df]], overflow=ub>(array_decay<ptr<const @type[[TYPE_Df]]>, length=Some(8)>(%[[VALUE_Tf]]), read<i32>(%[[VALUE_i]])))))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f32>>(%[[VALUE_r]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const f32>>(field2(deref(ptr_offset<ptr<const @type[[TYPE_Df]]>, subtract=false, element=@type[[TYPE_Df]], overflow=ub>(array_decay<ptr<const @type[[TYPE_Df]]>, length=Some(8)>(%[[VALUE_Tf]]), read<i32>(%[[VALUE_i]])))))), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysign:[0-9]+]] @__builtin_copysign(%[[VALUE5:[0-9]+]] <unnamed>: f64, %[[VALUE6:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_c:[0-9]+]] @c(%[[VALUE_x_2:[0-9]+]] x: f64, %[[VALUE_y_2:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], read<f64>(%[[VALUE_x_2]]), read<f64>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(192), const<u64>(24))));
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: f64 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_n_2]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_r_2]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_c]], read<f64>(field0(deref(ptr_offset<ptr<const @type[[TYPE_D]]>, subtract=false, element=@type[[TYPE_D]], overflow=ub>(array_decay<ptr<const @type[[TYPE_D]]>, length=Some(8)>(%[[VALUE_T]]), read<i32>(%[[VALUE_i_2]]))))), read<f64>(field1(deref(ptr_offset<ptr<const @type[[TYPE_D]]>, subtract=false, element=@type[[TYPE_D]], overflow=ub>(array_decay<ptr<const @type[[TYPE_D]]>, length=Some(8)>(%[[VALUE_T]]), read<i32>(%[[VALUE_i_2]])))))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f64>>(%[[VALUE_r_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const f64>>(field2(deref(ptr_offset<ptr<const @type[[TYPE_D]]>, subtract=false, element=@type[[TYPE_D]], overflow=ub>(array_decay<ptr<const @type[[TYPE_D]]>, length=Some(8)>(%[[VALUE_T]]), read<i32>(%[[VALUE_i_2]])))))), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysignl:[0-9]+]] @__builtin_copysignl(%[[VALUE10:[0-9]+]] <unnamed>: f80, %[[VALUE11:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_cl:[0-9]+]] @cl(%[[VALUE_x_3:[0-9]+]] x: f80, %[[VALUE_y_3:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE___builtin_copysignl]], read<f80>(%[[VALUE_x_3]]), read<f80>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testl:[0-9]+]] @testl() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_3:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(384), const<u64>(48))));
// DEFAULT-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: f80 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), read<i32>(%[[VALUE_n_3]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f80>(%[[VALUE_r_3]], call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_cl]], read<f80>(field0(deref(ptr_offset<ptr<const @type[[TYPE_Dl]]>, subtract=false, element=@type[[TYPE_Dl]], overflow=ub>(array_decay<ptr<const @type[[TYPE_Dl]]>, length=Some(8)>(%[[VALUE_Tl]]), read<i32>(%[[VALUE_i_3]]))))), read<f80>(field1(deref(ptr_offset<ptr<const @type[[TYPE_Dl]]>, subtract=false, element=@type[[TYPE_Dl]], overflow=ub>(array_decay<ptr<const @type[[TYPE_Dl]]>, length=Some(8)>(%[[VALUE_Tl]]), read<i32>(%[[VALUE_i_3]])))))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<f80>>(%[[VALUE_r_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const f80>>(field2(deref(ptr_offset<ptr<const @type[[TYPE_Dl]]>, subtract=false, element=@type[[TYPE_Dl]], overflow=ub>(array_decay<ptr<const @type[[TYPE_Dl]]>, length=Some(8)>(%[[VALUE_Tl]]), read<i32>(%[[VALUE_i_3]])))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testf]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testl]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
