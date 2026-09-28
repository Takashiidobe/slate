/* PR c/80731 - poor -Woverflow warnings, missing detail
   { dg-do compile }
   { dg-options "-Wconversion -Woverflow -Wno-override-init -std=c99" }
   { dg-require-effective-target int32plus } */

#include <limits.h>

struct Types
{
  signed char sc;
  unsigned char uc;
  signed short ss;
  unsigned short us;
  signed int si;
  unsigned int ui;
  signed long sl;
  unsigned long ul;
  signed long long sll;
  unsigned long long ull;
};

const struct Types t1 = {
  /* According to 6.3.1.3 of C11:
     -2-  Otherwise, if the new type is unsigned, the value is converted
	  by repeatedly adding or subtracting one more than the maximum
	  value that can be represented in the new type until the value
	  is in the range of the new type.

     These conversions are diagnosed by -Wsign-conversion and -Wconversion,
     respectively, by mentioning "unsigned conversion" if the conversion
     results in sign change, and just "conversion" otherwise, as follows:  */

  .uc = SCHAR_MIN,          /* { dg-warning "unsigned conversion from .int. to .unsigned char. changes value from .-128. to .128." } */
  .uc = -1,                 /* { dg-warning "unsigned conversion from .int. to .unsigned char. changes value from .-1. to .255." } */

  .uc = UCHAR_MAX + 1,      /* { dg-warning "conversion from 'int' to 'unsigned char' changes value from .256. to .0." } */
  .uc = UCHAR_MAX * 2,      /* { dg-warning "conversion from 'int' to 'unsigned char' changes value from .510. to .254." } */

  /* According to 6.3.1.3 of C11:
     -3-  Otherwise, the new type is signed and the value cannot be
	  represented in it; either the result is implementation-defined
	  or an implementation-defined signal is raised.

     In GCC such conversions wrap and are diagnosed by mentioning "overflow"
     if the absolute value of the operand is in excess of the maximum of
     the destination of type, and "conversion" otherwise, as follows:  */

  .sc = SCHAR_MAX + 1,      /* { dg-warning "conversion from .int. to .signed char. changes value from .128. to .-128." } */
  .sc = SCHAR_MAX + 2,      /* { dg-warning "conversion from .int. to .signed char. changes value from .129. to .-127." } */
  .sc = SCHAR_MAX * 2,      /* { dg-warning "conversion from .int. to .signed char. changes value from .254. to .-2." } */
  .sc = SCHAR_MAX * 2 + 3,  /* { dg-warning "conversion from .int. to .signed char. changes value from .257. to .1." } */
  .sc = SCHAR_MAX * 3 + 3,  /* { dg-warning "conversion from .int. to .signed char. changes value from .384. to .-128." } */


  .ss = SHRT_MAX + 1,       /* { dg-warning "conversion from 'int' to 'short int' changes value from .32768. to .-32768." } */
  .us = USHRT_MAX + 1,      /* { dg-warning "unsigned conversion from .int. to .short unsigned int. changes value from .65536. to .0." } */

  .si = INT_MAX + 1LU,      /* { dg-warning "signed conversion from 'long unsigned int. to 'int' changes value from .2147483648. to .-2147483648." } */
  .ui = UINT_MAX + 1L,      /* { dg-warning "signed conversion from .long int. to .unsigned int. changes value from .4294967296. to .0." "lp64" { target lp64 } } */
  .ui = UINT_MAX + 1LU,     /* { dg-warning "conversion from .long unsigned int. to .unsigned int. changes value from .4294967296. to .0." "lp64" { target lp64 } } */

  .sl = LONG_MAX + 1LU,     /* { dg-warning "signed conversion from .long unsigned int. to .long int. changes value from .9223372036854775808. to .-9223372036854775808." "lp64" { target lp64 } } */
  /* { dg-warning "signed conversion from .long unsigned int. to .long int. changes value from .2147483648. to .-2147483648." "ilp32" { target ilp32 } .-1 } */
  /* { dg-warning "signed conversion from .long unsigned int. to .long int. changes value from .2147483648. to .-2147483648." "llp64" { target llp64 } .-2 } */
  .ul = ULONG_MAX + 1LU     /* there should be some warning here */
};

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     type @type0 Types = struct {
// DEFAULT-NEXT:         field0 sc: i8;
// DEFAULT-NEXT:         field1 uc: u8;
// DEFAULT-NEXT:         field2 ss: i16;
// DEFAULT-NEXT:         field3 us: u16;
// DEFAULT-NEXT:         field4 si: i32;
// DEFAULT-NEXT:         field5 ui: u32;
// DEFAULT-NEXT:         field6 sl: i64;
// DEFAULT-NEXT:         field7 ul: u64;
// DEFAULT-NEXT:         field8 sll: i64;
// DEFAULT-NEXT:         field9 ull: u64;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 1, 2, 4, 8, 12, 16, 24, 32, 40]];
// DEFAULT-NEXT:     global %1 t1: @type0 [storage=static] [const] = aggregate<@type0, zero_fill=true>(field0 = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(3)), const<i32>(3))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)), const<i32>(2)))), field2 = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), field3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), const<i32>(1)), const<i32>(1)))), field4 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2147483647))), const<u64>(1)))), field5 = truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1))), field6 = reinterpret<i64, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), field7 = add<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), const<u64>(1)), const<u64>(1))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
