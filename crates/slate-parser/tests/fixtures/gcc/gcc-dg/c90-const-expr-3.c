/* Test for constant expressions: broken optimization with const variables.  */
/* Reference: ISO 9989:1990 6.5.15 */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -O2" } */
/* Note: not using -pedantic since the -std option alone should be enough
   to give the correct behavior to conforming programs.  */

static const int ZERO = 0;
static const double DZERO = 0;

int *a;
int b;
long *c;

/* Assertion that n is a constant zero: so the conditional expression
   has type 'int *' instead of 'void *'.
*/
#define ASSERT_NPC(n)	(b = *(1 ? a : (n)))
/* Assertion that n is not a constant zero: so the conditional
   expressions has type 'void *' instead of 'int *'.
*/
#define ASSERT_NOT_NPC(n)	(c = (1 ? a : (void *)(__SIZE_TYPE__)(n)))

void
foo (void)
{
  ASSERT_NPC (0);
  ASSERT_NOT_NPC (ZERO);
  ASSERT_NPC (0 + 0);
  ASSERT_NOT_NPC (ZERO + 0); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
  ASSERT_NOT_NPC (ZERO + ZERO); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
  ASSERT_NPC (+0);
  ASSERT_NOT_NPC (+ZERO); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
  ASSERT_NPC (-0);
  ASSERT_NOT_NPC (-ZERO); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
  ASSERT_NPC ((char) 0);
  ASSERT_NOT_NPC ((char) ZERO);
  ASSERT_NPC ((int) 0);
  ASSERT_NOT_NPC ((int) ZERO);
  ASSERT_NPC ((int) 0.0);
  ASSERT_NOT_NPC ((int) DZERO);
  ASSERT_NOT_NPC ((int) +0.0); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
  ASSERT_NOT_NPC ((int) (0.0+0.0)); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
  ASSERT_NOT_NPC ((int) (double)0.0); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT iso9899:1990
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
// DEFAULT-NEXT:     global %0 ZERO: i32 [storage=static] [const] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %1 DZERO: f64 [storage=static] [const] = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %2 a: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 c: ptr<i64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%2), null<ptr<i32>>))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%0)))))));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%2), null<ptr<i32>>))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(add<i32, overflow=ub>(read<i32>(%0), const<i32>(0))))))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(add<i32, overflow=ub>(read<i32>(%0), read<i32>(%0))))))));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%2), null<ptr<i32>>))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%0)))))));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%2), null<ptr<i32>>))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(read<i32>(%0))))))));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%2), null<ptr<i32>>))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%0))))))));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%2), null<ptr<i32>>))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%0)))))));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%2), int_to_ptr<ptr<i32>, reason=usual_arith>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(0.0)))))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f64>(%1))))))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(0.0))))))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=off>(const<f64>(0.0), const<f64>(0.0)))))))));
// DEFAULT-NEXT:         write<ptr<i64>>(%4, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%2)), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(0.0))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
