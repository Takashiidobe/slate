/* PR middle-end/112668 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -fnon-call-exceptions" } */

#if __BITINT_MAXWIDTH__ >= 156
struct T156 { _BitInt(156) a : 2; unsigned _BitInt(156) b : 135; _BitInt(156) c : 2; };
extern void foo156 (struct T156 *);

unsigned _BitInt(156)
bar156 (int i)
{
  struct T156 r156[12];
  foo156 (&r156[0]);
  return r156[i].b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 495
struct T495 { _BitInt(495) a : 2; unsigned _BitInt(495) b : 471; _BitInt(495) c : 2; };
extern void foo495 (struct T495 *r495);

unsigned _BitInt(495)
bar495 (int i)
{
  struct T495 r495[12];
  foo495 (r495);
  return r495[i].b;
}
#endif

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type0 T156 = struct {
// DEFAULT-NEXT:         field0 a: i156b : 2;
// DEFAULT-NEXT:         field1 b: u156b : 135;
// DEFAULT-NEXT:         field2 c: i156b : 2;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 0, 17], bit_offsets=[Some(0), Some(2), Some(137)], bit_units=[(0, 18)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 T495 = struct {
// DEFAULT-NEXT:         field0 a: i495b : 2;
// DEFAULT-NEXT:         field1 b: u495b : 471;
// DEFAULT-NEXT:         field2 c: i495b : 2;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 0, 59], bit_offsets=[Some(0), Some(2), Some(473)], bit_units=[(0, 60)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %1 @foo156(%10 <unnamed>: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @bar156(%3 i: i32) -> u156b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 r156: array<@type0, 12> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(12)>(%4), const<i32>(0)))));
// DEFAULT-NEXT:         return read<u156b>(bitfield1<unit=0, bytes=0..18, bits=2..137>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(12)>(%4), read<i32>(%3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo495(%11 r495: ptr<@type1>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @bar495(%8 i: i32) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 r495: array<@type1, 12> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%6, array_decay<ptr<@type1>, length=Some(12)>(%9));
// DEFAULT-NEXT:         return read<u495b>(bitfield1<unit=0, bytes=0..60, bits=2..473>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(12)>(%9), read<i32>(%8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
