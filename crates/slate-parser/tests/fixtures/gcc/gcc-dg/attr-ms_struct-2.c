/* Test for MS structure sizes.  */
/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-require-effective-target ilp32 } */
/* { dg-options "-std=gnu99" } */

extern void abort ();

#define ATTR __attribute__((__ms_struct__))

struct _struct_0
{
  long  member_0   : 25 ;
  short  member_1   : 6 ;
  char  member_2   : 2 ;
  unsigned  short  member_3   : 1 ;
  unsigned  char  member_4   : 7 ;
  short  member_5   : 16 ;
  long  : 0 ;
  char  member_7  ;

} ATTR;
typedef struct _struct_0 struct_0;

#define size_struct_0 20

struct_0 test_struct_0 = { 18557917, 17, 3, 0, 80, 6487, 93 };

int
main (void)
{

  if (size_struct_0 != sizeof (struct_0))
    abort ();

  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     type @type0 _struct_0 = struct {
// DEFAULT-NEXT:         field0 member_0: i64 : 25;
// DEFAULT-NEXT:         field1 member_1: i16 : 6;
// DEFAULT-NEXT:         field2 member_2: i8 : 2;
// DEFAULT-NEXT:         field3 member_3: u16 : 1;
// DEFAULT-NEXT:         field4 member_4: u8 : 7;
// DEFAULT-NEXT:         field5 member_5: i16 : 16;
// DEFAULT-NEXT:         field6 <anonymous>: i64 : 0;
// DEFAULT-NEXT:         field7 member_7: i8;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 10, 12, 14, 16, 24, 24], bit_offsets=[Some(0), Some(64), Some(80), Some(96), Some(112), Some(128), Some(192), None], bit_units=[(0, 8), (8, 2), (10, 1), (12, 2), (14, 1), (16, 2)], field_units=[Some(0), Some(1), Some(2), Some(3), Some(4), Some(5), None, None]];
// DEFAULT-NEXT:     type @type1 struct_0 = @type0;
// DEFAULT-NEXT:     global %3 test_struct_0: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(18557917)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(17)), field2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), field3 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), field4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(80))), field5 = truncate<i16, reason=assign, fits=always>(const<i32>(6487)), field7 = truncate<i8, reason=assign, fits=always>(const<i32>(93))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort(unprototyped) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(20))), const<u64>(32))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
