/* Test for compound literals: in C99 only.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors" } */

struct s { int a; int b; };
union u { int c; int d; };

void
foo (void)
{
  (int) { 1 }; /* { dg-bogus "warning" "warning in place of error" } */
  /* { dg-error "compound literal" "scalar" { target *-*-* } .-1 } */
  (struct s) { 1, 2 }; /* { dg-bogus "warning" "warning in place of error" } */
  /* { dg-error "compound literal" "struct" { target *-*-* } .-1 } */
  (union u) { 1 }; /* { dg-bogus "warning" "warning in place of error" } */
  /* { dg-error "compound literal" "union" { target *-*-* } .-1 } */
  (int [1]) { 1 }; /* { dg-bogus "warning" "warning in place of error" } */
  /* { dg-error "compound literal" "array" { target *-*-* } .-1 } */
}

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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:         field1 d: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<i32>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = const<i32>(1));
// DEFAULT-NEXT:         read<@type[[TYPE_s]]>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)));
// DEFAULT-NEXT:         read<@type[[TYPE_u]]>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_u]], zero_fill=false>(field0 = const<i32>(1)));
// DEFAULT-NEXT:         array_decay<ptr<i32>, length=Some(1)>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
