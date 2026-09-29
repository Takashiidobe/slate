/* { dg-do run { target m68k-*-* fido-*-* sparc-*-* } } */
/* { dg-options { -O2 } } */

extern void abort (void);

typedef float SFtype __attribute__ ((mode (SF)));
typedef float DFtype __attribute__ ((mode (DF)));

typedef int HItype __attribute__ ((mode (HI)));
typedef int SItype __attribute__ ((mode (SI)));
typedef int DItype __attribute__ ((mode (DI)));

typedef unsigned int UHItype __attribute__ ((mode (HI)));
typedef unsigned int USItype __attribute__ ((mode (SI)));
typedef unsigned int UDItype __attribute__ ((mode (DI)));

typedef UDItype fractype;
typedef USItype halffractype;
typedef DFtype FLO_type;
typedef DItype intfrac;


typedef union
{
  long long foo;
  FLO_type value;
  struct
    {
      fractype fraction:52 __attribute__ ((packed));
      unsigned int exp:11 __attribute__ ((packed));
      unsigned int sign:1 __attribute__ ((packed));
    }
  bits;
} FLO_union_type;

void foo (long long a);
long long x; 

void
pack_d ()
{
  FLO_union_type dst = { 0x0123456789abcdefLL };

  x = dst.bits.fraction;
}

int
main ()
{
  pack_d ();
  foo (x);
  return 0;
}

void
foo (long long a)
{
  if (a != 0x0123456789abcLL)
    abort ();
}

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
// DEFAULT-NEXT:     type @type[[TYPE_SFtype:[0-9]+]] SFtype = f32;
// DEFAULT-NEXT:     type @type[[TYPE_DFtype:[0-9]+]] DFtype = f64;
// DEFAULT-NEXT:     type @type[[TYPE_HItype:[0-9]+]] HItype = i16;
// DEFAULT-NEXT:     type @type[[TYPE_SItype:[0-9]+]] SItype = i32;
// DEFAULT-NEXT:     type @type[[TYPE_DItype:[0-9]+]] DItype = i64;
// DEFAULT-NEXT:     type @type[[TYPE_UHItype:[0-9]+]] UHItype = u16;
// DEFAULT-NEXT:     type @type[[TYPE_USItype:[0-9]+]] USItype = u32;
// DEFAULT-NEXT:     type @type[[TYPE_UDItype:[0-9]+]] UDItype = u64;
// DEFAULT-NEXT:     type @type[[TYPE_fractype:[0-9]+]] fractype = u64;
// DEFAULT-NEXT:     type @type[[TYPE_halffractype:[0-9]+]] halffractype = u32;
// DEFAULT-NEXT:     type @type[[TYPE_FLO_type:[0-9]+]] FLO_type = f64;
// DEFAULT-NEXT:     type @type[[TYPE_intfrac:[0-9]+]] intfrac = i64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 foo: i64;
// DEFAULT-NEXT:         field1 value: f64;
// DEFAULT-NEXT:         field2 bits: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = struct {
// DEFAULT-NEXT:         field0 fraction: u64 : 52;
// DEFAULT-NEXT:         field1 exp: u32 : 11;
// DEFAULT-NEXT:         field2 sign: u32 : 1;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0, 6, 7], bit_offsets=[Some(0), Some(52), Some(63)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_FLO_union_type:[0-9]+]] FLO_union_type = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_a]]), const<i64>(20015998343868))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pack_d:[0-9]+]] @pack_d(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_dst:[0-9]+]] dst: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i64>(81985529216486895));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], reinterpret<i64, reason=assign, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(field2(%[[VALUE_dst]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_pack_d]]);
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_foo]], read<i64>(%[[VALUE_x]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
