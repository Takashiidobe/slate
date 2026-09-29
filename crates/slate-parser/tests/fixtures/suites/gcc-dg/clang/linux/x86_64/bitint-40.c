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
// DEFAULT-NEXT:     type @type[[TYPE_T156:[0-9]+]] T156 = struct {
// DEFAULT-NEXT:         field0 a: i156b : 2;
// DEFAULT-NEXT:         field1 b: u156b : 135;
// DEFAULT-NEXT:         field2 c: i156b : 2;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 0, 17], bit_offsets=[Some(0), Some(2), Some(137)], bit_units=[(0, 18)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_T495:[0-9]+]] T495 = struct {
// DEFAULT-NEXT:         field0 a: i495b : 2;
// DEFAULT-NEXT:         field1 b: u495b : 471;
// DEFAULT-NEXT:         field2 c: i495b : 2;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 0, 59], bit_offsets=[Some(0), Some(2), Some(473)], bit_units=[(0, 60)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_foo156:[0-9]+]] @foo156(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_T156]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar156:[0-9]+]] @bar156(%[[VALUE_i:[0-9]+]] i: i32) -> u156b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r156:[0-9]+]] r156: array<@type[[TYPE_T156]], 12> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_T156]]>) -> void>(%[[VALUE_foo156]], addr_of<ptr<@type[[TYPE_T156]]>>(deref(ptr_offset<ptr<@type[[TYPE_T156]]>, subtract=false, element=@type[[TYPE_T156]], overflow=ub>(array_decay<ptr<@type[[TYPE_T156]]>, length=Some(12)>(%[[VALUE_r156]]), const<i32>(0)))));
// DEFAULT-NEXT:         return read<u156b>(bitfield1<unit=0, bytes=0..18, bits=2..137>(deref(ptr_offset<ptr<@type[[TYPE_T156]]>, subtract=false, element=@type[[TYPE_T156]], overflow=ub>(array_decay<ptr<@type[[TYPE_T156]]>, length=Some(12)>(%[[VALUE_r156]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo495:[0-9]+]] @foo495(%[[VALUE_r495:[0-9]+]] r495: ptr<@type[[TYPE_T495]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar495:[0-9]+]] @bar495(%[[VALUE_i_2:[0-9]+]] i: i32) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r495_2:[0-9]+]] r495: array<@type[[TYPE_T495]], 12> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_T495]]>) -> void>(%[[VALUE_foo495]], array_decay<ptr<@type[[TYPE_T495]]>, length=Some(12)>(%[[VALUE_r495_2]]));
// DEFAULT-NEXT:         return read<u495b>(bitfield1<unit=0, bytes=0..60, bits=2..473>(deref(ptr_offset<ptr<@type[[TYPE_T495]]>, subtract=false, element=@type[[TYPE_T495]], overflow=ub>(array_decay<ptr<@type[[TYPE_T495]]>, length=Some(12)>(%[[VALUE_r495_2]]), read<i32>(%[[VALUE_i_2]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
