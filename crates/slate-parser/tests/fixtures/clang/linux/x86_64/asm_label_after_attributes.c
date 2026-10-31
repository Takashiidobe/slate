// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
int after_attribute(int) __attribute__((unused)) __asm__("after_attribute_label");
int between_attributes(int) __attribute__((unused)) __asm__("between_attributes_label") __attribute__((pure));
int *returns_pointer(void) __attribute__((unused)) __asm__("returns_pointer_label");
int before_attribute(int) __asm__("before_attribute_label") __attribute__((unused));
int overloaded(int fd, unsigned op, ...) __attribute__((__overloadable__)) __attribute__((enable_if(1, ""))) __asm__("overloaded_label");

int use(void) {
    return after_attribute(1) + between_attributes(2) + *returns_pointer() + before_attribute(3) + overloaded(4, 5u);
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
// DEFAULT-NEXT:     fn %[[VALUE_after_attribute:[0-9]+]] @after_attribute(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [asm_name="after_attribute_label"];
// DEFAULT-NEXT:     fn %[[VALUE_between_attributes:[0-9]+]] @between_attributes(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [asm_name="between_attributes_label"] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_returns_pointer:[0-9]+]] @returns_pointer() -> ptr<i32> [linkage=external] [asm_name="returns_pointer_label"];
// DEFAULT-NEXT:     fn %[[VALUE_before_attribute:[0-9]+]] @before_attribute(%[[VALUE2:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [asm_name="before_attribute_label"];
// DEFAULT-NEXT:     fn %[[VALUE_overloaded:[0-9]+]] @overloaded(%[[VALUE_fd:[0-9]+]] fd: i32, %[[VALUE_op:[0-9]+]] op: u32, ...) -> i32 [linkage=external] [asm_name="overloaded_label"] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE_use:[0-9]+]] @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32>(add<i32>(add<i32>(add<i32>(call<i32>(%[[VALUE_after_attribute]], const<i32>(1)), call<i32>(%[[VALUE_between_attributes]], const<i32>(2))), read<i32>(deref(call<ptr<i32>>(%[[VALUE_returns_pointer]])))), call<i32>(%[[VALUE_before_attribute]], const<i32>(3))), call<i32>(%[[VALUE_overloaded]], const<i32>(4), const<u32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
