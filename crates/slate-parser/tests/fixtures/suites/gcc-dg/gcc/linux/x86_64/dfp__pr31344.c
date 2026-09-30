/* { dg-do compile } */
/* { dg-options "-O -mtune=i386" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */
/* { dg-options "-O" } */

typedef struct
{
  unsigned char bits;
} decNumber;

typedef struct
{
  unsigned char bytes[1];
} decimal32;

extern decNumber *__decimal32ToNumber (const decimal32 *, decNumber *);
extern void __host_to_ieee_32 (_Decimal32, decimal32 *);

void
foo (_Decimal32 arg)
{
  decNumber dn;
  decimal32 d32;
  __host_to_ieee_32 (arg, &d32);
  __decimal32ToNumber (&d32, &dn);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 bits: u8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_decNumber:[0-9]+]] decNumber = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 bytes: array<u8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_decimal32:[0-9]+]] decimal32 = @type[[TYPE1]];
// DEFAULT-NEXT:     fn %[[VALUE___decimal32ToNumber:[0-9]+]] @__decimal32ToNumber(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const @type[[TYPE1]]>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE0]]>) -> ptr<@type[[TYPE0]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___host_to_ieee_32:[0-9]+]] @__host_to_ieee_32(%[[VALUE2:[0-9]+]] <unnamed>: d32, %[[VALUE3:[0-9]+]] <unnamed>: ptr<@type[[TYPE1]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_arg:[0-9]+]] arg: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_dn:[0-9]+]] dn: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d32:[0-9]+]] d32: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(d32, ptr<@type[[TYPE1]]>) -> void>(%[[VALUE___host_to_ieee_32]], read<d32>(%[[VALUE_arg]]), addr_of<ptr<@type[[TYPE1]]>>(%[[VALUE_d32]]));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE0]]>, signature=fn(ptr<const @type[[TYPE1]]>, ptr<@type[[TYPE0]]>) -> ptr<@type[[TYPE0]]>>(%[[VALUE___decimal32ToNumber]], pointer_cast<ptr<const @type[[TYPE1]]>, reason=arg>(addr_of<ptr<@type[[TYPE1]]>>(%[[VALUE_d32]])), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_dn]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
