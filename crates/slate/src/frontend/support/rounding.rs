mod __slate_rounding {
    use std::hint::black_box;

    fn pow2_f64(shift: u32) -> f64 {
        f64::from_bits((1023 + shift as u64) << 52)
    }

    fn pow2_f32(shift: u32) -> f32 {
        f32::from_bits((127 + shift) << 23)
    }

    pub fn u64_to_f64(x: u64) -> f64 {
        if (x as i64) >= 0 {
            black_box(x as i64) as f64
        } else {
            black_box(((x >> 1) | (x & 1)) as i64) as f64 * 2.0
        }
    }

    pub fn u64_to_f32(x: u64) -> f32 {
        if (x as i64) >= 0 {
            black_box(x as i64) as f32
        } else {
            black_box(((x >> 1) | (x & 1)) as i64) as f32 * 2.0
        }
    }

    fn reduce_i128(x: i128) -> (i64, u32) {
        let magnitude_bits = 128 - (if x < 0 { !x } else { x }).leading_zeros();
        let shift = magnitude_bits - 62;
        let sticky = (x & ((1i128 << shift) - 1) != 0) as i64;
        (((x >> shift) as i64) | sticky, shift)
    }

    fn reduce_u128(x: u128) -> (i64, u32) {
        let shift = 128 - x.leading_zeros() - 62;
        let sticky = (x & ((1u128 << shift) - 1) != 0) as i64;
        (((x >> shift) as i64) | sticky, shift)
    }

    pub fn i128_to_f64(x: i128) -> f64 {
        if let Ok(small) = i64::try_from(x) {
            return black_box(small) as f64;
        }
        let (reduced, shift) = reduce_i128(x);
        black_box(reduced) as f64 * pow2_f64(shift)
    }

    pub fn i128_to_f32(x: i128) -> f32 {
        if let Ok(small) = i64::try_from(x) {
            return black_box(small) as f32;
        }
        let (reduced, shift) = reduce_i128(x);
        black_box(reduced) as f32 * pow2_f32(shift)
    }

    pub fn u128_to_f64(x: u128) -> f64 {
        if let Ok(small) = u64::try_from(x) {
            return u64_to_f64(small);
        }
        let (reduced, shift) = reduce_u128(x);
        black_box(reduced) as f64 * pow2_f64(shift)
    }

    pub fn u128_to_f32(x: u128) -> f32 {
        if let Ok(small) = u64::try_from(x) {
            return u64_to_f32(small);
        }
        let (reduced, shift) = reduce_u128(x);
        black_box(reduced) as f32 * pow2_f32(shift)
    }
}
