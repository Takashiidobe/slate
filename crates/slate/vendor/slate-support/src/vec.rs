use core::mem::MaybeUninit;
use std::vec::Vec;

pub trait VecExt<T>: Sized {
    fn try_alloc(capacity: usize) -> Option<Self>;
    fn try_uninit(len: usize) -> Option<Vec<MaybeUninit<T>>>;
    fn try_reserve_len(&mut self, len: usize) -> Option<&mut Self>;
    fn try_resize_with(&mut self, len: usize, init: impl FnMut() -> T) -> Option<&mut Self>;
    fn try_filled(len: usize, value: T) -> Option<Self>
    where
        T: Clone;
    fn try_resize(&mut self, len: usize, value: T) -> Option<&mut Self>
    where
        T: Clone;
}

impl<T> VecExt<T> for Vec<T> {
    #[inline]
    fn try_alloc(capacity: usize) -> Option<Self> {
        let mut buffer = Self::new();
        buffer.try_reserve_len(capacity)?;
        Some(buffer)
    }

    #[inline]
    fn try_uninit(len: usize) -> Option<Vec<MaybeUninit<T>>> {
        let mut buffer = Vec::try_alloc(len)?;
        buffer.resize_with(len, MaybeUninit::uninit);
        Some(buffer)
    }

    #[inline]
    fn try_reserve_len(&mut self, len: usize) -> Option<&mut Self> {
        self.try_reserve_exact(len.saturating_sub(self.len()))
            .ok()?;
        Some(self)
    }

    #[inline]
    fn try_resize_with(&mut self, len: usize, init: impl FnMut() -> T) -> Option<&mut Self> {
        self.try_reserve_len(len)?;
        self.resize_with(len, init);
        Some(self)
    }

    #[inline]
    fn try_filled(len: usize, value: T) -> Option<Self>
    where
        T: Clone,
    {
        let mut buffer = Self::try_alloc(len)?;
        buffer.resize(len, value);
        Some(buffer)
    }

    #[inline]
    fn try_resize(&mut self, len: usize, value: T) -> Option<&mut Self>
    where
        T: Clone,
    {
        self.try_reserve_len(len)?;
        self.resize(len, value);
        Some(self)
    }
}
