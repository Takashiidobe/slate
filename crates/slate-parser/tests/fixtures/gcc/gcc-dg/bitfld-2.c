/* Copyright (C) 2002 Free Software Foundation, Inc.

   Tests we warn about overly-large assignments to bitfields.

   Source: Neil Booth, 28 Jan 2002.
*/

struct bf
{
  unsigned int a: 2;
  int b: 2;
};

struct bf p = {4, 0};		/* { dg-warning "unsigned conversion from .int. to 'unsigned char:2' changes value from .4. to .0." } */
struct bf q = {0, 2};		/* { dg-warning "overflow in conversion from .int. to .signed char:2. changes value from .2. to .-2." } */
struct bf r = {3, -2};		/* { dg-bogus "(trunc|overflow)" } */

void foo ()
{
  p.a = 4, p.b = 0;		/* { dg-warning "unsigned conversion from .int. to .unsigned char:2. changes value from .4. to .0." } */
  q.a = 0, q.b = 2;		/* { dg-warning "overflow in conversion from .int. to .signed char:2. changes value from .2. to .-2." } */
  r.a = 3, r.b = -2;		/* { dg-bogus "(trunc|overflow)" } */
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 bf = struct {
// DEFAULT-NEXT:         field0 a: u32 : 2;
// DEFAULT-NEXT:         field1 b: i32 : 2;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(2)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %1 p: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(4)), field1 = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %2 q: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field1 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %3 r: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(3)), field1 = neg<i32, overflow=ub>(const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..1, bits=0..2>(%1), reinterpret<u32, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..1, bits=2..4>(%1), const<i32>(0));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..1, bits=0..2>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..1, bits=2..4>(%2), const<i32>(2));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..1, bits=0..2>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..1, bits=2..4>(%3), neg<i32, overflow=ub>(const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
