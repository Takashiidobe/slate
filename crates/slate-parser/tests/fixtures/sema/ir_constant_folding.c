// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

unsigned long constants(void) {
    2 + 3 * 4;
    0xffffffffU + 1U;
    -7 / 3;
    -7 % 3;
    (unsigned char)257;
    1 < 2;
    0 && (1 / 0);
    1 ? 9 : (1 / 0);
    1 / 0;
    2147483647 + 1;
    1 << 32;
    sizeof(int) * 8;
    (_BitInt(sizeof(int) * 64))1 << 200;
    (unsigned _BitInt(sizeof(int) * 8))4294967297ULL;
    return sizeof(int) + _Alignof(double);
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
// IR-NEXT:     fn %0 @constants() -> u64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="unsigned long"] [c="unsigned long(void)"] {
// IR-NEXT:         add<i32, overflow=ub>(const<i32>(2), mul<i32, overflow=ub>(const<i32>(3), const<i32>(4)));
// IR-NEXT:         add<u32, overflow=wrap>(const<u32>(4294967295), const<u32>(1));
// IR-NEXT:         div<i32, by_zero=ub, min_by_neg_one=ub>(neg<i32, overflow=ub>(const<i32>(7)), const<i32>(3));
// IR-NEXT:         rem<i32, by_zero=ub, min_by_neg_one=ub>(neg<i32, overflow=ub>(const<i32>(7)), const<i32>(3));
// IR-NEXT:         reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(257)));
// IR-NEXT:         lt<i32>(const<i32>(1), const<i32>(2));
// IR-NEXT:         logical_and<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(0)), const<i32>(0)));
// IR-NEXT:         conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), const<i32>(9), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(0)));
// IR-NEXT:         div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(0));
// IR-NEXT:         add<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1));
// IR-NEXT:         shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(32));
// IR-NEXT:         mul<u64, overflow=wrap>(const<u64>(4) [size_of="i32"], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))));
// IR-NEXT:         shl<i256b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i256b, reason=explicit>(const<i32>(1)), const<i32>(200));
// IR-NEXT:         truncate<u32b, reason=explicit, fits=unknown>(const<u64>(4294967297));
// IR-NEXT:         return add<u64, overflow=wrap>(const<u64>(4) [size_of="i32"], const<u64>(8) [align_of="f64"]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
