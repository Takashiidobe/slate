// SLATE-FILECHECK-DEFINES DEFAULT

/* The bit-field below would have a problem if __INT_MAX__ is too
   small.  */
#if __INT_MAX__ < 2147483647
int a;
#else
unsigned int  x0  = 0;

typedef struct {
  unsigned int  field1 : 20;
  unsigned int  field2 : 12;
} XX;

static XX yy;

static void foo (void)
{
  yy.field1 = (unsigned int ) (&x0);
}
#endif

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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 field1: u32 : 20;
// DEFAULT-NEXT:         field1 field2: u32 : 12;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 2], bit_offsets=[Some(0), Some(20)], bit_units=[(0, 4)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 XX = @type0;
// DEFAULT-NEXT:     global %0 x0: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %3 yy: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %4 @foo() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..20>(%3), ptr_to_int<u32, reason=explicit>(addr_of<ptr<u32>>(%0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
