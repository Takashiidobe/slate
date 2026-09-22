// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

__attribute__((scalar_storage_order("big-endian"))) int ordered;
__attribute__((address_space(1 + 2))) int *device_memory;
__attribute__((aligned(4 + 4))) int aligned_value;

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
// IR-NEXT:     global %0 ordered: i32 [storage=static] [linkage=external] [c="int"] [c_attributes="[ScalarStorageOrder(\"big-endian\")]"];
// IR-NEXT:     global %1 device_memory: ptr<i32> [storage=static] [linkage=external] [c="int *"] [c_attributes="[AddressSpace(IntegerLiteral(IntegerLiteral { value: 3, radix: Decimal, suffix: IntegerSuffix { unsigned: false, size: None }, spelling: \"3\" }))]"];
// IR-NEXT:     global %2 aligned_value: i32 [storage=static] [align=8] [linkage=external] [c="int"] [c_attributes="[Aligned(IntegerLiteral(IntegerLiteral { value: 8, radix: Decimal, suffix: IntegerSuffix { unsigned: false, size: None }, spelling: \"8\" }))]"];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
