// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES CHECK
int __asm__(const char *text) { return text[0]; }
int asm(int value) { return value + 1; }
int use(void) { return __asm__("n") + asm(1); }

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "x86_64-pc-windows-msvc" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=8, align=8];
// CHECK-NEXT:         stack_alignment = 16;
// CHECK-NEXT:         long_double = f64;
// CHECK-NEXT:         storage bool [size=1, align=1];
// CHECK-NEXT:         storage i8, u8 [size=1, align=1];
// CHECK-NEXT:         storage i16, u16 [size=2, align=2];
// CHECK-NEXT:         storage i32, u32 [size=4, align=4];
// CHECK-NEXT:         storage i64, u64 [size=8, align=8];
// CHECK-NEXT:         storage i128, u128 [size=16, align=16];
// CHECK-NEXT:         storage bf16 [size=2, align=2];
// CHECK-NEXT:         storage f16 [size=2, align=2];
// CHECK-NEXT:         storage f32 [size=4, align=4];
// CHECK-NEXT:         storage f64 [size=8, align=8];
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:         storage d32 [size=4, align=4];
// CHECK-NEXT:         storage d64 [size=8, align=8];
// CHECK-NEXT:         storage d128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([110, 0]) [linkage=internal];
// CHECK-NEXT:     fn %[[VALUE___asm__:[0-9]+]] @__asm__(%[[VALUE_text:[0-9]+]] text: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return widen<i32>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false>(read<ptr<const i8>>(%[[VALUE_text]]), const<i32>(0)))));
// CHECK-NEXT:     }
// CHECK-NEXT:     fn %[[VALUE_asm:[0-9]+]] @asm(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return add<i32>(read<i32>(%[[VALUE_value]]), const<i32>(1));
// CHECK-NEXT:     }
// CHECK-NEXT:     fn %[[VALUE_use:[0-9]+]] @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return add<i32>(call<i32>(%[[VALUE___asm__]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]]))), call<i32>(%[[VALUE_asm]], const<i32>(1)));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
