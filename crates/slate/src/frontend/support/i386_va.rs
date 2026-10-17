mod __slate_i386_va {
    pub unsafe fn arg<L, T>(list: *mut L) -> T {
        unsafe {
            let cursor = &mut *(list as *mut *const u8);
            let size = core::mem::size_of::<T>();
            let value = core::ptr::read_unaligned(*cursor as *const T);
            *cursor = cursor.add((size + 3) & !3);
            value
        }
    }
}
