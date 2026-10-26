mod __slate_rounding {
    fn pow2_f64(shift: u32) -> f64 {
        f64::from_bits((1023 + shift as u64) << 52)
    }

    fn pow2_f32(shift: u32) -> f32 {
        f32::from_bits((127 + shift) << 23)
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

    pub fn i128_to_f64(x: i128, convert: unsafe extern "C" fn(i64) -> f64) -> f64 {
        if let Ok(small) = i64::try_from(x) {
            return unsafe { convert(small) };
        }
        let (reduced, shift) = reduce_i128(x);
        unsafe { convert(reduced) * pow2_f64(shift) }
    }

    pub fn i128_to_f32(x: i128, convert: unsafe extern "C" fn(i64) -> f32) -> f32 {
        if let Ok(small) = i64::try_from(x) {
            return unsafe { convert(small) };
        }
        let (reduced, shift) = reduce_i128(x);
        unsafe { convert(reduced) * pow2_f32(shift) }
    }

    pub fn u128_to_f64(x: u128, convert: unsafe extern "C" fn(i64) -> f64) -> f64 {
        if let Ok(small) = i64::try_from(x) {
            return unsafe { convert(small) };
        }
        let (reduced, shift) = reduce_u128(x);
        unsafe { convert(reduced) * pow2_f64(shift) }
    }

    pub fn u128_to_f32(x: u128, convert: unsafe extern "C" fn(i64) -> f32) -> f32 {
        if let Ok(small) = i64::try_from(x) {
            return unsafe { convert(small) };
        }
        let (reduced, shift) = reduce_u128(x);
        unsafe { convert(reduced) * pow2_f32(shift) }
    }
}
