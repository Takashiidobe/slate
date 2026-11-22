use core::ffi::c_int;

pub fn __slate_exit(status: c_int) -> ! {
    std::process::exit(status)
}
