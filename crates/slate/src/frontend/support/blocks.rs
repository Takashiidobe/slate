mod __slate_blocks {
    use std::ffi::c_void;
    use std::mem::size_of;

    #[cfg(not(target_vendor = "apple"))]
    use slate_blocks as _;

    unsafe extern "C" {
        static _NSConcreteMallocBlock: [*const c_void; 32];
        static _NSConcreteGlobalBlock: [*const c_void; 32];
        fn malloc(size: usize) -> *mut c_void;
    }

    const NEEDS_FREE: i32 = 1 << 24;
    const REFCOUNT_ONE: i32 = 2;
    const IS_GLOBAL: i32 = 1 << 28;

    #[repr(C)]
    pub struct Descriptor {
        reserved: usize,
        size: usize,
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

    impl<C> Block<C> {
        const DESCRIPTOR: Descriptor = Descriptor {
            reserved: 0,
            size: size_of::<Block<C>>(),
        };
    }

    pub unsafe fn new<C>(invoke: *const c_void, captures: C) -> *mut c_void {
        unsafe {
            let block = malloc(size_of::<Block<C>>()) as *mut Block<C>;
            block.write(Block {
                isa: (&raw const _NSConcreteMallocBlock).cast(),
                flags: NEEDS_FREE | REFCOUNT_ONE,
                reserved: 0,
                invoke,
                descriptor: &Block::<C>::DESCRIPTOR,
                captures,
            });
            block.cast()
        }
    }

    unsafe impl<C> Sync for Block<C> {}

    pub const fn global(invoke: *const c_void) -> Block<()> {
        Block {
            isa: (&raw const _NSConcreteGlobalBlock).cast(),
            flags: IS_GLOBAL,
            reserved: 0,
            invoke,
            descriptor: &Block::<()>::DESCRIPTOR,
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
