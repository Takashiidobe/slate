use std::vec::Vec;

pub trait VecExt<T: Clone>: Sized {
    fn try_filled(len: usize, value: T) -> Option<Self>;
    fn try_resize(&mut self, len: usize, value: T) -> Option<&mut Self>;
}

impl<T: Clone> VecExt<T> for Vec<T> {
    #[inline]
    fn try_filled(len: usize, value: T) -> Option<Self> {
        let mut buffer = Self::new();
        buffer.try_resize(len, value)?;
        Some(buffer)
    }

    #[inline]
    fn try_resize(&mut self, len: usize, value: T) -> Option<&mut Self> {
        self.try_reserve_exact(len.saturating_sub(self.len()))
            .ok()?;
        self.resize(len, value);
        Some(self)
    }
}
