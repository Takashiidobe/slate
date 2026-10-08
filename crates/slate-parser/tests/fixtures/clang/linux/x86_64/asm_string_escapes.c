// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

void escaped(int x) {
  asm volatile("addl $1, %0\n\t"
               "subl $2, %0\x0a"
               "# \"quoted\" \\ \045\045 %%"
               : "+\162"(x) : : "\143c");
}

void basic(void) {
  asm volatile("nop\n\tnop\012# \\n stays literal");
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
// IR-NEXT:     fn %[[VALUE_escaped:[0-9]+]] @escaped(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile "addl $1, %0\n\tsubl $2, %0\n# \"quoted\" \\ %% %%" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "addl $1, " %0 "\n\tsubl $2, " %0 "\n# \"quoted\" \\ " %% " " %%;
// IR-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_x]]);
// IR-NEXT:             clobbers: cc;
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_basic:[0-9]+]] @basic() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile "nop\n\tnop\n# \\n stays literal" [dialect=att] [options=nostack];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
