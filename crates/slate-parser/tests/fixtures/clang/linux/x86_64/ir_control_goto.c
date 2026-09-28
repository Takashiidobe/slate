// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int jump(int x) {
    goto done;
    done: return x;
}
void local_labels(void) {
    { __label__ done; goto done; done: ; }
    { __label__ done; goto done; done: {} }
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %0 @jump(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         goto %1;
// IR-NEXT:         label %1 done:
// IR-NEXT:             return read<i32>(%2);
// IR-NEXT:     }
// IR-NEXT:     fn %3 @local_labels() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         {
// IR-NEXT:             goto %4;
// IR-NEXT:             label %4 done:
// IR-NEXT:                 ;
// IR-NEXT:         }
// IR-NEXT:         {
// IR-NEXT:             goto %5;
// IR-NEXT:             label %5 done:
// IR-NEXT:                 {
// IR-NEXT:                 }
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
