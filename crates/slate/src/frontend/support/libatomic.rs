mod __slate_libatomic {
    use std::sync::atomic::Ordering;

    #[link(name = "atomic")]
    unsafe extern "C" {
        pub fn __atomic_load_16(pointer: *const u128, order: i32) -> u128;
        pub fn __atomic_store_16(pointer: *mut u128, value: u128, order: i32);
        fn __atomic_compare_exchange_16(
            pointer: *mut u128,
            expected: *mut u128,
            desired: u128,
            weak: bool,
            success: i32,
            failure: i32,
        ) -> bool;
    }

    fn order(order: Ordering) -> i32 {
        match order {
            Ordering::Relaxed => 0,
            Ordering::Acquire => 2,
            Ordering::Release => 3,
            Ordering::AcqRel => 4,
            _ => 5,
        }
    }

    fn failure(success: Ordering) -> Ordering {
        match success {
            Ordering::Release => Ordering::Relaxed,
            Ordering::AcqRel => Ordering::Acquire,
            other => other,
        }
    }

    macro_rules! atomic_int {
        ($name:ident, $ty:ty) => {
            pub struct $name(*mut u128);

            impl $name {
                pub unsafe fn from_ptr(pointer: *mut $ty) -> Self {
                    Self(pointer.cast())
                }

                pub fn load(&self, ordering: Ordering) -> $ty {
                    unsafe { __atomic_load_16(self.0, order(ordering)) as $ty }
                }

                fn exchange(
                    &self,
                    current: $ty,
                    new: $ty,
                    weak: bool,
                    success: Ordering,
                    failure: Ordering,
                ) -> Result<$ty, $ty> {
                    let mut expected = current as u128;
                    if unsafe {
                        __atomic_compare_exchange_16(
                            self.0,
                            &mut expected,
                            new as u128,
                            weak,
                            order(success),
                            order(failure),
                        )
                    } {
                        Ok(current)
                    } else {
                        Err(expected as $ty)
                    }
                }

                pub fn compare_exchange(
                    &self,
                    current: $ty,
                    new: $ty,
                    success: Ordering,
                    failure: Ordering,
                ) -> Result<$ty, $ty> {
                    self.exchange(current, new, false, success, failure)
                }

                pub fn compare_exchange_weak(
                    &self,
                    current: $ty,
                    new: $ty,
                    success: Ordering,
                    failure: Ordering,
                ) -> Result<$ty, $ty> {
                    self.exchange(current, new, true, success, failure)
                }

                pub fn fetch_update(
                    &self,
                    set_order: Ordering,
                    fetch_order: Ordering,
                    mut f: impl FnMut($ty) -> Option<$ty>,
                ) -> Result<$ty, $ty> {
                    let mut previous = self.load(fetch_order);
                    while let Some(next) = f(previous) {
                        match self.exchange(previous, next, true, set_order, fetch_order) {
                            Ok(value) => return Ok(value),
                            Err(observed) => previous = observed,
                        }
                    }
                    Err(previous)
                }

                fn rmw(&self, ordering: Ordering, f: impl Fn($ty) -> $ty) -> $ty {
                    let mut previous = self.load(Ordering::Relaxed);
                    loop {
                        match self.exchange(
                            previous,
                            f(previous),
                            true,
                            ordering,
                            failure(ordering),
                        ) {
                            Ok(value) => return value,
                            Err(observed) => previous = observed,
                        }
                    }
                }

                pub fn swap(&self, value: $ty, ordering: Ordering) -> $ty {
                    self.rmw(ordering, |_| value)
                }

                pub fn fetch_add(&self, value: $ty, ordering: Ordering) -> $ty {
                    self.rmw(ordering, |old| old.wrapping_add(value))
                }

                pub fn fetch_sub(&self, value: $ty, ordering: Ordering) -> $ty {
                    self.rmw(ordering, |old| old.wrapping_sub(value))
                }

                pub fn fetch_and(&self, value: $ty, ordering: Ordering) -> $ty {
                    self.rmw(ordering, |old| old & value)
                }

                pub fn fetch_nand(&self, value: $ty, ordering: Ordering) -> $ty {
                    self.rmw(ordering, |old| !(old & value))
                }

                pub fn fetch_or(&self, value: $ty, ordering: Ordering) -> $ty {
                    self.rmw(ordering, |old| old | value)
                }

                pub fn fetch_xor(&self, value: $ty, ordering: Ordering) -> $ty {
                    self.rmw(ordering, |old| old ^ value)
                }
            }
        };
    }

    atomic_int!(AtomicI128, i128);
    atomic_int!(AtomicU128, u128);

    #[link(name = "atomic")]
    unsafe extern "C" {
        #[link_name = "__atomic_load"]
        fn atomic_load(size: usize, pointer: *mut u8, result: *mut u8, order: i32);
        #[link_name = "__atomic_store"]
        fn atomic_store(size: usize, pointer: *mut u8, value: *mut u8, order: i32);
        #[link_name = "__atomic_exchange"]
        fn atomic_exchange(
            size: usize,
            pointer: *mut u8,
            value: *mut u8,
            result: *mut u8,
            order: i32,
        );
        #[link_name = "__atomic_compare_exchange"]
        fn atomic_compare_exchange(
            size: usize,
            pointer: *mut u8,
            expected: *mut u8,
            desired: *mut u8,
            success: i32,
            failure: i32,
        ) -> bool;
    }

    pub struct AtomicGeneric<T>(*mut T);

    impl<T: Copy> AtomicGeneric<T> {
        const SIZE: usize = std::mem::size_of::<T>();

        pub unsafe fn from_ptr(pointer: *mut T) -> Self {
            Self(pointer)
        }

        pub fn load(&self, ordering: Ordering) -> T {
            let mut result = std::mem::MaybeUninit::<T>::uninit();
            unsafe {
                atomic_load(
                    Self::SIZE,
                    self.0.cast(),
                    result.as_mut_ptr().cast(),
                    order(ordering),
                );
                result.assume_init()
            }
        }

        pub fn store(&self, mut value: T, ordering: Ordering) {
            unsafe {
                atomic_store(
                    Self::SIZE,
                    self.0.cast(),
                    (&raw mut value).cast(),
                    order(ordering),
                )
            }
        }

        pub fn swap(&self, mut value: T, ordering: Ordering) -> T {
            let mut result = std::mem::MaybeUninit::<T>::uninit();
            unsafe {
                atomic_exchange(
                    Self::SIZE,
                    self.0.cast(),
                    (&raw mut value).cast(),
                    result.as_mut_ptr().cast(),
                    order(ordering),
                );
                result.assume_init()
            }
        }

        pub fn compare_exchange(
            &self,
            current: T,
            mut new: T,
            success: Ordering,
            failure: Ordering,
        ) -> Result<T, T> {
            let mut expected = current;
            if unsafe {
                atomic_compare_exchange(
                    Self::SIZE,
                    self.0.cast(),
                    (&raw mut expected).cast(),
                    (&raw mut new).cast(),
                    order(success),
                    order(failure),
                )
            } {
                Ok(current)
            } else {
                Err(expected)
            }
        }

        pub fn compare_exchange_weak(
            &self,
            current: T,
            new: T,
            success: Ordering,
            failure: Ordering,
        ) -> Result<T, T> {
            self.compare_exchange(current, new, success, failure)
        }

        pub fn fetch_update(
            &self,
            set_order: Ordering,
            fetch_order: Ordering,
            mut f: impl FnMut(T) -> Option<T>,
        ) -> Result<T, T> {
            let mut previous = self.load(fetch_order);
            while let Some(next) = f(previous) {
                match self.compare_exchange(previous, next, set_order, fetch_order) {
                    Ok(value) => return Ok(value),
                    Err(observed) => previous = observed,
                }
            }
            Err(previous)
        }
    }
}
