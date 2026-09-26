/*
 This test checks promotion of bitfields.  Bitfields should be promoted
 very much like chars and shorts:

 Bitfields (signed or unsigned) should be promoted to signed int if their
 value will fit in a signed int, otherwise to an unsigned int if their
 value will fit in an unsigned int, otherwise we don't promote them (ANSI/ISO
 does not specify the behavior of bitfields larger than an unsigned int).

 We test the behavior by subtracting two from the promoted value: this will
 result in a negitive value for signed types, a positive value for unsigned
 types.  This test (of course) assumes that the compiler is correctly
 implementing signed and unsigned arithmetic.
 */

void abort(void);
void exit(int);

struct X {
  unsigned int       u3    : 3;
  signed long int    s31   : 31;
  signed long int    s32   : 32;
  unsigned long int  u31   : 31;
  unsigned long int  u32   : 32;
  unsigned long long ull3  : 3;
  unsigned long long ull35 : 35;
  unsigned           u15   : 15;
};

struct X x;

int main(void) {
  if ((x.u3 - 2) >= 0) /* promoted value should be signed */
    abort();

  if ((x.s31 - 2) >= 0) /* promoted value should be signed */
    abort();

  if ((x.s32 - 2) >= 0) /* promoted value should be signed */
    abort();

  if ((x.u15 - 2) >= 0) /* promoted value should be signed */
    abort();

  /* Conditionalize check on whether integers are 4 bytes or larger, i.e.
     larger than a 31 bit bitfield.  */
  if (sizeof(int) >= 4) {
    if ((x.u31 - 2) >= 0) /* promoted value should be signed */
      abort();
  } else {
    if ((x.u31 - 2) < 0) /* promoted value should be UNsigned */
      abort();
  }

  if ((x.u32 - 2) < 0) /* promoted value should be UNsigned */
    abort();

  if ((x.ull3 - 2) >= 0) /* promoted value should be signed */
    abort();

  if ((x.ull35 - 2) < 0) /* promoted value should be UNsigned */
    abort();

  exit(0);
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
// DEFAULT-NEXT:     type @type0 X = struct {
// DEFAULT-NEXT:         field0 u3: u32 : 3;
// DEFAULT-NEXT:         field1 s31: i64 : 31;
// DEFAULT-NEXT:         field2 s32: i64 : 32;
// DEFAULT-NEXT:         field3 u31: u64 : 31;
// DEFAULT-NEXT:         field4 u32: u64 : 32;
// DEFAULT-NEXT:         field5 ull3: u64 : 3;
// DEFAULT-NEXT:         field6 ull35: u64 : 35;
// DEFAULT-NEXT:         field7 u15: u32 : 15;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 0, 8, 12, 16, 20, 24, 28], bit_offsets=[Some(0), Some(3), Some(64), Some(96), Some(128), Some(160), Some(192), Some(227)], bit_units=[(0, 5), (8, 8), (16, 5), (24, 7)], field_units=[Some(0), Some(0), Some(1), Some(1), Some(2), Some(2), Some(3), Some(3)]];
// DEFAULT-NEXT:     global %3 x: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%5 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ge<i32>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..5, bits=0..3>(%3))), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ge<i32>(sub<i32, overflow=ub>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..5, bits=3..34>(%3))), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ge<i64>(sub<i64, overflow=ub>(read<i64>(bitfield2<unit=1, bytes=8..16, bits=0..32>(%3)), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ge<i32>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=3, bytes=24..31, bits=35..50>(%3))), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ge<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ge<i32>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=1, bytes=8..16, bits=32..63>(%3)))), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if lt<i32>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=1, bytes=8..16, bits=32..63>(%3)))), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if lt<u64>(sub<u64, overflow=wrap>(read<u64>(bitfield4<unit=2, bytes=16..21, bits=0..32>(%3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ge<i32>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield5<unit=2, bytes=16..21, bits=32..35>(%3)))), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if lt<u64>(sub<u64, overflow=wrap>(read<u64>(bitfield6<unit=3, bytes=24..31, bits=0..35>(%3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
