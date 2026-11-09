mod __slate_darwin_arm64_va {
    pub unsafe fn arg<L, T>(list: *mut L) -> T {
        unsafe {
            let cursor = &mut *(list as *mut *const u8);
            let align = core::mem::align_of::<T>().max(8);
            *cursor = cursor.map_addr(|address| (address + align - 1) & !(align - 1));
            let value = core::ptr::read(*cursor as *const T);
            *cursor = cursor.add(core::mem::size_of::<T>().next_multiple_of(8));
            value
        }
    }

    pub unsafe fn indirect<L, T>(list: *mut L) -> T {
        unsafe {
            let cursor = &mut *(list as *mut *const *const T);
            let value = core::ptr::read(*cursor).read();
            *cursor = cursor.add(1);
            value
        }
    }
}
