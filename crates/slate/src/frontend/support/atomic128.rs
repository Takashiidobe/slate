mod __slate_atomic128 {
    #[link(name = "atomic")]
    unsafe extern "C" {
        pub fn __atomic_load_16(pointer: *const u128, order: i32) -> u128;
        pub fn __atomic_store_16(pointer: *mut u128, value: u128, order: i32);
    }
}
