/* PR 30260  */
/* { dg-do link } */
/* { dg-options "-std=gnu11 -pedantic -O" } */
#include <limits.h>

void link_error (void);

enum A {
  A1 = 0, 
  A2 = A1 - 1
};
enum B {
  B1 = 0u, 
  B2 = B1 - 1 /* { dg-bogus "ISO C restricts enumerator values to range of 'int'" } */
};
int main(void)
{
  enum A a = -1;
  enum B b = -1;

  if (!(a < 0))
    link_error ();
  if (!(A2 < 0))
    link_error ();
  if (!(b < 0))
    link_error ();
  if (!(B2 < 0))
    link_error ();

  return 0;
}

enum E1 { e10 = INT_MAX, e11 }; /* { dg-warning "ISO C restricts enumerator values to range of 'int' before C23" } */
enum E2 { e20 = (unsigned) INT_MAX, e21 }; /* { dg-warning "ISO C restricts enumerator values to range of 'int' before C23" } */

// SLATE-FILECHECK-STD DEFAULT gnu11
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
// DEFAULT-NEXT:     type @type0 A = enum : i32 {
// DEFAULT-NEXT:         %0 A1 = const<i32>(0);
// DEFAULT-NEXT:         %1 A2 = const<i32>(-1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 B = enum : i32 {
// DEFAULT-NEXT:         %0 B1 = const<i32>(0);
// DEFAULT-NEXT:         %1 B2 = const<i32>(-1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type2 E1 = enum : u32 {
// DEFAULT-NEXT:         %0 e10 = const<u32>(2147483647);
// DEFAULT-NEXT:         %1 e11 = const<u32>(2147483648);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type3 E2 = enum : u32 {
// DEFAULT-NEXT:         %0 e20 = const<u32>(2147483647);
// DEFAULT-NEXT:         %1 e21 = const<u32>(2147483648);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %0 @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 a: @type0 [storage=automatic] = int_to_enum<@type0, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         let %9 b: @type1 [storage=automatic] = int_to_enum<@type1, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         if not<bool>(lt<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%8)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if not<bool>(lt<i32>(const<i32>(-1), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if not<bool>(lt<i32>(enum_to_int<i32, reason=promotion>(read<@type1>(%9)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if not<bool>(lt<i32>(const<i32>(-1), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
