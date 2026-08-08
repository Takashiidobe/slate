#[macro_export]
macro_rules! triple_part {
    {
        $name:ident {$($variant:ident),* $(,)?}
        canon($self:ident) $canonb:block
        parse($src:ident) $parseb:block
        bits($self2:ident) $bitsb:block
    } => {
        #[derive(PartialEq, Eq, Copy, Clone, strum::IntoStaticStr, Debug, core::cmp::PartialOrd, Hash)]
        #[cfg_attr(feature = "serde", derive(::serde::Serialize, ::serde::Deserialize))]
        #[strum(serialize_all = "lowercase")]
        #[non_exhaustive]
        pub enum $name {
            $($variant,)*
        }

        impl crate::Canonicalizable for $name {
            fn canonicalize(&$self) -> &'static str $canonb
        }

        #[allow(unused)]
        macro_rules! prefix {
            (
                $s:literal
                $x:expr
            ) => (
                $x.eq_ignore_ascii_case($s) ||
                $x.get(..concat!($s, "-").len()).
                map_or(false, |start| start.eq_ignore_ascii_case(concat!($s, "-")))
            )
        }

        impl crate::Parsable for $name {
            fn reduce($src: &'_ str) -> Option<($name, usize)> $parseb
        }

        impl crate::BitnessObservable for $name {
            fn bitness(&$self2) -> u16 $bitsb
        }
    }
}
