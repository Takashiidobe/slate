// SLATE-FILECHECK-DEFINES DEFAULT

/* Origin: Joseph Myers <jsm28@cam.ac.uk>.  */
/* As an extension, GCC allows a struct or union to be cast to its own
   type, but failed to allow this when a typedef was involved.
   Reported as PR c/2735 by <cowan@ccil.org>.  */
union u { int i; };
typedef union u uu;
union u a;
uu b;

void
foo (void)
{
  a = (union u) b;
  a = (uu) b;
  b = (union u) a;
  b = (uu) a;
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
// DEFAULT-NEXT:     type @type0 u = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 uu = @type0;
// DEFAULT-NEXT:     global %2 a: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type0>(%2, copy<@type0, reason=assign>(read<@type0>(%3)));
// DEFAULT-NEXT:         write<@type0>(%2, copy<@type0, reason=assign>(read<@type0>(%3)));
// DEFAULT-NEXT:         write<@type0>(%3, copy<@type0, reason=assign>(read<@type0>(%2)));
// DEFAULT-NEXT:         write<@type0>(%3, copy<@type0, reason=assign>(read<@type0>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
