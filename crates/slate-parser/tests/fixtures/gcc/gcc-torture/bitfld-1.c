/* Copyright 2002 Free Software Foundation, Inc.

   Tests correct signedness of operations on bitfields; in particular
   that integer promotions are done correctly, including the case when
   casts are present.

   The C front end was eliding the cast of an unsigned bitfield to
   unsigned as a no-op, when in fact it forces a conversion to a
   full-width unsigned int. (At the time of writing, the C++ front end
   has a different bug; it erroneously promotes the uncast unsigned
   bitfield to an unsigned int).

   Source: Neil Booth, 25 Jan 2002, based on PR 3325 (and 3326, which
   is a different manifestation of the same bug).
*/

extern void abort();

int main(int argc, char *argv[]) {
  struct x {
    signed int   i : 7;
    unsigned int u : 7;
  } bit;

  unsigned int u;
  int          i;
  unsigned int unsigned_result = -13U % 61;
  int          signed_result   = -13 % 61;

  bit.u = 61, u = 61;
  bit.i = -13, i = -13;

  if (i % u != unsigned_result)
    abort();
  if (i % (unsigned int)u != unsigned_result)
    abort();

  /* Somewhat counter-intuitively, bit.u is promoted to an int, making
     the operands and result an int.  */
  if (i % bit.u != signed_result)
    abort();

  if (bit.i % bit.u != signed_result)
    abort();

  /* But with a cast to unsigned int, the unsigned int is promoted to
     itself as a no-op, and the operands and result are unsigned.  */
  if (i % (unsigned int)bit.u != unsigned_result)
    abort();

  if (bit.i % (unsigned int)bit.u != unsigned_result)
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
// DEFAULT-NEXT:     type @type0 x = struct {
// DEFAULT-NEXT:         field0 i: i32 : 7;
// DEFAULT-NEXT:         field1 u: u32 : 7;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(7)], bit_units=[(0, 2)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @main(%2 argc: i32, %3 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 bit: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %6 u: u32 [storage=automatic];
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 unsigned_result: u32 [storage=automatic] = rem<u32, by_zero=ub>(neg<u32, overflow=wrap>(const<u32>(13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(61)));
// DEFAULT-NEXT:         let %9 signed_result: i32 [storage=automatic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(neg<i32, overflow=ub>(const<i32>(13)), const<i32>(61));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..2, bits=7..14>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(61)));
// DEFAULT-NEXT:         write<u32>(%6, reinterpret<u32, reason=assign, fits=always>(const<i32>(61)));
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..2, bits=0..7>(%5), neg<i32, overflow=ub>(const<i32>(13)));
// DEFAULT-NEXT:         write<i32>(%7, neg<i32, overflow=ub>(const<i32>(13)));
// DEFAULT-NEXT:         if ne<u32>(rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%7)), read<u32>(%6)), read<u32>(%8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%7)), read<u32>(%6)), read<u32>(%8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%7), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..2, bits=7..14>(%5)))), read<i32>(%9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(bitfield0<unit=0, bytes=0..2, bits=0..7>(%5)), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..2, bits=7..14>(%5)))), read<i32>(%9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%7)), reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..2, bits=7..14>(%5))))), read<u32>(%8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(bitfield0<unit=0, bytes=0..2, bits=0..7>(%5))), reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..2, bits=7..14>(%5))))), read<u32>(%8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
