mod __slate_vector {
    use std::ops::*;

    #[repr(C)]
    pub struct __SlateVector<T, const N: usize, A>(pub [T; N], pub [A; 0]);

    impl<T: Copy, const N: usize, A: Copy> Clone for __SlateVector<T, N, A> {
        fn clone(&self) -> Self {
            *self
        }
    }

    impl<T: Copy, const N: usize, A: Copy> Copy for __SlateVector<T, N, A> {}

    macro_rules! alignments {
        ($($name:ident = $align:literal),*) => {$(
            #[repr(align($align))]
            #[derive(Clone, Copy)]
            pub struct $name;
        )*};
    }

    alignments!(
        Align1 = 1,
        Align2 = 2,
        Align4 = 4,
        Align8 = 8,
        Align16 = 16,
        Align32 = 32,
        Align64 = 64,
        Align128 = 128,
        Align256 = 256,
        Align512 = 512,
        Align1024 = 1024,
        Align2048 = 2048,
        Align4096 = 4096
    );

    impl<T: Copy, const N: usize, A: Copy> __SlateVector<T, N, A> {
        pub const fn from_array(lanes: [T; N]) -> Self {
            Self(lanes, [])
        }

        pub const fn splat(lane: T) -> Self {
            Self([lane; N], [])
        }

        pub const fn to_array(self) -> [T; N] {
            self.0
        }

        fn map(self, f: impl Fn(T) -> T) -> Self {
            Self(self.0.map(f), [])
        }

        fn zip(self, other: Self, f: impl Fn(T, T) -> T) -> Self {
            let mut lanes = self.0;
            for (lane, other) in lanes.iter_mut().zip(other.0) {
                *lane = f(*lane, other);
            }
            Self(lanes, [])
        }
    }

    impl<T, const N: usize, A> Index<usize> for __SlateVector<T, N, A> {
        type Output = T;

        fn index(&self, index: usize) -> &T {
            &self.0[index]
        }
    }

    impl<T, const N: usize, A> IndexMut<usize> for __SlateVector<T, N, A> {
        fn index_mut(&mut self, index: usize) -> &mut T {
            &mut self.0[index]
        }
    }

    macro_rules! binary {
        ($t:ty, $($trait:ident $method:ident $assign:ident $assign_method:ident $f:expr),*) => {$(
            impl<const N: usize, A: Copy> $trait for __SlateVector<$t, N, A> {
                type Output = Self;

                fn $method(self, other: Self) -> Self {
                    self.zip(other, $f)
                }
            }

            impl<const N: usize, A: Copy> $assign for __SlateVector<$t, N, A> {
                fn $assign_method(&mut self, other: Self) {
                    *self = self.zip(other, $f);
                }
            }
        )*};
    }

    macro_rules! integer {
        ($($t:ty),*) => {$(
            binary!(
                $t,
                Add add AddAssign add_assign <$t>::wrapping_add,
                Sub sub SubAssign sub_assign <$t>::wrapping_sub,
                Mul mul MulAssign mul_assign <$t>::wrapping_mul,
                Div div DivAssign div_assign <$t>::wrapping_div,
                Rem rem RemAssign rem_assign <$t>::wrapping_rem,
                BitAnd bitand BitAndAssign bitand_assign |a: $t, b: $t| a & b,
                BitOr bitor BitOrAssign bitor_assign |a: $t, b: $t| a | b,
                BitXor bitxor BitXorAssign bitxor_assign |a: $t, b: $t| a ^ b,
                Shl shl ShlAssign shl_assign |a: $t, b: $t| a.wrapping_shl(b as u32),
                Shr shr ShrAssign shr_assign |a: $t, b: $t| a.wrapping_shr(b as u32)
            );

            impl<const N: usize, A: Copy> Neg for __SlateVector<$t, N, A> {
                type Output = Self;

                fn neg(self) -> Self {
                    self.map(<$t>::wrapping_neg)
                }
            }

            impl<const N: usize, A: Copy> Not for __SlateVector<$t, N, A> {
                type Output = Self;

                fn not(self) -> Self {
                    self.map(|a| !a)
                }
            }
        )*};
    }

    macro_rules! float {
        ($($t:ty),*) => {$(
            binary!(
                $t,
                Add add AddAssign add_assign |a: $t, b: $t| a + b,
                Sub sub SubAssign sub_assign |a: $t, b: $t| a - b,
                Mul mul MulAssign mul_assign |a: $t, b: $t| a * b,
                Div div DivAssign div_assign |a: $t, b: $t| a / b,
                Rem rem RemAssign rem_assign |a: $t, b: $t| a % b
            );

            impl<const N: usize, A: Copy> Neg for __SlateVector<$t, N, A> {
                type Output = Self;

                fn neg(self) -> Self {
                    self.map(|a| -a)
                }
            }
        )*};
    }

    integer!(i8, u8, i16, u16, i32, u32, i64, u64, i128, u128);
    float!(f32, f64);
}
