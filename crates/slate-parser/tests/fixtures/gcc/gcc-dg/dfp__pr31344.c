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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 bits: u8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 decNumber = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 bytes: array<u8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type3 decimal32 = @type2;
// DEFAULT-NEXT:     fn %4 @__decimal32ToNumber(%10 <unnamed>: ptr<const @type2>, %11 <unnamed>: ptr<@type0>) -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %5 @__host_to_ieee_32(%12 <unnamed>: d32, %13 <unnamed>: ptr<@type2>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo(%7 arg: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 dn: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %9 d32: @type2 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(d32, ptr<@type2>) -> void>(%5, read<d32>(%7), addr_of<ptr<@type2>>(%9));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn(ptr<const @type2>, ptr<@type0>) -> ptr<@type0>>(%4, pointer_cast<ptr<const @type2>, reason=arg>(addr_of<ptr<@type2>>(%9)), addr_of<ptr<@type0>>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
