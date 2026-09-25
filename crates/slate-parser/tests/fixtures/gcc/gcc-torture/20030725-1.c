// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase caused ICE on any 64-bit arch at -O2/-O3 due to
   fold/extract_muldiv/convert destroying its argument.  */
int x, *y, z, *p;

void
foo (void)
{
  p = y + (8 * (x == 1 || x == 3) + z);
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
// DEFAULT-NEXT:     global %0 x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 y: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 z: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 p: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(%3, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%1), add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), from_bool<i32, reason=promotion>(logical_or<bool>(eq<i32>(read<i32>(%0), const<i32>(1)), eq<i32>(read<i32>(%0), const<i32>(3))))), read<i32>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
