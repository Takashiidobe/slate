// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

const char *utf8 = u8"a";
unsigned char buffer[4];
char *bytes = buffer;
unsigned *to_unsigned(int *value) { return value; }
void assign(unsigned *value) {
    int *signed_value;
    signed_value = value;
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
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<u8, 2> [storage=static] = code_units<array<u8, 2>>([97, 0]) [linkage=internal];
// IR-NEXT:     global %[[VALUE_utf8:[0-9]+]] utf8: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<u8>, length=Some(2)>(%[[VALUE_str]])) [linkage=external];
// IR-NEXT:     global %[[VALUE_buffer:[0-9]+]] buffer: array<u8, 4> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_bytes:[0-9]+]] bytes: ptr<i8> [storage=static] = pointer_cast<ptr<i8>, reason=assign>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_buffer]])) [linkage=external];
// IR-NEXT:     fn %[[VALUE_to_unsigned:[0-9]+]] @to_unsigned(%[[VALUE_value:[0-9]+]] value: ptr<i32>) -> ptr<u32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return pointer_cast<ptr<u32>, reason=return>(read<ptr<i32>>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_assign:[0-9]+]] @assign(%[[VALUE_value_2:[0-9]+]] value: ptr<u32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_signed_value:[0-9]+]] signed_value: ptr<i32> [storage=automatic];
// IR-NEXT:         write<ptr<i32>>(%[[VALUE_signed_value]], pointer_cast<ptr<i32>, reason=assign>(read<ptr<u32>>(%[[VALUE_value_2]])));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
