use std::cmp::Ordering;
use std::ops::{
    Add, AddAssign, BitAnd, BitAndAssign, BitOr, BitOrAssign, BitXor, BitXorAssign, Div, DivAssign,
    Mul, MulAssign, Neg, Not, Rem, RemAssign, Shl, ShlAssign, Shr, ShrAssign, Sub, SubAssign,
};

// BITS is C's exact _BitInt(N) width; LIMBS is the caller-computed
// ceil(BITS / 64) little-endian u64 word count. Every stored value keeps
// bits above BITS zeroed so wrapping arithmetic can truncate at the u64
// boundary (dropping limbs past index LIMBS - 1) and mask once at the end
// instead of tracking BITS inside every intermediate step.
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct BUint<const BITS: usize, const LIMBS: usize> {
    limbs: [u64; LIMBS],
}

impl<const BITS: usize, const LIMBS: usize> BUint<BITS, LIMBS> {
    const TOP_BITS: u32 = (BITS % 64) as u32;
    const TOP_MASK: u64 = if Self::TOP_BITS == 0 {
        u64::MAX
    } else {
        (1u64 << Self::TOP_BITS) - 1
    };

    pub const ZERO: Self = Self { limbs: [0; LIMBS] };
    pub const ONE: Self = Self::from_low_limb(1);

    const fn from_low_limb(low: u64) -> Self {
        let mut limbs = [0u64; LIMBS];
        if LIMBS > 0 {
            limbs[0] = low;
        }
        Self::masked(limbs)
    }

    const fn masked(mut limbs: [u64; LIMBS]) -> Self {
        if LIMBS > 0 {
            limbs[LIMBS - 1] &= Self::TOP_MASK;
        }
        Self { limbs }
    }

    pub const fn from_i128(v: i128) -> Self {
        let fill: u64 = if v < 0 { u64::MAX } else { 0 };
        let mut limbs = [fill; LIMBS];
        let bits = v as u128;
        if LIMBS > 0 {
            limbs[0] = bits as u64;
        }
        if LIMBS > 1 {
            limbs[1] = (bits >> 64) as u64;
        }
        Self::masked(limbs)
    }

    pub const fn from_u128(v: u128) -> Self {
        let mut limbs = [0; LIMBS];
        if LIMBS > 0 {
            limbs[0] = v as u64;
        }
        if LIMBS > 1 {
            limbs[1] = (v >> 64) as u64;
        }
        Self::masked(limbs)
    }

    pub const fn to_u128(self) -> u128 {
        let mut value = 0;
        if LIMBS > 0 {
            value = self.limbs[0] as u128;
        }
        if LIMBS > 1 {
            value |= (self.limbs[1] as u128) << 64;
        }
        value
    }

    pub const fn from_decimal_str(s: &str) -> Self {
        let bytes = s.as_bytes();
        let ten = Self::from_low_limb(10);
        let mut acc = Self::ZERO;
        let mut i = 0;
        while i < bytes.len() {
            let digit = Self::from_low_limb((bytes[i] - b'0') as u64);
            acc = acc.wrapping_mul(ten).wrapping_add(digit);
            i += 1;
        }
        acc
    }

    pub const fn wrapping_add(self, rhs: Self) -> Self {
        let mut out = [0u64; LIMBS];
        let mut carry: u128 = 0;
        let mut i = 0;
        while i < LIMBS {
            let sum = self.limbs[i] as u128 + rhs.limbs[i] as u128 + carry;
            out[i] = sum as u64;
            carry = sum >> 64;
            i += 1;
        }
        Self::masked(out)
    }

    pub const fn wrapping_neg(self) -> Self {
        let mut inv = [0u64; LIMBS];
        let mut i = 0;
        while i < LIMBS {
            inv[i] = !self.limbs[i];
            i += 1;
        }
        Self { limbs: inv }.wrapping_add(Self::ONE)
    }

    pub const fn wrapping_sub(self, rhs: Self) -> Self {
        self.wrapping_add(rhs.wrapping_neg())
    }

    pub const fn wrapping_mul(self, rhs: Self) -> Self {
        let mut out = [0u64; LIMBS];
        let mut i = 0;
        while i < LIMBS {
            let mut carry: u128 = 0;
            let mut j = 0;
            while i + j < LIMBS {
                let idx = i + j;
                let prod =
                    (self.limbs[i] as u128) * (rhs.limbs[j] as u128) + out[idx] as u128 + carry;
                out[idx] = prod as u64;
                carry = prod >> 64;
                j += 1;
            }
            i += 1;
        }
        Self::masked(out)
    }

