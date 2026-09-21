// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -fwrapv

void operators(int s, unsigned u, short small, unsigned long amount, double f) {
    s + s;
    s - s;
    s * s;
    -s;
    u + u;
    u - u;
    u * u;
    -u;
    s / s;
    s % s;
    u / u;
    u % u;
    s << amount;
    u << amount;
    s >> amount;
    u >> amount;
    small << small;
    s & s;
    ~s;
    f / f;
    s /= s;
    u %= u;
    s <<= amount;
    u >>= amount;
    s++;
    --u;
    1 / 0;
    (-2147483647 - 1) % -1;
    -1 << 1;
    1U << 32;
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
// IR-NEXT:     fn %0 @operators(%1 s: i32, %2 u: u32, %3 small: i16, %4 amount: u64, %5 f: f64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         add<i32, overflow=wrap>(read<i32>(%1), read<i32>(%1));
// IR-NEXT:         sub<i32, overflow=wrap>(read<i32>(%1), read<i32>(%1));
// IR-NEXT:         mul<i32, overflow=wrap>(read<i32>(%1), read<i32>(%1));
// IR-NEXT:         neg<i32, overflow=wrap>(read<i32>(%1));
// IR-NEXT:         add<u32, overflow=wrap>(read<u32>(%2), read<u32>(%2));
// IR-NEXT:         sub<u32, overflow=wrap>(read<u32>(%2), read<u32>(%2));
// IR-NEXT:         mul<u32, overflow=wrap>(read<u32>(%2), read<u32>(%2));
// IR-NEXT:         neg<u32, overflow=wrap>(read<u32>(%2));
// IR-NEXT:         div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%1), read<i32>(%1));
// IR-NEXT:         rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%1), read<i32>(%1));
// IR-NEXT:         div<u32, by_zero=ub>(read<u32>(%2), read<u32>(%2));
// IR-NEXT:         rem<u32, by_zero=ub>(read<u32>(%2), read<u32>(%2));
// IR-NEXT:         shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%1), read<u64>(%4));
// IR-NEXT:         shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%2), read<u64>(%4));
// IR-NEXT:         shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%1), read<u64>(%4));
// IR-NEXT:         shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%2), read<u64>(%4));
// IR-NEXT:         shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%3)), widen<i32, reason=promotion>(read<i16>(%3)));
// IR-NEXT:         and<i32>(read<i32>(%1), read<i32>(%1));
// IR-NEXT:         not<i32>(read<i32>(%1));
// IR-NEXT:         div<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%5), read<f64>(%5));
// IR-NEXT:         let %6: i32 [synthetic] = read<i32>(%1);
// IR-NEXT:         let %7: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%6), read<i32>(%1));
// IR-NEXT:         write<i32>(%1, read<i32>(%7));
// IR-NEXT:         let %8: u32 [synthetic] = read<u32>(%2);
// IR-NEXT:         let %9: u32 [synthetic] = rem<u32, by_zero=ub>(read<u32>(%8), read<u32>(%2));
// IR-NEXT:         write<u32>(%2, read<u32>(%9));
// IR-NEXT:         let %10: i32 [synthetic] = read<i32>(%1);
// IR-NEXT:         let %11: i32 [synthetic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%10), read<u64>(%4));
// IR-NEXT:         write<i32>(%1, read<i32>(%11));
// IR-NEXT:         let %12: u32 [synthetic] = read<u32>(%2);
// IR-NEXT:         let %13: u32 [synthetic] = shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%12), read<u64>(%4));
// IR-NEXT:         write<u32>(%2, read<u32>(%13));
// IR-NEXT:         let %14: i32 [synthetic] = read<i32>(%1);
// IR-NEXT:         let %15: i32 [synthetic] = add<i32, overflow=wrap>(read<i32>(%14), const<i32>(1));
// IR-NEXT:         write<i32>(%1, read<i32>(%15));
// IR-NEXT:         let %16: u32 [synthetic] = read<u32>(%2);
// IR-NEXT:         let %17: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%16), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// IR-NEXT:         write<u32>(%2, read<u32>(%17));
// IR-NEXT:         div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(0));
// IR-NEXT:         rem<i32, by_zero=ub, min_by_neg_one=ub>(sub<i32, overflow=wrap>(neg<i32, overflow=wrap>(const<i32>(2147483647)), const<i32>(1)), neg<i32, overflow=wrap>(const<i32>(1)));
// IR-NEXT:         shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i32, overflow=wrap>(const<i32>(1)), const<i32>(1));
// IR-NEXT:         shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(32));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
