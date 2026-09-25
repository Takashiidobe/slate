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
// DEFAULT-NEXT:     global %2 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([116, 101, 115, 116, 95, 102, 108, 111, 97, 116, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([116, 101, 115, 116, 95, 100, 111, 117, 98, 108, 101, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([116, 101, 115, 116, 95, 108, 111, 110, 103, 95, 100, 111, 117, 98, 108, 101, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([116, 101, 115, 116, 95, 105, 110, 116, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([116, 101, 115, 116, 95, 108, 111, 110, 103, 95, 105, 110, 116, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%34 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @ctest_float(%4 x: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(coerce<pair<f32>>) -> coerce<pair<f32>>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 res: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f32>>(%5, not<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore>(read<complex<f32>>(%4)));
// DEFAULT-NEXT:         return read<complex<f32>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_float() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 res: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         let %8 x: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f32>>(%8, complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=ignore>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         write<complex<f32>>(%7, call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(coerce<pair<f32>>) -> coerce<pair<f32>>>(%3, read<complex<f32>>(%8)));
// DEFAULT-NEXT:         call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(coerce<pair<f32>>) -> coerce<pair<f32>>>(%3, read<complex<f32>>(%8));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=ignore>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%7)), sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%35)));
// DEFAULT-NEXT:                 let %40: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%41));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @ctest_double(%10 x: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 res: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f64>>(%11, not<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(read<complex<f64>>(%10)));
// DEFAULT-NEXT:         return read<complex<f64>>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_double() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 res: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %14 x: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f64>>(%14, add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))));
// DEFAULT-NEXT:         write<complex<f64>>(%13, call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>>(%9, read<complex<f64>>(%14)));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>>(%9, read<complex<f64>>(%14));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%13), sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(20)>(%36)));
// DEFAULT-NEXT:                 let %42: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%43));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @ctest_long_double(%16 x: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 res: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f80>>(%17, not<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore>(read<complex<f80>>(%16)));
// DEFAULT-NEXT:         return read<complex<f80>>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_long_double() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 res: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         let %20 x: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f80>>(%20, complex_convert<complex<f80>, reason=assign>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         write<complex<f80>>(%19, call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%15, read<complex<f80>>(%20)));
// DEFAULT-NEXT:         call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%15, read<complex<f80>>(%20));
// DEFAULT-NEXT:         if ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%19), complex_convert<complex<f80>, reason=usual_arith>(sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%37)));
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%45));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @ctest_int(%22 x: complex<i32>) -> complex<i32> [linkage=external] [abi=sysv64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 res: complex<i32> [storage=automatic];
// DEFAULT-NEXT:         write<complex<i32>>(%23, not<complex<i32>, complex=true, overflow=ub>(read<complex<i32>>(%22)));
// DEFAULT-NEXT:         return read<complex<i32>>(%23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @test_int() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %25 res: complex<i32> [storage=automatic];
// DEFAULT-NEXT:         let %26 x: complex<i32> [storage=automatic];
// DEFAULT-NEXT:         write<complex<i32>>(%26, complex_convert<complex<i32>, reason=assign, out_of_range=ub, exceptions=ignore>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         write<complex<i32>>(%25, call<complex<i32>, signature=fn(complex<i32>) -> complex<i32>, abi=sysv64(coerce<i64>) -> coerce<i64>>(%21, read<complex<i32>>(%26)));
// DEFAULT-NEXT:         call<complex<i32>, signature=fn(complex<i32>) -> complex<i32>, abi=sysv64(coerce<i64>) -> coerce<i64>>(%21, read<complex<i32>>(%26));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=ignore>(complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<complex<i32>>(%25)), sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%38)));
// DEFAULT-NEXT:                 let %46: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%47));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @ctest_long_int(%28 x: complex<i64>) -> complex<i64> [linkage=external] [abi=sysv64(coerce<i64, i64>) -> coerce<i64, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %29 res: complex<i64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<i64>>(%29, not<complex<i64>, complex=true, overflow=ub>(read<complex<i64>>(%28)));
// DEFAULT-NEXT:         return read<complex<i64>>(%29);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @test_long_int() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %31 res: complex<i64> [storage=automatic];
// DEFAULT-NEXT:         let %32 x: complex<i64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<i64>>(%32, complex_convert<complex<i64>, reason=assign, out_of_range=ub, exceptions=ignore>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         write<complex<i64>>(%31, call<complex<i64>, signature=fn(complex<i64>) -> complex<i64>, abi=sysv64(coerce<i64, i64>) -> coerce<i64, i64>>(%27, read<complex<i64>>(%32)));
// DEFAULT-NEXT:         call<complex<i64>, signature=fn(complex<i64>) -> complex<i64>, abi=sysv64(coerce<i64, i64>) -> coerce<i64, i64>>(%27, read<complex<i64>>(%32));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=ignore>(complex_convert<complex<f64>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<complex<i64>>(%31)), sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%39)));
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%49));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%24);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%30);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
