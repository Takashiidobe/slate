mod __slate_alloca {
    use std::alloc::{Layout, alloc, dealloc, handle_alloc_error};
    use std::cell::UnsafeCell;

    const ALIGN: usize = 16;
    const BLOCK: usize = 1 << 20;

    struct Block {
        base: *mut u8,
        size: usize,
    }

    impl Block {
        fn new(size: usize) -> Self {
            let layout = Layout::from_size_align(size, ALIGN).unwrap();
            let base = unsafe { alloc(layout) };
            if base.is_null() {
                handle_alloc_error(layout);
            }
            Block { base, size }
        }
    }

    impl Drop for Block {
        fn drop(&mut self) {
            unsafe { dealloc(self.base, Layout::from_size_align_unchecked(self.size, ALIGN)) }
        }
    }

    struct Stack {
        blocks: Vec<Block>,
        block: usize,
        offset: usize,
    }

    impl Stack {
        fn allocate(&mut self, size: usize, align: usize) -> *mut u8 {
            if self.blocks.is_empty() {
                self.blocks.push(Block::new(BLOCK));
            }
            let needed = size.saturating_add(align);
            loop {
                let block = &self.blocks[self.block];
                let base = block.base as usize;
                let start = (base + self.offset).next_multiple_of(align) - base;
                if start <= block.size && block.size - start >= size {
                    self.offset = start + size;
                    unsafe {
                        let pointer = block.base.add(start);
                        pointer.write_bytes(0, size);
                        return pointer;
                    }
                }
                self.block += 1;
                self.offset = 0;
                if self.block == self.blocks.len() || self.blocks[self.block].size < needed {
                    self.blocks.truncate(self.block);
                    self.blocks.push(Block::new(needed.next_multiple_of(ALIGN).max(BLOCK)));
                }
            }
        }
    }

    thread_local! {
        static STACK: UnsafeCell<Stack> = const {
            UnsafeCell::new(Stack { blocks: Vec::new(), block: 0, offset: 0 })
        };
    }

    pub struct Mark {
        block: usize,
        offset: usize,
    }

    impl Mark {
        #[inline]
        pub fn new() -> Self {
            STACK.with(|stack| {
                let stack = unsafe { &*stack.get() };
                Mark { block: stack.block, offset: stack.offset }
            })
        }
    }

    impl Drop for Mark {
        #[inline]
        fn drop(&mut self) {
            STACK.with(|stack| {
                let stack = unsafe { &mut *stack.get() };
                stack.block = self.block;
                stack.offset = self.offset;
            })
        }
    }

    #[inline]
    pub fn alloca(size: usize, align: usize) -> *mut std::ffi::c_void {
        STACK.with(|stack| unsafe { (*stack.get()).allocate(size, align.max(ALIGN)) }).cast()
    }
}
