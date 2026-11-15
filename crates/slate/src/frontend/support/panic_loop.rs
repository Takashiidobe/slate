mod __slate_panic {
    #[panic_handler]
    fn __slate_panic_handler(_: &core::panic::PanicInfo) -> ! {
        loop {}
    }
}
