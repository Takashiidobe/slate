mod __slate_aapcs_va {
    #[repr(C)]
    struct Tag {
        stack: *const u8,
        gr_top: *const u8,
        vr_top: *const u8,
        gr_offs: i32,
        vr_offs: i32,
    }

    unsafe fn from_stack<T>(tag: &mut Tag) -> T {
        unsafe {
            let mut slot = tag.stack;
            if core::mem::align_of::<T>() > 8 {
                slot = ((slot as usize + 15) & !15) as *const u8;
            }
            let value = core::ptr::read_unaligned(slot as *const T);
            tag.stack = slot.add((core::mem::size_of::<T>() + 7) & !7);
            value
        }
    }

    pub unsafe fn gp<L, T>(list: *mut L) -> T {
        unsafe {
            let tag = &mut *(list as *mut Tag);
            let size = core::mem::size_of::<T>();
            if size == 0 {
                return core::mem::zeroed();
            }
            let mut offset = tag.gr_offs;
            if offset < 0 {
                if core::mem::align_of::<T>() > 8 {
                    offset = (offset + 15) & !15;
                }
                let next = offset + ((size as i32 + 7) & !7);
                tag.gr_offs = next;
                if next <= 0 {
                    return core::ptr::read_unaligned(
                        tag.gr_top.offset(offset as isize) as *const T
                    );
                }
            }
            from_stack(tag)
        }
    }

    pub unsafe fn hfa<L, T>(list: *mut L, count: usize, width: usize) -> T {
        unsafe {
            let tag = &mut *(list as *mut Tag);
            let offset = tag.vr_offs;
            if offset < 0 {
                let next = offset + 16 * count as i32;
                tag.vr_offs = next;
                if next <= 0 {
                    let mut out = core::mem::MaybeUninit::<T>::zeroed();
                    let dst = out.as_mut_ptr() as *mut u8;
                    for member in 0..count {
                        core::ptr::copy_nonoverlapping(
                            tag.vr_top.offset(offset as isize + 16 * member as isize),
                            dst.add(member * width),
                            width,
                        );
                    }
                    return out.assume_init();
                }
            }
            from_stack(tag)
        }
    }

    pub unsafe fn indirect<L, T>(list: *mut L) -> T {
        unsafe { core::ptr::read_unaligned(gp::<L, *const T>(list)) }
    }
}
