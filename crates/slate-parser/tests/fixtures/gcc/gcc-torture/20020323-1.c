// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase caused ICE on powerpc at -O3, because regrename did
   not handle match_dup of match_operator if the RTLs were not shared.  */

struct A
{
  unsigned char *a0, *a1;
  int a2;
};

void bar (struct A *);

unsigned int
foo (int x)
{
  struct A a;
  unsigned int b;

  if (x < -128 || x > 255 || x == -1)
    return 26;

  a.a0 = (unsigned char *) &b;
  a.a1 = a.a0 + sizeof (unsigned int);
  a.a2 = 0;
  bar (&a);
  return b;
}

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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a0: ptr<u8>;
// DEFAULT-NEXT:         field1 a1: ptr<u8>;
// DEFAULT-NEXT:         field2 a2: i32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     fn %1 @bar(%6 <unnamed>: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: i32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %5 b: u32 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(lt<i32>(read<i32>(%3), neg<i32, overflow=ub>(const<i32>(128))), gt<i32>(read<i32>(%3), const<i32>(255))), eq<i32>(read<i32>(%3), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             return reinterpret<u32, reason=return, fits=always>(const<i32>(26));
// DEFAULT-NEXT:         write<ptr<u8>>(field0(%4), pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(%5)));
// DEFAULT-NEXT:         write<ptr<u8>>(field1(%4), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field0(%4)), const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(field2(%4), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, addr_of<ptr<@type0>>(%4));
// DEFAULT-NEXT:         return read<u32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