    const fn cmp_magnitude(&self, other: &Self) -> Ordering {
        let mut i = LIMBS;
        while i > 0 {
            i -= 1;
            if self.limbs[i] != other.limbs[i] {
                return if self.limbs[i] < other.limbs[i] {
                    Ordering::Less
                } else {
                    Ordering::Greater
                };
            }
        }
        Ordering::Equal
    }

    const fn is_zero(&self) -> bool {
        let mut i = 0;
        while i < LIMBS {
            if self.limbs[i] != 0 {
                return false;
            }
            i += 1;
        }
        true
    }

    const fn bit_len(&self) -> usize {
        let mut i = LIMBS;

        while i > 0 {
            i -= 1;

            let limb = self.limbs[i];
            if limb != 0 {
                return i * 64 + (64 - limb.leading_zeros() as usize);
            }
        }

        0
    }

    const fn shl_trunc(self, shift: usize) -> Self {
        let mut out = [0u64; LIMBS];

        let limb_shift = shift / 64;
        let bit_shift = shift % 64;

        let mut i = 0;
        while i < LIMBS {
            let dst = i + limb_shift;

            if dst < LIMBS {
                out[dst] |= self.limbs[i] << bit_shift;

                if bit_shift != 0 && dst + 1 < LIMBS {
                    out[dst + 1] |= self.limbs[i] >> (64 - bit_shift);
                }
            }

            i += 1;
        }

        Self::masked(out)
    }

    const fn shr_one(self) -> Self {
        let mut out = [0u64; LIMBS];
        let mut carry = 0u64;

        let mut i = LIMBS;
        while i > 0 {
            i -= 1;

            out[i] = (self.limbs[i] >> 1) | carry;
            carry = self.limbs[i] << 63;
        }

        Self { limbs: out }
    }

    const fn div_rem_unsigned(self, rhs: Self) -> (Self, Self) {
        if rhs.is_zero() {
            panic!("attempt to divide by zero");
        }

        match self.cmp_magnitude(&rhs) {
            Ordering::Less => {
                return (Self::ZERO, self);
            }
            Ordering::Equal => {
                return (Self::ONE, Self::ZERO);
            }
            Ordering::Greater => {}
        }

        let lhs_bits = self.bit_len();
        let rhs_bits = rhs.bit_len();

        let mut shift = lhs_bits - rhs_bits;
        let mut divisor = rhs.shl_trunc(shift);
        let mut remainder = self;
        let mut quotient = [0u64; LIMBS];

        loop {
            match remainder.cmp_magnitude(&divisor) {
                Ordering::Less => {}
                Ordering::Equal | Ordering::Greater => {
                    remainder = remainder.wrapping_sub(divisor);

                    let limb = shift / 64;
                    let bit = shift % 64;

                    quotient[limb] |= 1u64 << bit;
                }
            }

            if shift == 0 {
                break;
            }

            divisor = divisor.shr_one();
            shift -= 1;
        }

        (Self::masked(quotient), remainder)
    }

    pub const fn wrapping_div(self, rhs: Self) -> Self {
        self.div_rem_unsigned(rhs).0
    }

    pub const fn wrapping_rem(self, rhs: Self) -> Self {
        self.div_rem_unsigned(rhs).1
    }

    pub const fn wrapping_div_rem(self, rhs: Self) -> (Self, Self) {
        self.div_rem_unsigned(rhs)
    }

    pub const fn bitand_bits(self, rhs: Self) -> Self {
        let mut out = [0u64; LIMBS];

        let mut i = 0;
        while i < LIMBS {
            out[i] = self.limbs[i] & rhs.limbs[i];
            i += 1;
        }

        Self::masked(out)
    }

    pub const fn bitor_bits(self, rhs: Self) -> Self {
        let mut out = [0u64; LIMBS];

        let mut i = 0;
        while i < LIMBS {
            out[i] = self.limbs[i] | rhs.limbs[i];
            i += 1;
        }

        Self::masked(out)
    }

    pub const fn bitxor_bits(self, rhs: Self) -> Self {
        let mut out = [0u64; LIMBS];

        let mut i = 0;
        while i < LIMBS {
            out[i] = self.limbs[i] ^ rhs.limbs[i];
            i += 1;
        }

        Self::masked(out)
    }

    pub const fn bitnot(self) -> Self {
        let mut out = [0u64; LIMBS];

        let mut i = 0;
        while i < LIMBS {
            out[i] = !self.limbs[i];
            i += 1;
        }

        Self::masked(out)
    }

