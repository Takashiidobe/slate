/* { dg-skip-if "requires io" { freestanding } }  */

/* This test tests complex conjugate and passing/returning of
   complex parameter.  */

#include <stdio.h>
#include <stdlib.h>

int e;

#define TEST(TYPE, FUNC)                                                       \
  __complex__ TYPE ctest_##FUNC(__complex__ TYPE x) {                          \
    __complex__ TYPE res;                                                      \
                                                                               \
    res = ~x;                                                                  \
                                                                               \
    return res;                                                                \
  }                                                                            \
                                                                               \
  void test_##FUNC(void) {                                                     \
    __complex__ TYPE res, x;                                                   \
                                                                               \
    x = 1.0 + 2.0i;                                                            \
                                                                               \
    res = ctest_##FUNC(x);                                                     \
                                                                               \
    if (res != 1.0 - 2.0i) {                                                   \
      printf("test_" #FUNC " failed\n");                                       \
      ++e;                                                                     \
    }                                                                          \
  }

TEST(float, float)
TEST(double, double)
TEST(long double, long_double)
TEST(int, int)
TEST(long int, long_int)

int main(void) {

  e = 0;

  test_float();
  test_double();
  test_long_double();
  test_int();
  test_long_int();

  if (e != 0)
    abort();

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
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([116, 101, 115, 116, 95, 102, 108, 111, 97, 116, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([116, 101, 115, 116, 95, 100, 111, 117, 98, 108, 101, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([116, 101, 115, 116, 95, 108, 111, 110, 103, 95, 100, 111, 117, 98, 108, 101, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([116, 101, 115, 116, 95, 105, 110, 116, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([116, 101, 115, 116, 95, 108, 111, 110, 103, 95, 105, 110, 116, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_ctest_float:[0-9]+]] @ctest_float(%[[VALUE_x:[0-9]+]] x: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res:[0-9]+]] res: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_res]], not<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_x]])));
// DEFAULT-NEXT:         return read<complex<f32>>(%[[VALUE_res]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_float:[0-9]+]] @test_float() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_res_2:[0-9]+]] res: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_x_2]], complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_res_2]], call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(native_c) -> native_c>(%[[VALUE_ctest_float]], read<complex<f32>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=observable>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%[[VALUE_res_2]])), sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str]])));
// DEFAULT-NEXT:                 let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ctest_double:[0-9]+]] @ctest_double(%[[VALUE_x_3:[0-9]+]] x: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res_3:[0-9]+]] res: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_res_3]], not<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_x_3]])));
// DEFAULT-NEXT:         return read<complex<f64>>(%[[VALUE_res_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_double:[0-9]+]] @test_double() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_res_4:[0-9]+]] res: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_x_4]], add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_res_4]], call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_ctest_double]], read<complex<f64>>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE_res_4]]), sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(20)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ctest_long_double:[0-9]+]] @ctest_long_double(%[[VALUE_x_5:[0-9]+]] x: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res_5:[0-9]+]] res: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_res_5]], not<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%[[VALUE_x_5]])));
// DEFAULT-NEXT:         return read<complex<f80>>(%[[VALUE_res_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_long_double:[0-9]+]] @test_long_double() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_res_6:[0-9]+]] res: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_6:[0-9]+]] x: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_x_6]], complex_convert<complex<f80>, reason=assign>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_res_6]], call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_ctest_long_double]], read<complex<f80>>(%[[VALUE_x_6]])));
// DEFAULT-NEXT:         if ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE_res_6]]), complex_convert<complex<f80>, reason=usual_arith>(sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ctest_int:[0-9]+]] @ctest_int(%[[VALUE_x_7:[0-9]+]] x: complex<i32>) -> complex<i32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res_7:[0-9]+]] res: complex<i32> [storage=automatic];
// DEFAULT-NEXT:         write<complex<i32>>(%[[VALUE_res_7]], not<complex<i32>, complex=true, overflow=ub>(read<complex<i32>>(%[[VALUE_x_7]])));
// DEFAULT-NEXT:         return read<complex<i32>>(%[[VALUE_res_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_int:[0-9]+]] @test_int() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_res_8:[0-9]+]] res: complex<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_8:[0-9]+]] x: complex<i32> [storage=automatic];
// DEFAULT-NEXT:         write<complex<i32>>(%[[VALUE_x_8]], complex_convert<complex<i32>, reason=assign, out_of_range=ub, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         write<complex<i32>>(%[[VALUE_res_8]], call<complex<i32>, signature=fn(complex<i32>) -> complex<i32>, abi=sysv64(native_c) -> native_c>(%[[VALUE_ctest_int]], read<complex<i32>>(%[[VALUE_x_8]])));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=observable>(complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<complex<i32>>(%[[VALUE_res_8]])), sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ctest_long_int:[0-9]+]] @ctest_long_int(%[[VALUE_x_9:[0-9]+]] x: complex<i64>) -> complex<i64> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res_9:[0-9]+]] res: complex<i64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<i64>>(%[[VALUE_res_9]], not<complex<i64>, complex=true, overflow=ub>(read<complex<i64>>(%[[VALUE_x_9]])));
// DEFAULT-NEXT:         return read<complex<i64>>(%[[VALUE_res_9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_long_int:[0-9]+]] @test_long_int() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_res_10:[0-9]+]] res: complex<i64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_10:[0-9]+]] x: complex<i64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<i64>>(%[[VALUE_x_10]], complex_convert<complex<i64>, reason=assign, out_of_range=ub, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         write<complex<i64>>(%[[VALUE_res_10]], call<complex<i64>, signature=fn(complex<i64>) -> complex<i64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_ctest_long_int]], read<complex<i64>>(%[[VALUE_x_10]])));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=observable>(complex_convert<complex<f64>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<complex<i64>>(%[[VALUE_res_10]])), sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_5]])));
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_e]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_float]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_double]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_long_double]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_int]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_long_int]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
