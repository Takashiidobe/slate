/* Test for constant expressions: details of what is a null pointer
   constant.
*/
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999" } */
/* Note: not using -pedantic since the -std option alone should be enough
   to give the correct behavior to conforming programs.  If -pedantic is
   needed to make (say) (0, 0) not be a constant expression, this is a
   bug.
*/

int *a;
int b;
long *c;

#if defined(_LP64)
#define ZERO 0L
#elif defined(_WIN64)
#define ZERO 0LL
#else
#define ZERO 0
#endif

/* Assertion that n is a null pointer constant: so the conditional expression
   has type 'int *' instead of 'void *'.
*/
#define ASSERT_NPC(n)	(b = *(1 ? a : (n)))
/* Assertion that n is not a null pointer constant: so the conditional
   expressions has type 'void *' instead of 'int *'.
*/
#define ASSERT_NOT_NPC(n)	(c = (1 ? a : (n)))

void
foo (void)
{
  ASSERT_NPC (0);
  ASSERT_NPC ((void *)0);
  ASSERT_NOT_NPC ((void *)(void *)0); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
  ASSERT_NOT_NPC ((void *)(char *)0); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
  ASSERT_NOT_NPC ((void *)(0, ZERO)); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
  ASSERT_NOT_NPC ((void *)(&"Foobar"[0] - &"Foobar"[0])); /* { dg-bogus "incompatible" "bogus null pointer constant" } */
  /* This last one is a null pointer constant in C99 only.  */
  ASSERT_NPC ((void *)(1 ? 0 : (0, 0)));
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<i64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([70, 111, 111, 98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([70, 111, 111, 98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%[[VALUE_a]]), null<ptr<i32>>))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%[[VALUE_a]]), null<ptr<i32>>))));
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_c]], pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%[[VALUE_a]])), null<ptr<void>>)));
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_c]], pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%[[VALUE_a]])), pointer_cast<ptr<void>, reason=explicit>(null<ptr<i8>>))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE0]], pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE0]], int_to_ptr<ptr<void>, reason=explicit>(const<i64>(0)));
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_c]], pointer_cast<ptr<i64>, reason=assign>(read<ptr<void>>(%[[VALUE0]])));
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_c]], pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%[[VALUE_a]])), int_to_ptr<ptr<void>, reason=explicit>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]]), const<i32>(0)))), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_2]]), const<i32>(0)))))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%[[VALUE_a]]), null<ptr<i32>>))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
