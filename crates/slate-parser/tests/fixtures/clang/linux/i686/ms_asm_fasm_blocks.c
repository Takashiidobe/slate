int value;

void set_value(void) {
    __asm { mov value, 1 }
}

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES BLOCKS
// SLATE-FILECHECK-PREFIX-ARGS BLOCKS -fasm-blocks
// SLATE-FILECHECK-DEFINES LAST
// SLATE-FILECHECK-PREFIX-ARGS LAST -fno-asm-blocks -fasm-blocks
// SLATE-FILECHECK-DEFINES EXT
// SLATE-FILECHECK-PREFIX-ARGS EXT -fms-extensions -fno-asm-blocks

// SLATE-FILECHECK-BEGIN BLOCKS
// BLOCKS: module {
// BLOCKS-NEXT:     target "i686-unknown-linux-gnu" {
// BLOCKS-NEXT:         endian = little;
// BLOCKS-NEXT:         pointer [size=4, align=4];
// BLOCKS-NEXT:         stack_alignment = 16;
// BLOCKS-NEXT:         long_double = f80;
// BLOCKS-NEXT:         storage bool [size=1, align=1];
// BLOCKS-NEXT:         storage i8, u8 [size=1, align=1];
// BLOCKS-NEXT:         storage i16, u16 [size=2, align=2];
// BLOCKS-NEXT:         storage i32, u32 [size=4, align=4];
// BLOCKS-NEXT:         storage i64, u64 [size=8, align=4];
// BLOCKS-NEXT:         storage i128, u128 [size=16, align=16];
// BLOCKS-NEXT:         storage bf16 [size=2, align=2];
// BLOCKS-NEXT:         storage f16 [size=2, align=2];
// BLOCKS-NEXT:         storage f32 [size=4, align=4];
// BLOCKS-NEXT:         storage f64 [size=8, align=4];
// BLOCKS-NEXT:         storage f80 [size=12, align=4];
// BLOCKS-NEXT:         storage f128 [size=16, align=16];
// BLOCKS-NEXT:         storage d32 [size=4, align=4];
// BLOCKS-NEXT:         storage d64 [size=8, align=8];
// BLOCKS-NEXT:         storage d128 [size=16, align=16];
// BLOCKS-NEXT:     }
// BLOCKS-NEXT:     global %[[VALUE_value:[0-9]+]] value: i32 [storage=static] [linkage=external];
// BLOCKS-NEXT:     fn %[[VALUE_set_value:[0-9]+]] @set_value() -> void [linkage=external] [fallthrough=ret_void] {
// BLOCKS-NEXT:         asm volatile "mov value, 1" [dialect=intel] {
// BLOCKS-NEXT:             template: "mov " addr<dword>(%0) ", 1";
// BLOCKS-NEXT:             in 0 [value] mem<write> place<i32>(%[[VALUE_value]]);
// BLOCKS-NEXT:         }
// BLOCKS-NEXT:     }
// BLOCKS-NEXT: }
// SLATE-FILECHECK-END BLOCKS
// SLATE-FILECHECK-BEGIN LAST
// LAST: module {
// LAST-NEXT:     target "i686-unknown-linux-gnu" {
// LAST-NEXT:         endian = little;
// LAST-NEXT:         pointer [size=4, align=4];
// LAST-NEXT:         stack_alignment = 16;
// LAST-NEXT:         long_double = f80;
// LAST-NEXT:         storage bool [size=1, align=1];
// LAST-NEXT:         storage i8, u8 [size=1, align=1];
// LAST-NEXT:         storage i16, u16 [size=2, align=2];
// LAST-NEXT:         storage i32, u32 [size=4, align=4];
// LAST-NEXT:         storage i64, u64 [size=8, align=4];
// LAST-NEXT:         storage i128, u128 [size=16, align=16];
// LAST-NEXT:         storage bf16 [size=2, align=2];
// LAST-NEXT:         storage f16 [size=2, align=2];
// LAST-NEXT:         storage f32 [size=4, align=4];
// LAST-NEXT:         storage f64 [size=8, align=4];
// LAST-NEXT:         storage f80 [size=12, align=4];
// LAST-NEXT:         storage f128 [size=16, align=16];
// LAST-NEXT:         storage d32 [size=4, align=4];
// LAST-NEXT:         storage d64 [size=8, align=8];
// LAST-NEXT:         storage d128 [size=16, align=16];
// LAST-NEXT:     }
// LAST-NEXT:     global %[[VALUE_value:[0-9]+]] value: i32 [storage=static] [linkage=external];
// LAST-NEXT:     fn %[[VALUE_set_value:[0-9]+]] @set_value() -> void [linkage=external] [fallthrough=ret_void] {
// LAST-NEXT:         asm volatile "mov value, 1" [dialect=intel] {
// LAST-NEXT:             template: "mov " addr<dword>(%0) ", 1";
// LAST-NEXT:             in 0 [value] mem<write> place<i32>(%[[VALUE_value]]);
// LAST-NEXT:         }
// LAST-NEXT:     }
// LAST-NEXT: }
// SLATE-FILECHECK-END LAST
// SLATE-FILECHECK-BEGIN EXT
// EXT: module {
// EXT-NEXT:     target "i686-unknown-linux-gnu" {
// EXT-NEXT:         endian = little;
// EXT-NEXT:         pointer [size=4, align=4];
// EXT-NEXT:         stack_alignment = 16;
// EXT-NEXT:         long_double = f80;
// EXT-NEXT:         storage bool [size=1, align=1];
// EXT-NEXT:         storage i8, u8 [size=1, align=1];
// EXT-NEXT:         storage i16, u16 [size=2, align=2];
// EXT-NEXT:         storage i32, u32 [size=4, align=4];
// EXT-NEXT:         storage i64, u64 [size=8, align=4];
// EXT-NEXT:         storage i128, u128 [size=16, align=16];
// EXT-NEXT:         storage bf16 [size=2, align=2];
// EXT-NEXT:         storage f16 [size=2, align=2];
// EXT-NEXT:         storage f32 [size=4, align=4];
// EXT-NEXT:         storage f64 [size=8, align=4];
// EXT-NEXT:         storage f80 [size=12, align=4];
// EXT-NEXT:         storage f128 [size=16, align=16];
// EXT-NEXT:         storage d32 [size=4, align=4];
// EXT-NEXT:         storage d64 [size=8, align=8];
// EXT-NEXT:         storage d128 [size=16, align=16];
// EXT-NEXT:     }
// EXT-NEXT:     global %[[VALUE_value:[0-9]+]] value: i32 [storage=static] [linkage=external];
// EXT-NEXT:     fn %[[VALUE_set_value:[0-9]+]] @set_value() -> void [linkage=external] [fallthrough=ret_void] {
// EXT-NEXT:         asm volatile "mov value, 1" [dialect=intel] {
// EXT-NEXT:             template: "mov " addr<dword>(%0) ", 1";
// EXT-NEXT:             in 0 [value] mem<write> place<i32>(%[[VALUE_value]]);
// EXT-NEXT:         }
// EXT-NEXT:     }
// EXT-NEXT: }
// SLATE-FILECHECK-END EXT
