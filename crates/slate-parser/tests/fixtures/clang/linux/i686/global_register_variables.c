register unsigned long current_stack_pointer asm("esp");
register unsigned long frame_pointer asm("ebp");

unsigned long stack_pointer(void) { return current_stack_pointer; }

// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "i686-unknown-linux-gnu" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=4, align=4];
// CHECK-NEXT:         stack_alignment = 16;
// CHECK-NEXT:         long_double = f80;
// CHECK-NEXT:         storage bool [size=1, align=1];
// CHECK-NEXT:         storage i8, u8 [size=1, align=1];
// CHECK-NEXT:         storage i16, u16 [size=2, align=2];
// CHECK-NEXT:         storage i32, u32 [size=4, align=4];
// CHECK-NEXT:         storage i64, u64 [size=8, align=4];
// CHECK-NEXT:         storage i128, u128 [size=16, align=16];
// CHECK-NEXT:         storage bf16 [size=2, align=2];
// CHECK-NEXT:         storage f16 [size=2, align=2];
// CHECK-NEXT:         storage f32 [size=4, align=4];
// CHECK-NEXT:         storage f64 [size=8, align=4];
// CHECK-NEXT:         storage f80 [size=12, align=4];
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:         storage d32 [size=4, align=4];
// CHECK-NEXT:         storage d64 [size=8, align=8];
// CHECK-NEXT:         storage d128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     global %[[VALUE_current_stack_pointer:[0-9]+]] current_stack_pointer: u32 [storage=static] [register="esp"] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_frame_pointer:[0-9]+]] frame_pointer: u32 [storage=static] [register="ebp"] [linkage=external];
// CHECK-NEXT:     fn %[[VALUE_stack_pointer:[0-9]+]] @stack_pointer() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return read<u32>(%[[VALUE_current_stack_pointer]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
