pub(super) struct UndoLog<T> {
    live: usize,
    entries: Vec<T>,
}

impl<T> Default for UndoLog<T> {
    fn default() -> Self {
        Self {
            live: 0,
            entries: Vec::new(),
        }
    }
}

impl<T> UndoLog<T> {
    pub fn mark(&mut self) -> usize {
        self.live += 1;
        self.entries.len()
    }

    pub fn record(&mut self, entry: impl FnOnce() -> T) {
        if self.live > 0 {
            self.entries.push(entry());
        }
    }

    pub fn commit(&mut self) {
        self.release();
    }

    pub fn rollback(&mut self, mark: usize) -> impl Iterator<Item = T> + use<T> {
        let undone = self.entries.split_off(mark.min(self.entries.len()));
        self.release();
        undone.into_iter().rev()
    }

    fn release(&mut self) {
        self.live = self.live.saturating_sub(1);
        if self.live == 0 {
            self.entries.clear();
        }
    }
}
