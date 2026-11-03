const _: () = {
    assert!(core::mem::size_of::<slate_support::cpuset_t>() == 128);
    assert!(slate_support::CPU_SETSIZE == 1024);
    assert!(core::mem::size_of::<slate_support::tcp_info>() == 256);
    assert!(core::mem::offset_of!(slate_support::tcp_info, tcpi_total_tlp_bytes) == 168);
};
