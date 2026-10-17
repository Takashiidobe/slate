mod __slate_cleanup {
    pub struct Guard<F: FnMut()>(pub F);

    impl<F: FnMut()> Drop for Guard<F> {
        fn drop(&mut self) {
            (self.0)()
        }
    }
}