    pub const fn shl_bits(self, shift: usize) -> Self {
        if shift >= BITS {
            return Self::ZERO;
        }

        if shift == 0 {
            return self;
        }

        let limb_shift = shift / 64;
        let bit_shift = shift % 64;

        let mut out = [0u64; LIMBS];

        let mut dst = LIMBS;
        while dst > 0 {
            dst -= 1;

            if dst < limb_shift {
                continue;
            }

            let src = dst - limb_shift;

            out[dst] |= self.limbs[src] << bit_shift;

            if bit_shift != 0 && src > 0 {
                out[dst] |= self.limbs[src - 1] >> (64 - bit_shift);
            }
        }

        Self::masked(out)
    }

    pub const fn shr_bits(self, shift: usize) -> Self {
        if shift >= BITS {
            return Self::ZERO;
        }

        if shift == 0 {
            return self;
        }

        let limb_shift = shift / 64;
        let bit_shift = shift % 64;

        let mut out = [0u64; LIMBS];

        let mut dst = 0;
        while dst < LIMBS {
            let src = dst + limb_shift;

            if src < LIMBS {
                out[dst] |= self.limbs[src] >> bit_shift;

                if bit_shift != 0 && src + 1 < LIMBS {
                    out[dst] |= self.limbs[src + 1] << (64 - bit_shift);
                }
            }

            dst += 1;
        }

        Self::masked(out)
    }
}

