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
// DEFAULT-NEXT:     type @type[[TYPE_uint8_t:[0-9]+]] uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 flag1: u8 : 2;
// DEFAULT-NEXT:         field1 flag2: u8 : 1;
// DEFAULT-NEXT:         field2 flag3: u8 : 1;
// DEFAULT-NEXT:         field3 flag4: u8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 0, 0, 1], bit_offsets=[Some(0), Some(2), Some(3), None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_MyType:[0-9]+]] MyType = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE0]]> [storage=automatic] = addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..2>(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_b]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield1<unit=0, bytes=0..1, bits=2..3>(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_b]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=3..4>(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_b]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_b]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<@type[[TYPE0]]> [synthetic] = read<ptr<@type[[TYPE0]]>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u8 [synthetic] = read<u8>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE0]]))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE1]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE0]]))), read<u8>(%[[VALUE2]]));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield0<unit=0, bytes=0..1, bits=0..2>(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_b]])))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
