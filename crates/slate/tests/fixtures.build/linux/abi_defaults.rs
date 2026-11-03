const _: () = {
    assert!(core::mem::size_of::<slate_support::off_t>() == 8);
    assert!(core::mem::size_of::<slate_support::time_t>() == 8);
};
