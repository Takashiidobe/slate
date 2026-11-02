mod __slate_blocks {
    use std::ffi::c_void;
    use std::mem::{MaybeUninit, offset_of, size_of};

    #[cfg(not(target_vendor = "apple"))]
    use slate_blocks as _;

    unsafe extern "C" {
        static _NSConcreteStackBlock: [*const c_void; 32];
        static _NSConcreteGlobalBlock: [*const c_void; 32];
        fn _Block_object_assign(destination: *mut c_void, object: *const c_void, flags: i32);
        fn _Block_object_dispose(object: *const c_void, flags: i32);
    }

    const HAS_COPY_DISPOSE: i32 = 1 << 25;
    const IS_GLOBAL: i32 = 1 << 28;
    const FIELD_IS_BLOCK: i32 = 7;

    pub const fn block_field(offset: usize) -> (usize, i32) {
        (offset, FIELD_IS_BLOCK)
    }

    #[repr(C)]
    pub struct Descriptor {
        reserved: usize,
        size: usize,
        copy: unsafe extern "C" fn(*mut c_void, *const c_void),
        dispose: unsafe extern "C" fn(*const c_void),
        captures_offset: usize,
        fields: &'static [(usize, i32)],
    }

    #[repr(C)]
    pub struct Block<C> {
        isa: *const c_void,
        flags: i32,
        reserved: i32,
        invoke: *const c_void,
        descriptor: &'static Descriptor,
        captures: C,
    }

    pub const fn descriptor<C>(fields: &'static [(usize, i32)]) -> Descriptor {
        Descriptor {
            reserved: 0,
            size: size_of::<Block<C>>(),
            copy: copy_helper,
            dispose: dispose_helper,
            captures_offset: offset_of!(Block<C>, captures),
            fields,
        }
    }

    unsafe fn field_addresses(
        block: *const c_void,
    ) -> impl Iterator<Item = (*mut *const c_void, i32)> {
        let descriptor = unsafe { (*(block as *const Block<()>)).descriptor };
        descriptor.fields.iter().map(move |&(offset, kind)| {
            let address = block.wrapping_byte_add(descriptor.captures_offset + offset);
            (address as *mut *const c_void, kind)
        })
    }

    unsafe extern "C" fn copy_helper(destination: *mut c_void, source: *const c_void) {
        unsafe {
            for (field, kind) in field_addresses(source) {
                let offset = field.byte_offset_from(source);
                _Block_object_assign(destination.byte_offset(offset), *field, kind);
            }
        }
    }

    unsafe extern "C" fn dispose_helper(block: *const c_void) {
        unsafe {
            for (field, kind) in field_addresses(block) {
                _Block_object_dispose(*field, kind);
            }
        }
    }

    pub unsafe fn stack<C>(
        slot: *mut MaybeUninit<Block<C>>,
        invoke: *const c_void,
        descriptor: &'static Descriptor,
        captures: C,
    ) -> *mut c_void {
        unsafe {
            let flags = if descriptor.fields.is_empty() {
                0
            } else {
                HAS_COPY_DISPOSE
            };
            (*slot).write(Block {
                isa: (&raw const _NSConcreteStackBlock).cast(),
                flags,
                reserved: 0,
                invoke,
                descriptor,
                captures,
            });
            slot.cast()
        }
    }

    unsafe impl<C> Sync for Block<C> {}

    static GLOBAL_DESCRIPTOR: Descriptor = descriptor::<()>(&[]);

    pub const fn global(invoke: *const c_void) -> Block<()> {
        Block {
            isa: (&raw const _NSConcreteGlobalBlock).cast(),
            flags: IS_GLOBAL,
            reserved: 0,
            invoke,
            descriptor: &GLOBAL_DESCRIPTOR,
            captures: (),
        }
    }

    pub fn cell<T>(value: T) -> *mut T {
        Box::into_raw(Box::new(value))
    }

    pub unsafe fn invoke(block: *mut c_void) -> *const c_void {
        unsafe { (*(block as *const Block<()>)).invoke }
    }

    pub unsafe fn captures<C>(block: *mut c_void) -> C {
        unsafe { (&raw const (*(block as *const Block<C>)).captures).read() }
    }
}
