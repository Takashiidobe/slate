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
// IR-NEXT:     fn %[[VALUE_operators:[0-9]+]] @operators(%[[VALUE_s:[0-9]+]] s: i32, %[[VALUE_u:[0-9]+]] u: u32, %[[VALUE_small:[0-9]+]] small: i16, %[[VALUE_amount:[0-9]+]] amount: u64, %[[VALUE_f:[0-9]+]] f: f64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         add<i32, overflow=wrap>(read<i32>(%[[VALUE_s]]), read<i32>(%[[VALUE_s]]));
// IR-NEXT:         sub<i32, overflow=wrap>(read<i32>(%[[VALUE_s]]), read<i32>(%[[VALUE_s]]));
// IR-NEXT:         mul<i32, overflow=wrap>(read<i32>(%[[VALUE_s]]), read<i32>(%[[VALUE_s]]));
// IR-NEXT:         neg<i32, overflow=wrap>(read<i32>(%[[VALUE_s]]));
// IR-NEXT:         add<u32, overflow=wrap>(read<u32>(%[[VALUE_u]]), read<u32>(%[[VALUE_u]]));
// IR-NEXT:         sub<u32, overflow=wrap>(read<u32>(%[[VALUE_u]]), read<u32>(%[[VALUE_u]]));
// IR-NEXT:         mul<u32, overflow=wrap>(read<u32>(%[[VALUE_u]]), read<u32>(%[[VALUE_u]]));
// IR-NEXT:         neg<u32, overflow=wrap>(read<u32>(%[[VALUE_u]]));
// IR-NEXT:         div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_s]]), read<i32>(%[[VALUE_s]]));
// IR-NEXT:         rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_s]]), read<i32>(%[[VALUE_s]]));
// IR-NEXT:         div<u32, by_zero=ub>(read<u32>(%[[VALUE_u]]), read<u32>(%[[VALUE_u]]));
// IR-NEXT:         rem<u32, by_zero=ub>(read<u32>(%[[VALUE_u]]), read<u32>(%[[VALUE_u]]));
// IR-NEXT:         shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_s]]), read<u64>(%[[VALUE_amount]]));
// IR-NEXT:         shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_u]]), read<u64>(%[[VALUE_amount]]));
// IR-NEXT:         shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_s]]), read<u64>(%[[VALUE_amount]]));
// IR-NEXT:         shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_u]]), read<u64>(%[[VALUE_amount]]));
// IR-NEXT:         shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_small]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_small]])));
// IR-NEXT:         and<i32>(read<i32>(%[[VALUE_s]]), read<i32>(%[[VALUE_s]]));
// IR-NEXT:         not<i32>(read<i32>(%[[VALUE_s]]));
// IR-NEXT:         div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_f]]), read<f64>(%[[VALUE_f]]));
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE0]]), read<i32>(%[[VALUE_s]]));
// IR-NEXT:         write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_u]]);
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: u32 [synthetic] = rem<u32, by_zero=ub>(read<u32>(%[[VALUE2]]), read<u32>(%[[VALUE_u]]));
// IR-NEXT:         write<u32>(%[[VALUE_u]], read<u32>(%[[VALUE3]]));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE4]]), read<u64>(%[[VALUE_amount]]));
// IR-NEXT:         write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE5]]));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_u]]);
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: u32 [synthetic] = shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE6]]), read<u64>(%[[VALUE_amount]]));
// IR-NEXT:         write<u32>(%[[VALUE_u]], read<u32>(%[[VALUE7]]));
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=wrap>(read<i32>(%[[VALUE8]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE9]]));
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_u]]);
// IR-NEXT:         let %[[VALUE11:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%[[VALUE10]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// IR-NEXT:         write<u32>(%[[VALUE_u]], read<u32>(%[[VALUE11]]));
// IR-NEXT:         div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(0));
// IR-NEXT:         rem<i32, by_zero=ub, min_by_neg_one=ub>(sub<i32, overflow=wrap>(neg<i32, overflow=wrap>(const<i32>(2147483647)), const<i32>(1)), neg<i32, overflow=wrap>(const<i32>(1)));
// IR-NEXT:         shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i32, overflow=wrap>(const<i32>(1)), const<i32>(1));
// IR-NEXT:         shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(32));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
