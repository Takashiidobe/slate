mod __slate_win64_va {
    pub unsafe fn arg<L, T>(list: *mut *mut L) -> T {
        unsafe {
            let slot = *list as *const u8;
            *list = slot.add(8) as *mut L;
            let size = core::mem::size_of::<T>();
            if size > 8 || !size.is_power_of_two() {
                (*(slot as *const *const T)).read_unaligned()
            } else {
                (slot as *const T).read_unaligned()
            }
        }
    }
}
