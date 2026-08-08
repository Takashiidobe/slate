#![cfg_attr(not(feature = "std"), no_std)]
#![allow(dead_code)]

#[macro_use]
mod macros;

mod arch;
mod env;
mod error;
mod kernel;
mod obj;
mod triple;
mod util;
mod vendor;

pub use arch::ArchPart;
pub use env::Env;
pub use error::ParseError;
pub use kernel::Kernel;
pub use obj::Obj;
pub use triple::Triple;
pub use vendor::Vendor;

pub use util::get_host_triple;

pub(crate) trait Parsable {
    fn reduce(src: &'_ str) -> Option<(Self, usize)>
    where
        Self: Sized;
}

pub trait Canonicalizable {
    fn canonicalize(&self) -> &'static str;
}

pub(crate) trait BitnessObservable {
    fn bitness(&self) -> u16;
}

#[cfg(test)]
mod tests;
