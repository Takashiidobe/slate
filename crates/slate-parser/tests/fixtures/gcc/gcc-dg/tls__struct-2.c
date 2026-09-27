/* { dg-do assemble } */
/* { dg-require-effective-target tls } */
/* { dg-add-options tls } */

struct pixel
{
  unsigned int r, g, b;
};

struct line
{
  unsigned int length;
  struct pixel data[16];
};

__thread struct line L;

unsigned int read_r (unsigned int i)
{
  return i < L.length ? L.data[i].r : 0;
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 pixel = struct {
// DEFAULT-NEXT:         field0 r: u32;
// DEFAULT-NEXT:         field1 g: u32;
// DEFAULT-NEXT:         field2 b: u32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 line = struct {
// DEFAULT-NEXT:         field0 length: u32;
// DEFAULT-NEXT:         field1 data: array<@type0, 16>;
// DEFAULT-NEXT:     } [size=196, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %2 L: @type1 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %3 @read_r(%4 i: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(lt<u32>(read<u32>(%4), read<u32>(field0(%2))), read<u32>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(field1(%2)), read<u32>(%4))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
