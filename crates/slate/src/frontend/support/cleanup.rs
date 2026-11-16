mod __slate_cleanup {
    pub struct Guard<'a>(pub &'a mut dyn FnMut());

    impl Drop for Guard<'_> {
        fn drop(&mut self) {
            (self.0)()
        }
    }
}