impl<const BITS: usize, const LIMBS: usize> Add for BUint<BITS, LIMBS> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self {
        self.wrapping_add(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Sub for BUint<BITS, LIMBS> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self {
        self.wrapping_sub(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Mul for BUint<BITS, LIMBS> {
    type Output = Self;
    fn mul(self, rhs: Self) -> Self {
        self.wrapping_mul(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Neg for BUint<BITS, LIMBS> {
    type Output = Self;
    fn neg(self) -> Self {
        self.wrapping_neg()
    }
}

impl<const BITS: usize, const LIMBS: usize> Div for BUint<BITS, LIMBS> {
    type Output = Self;

    fn div(self, rhs: Self) -> Self::Output {
        self.wrapping_div(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Rem for BUint<BITS, LIMBS> {
    type Output = Self;

    fn rem(self, rhs: Self) -> Self::Output {
        self.wrapping_rem(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> AddAssign for BUint<BITS, LIMBS> {
    fn add_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_add(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> SubAssign for BUint<BITS, LIMBS> {
    fn sub_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_sub(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> MulAssign for BUint<BITS, LIMBS> {
    fn mul_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_mul(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> DivAssign for BUint<BITS, LIMBS> {
    fn div_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_div(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> RemAssign for BUint<BITS, LIMBS> {
    fn rem_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_rem(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> PartialOrd for BUint<BITS, LIMBS> {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl<const BITS: usize, const LIMBS: usize> Ord for BUint<BITS, LIMBS> {
    fn cmp(&self, other: &Self) -> Ordering {
        self.cmp_magnitude(other)
    }
}

impl<const BITS: usize, const LIMBS: usize> Not for BUint<BITS, LIMBS> {
    type Output = Self;

    fn not(self) -> Self::Output {
        self.bitnot()
    }
}

impl<const BITS: usize, const LIMBS: usize> BitAnd for BUint<BITS, LIMBS> {
    type Output = Self;

    fn bitand(self, rhs: Self) -> Self::Output {
        self.bitand_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> BitOr for BUint<BITS, LIMBS> {
    type Output = Self;

    fn bitor(self, rhs: Self) -> Self::Output {
        self.bitor_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> BitXor for BUint<BITS, LIMBS> {
    type Output = Self;

    fn bitxor(self, rhs: Self) -> Self::Output {
        self.bitxor_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Shl<usize> for BUint<BITS, LIMBS> {
    type Output = Self;

    fn shl(self, rhs: usize) -> Self::Output {
        self.shl_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Shr<usize> for BUint<BITS, LIMBS> {
    type Output = Self;

    fn shr(self, rhs: usize) -> Self::Output {
        self.shr_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> BitAndAssign for BUint<BITS, LIMBS> {
    fn bitand_assign(&mut self, rhs: Self) {
        *self = (*self).bitand_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> BitOrAssign for BUint<BITS, LIMBS> {
    fn bitor_assign(&mut self, rhs: Self) {
        *self = (*self).bitor_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> BitXorAssign for BUint<BITS, LIMBS> {
    fn bitxor_assign(&mut self, rhs: Self) {
        *self = (*self).bitxor_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> ShlAssign<usize> for BUint<BITS, LIMBS> {
    fn shl_assign(&mut self, rhs: usize) {
        *self = (*self).shl_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> ShrAssign<usize> for BUint<BITS, LIMBS> {
    fn shr_assign(&mut self, rhs: usize) {
        *self = (*self).shr_bits(rhs);
    }
}

// Shares BUint's bit pattern and wrapping arithmetic (two's-complement
// wraparound is identical either way); only comparison needs to be
// sign-aware.
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct BInt<const BITS: usize, const LIMBS: usize> {
    bits: BUint<BITS, LIMBS>,
}

impl<const BITS: usize, const LIMBS: usize> BInt<BITS, LIMBS> {
    const SIGN_LIMB: usize = (BITS - 1) / 64;
    const SIGN_BIT: u32 = ((BITS - 1) % 64) as u32;

    pub const ZERO: Self = Self { bits: BUint::ZERO };

    pub const fn from_i128(v: i128) -> Self {
        Self {
            bits: BUint::from_i128(v),
        }
    }

    pub const fn from_u128(v: u128) -> Self {
        Self {
            bits: BUint::from_u128(v),
        }
    }

    pub const fn to_i128(self) -> i128 {
        let mut value = self.bits.to_u128();
        if BITS < 128 && self.is_negative() {
            value |= u128::MAX << (BITS % 128);
        }
        value as i128
    }

    pub const fn to_u128(self) -> u128 {
        self.bits.to_u128()
    }

    pub const fn from_decimal_str(s: &str) -> Self {
        let bytes = s.as_bytes();
        let neg = !bytes.is_empty() && bytes[0] == b'-';
        let ten = BUint::<BITS, LIMBS>::from_low_limb(10);
        let mut acc = BUint::<BITS, LIMBS>::ZERO;
        let mut i = if neg { 1 } else { 0 };
        while i < bytes.len() {
            let digit = BUint::<BITS, LIMBS>::from_low_limb((bytes[i] - b'0') as u64);
            acc = acc.wrapping_mul(ten).wrapping_add(digit);
            i += 1;
        }
        let magnitude = Self { bits: acc };
        if neg {
            magnitude.wrapping_neg()
        } else {
            magnitude
        }
    }

    const fn is_negative(&self) -> bool {
        (self.bits.limbs[Self::SIGN_LIMB] >> Self::SIGN_BIT) & 1 == 1
    }

    pub const fn wrapping_add(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.wrapping_add(rhs.bits),
        }
    }

    pub const fn wrapping_sub(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.wrapping_sub(rhs.bits),
        }
    }

    pub const fn wrapping_mul(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.wrapping_mul(rhs.bits),
        }
    }

    pub const fn wrapping_neg(self) -> Self {
        Self {
            bits: self.bits.wrapping_neg(),
        }
    }

    pub const fn wrapping_div_rem(self, rhs: Self) -> (Self, Self) {
        let lhs_negative = self.is_negative();
        let rhs_negative = rhs.is_negative();

        // Convert the two's-complement values into unsigned magnitudes.
        //
        // This also works for MIN:
        //
        //     -MIN == MIN
        //
        // as a BInt bit pattern, but that same pattern interpreted as
        // BUint is exactly 2^(BITS - 1), which is the magnitude we want.
        let lhs_magnitude = if lhs_negative {
            self.bits.wrapping_neg()
        } else {
            self.bits
        };

        let rhs_magnitude = if rhs_negative {
            rhs.bits.wrapping_neg()
        } else {
            rhs.bits
        };

        let (quotient, remainder) = lhs_magnitude.div_rem_unsigned(rhs_magnitude);

        let quotient = if lhs_negative != rhs_negative {
            quotient.wrapping_neg()
        } else {
            quotient
        };

        // Rust/C signed remainder follows the dividend's sign.
        let remainder = if lhs_negative {
            remainder.wrapping_neg()
        } else {
            remainder
        };

        (Self { bits: quotient }, Self { bits: remainder })
    }

    pub const fn wrapping_div(self, rhs: Self) -> Self {
        self.wrapping_div_rem(rhs).0
    }

    pub const fn wrapping_rem(self, rhs: Self) -> Self {
        self.wrapping_div_rem(rhs).1
    }

    pub const fn bitand_bits(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.bitand_bits(rhs.bits),
        }
    }

    pub const fn bitor_bits(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.bitor_bits(rhs.bits),
        }
    }

    pub const fn bitxor_bits(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.bitxor_bits(rhs.bits),
        }
    }

    pub const fn bitnot(self) -> Self {
        Self {
            bits: self.bits.bitnot(),
        }
    }

    pub const fn shl_bits(self, shift: usize) -> Self {
        Self {
            bits: self.bits.shl_bits(shift),
        }
    }

    pub const fn shr_bits(self, shift: usize) -> Self {
        let negative = self.is_negative();

        if shift == 0 {
            return self;
        }

        if shift >= BITS {
            return if negative {
                Self {
                    bits: BUint::masked([u64::MAX; LIMBS]),
                }
            } else {
                Self::ZERO
            };
        }

        // First perform an unsigned/logical right shift.
        let mut out = self.bits.shr_bits(shift);

        // A positive signed value needs no additional work.
        if !negative {
            return Self { bits: out };
        }

        // For a negative value, fill bits
        //
        //     [BITS - shift, BITS)
        //
        // with ones.
        let first_bit = BITS - shift;
        let first_limb = first_bit / 64;
        let first_offset = first_bit % 64;

        let mut i = first_limb;

        if i < LIMBS {
            out.limbs[i] |= u64::MAX << first_offset;
            i += 1;
        }

        while i < LIMBS {
            out.limbs[i] = u64::MAX;
            i += 1;
        }

        Self {
            bits: BUint::masked(out.limbs),
        }
    }
}

impl<const BITS: usize, const LIMBS: usize> Add for BInt<BITS, LIMBS> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self {
        self.wrapping_add(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Sub for BInt<BITS, LIMBS> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self {
        self.wrapping_sub(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Mul for BInt<BITS, LIMBS> {
    type Output = Self;
    fn mul(self, rhs: Self) -> Self {
        self.wrapping_mul(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Neg for BInt<BITS, LIMBS> {
    type Output = Self;
    fn neg(self) -> Self {
        self.wrapping_neg()
    }
}

impl<const BITS: usize, const LIMBS: usize> PartialOrd for BInt<BITS, LIMBS> {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl<const BITS: usize, const LIMBS: usize> Ord for BInt<BITS, LIMBS> {
    fn cmp(&self, other: &Self) -> Ordering {
        match (self.is_negative(), other.is_negative()) {
            (true, false) => Ordering::Less,
            (false, true) => Ordering::Greater,
            _ => self.bits.cmp_magnitude(&other.bits),
        }
    }
}

impl<const BITS: usize, const LIMBS: usize> Div for BInt<BITS, LIMBS> {
    type Output = Self;

    fn div(self, rhs: Self) -> Self {
        self.wrapping_div(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Rem for BInt<BITS, LIMBS> {
    type Output = Self;

    fn rem(self, rhs: Self) -> Self {
        self.wrapping_rem(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> BitAnd for BInt<BITS, LIMBS> {
    type Output = Self;

    fn bitand(self, rhs: Self) -> Self {
        self.bitand_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> BitOr for BInt<BITS, LIMBS> {
    type Output = Self;

    fn bitor(self, rhs: Self) -> Self {
        self.bitor_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> BitXor for BInt<BITS, LIMBS> {
    type Output = Self;

    fn bitxor(self, rhs: Self) -> Self {
        self.bitxor_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> AddAssign for BInt<BITS, LIMBS> {
    fn add_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_add(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> SubAssign for BInt<BITS, LIMBS> {
    fn sub_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_sub(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> MulAssign for BInt<BITS, LIMBS> {
    fn mul_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_mul(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> DivAssign for BInt<BITS, LIMBS> {
    fn div_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_div(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> RemAssign for BInt<BITS, LIMBS> {
    fn rem_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_rem(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> BitAndAssign for BInt<BITS, LIMBS> {
    fn bitand_assign(&mut self, rhs: Self) {
        *self = (*self).bitand_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> BitOrAssign for BInt<BITS, LIMBS> {
    fn bitor_assign(&mut self, rhs: Self) {
        *self = (*self).bitor_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> BitXorAssign for BInt<BITS, LIMBS> {
    fn bitxor_assign(&mut self, rhs: Self) {
        *self = (*self).bitxor_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> Not for BInt<BITS, LIMBS> {
    type Output = Self;

    fn not(self) -> Self::Output {
        self.bitnot()
    }
}

impl<const BITS: usize, const LIMBS: usize> Shl<usize> for BInt<BITS, LIMBS> {
    type Output = Self;

    fn shl(self, rhs: usize) -> Self::Output {
        self.shl_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> Shr<usize> for BInt<BITS, LIMBS> {
    type Output = Self;

    fn shr(self, rhs: usize) -> Self::Output {
        self.shr_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize> ShlAssign<usize> for BInt<BITS, LIMBS> {
    fn shl_assign(&mut self, rhs: usize) {
        *self = (*self).shl_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize> ShrAssign<usize> for BInt<BITS, LIMBS> {
    fn shr_assign(&mut self, rhs: usize) {
        *self = (*self).shr_bits(rhs);
    }
}
