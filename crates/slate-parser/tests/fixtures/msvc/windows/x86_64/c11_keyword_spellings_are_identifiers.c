// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES CHECK
int _Noreturn;
int _Generic;
int _Thread_local;
int _Alignas;
int _Static_assert;
static_assert(sizeof(int) == 4, "int");
int alignment = _Alignof(double);
_Bool flag;

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
// CHECK-NEXT:     global %[[VALUE__Noreturn:[0-9]+]] _Noreturn: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE__Generic:[0-9]+]] _Generic: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE__Thread_local:[0-9]+]] _Thread_local: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE__Alignas:[0-9]+]] _Alignas: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE__Static_assert:[0-9]+]] _Static_assert: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_alignment:[0-9]+]] alignment: i32 [storage=static] = reinterpret<i32>(truncate<u32>(align_of<u64, f64>(8) [c="__size_t"])) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_flag:[0-9]+]] flag: bool [storage=static] [linkage=external];
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
