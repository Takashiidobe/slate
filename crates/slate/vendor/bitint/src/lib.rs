use std::cmp::Ordering;
use std::ops::{
    Add, AddAssign, BitAnd, BitAndAssign, BitOr, BitOrAssign, BitXor, BitXorAssign, Div, DivAssign,
    Mul, MulAssign, Neg, Not, Rem, RemAssign, Shl, ShlAssign, Shr, ShrAssign, Sub, SubAssign,
};

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct BUint<const BITS: usize, const LIMBS: usize, const BYTES: usize> {
    bytes: [u8; BYTES],
}

const fn bytes_to_limbs<const LIMBS: usize, const BYTES: usize>(
    bytes: &[u8; BYTES],
) -> [u64; LIMBS] {
    let mut limbs = [0u64; LIMBS];
    let mut i = 0;
    while i < BYTES {
        limbs[i / 8] |= (bytes[i] as u64) << ((i % 8) * 8);
        i += 1;
    }
    limbs
}

const fn limbs_to_bytes<const LIMBS: usize, const BYTES: usize>(
    limbs: &[u64; LIMBS],
) -> [u8; BYTES] {
    let mut bytes = [0u8; BYTES];
    let mut i = 0;
    while i < BYTES {
        bytes[i] = (limbs[i / 8] >> ((i % 8) * 8)) as u8;
        i += 1;
    }
    bytes
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BUint<BITS, LIMBS, BYTES> {
    pub const ZERO: Self = Self { bytes: [0; BYTES] };
    pub const ONE: Self = Self::from_limbs(BUintLimbs::<BITS, LIMBS>::ONE);

    const fn from_limbs(v: BUintLimbs<BITS, LIMBS>) -> Self {
        Self {
            bytes: limbs_to_bytes(&v.limbs),
        }
    }

    const fn to_limbs(self) -> BUintLimbs<BITS, LIMBS> {
        BUintLimbs::masked(bytes_to_limbs(&self.bytes))
    }

    pub const fn from_i128(v: i128) -> Self {
        Self::from_limbs(BUintLimbs::from_i128(v))
    }

    pub const fn from_u128(v: u128) -> Self {
        Self::from_limbs(BUintLimbs::from_u128(v))
    }

    pub const fn to_u128(self) -> u128 {
        self.to_limbs().to_u128()
    }

    pub const fn from_decimal_str(s: &str) -> Self {
        Self::from_limbs(BUintLimbs::from_decimal_str(s))
    }

    pub const fn wrapping_add(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().wrapping_add(rhs.to_limbs()))
    }

    pub const fn wrapping_neg(self) -> Self {
        Self::from_limbs(self.to_limbs().wrapping_neg())
    }

    pub const fn wrapping_sub(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().wrapping_sub(rhs.to_limbs()))
    }

    pub const fn wrapping_mul(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().wrapping_mul(rhs.to_limbs()))
    }

    pub const fn wrapping_div(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().wrapping_div(rhs.to_limbs()))
    }

    pub const fn wrapping_rem(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().wrapping_rem(rhs.to_limbs()))
    }

    pub const fn wrapping_div_rem(self, rhs: Self) -> (Self, Self) {
        let (q, r) = self.to_limbs().wrapping_div_rem(rhs.to_limbs());
        (Self::from_limbs(q), Self::from_limbs(r))
    }

    pub const fn bitand_bits(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().bitand_bits(rhs.to_limbs()))
    }

    pub const fn bitor_bits(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().bitor_bits(rhs.to_limbs()))
    }

    pub const fn bitxor_bits(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().bitxor_bits(rhs.to_limbs()))
    }

    pub const fn bitnot(self) -> Self {
        Self::from_limbs(self.to_limbs().bitnot())
    }

    pub const fn shl_bits(self, shift: usize) -> Self {
        Self::from_limbs(self.to_limbs().shl_bits(shift))
    }

    pub const fn shr_bits(self, shift: usize) -> Self {
        Self::from_limbs(self.to_limbs().shr_bits(shift))
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Add for BUint<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self {
        self.wrapping_add(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Sub for BUint<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self {
        self.wrapping_sub(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Mul for BUint<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn mul(self, rhs: Self) -> Self {
        self.wrapping_mul(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Neg for BUint<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn neg(self) -> Self {
        self.wrapping_neg()
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Div for BUint<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn div(self, rhs: Self) -> Self::Output {
        self.wrapping_div(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Rem for BUint<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn rem(self, rhs: Self) -> Self::Output {
        self.wrapping_rem(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> AddAssign
    for BUint<BITS, LIMBS, BYTES>
{
    fn add_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_add(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> SubAssign
    for BUint<BITS, LIMBS, BYTES>
{
    fn sub_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_sub(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> MulAssign
    for BUint<BITS, LIMBS, BYTES>
{
    fn mul_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_mul(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> DivAssign
    for BUint<BITS, LIMBS, BYTES>
{
    fn div_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_div(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> RemAssign
    for BUint<BITS, LIMBS, BYTES>
{
    fn rem_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_rem(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> PartialOrd
    for BUint<BITS, LIMBS, BYTES>
{
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Ord for BUint<BITS, LIMBS, BYTES> {
    fn cmp(&self, other: &Self) -> Ordering {
        self.to_limbs().cmp_magnitude(&other.to_limbs())
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Not for BUint<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn not(self) -> Self::Output {
        self.bitnot()
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitAnd
    for BUint<BITS, LIMBS, BYTES>
{
    type Output = Self;
    fn bitand(self, rhs: Self) -> Self::Output {
        self.bitand_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitOr
    for BUint<BITS, LIMBS, BYTES>
{
    type Output = Self;
    fn bitor(self, rhs: Self) -> Self::Output {
        self.bitor_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitXor
    for BUint<BITS, LIMBS, BYTES>
{
    type Output = Self;
    fn bitxor(self, rhs: Self) -> Self::Output {
        self.bitxor_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Shl<usize>
    for BUint<BITS, LIMBS, BYTES>
{
    type Output = Self;
    fn shl(self, rhs: usize) -> Self::Output {
        self.shl_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Shr<usize>
    for BUint<BITS, LIMBS, BYTES>
{
    type Output = Self;
    fn shr(self, rhs: usize) -> Self::Output {
        self.shr_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitAndAssign
    for BUint<BITS, LIMBS, BYTES>
{
    fn bitand_assign(&mut self, rhs: Self) {
        *self = (*self).bitand_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitOrAssign
    for BUint<BITS, LIMBS, BYTES>
{
    fn bitor_assign(&mut self, rhs: Self) {
        *self = (*self).bitor_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitXorAssign
    for BUint<BITS, LIMBS, BYTES>
{
    fn bitxor_assign(&mut self, rhs: Self) {
        *self = (*self).bitxor_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> ShlAssign<usize>
    for BUint<BITS, LIMBS, BYTES>
{
    fn shl_assign(&mut self, rhs: usize) {
        *self = (*self).shl_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> ShrAssign<usize>
    for BUint<BITS, LIMBS, BYTES>
{
    fn shr_assign(&mut self, rhs: usize) {
        *self = (*self).shr_bits(rhs);
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct BInt<const BITS: usize, const LIMBS: usize, const BYTES: usize> {
    bytes: [u8; BYTES],
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BInt<BITS, LIMBS, BYTES> {
    pub const ZERO: Self = Self { bytes: [0; BYTES] };

    const fn from_limbs(v: BIntLimbs<BITS, LIMBS>) -> Self {
        Self {
            bytes: limbs_to_bytes(&v.bits.limbs),
        }
    }

    const fn to_limbs(self) -> BIntLimbs<BITS, LIMBS> {
        BIntLimbs {
            bits: BUintLimbs::masked(bytes_to_limbs(&self.bytes)),
        }
    }

    pub const fn from_i128(v: i128) -> Self {
        Self::from_limbs(BIntLimbs::from_i128(v))
    }

    pub const fn from_u128(v: u128) -> Self {
        Self::from_limbs(BIntLimbs::from_u128(v))
    }

    pub const fn to_i128(self) -> i128 {
        self.to_limbs().to_i128()
    }

    pub const fn to_u128(self) -> u128 {
        self.to_limbs().to_u128()
    }

    pub const fn from_decimal_str(s: &str) -> Self {
        Self::from_limbs(BIntLimbs::from_decimal_str(s))
    }

    pub const fn wrapping_add(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().wrapping_add(rhs.to_limbs()))
    }

    pub const fn wrapping_sub(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().wrapping_sub(rhs.to_limbs()))
    }

    pub const fn wrapping_mul(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().wrapping_mul(rhs.to_limbs()))
    }

    pub const fn wrapping_neg(self) -> Self {
        Self::from_limbs(self.to_limbs().wrapping_neg())
    }

    pub const fn wrapping_div_rem(self, rhs: Self) -> (Self, Self) {
        let (q, r) = self.to_limbs().wrapping_div_rem(rhs.to_limbs());
        (Self::from_limbs(q), Self::from_limbs(r))
    }

    pub const fn wrapping_div(self, rhs: Self) -> Self {
        self.wrapping_div_rem(rhs).0
    }

    pub const fn wrapping_rem(self, rhs: Self) -> Self {
        self.wrapping_div_rem(rhs).1
    }

    pub const fn bitand_bits(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().bitand_bits(rhs.to_limbs()))
    }

    pub const fn bitor_bits(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().bitor_bits(rhs.to_limbs()))
    }

    pub const fn bitxor_bits(self, rhs: Self) -> Self {
        Self::from_limbs(self.to_limbs().bitxor_bits(rhs.to_limbs()))
    }

    pub const fn bitnot(self) -> Self {
        Self::from_limbs(self.to_limbs().bitnot())
    }

    pub const fn shl_bits(self, shift: usize) -> Self {
        Self::from_limbs(self.to_limbs().shl_bits(shift))
    }

    pub const fn shr_bits(self, shift: usize) -> Self {
        Self::from_limbs(self.to_limbs().shr_bits(shift))
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Add for BInt<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self {
        self.wrapping_add(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Sub for BInt<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self {
        self.wrapping_sub(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Mul for BInt<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn mul(self, rhs: Self) -> Self {
        self.wrapping_mul(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Neg for BInt<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn neg(self) -> Self {
        self.wrapping_neg()
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> PartialOrd
    for BInt<BITS, LIMBS, BYTES>
{
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Ord for BInt<BITS, LIMBS, BYTES> {
    fn cmp(&self, other: &Self) -> Ordering {
        self.to_limbs().cmp(&other.to_limbs())
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Div for BInt<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn div(self, rhs: Self) -> Self {
        self.wrapping_div(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Rem for BInt<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn rem(self, rhs: Self) -> Self {
        self.wrapping_rem(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitAnd
    for BInt<BITS, LIMBS, BYTES>
{
    type Output = Self;
    fn bitand(self, rhs: Self) -> Self {
        self.bitand_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitOr for BInt<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn bitor(self, rhs: Self) -> Self {
        self.bitor_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitXor
    for BInt<BITS, LIMBS, BYTES>
{
    type Output = Self;
    fn bitxor(self, rhs: Self) -> Self {
        self.bitxor_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> AddAssign
    for BInt<BITS, LIMBS, BYTES>
{
    fn add_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_add(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> SubAssign
    for BInt<BITS, LIMBS, BYTES>
{
    fn sub_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_sub(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> MulAssign
    for BInt<BITS, LIMBS, BYTES>
{
    fn mul_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_mul(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> DivAssign
    for BInt<BITS, LIMBS, BYTES>
{
    fn div_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_div(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> RemAssign
    for BInt<BITS, LIMBS, BYTES>
{
    fn rem_assign(&mut self, rhs: Self) {
        *self = (*self).wrapping_rem(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitAndAssign
    for BInt<BITS, LIMBS, BYTES>
{
    fn bitand_assign(&mut self, rhs: Self) {
        *self = (*self).bitand_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitOrAssign
    for BInt<BITS, LIMBS, BYTES>
{
    fn bitor_assign(&mut self, rhs: Self) {
        *self = (*self).bitor_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> BitXorAssign
    for BInt<BITS, LIMBS, BYTES>
{
    fn bitxor_assign(&mut self, rhs: Self) {
        *self = (*self).bitxor_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Not for BInt<BITS, LIMBS, BYTES> {
    type Output = Self;
    fn not(self) -> Self::Output {
        self.bitnot()
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Shl<usize>
    for BInt<BITS, LIMBS, BYTES>
{
    type Output = Self;
    fn shl(self, rhs: usize) -> Self::Output {
        self.shl_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> Shr<usize>
    for BInt<BITS, LIMBS, BYTES>
{
    type Output = Self;
    fn shr(self, rhs: usize) -> Self::Output {
        self.shr_bits(rhs)
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> ShlAssign<usize>
    for BInt<BITS, LIMBS, BYTES>
{
    fn shl_assign(&mut self, rhs: usize) {
        *self = (*self).shl_bits(rhs);
    }
}

impl<const BITS: usize, const LIMBS: usize, const BYTES: usize> ShrAssign<usize>
    for BInt<BITS, LIMBS, BYTES>
{
    fn shr_assign(&mut self, rhs: usize) {
        *self = (*self).shr_bits(rhs);
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
struct BUintLimbs<const BITS: usize, const LIMBS: usize> {
    limbs: [u64; LIMBS],
}

impl<const BITS: usize, const LIMBS: usize> BUintLimbs<BITS, LIMBS> {
    const TOP_BITS: u32 = (BITS % 64) as u32;
    const TOP_MASK: u64 = if Self::TOP_BITS == 0 {
        u64::MAX
    } else {
        (1u64 << Self::TOP_BITS) - 1
    };

    const ZERO: Self = Self { limbs: [0; LIMBS] };
    const ONE: Self = Self::from_low_limb(1);

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

    const fn from_i128(v: i128) -> Self {
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

    const fn from_u128(v: u128) -> Self {
        let mut limbs = [0; LIMBS];
        if LIMBS > 0 {
            limbs[0] = v as u64;
        }
        if LIMBS > 1 {
            limbs[1] = (v >> 64) as u64;
        }
        Self::masked(limbs)
    }

    const fn to_u128(self) -> u128 {
        let mut value = 0;
        if LIMBS > 0 {
            value = self.limbs[0] as u128;
        }
        if LIMBS > 1 {
            value |= (self.limbs[1] as u128) << 64;
        }
        value
    }

    const fn from_decimal_str(s: &str) -> Self {
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

    const fn wrapping_add(self, rhs: Self) -> Self {
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

    const fn wrapping_neg(self) -> Self {
        let mut inv = [0u64; LIMBS];
        let mut i = 0;
        while i < LIMBS {
            inv[i] = !self.limbs[i];
            i += 1;
        }
        Self { limbs: inv }.wrapping_add(Self::ONE)
    }

    const fn wrapping_sub(self, rhs: Self) -> Self {
        self.wrapping_add(rhs.wrapping_neg())
    }

    const fn wrapping_mul(self, rhs: Self) -> Self {
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

    const fn wrapping_div(self, rhs: Self) -> Self {
        self.div_rem_unsigned(rhs).0
    }

    const fn wrapping_rem(self, rhs: Self) -> Self {
        self.div_rem_unsigned(rhs).1
    }

    const fn wrapping_div_rem(self, rhs: Self) -> (Self, Self) {
        self.div_rem_unsigned(rhs)
    }

    const fn bitand_bits(self, rhs: Self) -> Self {
        let mut out = [0u64; LIMBS];

        let mut i = 0;
        while i < LIMBS {
            out[i] = self.limbs[i] & rhs.limbs[i];
            i += 1;
        }

        Self::masked(out)
    }

    const fn bitor_bits(self, rhs: Self) -> Self {
        let mut out = [0u64; LIMBS];

        let mut i = 0;
        while i < LIMBS {
            out[i] = self.limbs[i] | rhs.limbs[i];
            i += 1;
        }

        Self::masked(out)
    }

    const fn bitxor_bits(self, rhs: Self) -> Self {
        let mut out = [0u64; LIMBS];

        let mut i = 0;
        while i < LIMBS {
            out[i] = self.limbs[i] ^ rhs.limbs[i];
            i += 1;
        }

        Self::masked(out)
    }

    const fn bitnot(self) -> Self {
        let mut out = [0u64; LIMBS];

        let mut i = 0;
        while i < LIMBS {
            out[i] = !self.limbs[i];
            i += 1;
        }

        Self::masked(out)
    }

    const fn shl_bits(self, shift: usize) -> Self {
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

    const fn shr_bits(self, shift: usize) -> Self {
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

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
struct BIntLimbs<const BITS: usize, const LIMBS: usize> {
    bits: BUintLimbs<BITS, LIMBS>,
}

impl<const BITS: usize, const LIMBS: usize> BIntLimbs<BITS, LIMBS> {
    const SIGN_LIMB: usize = (BITS - 1) / 64;
    const SIGN_BIT: u32 = ((BITS - 1) % 64) as u32;

    const fn from_i128(v: i128) -> Self {
        Self {
            bits: BUintLimbs::from_i128(v),
        }
    }

    const fn from_u128(v: u128) -> Self {
        Self {
            bits: BUintLimbs::from_u128(v),
        }
    }

    const fn to_i128(self) -> i128 {
        let mut value = self.bits.to_u128();
        if BITS < 128 && self.is_negative() {
            value |= u128::MAX << (BITS % 128);
        }
        value as i128
    }

    const fn to_u128(self) -> u128 {
        self.bits.to_u128()
    }

    const fn from_decimal_str(s: &str) -> Self {
        let bytes = s.as_bytes();
        let neg = !bytes.is_empty() && bytes[0] == b'-';
        let ten = BUintLimbs::<BITS, LIMBS>::from_low_limb(10);
        let mut acc = BUintLimbs::<BITS, LIMBS>::ZERO;
        let mut i = if neg { 1 } else { 0 };
        while i < bytes.len() {
            let digit = BUintLimbs::<BITS, LIMBS>::from_low_limb((bytes[i] - b'0') as u64);
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

    const fn wrapping_add(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.wrapping_add(rhs.bits),
        }
    }

    const fn wrapping_sub(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.wrapping_sub(rhs.bits),
        }
    }

    const fn wrapping_mul(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.wrapping_mul(rhs.bits),
        }
    }

    const fn wrapping_neg(self) -> Self {
        Self {
            bits: self.bits.wrapping_neg(),
        }
    }

    const fn wrapping_div_rem(self, rhs: Self) -> (Self, Self) {
        let lhs_negative = self.is_negative();
        let rhs_negative = rhs.is_negative();

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

        let remainder = if lhs_negative {
            remainder.wrapping_neg()
        } else {
            remainder
        };

        (Self { bits: quotient }, Self { bits: remainder })
    }

    const fn bitand_bits(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.bitand_bits(rhs.bits),
        }
    }

    const fn bitor_bits(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.bitor_bits(rhs.bits),
        }
    }

    const fn bitxor_bits(self, rhs: Self) -> Self {
        Self {
            bits: self.bits.bitxor_bits(rhs.bits),
        }
    }

    const fn bitnot(self) -> Self {
        Self {
            bits: self.bits.bitnot(),
        }
    }

    const fn shl_bits(self, shift: usize) -> Self {
        Self {
            bits: self.bits.shl_bits(shift),
        }
    }

    const fn shr_bits(self, shift: usize) -> Self {
        let negative = self.is_negative();

        if shift == 0 {
            return self;
        }

        if shift >= BITS {
            return if negative {
                Self {
                    bits: BUintLimbs::masked([u64::MAX; LIMBS]),
                }
            } else {
                Self {
                    bits: BUintLimbs::ZERO,
                }
            };
        }

        let mut out = self.bits.shr_bits(shift);

        if !negative {
            return Self { bits: out };
        }

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
            bits: BUintLimbs::masked(out.limbs),
        }
    }

    const fn cmp(&self, other: &Self) -> Ordering {
        match (self.is_negative(), other.is_negative()) {
            (true, false) => Ordering::Less,
            (false, true) => Ordering::Greater,
            _ => self.bits.cmp_magnitude(&other.bits),
        }
    }
}
