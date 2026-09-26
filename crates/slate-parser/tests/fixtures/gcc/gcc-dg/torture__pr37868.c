/* { dg-do run } */
/* { dg-options "-fno-strict-aliasing" } */
/* { dg-skip-if "unaligned access" { arc*-*-* epiphany-*-* nds32*-*-* sparc*-*-* sh*-*-* tic6x-*-* } } */

extern void abort(void);
#if (__SIZEOF_INT__ <= 2)
struct X {
  unsigned char pad : 4;
  unsigned int  a   : 16;
  unsigned int  b   : 8;
  unsigned int  c   : 6;
} __attribute__((packed));
#else
struct X {
  unsigned char pad : 4;
  unsigned int  a   : 32;
  unsigned int  b   : 24;
  unsigned int  c   : 6;
} __attribute__((packed));

#endif

int main(void) {
  struct X     x;
  unsigned int bad_bits;

  x.pad = -1;
  x.a   = -1;
  x.b   = -1;
  x.c   = -1;

  bad_bits = ((unsigned int)-1) ^ *(1 + (unsigned int *)&x);
  if (bad_bits != 0)
    abort();
  return 0;
}



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
// DEFAULT-NEXT:     type @type0 X = struct {
// DEFAULT-NEXT:         field0 pad: u8 : 4;
// DEFAULT-NEXT:         field1 a: u32 : 32;
// DEFAULT-NEXT:         field2 b: u32 : 24;
// DEFAULT-NEXT:         field3 c: u32 : 6;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0, 0, 4, 7], bit_offsets=[Some(0), Some(4), Some(36), Some(60)], bit_units=[(0, 9)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %4 bad_bits: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..9, bits=0..4>(%3), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..9, bits=4..36>(%3), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..9, bits=36..60>(%3), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..9, bits=60..66>(%3), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%4, xor<u32>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(pointer_cast<ptr<u32>, reason=explicit>(addr_of<ptr<@type0>>(%3)), const<i32>(1))))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
