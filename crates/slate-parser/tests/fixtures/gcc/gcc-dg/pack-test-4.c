/* PR c/11885
   Bug: flag4 was allocated into the same byte as the other flags.
   { dg-options "" }
   { dg-do run } */

extern void abort (void);

typedef unsigned char uint8_t;

typedef struct {
    uint8_t flag1:2;
    uint8_t flag2:1;
    uint8_t flag3:1;
   
    uint8_t flag4;

} __attribute__ ((packed)) MyType;

int main (void)
{
  MyType a;
  MyType *b = &a;

  b->flag1 = 0;
  b->flag2 = 0;
  b->flag3 = 0;

  b->flag4 = 0;

  b->flag4++;
    
  if (b->flag1 != 0)
    abort ();

  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 uint8_t = u8;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 flag1: u8 : 2;
// DEFAULT-NEXT:         field1 flag2: u8 : 1;
// DEFAULT-NEXT:         field2 flag3: u8 : 1;
// DEFAULT-NEXT:         field3 flag4: u8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 0, 0, 1], bit_offsets=[Some(0), Some(2), Some(3), None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type2 MyType = @type1;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 a: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %6 b: ptr<@type1> [storage=automatic] = addr_of<ptr<@type1>>(%5);
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..2>(deref(read<ptr<@type1>>(%6))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield1<unit=0, bytes=0..1, bits=2..3>(deref(read<ptr<@type1>>(%6))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=3..4>(deref(read<ptr<@type1>>(%6))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(field3(deref(read<ptr<@type1>>(%6))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %7: ptr<@type1> [synthetic] = read<ptr<@type1>>(%6);
// DEFAULT-NEXT:         let %8: u8 [synthetic] = read<u8>(field3(deref(read<ptr<@type1>>(%7))));
// DEFAULT-NEXT:         let %9: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(field3(deref(read<ptr<@type1>>(%7))), read<u8>(%9));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield0<unit=0, bytes=0..1, bits=0..2>(deref(read<ptr<@type1>>(%6)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
