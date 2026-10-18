mod __slate_sysv_va {
    #[repr(C)]
    struct Tag {
        gp_offset: u32,
        fp_offset: u32,
        overflow: *const u8,
        registers: *const u8,
    }

    const GP_END: u32 = 48;
    const FP_END: u32 = 176;

    pub unsafe fn arg<L, T>(list: *mut L, classes: u32) -> T {
        unsafe {
            let tag = &mut *(list as *mut Tag);
            let size = core::mem::size_of::<T>();
            let mut out = core::mem::MaybeUninit::<T>::uninit();
            let dst = out.as_mut_ptr() as *mut u8;
            let (mut gp, mut sse) = (0, 0);
            let mut rest = classes;
            while rest != 0 {
                match rest & 3 {
                    1 => gp += 8,
                    _ => sse += 16,
                }
                rest >>= 2;
            }
            if classes != 0 && tag.gp_offset + gp <= GP_END && tag.fp_offset + sse <= FP_END {
                let mut copied = 0;
                let mut rest = classes;
                while rest != 0 {
                    let class = rest & 3;
                    let (offset, width) = if class == 1 {
                        (&mut tag.gp_offset, 8)
                    } else {
                        (&mut tag.fp_offset, if class == 3 { 16 } else { 8 })
                    };
                    let step = if class == 1 { 8 } else { 16 };
                    core::ptr::copy_nonoverlapping(
                        tag.registers.add(*offset as usize),
                        dst.add(copied),
                        width.min(size - copied),
                    );
                    *offset += step;
                    copied += width;
                    rest >>= 2;
                }
            } else {
                let align = core::mem::align_of::<T>().max(8);
                let slot = ((tag.overflow as usize + align - 1) & !(align - 1)) as *const u8;
                core::ptr::copy_nonoverlapping(slot, dst, size);
                tag.overflow = slot.add((size + 7) & !7);
            }
            out.assume_init()
        }
    }
}
