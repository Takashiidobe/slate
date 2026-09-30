/* PR middle-end/84108 - incorrect -Wattributes warning for packed/aligned
   conflict on struct members
   { dg-do compile }
   { dg-options "-Wall -Wattributes" } */

#define ATTR(list) __attribute__ (list)
#define ASSERT(e) _Static_assert (e, #e)

/* GCC is inconsistent in how it treats attribute aligned between
   variable and member declarations.  Attribute aligned alone is
   sufficient to reduce a variable's alignment requirement but
   the attribute must be paired with packed to have the same
   effect on a member.  Worse, declaring a variable both aligned
   and packed emits a warning.  */

/* Avoid exercising this since emitting a warning for these given
   the requirement for members seems like a misfeature:
   int a ATTR ((packed, aligned (2)));   // -Wattributes
   int b ATTR ((aligned (2), packed));   // -Wattributes
   ASSERT (_Alignof (a) == 2);
   ASSERT (_Alignof (b) == 2);  */

int c ATTR ((aligned (2)));           // okay (reduces alignment)
ASSERT (_Alignof (c) == 2);

struct {
  int a ATTR ((packed, aligned (2)));   /* { dg-bogus "\\\[-Wattributes" "" { target { ! default_packed } } } */
  /* { dg-warning "attribute ignored" "" { target { default_packed } } .-1 } */
  int b ATTR ((aligned (2), packed));   /* { dg-bogus "\\\[-Wattributes" "" { target { ! default_packed } } } */
  /* { dg-warning "attribute ignored" "" { target { default_packed } } .-1 } */

  /* Avoid exercising this since the attribute has no effect yet
     there is no warning.
     int c ATTR ((aligned (2)));           // missing warning?  */
} s;

ASSERT (_Alignof (s.a) == 2);
ASSERT (_Alignof (s.b) == 2);

/* ASSERT (_Alignof (s.c) == 4); */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [align=2] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
