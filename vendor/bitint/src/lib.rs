#![allow(dead_code)]

use std::cmp::Ordering;
use std::ops::{Add, Mul, Neg, Sub};

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
        if neg { magnitude.wrapping_neg() } else { magnitude }
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
