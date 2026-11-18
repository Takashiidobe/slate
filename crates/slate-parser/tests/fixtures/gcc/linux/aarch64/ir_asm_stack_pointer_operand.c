// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

register unsigned long current_stack_pointer __asm__("sp");

void call_thunk(void) {
    __asm__ volatile("bl thunk" : "+r"(current_stack_pointer) : : "x30");
}

unsigned long read_stack(void) {
    unsigned long copy;
    __asm__("mov %0, %1" : "=r"(copy) : "r"(current_stack_pointer));
    return copy;
}

unsigned long other_register(unsigned long value) {
    __asm__("" : "+r"(value));
    return value;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f128;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_current_stack_pointer:[0-9]+]] current_stack_pointer: u64 [storage=static] [register="sp"] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_call_thunk:[0-9]+]] @call_thunk() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "bl thunk" [options=nomem,preserves_flags] {
// DEFAULT-NEXT:             template: "bl thunk";
// DEFAULT-NEXT:             inlateout 0 "r" [reg] -> {sp} width 64 place<u64>(%[[VALUE_current_stack_pointer]]);
// DEFAULT-NEXT:             clobbers: "x30" as x30;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_read_stack:[0-9]+]] @read_stack() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_copy:[0-9]+]] copy: u64 [storage=automatic];
// DEFAULT-NEXT:         asm "mov %0, %1" [options=pure,nomem,preserves_flags] {
// DEFAULT-NEXT:             template: "mov " %0 ", " %1;
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 64 place<u64>(%[[VALUE_copy]]);
// DEFAULT-NEXT:             in 1 "r" [reg] -> {sp} width 64 read<u64>(%[[VALUE_current_stack_pointer]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_copy]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_other_register:[0-9]+]] @other_register(%[[VALUE_value:[0-9]+]] value: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "" [options=pure,nomem,nostack,preserves_flags] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 64 place<u64>(%[[VALUE_value]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_value]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
