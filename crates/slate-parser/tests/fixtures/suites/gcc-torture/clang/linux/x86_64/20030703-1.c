// SLATE-FILECHECK-DEFINES DEFAULT

/* Extracted from PR target/10700.  */
/* The following code used to cause an ICE on 64-bit targets.  */

int SAD_Block(int *);
void MBMotionEstimation(int *act_block, int block)
{
    SAD_Block(act_block + (  (8 * (block == 1 || block == 3))
                          + (8 * (block == 2 || block == 3))));
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
// DEFAULT-NEXT:     fn %0 @SAD_Block(%4 <unnamed>: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @MBMotionEstimation(%2 act_block: ptr<i32>, %3 block: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i32>) -> i32>(%0, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%2), add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), from_bool<i32, reason=promotion>(logical_or<bool>(eq<i32>(read<i32>(%3), const<i32>(1)), eq<i32>(read<i32>(%3), const<i32>(3))))), mul<i32, overflow=ub>(const<i32>(8), from_bool<i32, reason=promotion>(logical_or<bool>(eq<i32>(read<i32>(%3), const<i32>(2)), eq<i32>(read<i32>(%3), const<i32>(3))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
