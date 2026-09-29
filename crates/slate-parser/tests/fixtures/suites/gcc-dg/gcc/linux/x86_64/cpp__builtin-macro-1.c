/* Origin PR preprocessor/64803

   This test ensures that the value the __LINE__ macro expands to is
   constant and corresponds to the line of the macro expansion point
   the function-like macro expansion it's part of.

   { dg-do run }
   { dg-options -no-integrated-cpp }  */

#include <assert.h>

#define C(a, b) a ## b
#define L(x) C(L, x)
#define M(a) int L(__LINE__) = __LINE__; assert(L(__LINE__) == __LINE__);

int
main()
{
  M(a
    );

  assert(L19 == 19);		/* 19 is the line number of the
				   macro expansion point of the
				   invocation of the M macro.  Please
				   adjust in case the layout of this
				   file changes.  */
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([76, 40, 95, 95, 76, 73, 78, 69, 95, 95, 41, 32, 61, 61, 32, 95, 95, 76, 73, 78, 69, 95, 95, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, {{[0-9]+}}> [storage=static] = code_units<array<i8, {{[0-9]+}}>>({{\[[0-9, ]+\]}}) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([109, 97, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([76, 49, 57, 32, 61, 61, 32, 49, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, {{[0-9]+}}> [storage=static] = code_units<array<i8, {{[0-9]+}}>>({{\[[0-9, ]+\]}}) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([109, 97, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___assert_fail:[0-9]+]] @__assert_fail(%[[VALUE___assertion:[0-9]+]] __assertion: ptr<const i8>, %[[VALUE___file:[0-9]+]] __file: ptr<const i8>, %[[VALUE___line:[0-9]+]] __line: u32, %[[VALUE___function:[0-9]+]] __function: ptr<const i8>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_L19:[0-9]+]] L19: i32 [storage=automatic] = const<i32>(19);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_L19]]), const<i32>(19))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(ptr<const i8>, ptr<const i8>, u32, ptr<const i8>) -> void>(%[[VALUE___assert_fail]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some({{[0-9]+}})>(%[[VALUE_str_2]])), reinterpret<u32, reason=arg, fits=always>(const<i32>(19)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_L19]]), const<i32>(19))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(ptr<const i8>, ptr<const i8>, u32, ptr<const i8>) -> void>(%[[VALUE___assert_fail]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_4]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some({{[0-9]+}})>(%[[VALUE_str_5]])), reinterpret<u32, reason=arg, fits=always>(const<i32>(22)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_6]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
