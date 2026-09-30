/* { dg-do compile } */
/* Remove pedantic.  Allow the GCC extension to use char for bitfields.  */
/* { dg-options "" } */
/* { dg-options "-mno-ms-bitfields -Wno-packed-bitfield-compat" { target { i?86-*-mingw* x86_64-*-mingw* } } } */

struct t
{
  char a:4;
  char b:8;
  char c:4;
} __attribute__ ((packed)); /* { dg-message "note: offset of packed bit-field 'b' has changed in GCC 4.4" "" { target pcc_bitfield_type_matters } } */

int assrt[sizeof (struct t) == 2 ? 1 : -1];

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
// DEFAULT-NEXT:     type @type[[TYPE_t:[0-9]+]] t = struct {
// DEFAULT-NEXT:         field0 a: i8 : 4;
// DEFAULT-NEXT:         field1 b: i8 : 8;
// DEFAULT-NEXT:         field2 c: i8 : 4;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(4), Some(12)], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_assrt:[0-9]+]] assrt: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
