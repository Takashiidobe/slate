mod __slate_libgomp {
    use std::ffi::c_void;

    #[link(name = "gomp")]
    unsafe extern "C" {
        fn GOMP_parallel(
            function: unsafe extern "C" fn(*mut c_void),
            data: *mut c_void,
            num_threads: u32,
            flags: u32,
        );
    }

    unsafe extern "C" fn region<F: Fn()>(data: *mut c_void) {
        unsafe { (*(data as *const F))() }
    }

    pub fn parallel<F: Fn()>(num_threads: u32, body: F) {
        unsafe {
            GOMP_parallel(
                region::<F>,
                &raw const body as *mut c_void,
                num_threads,
                0,
            )
        }
    }
}
