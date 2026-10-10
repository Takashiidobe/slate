// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES GNU17
// SLATE-FILECHECK-STD GNU17 gnu17
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

void f(int v) {
  __asm__ volatile("nop" :: "r"(v));
  __asm__ volatile("nop" ::: "memory");
  __asm__("nop" : "=r"(v) :: "memory");
  __asm__ goto("jmp %l0" :::: out);
out:;
}

// SLATE-FILECHECK-BEGIN GNU17
// GNU17: module {
// GNU17-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU17-NEXT:         endian = little;
// GNU17-NEXT:         pointer [size=8, align=8];
// GNU17-NEXT:         stack_alignment = 16;
// GNU17-NEXT:         long_double = f80;
// GNU17-NEXT:         storage bool [size=1, align=1];
// GNU17-NEXT:         storage i8, u8 [size=1, align=1];
// GNU17-NEXT:         storage i16, u16 [size=2, align=2];
// GNU17-NEXT:         storage i32, u32 [size=4, align=4];
// GNU17-NEXT:         storage i64, u64 [size=8, align=8];
// GNU17-NEXT:         storage i128, u128 [size=16, align=16];
// GNU17-NEXT:         storage bf16 [size=2, align=2];
// GNU17-NEXT:         storage f16 [size=2, align=2];
// GNU17-NEXT:         storage f32 [size=4, align=4];
// GNU17-NEXT:         storage f64 [size=8, align=8];
// GNU17-NEXT:         storage f80 [size=16, align=16];
// GNU17-NEXT:         storage f128 [size=16, align=16];
// GNU17-NEXT:         storage d32 [size=4, align=4];
// GNU17-NEXT:         storage d64 [size=8, align=8];
// GNU17-NEXT:         storage d128 [size=16, align=16];
// GNU17-NEXT:     }
// GNU17-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_v:[0-9]+]] v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// GNU17-NEXT:         asm volatile "nop" [dialect=att] [options=nomem,nostack] {
// GNU17-NEXT:             template: "nop";
// GNU17-NEXT:             in 0 "r" [reg] width 32 read<i32>(%[[VALUE_v]]);
// GNU17-NEXT:         }
// GNU17-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// GNU17-NEXT:             template: "nop";
// GNU17-NEXT:             clobbers: memory;
// GNU17-NEXT:         }
// GNU17-NEXT:         asm "nop" [dialect=att] [options=nostack] {
// GNU17-NEXT:             template: "nop";
// GNU17-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_v]]);
// GNU17-NEXT:             clobbers: memory;
// GNU17-NEXT:         }
// GNU17-NEXT:         asm goto "jmp %l0" [dialect=att] [options=nomem,nostack] {
// GNU17-NEXT:             template: "jmp " %l0;
// GNU17-NEXT:             labels: %[[VALUE_out:[0-9]+]];
// GNU17-NEXT:         }
// GNU17-NEXT:         label %[[VALUE_out]] out:
// GNU17-NEXT:             ;
// GNU17-NEXT:     }
// GNU17-NEXT: }
// SLATE-FILECHECK-END GNU17
// SLATE-FILECHECK-BEGIN C17
// C17: module {
// C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// C17-NEXT:         endian = little;
// C17-NEXT:         pointer [size=8, align=8];
// C17-NEXT:         stack_alignment = 16;
// C17-NEXT:         long_double = f80;
// C17-NEXT:         storage bool [size=1, align=1];
// C17-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NEXT:         storage bf16 [size=2, align=2];
// C17-NEXT:         storage f16 [size=2, align=2];
// C17-NEXT:         storage f32 [size=4, align=4];
// C17-NEXT:         storage f64 [size=8, align=8];
// C17-NEXT:         storage f80 [size=16, align=16];
// C17-NEXT:         storage f128 [size=16, align=16];
// C17-NEXT:         storage d32 [size=4, align=4];
// C17-NEXT:         storage d64 [size=8, align=8];
// C17-NEXT:         storage d128 [size=16, align=16];
// C17-NEXT:     }
// C17-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_v:[0-9]+]] v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// C17-NEXT:         asm volatile "nop" [dialect=att] [options=nomem,nostack] {
// C17-NEXT:             template: "nop";
// C17-NEXT:             in 0 "r" [reg] width 32 read<i32>(%[[VALUE_v]]);
// C17-NEXT:         }
// C17-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// C17-NEXT:             template: "nop";
// C17-NEXT:             clobbers: memory;
// C17-NEXT:         }
// C17-NEXT:         asm "nop" [dialect=att] [options=nostack] {
// C17-NEXT:             template: "nop";
// C17-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_v]]);
// C17-NEXT:             clobbers: memory;
// C17-NEXT:         }
// C17-NEXT:         asm goto "jmp %l0" [dialect=att] [options=nomem,nostack] {
// C17-NEXT:             template: "jmp " %l0;
// C17-NEXT:             labels: %[[VALUE_out:[0-9]+]];
// C17-NEXT:         }
// C17-NEXT:         label %[[VALUE_out]] out:
// C17-NEXT:             ;
// C17-NEXT:     }
// C17-NEXT: }
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_v:[0-9]+]] v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// C23-NEXT:         asm volatile "nop" [dialect=att] [options=nomem,nostack] {
// C23-NEXT:             template: "nop";
// C23-NEXT:             in 0 "r" [reg] width 32 read<i32>(%[[VALUE_v]]);
// C23-NEXT:         }
// C23-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// C23-NEXT:             template: "nop";
// C23-NEXT:             clobbers: memory;
// C23-NEXT:         }
// C23-NEXT:         asm "nop" [dialect=att] [options=nostack] {
// C23-NEXT:             template: "nop";
// C23-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_v]]);
// C23-NEXT:             clobbers: memory;
// C23-NEXT:         }
// C23-NEXT:         asm goto "jmp %l0" [dialect=att] [options=nomem,nostack] {
// C23-NEXT:             template: "jmp " %l0;
// C23-NEXT:             labels: %[[VALUE_out:[0-9]+]];
// C23-NEXT:         }
// C23-NEXT:         label %[[VALUE_out]] out:
// C23-NEXT:             ;
// C23-NEXT:     }
// C23-NEXT: }
// SLATE-FILECHECK-END C23
