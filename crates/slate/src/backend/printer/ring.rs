use std::collections::VecDeque;
use std::ops::{Index, IndexMut, Range};

pub(super) struct RingBuffer<T> {
    data: VecDeque<T>,
    offset: usize,
}

impl<T> RingBuffer<T> {
    pub(super) fn new() -> Self {
        RingBuffer {
            data: VecDeque::new(),
            offset: 0,
        }
    }

    pub(super) fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    pub(super) fn len(&self) -> usize {
        self.data.len()
    }

    pub(super) fn push(&mut self, value: T) -> usize {
        let index = self.offset + self.data.len();
        self.data.push_back(value);
        index
    }

    pub(super) fn clear(&mut self) {
        self.data.clear();
    }

    pub(super) fn index_range(&self) -> Range<usize> {
        self.offset..self.offset + self.data.len()
    }

    pub(super) fn first(&self) -> &T {
        &self.data[0]
    }

    pub(super) fn first_mut(&mut self) -> &mut T {
        &mut self.data[0]
    }

    pub(super) fn pop_first(&mut self) -> T {
        self.offset += 1;
        self.data.pop_front().unwrap()
    }

    pub(super) fn last(&self) -> &T {
        self.data.back().unwrap()
    }

    pub(super) fn last_mut(&mut self) -> &mut T {
        self.data.back_mut().unwrap()
    }

    pub(super) fn second_last(&self) -> &T {
        &self.data[self.data.len() - 2]
    }

    pub(super) fn pop_last(&mut self) {
        self.data.pop_back().unwrap();
    }
}

impl<T> Index<usize> for RingBuffer<T> {
    type Output = T;
    fn index(&self, index: usize) -> &Self::Output {
        &self.data[index.checked_sub(self.offset).unwrap()]
    }
}

impl<T> IndexMut<usize> for RingBuffer<T> {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        &mut self.data[index.checked_sub(self.offset).unwrap()]
    }
}
