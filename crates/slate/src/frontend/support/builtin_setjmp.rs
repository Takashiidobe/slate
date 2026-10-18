mod __slate_builtin_setjmp {
    #[repr(C, align(16))]
    pub struct Buffer([u8; 512]);

    unsafe extern "C" {
        pub fn _setjmp(env: *mut Buffer) -> i32;
        pub fn _longjmp(env: *mut Buffer, value: i32);
    }
}
