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
        used: usize,
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
                let top = base + block.size - self.used;
                if let Some(start) = top.checked_sub(size).map(|start| start & !(align - 1))
                    && start >= base
                {
                    self.used = base + block.size - start;
                    unsafe {
                        let pointer = block.base.add(start - base);
                        pointer.write_bytes(0, size);
                        return pointer;
                    }
                }
                self.block += 1;
                self.used = 0;
                if self.block == self.blocks.len() || self.blocks[self.block].size < needed {
                    self.blocks.truncate(self.block);
                    self.blocks.push(Block::new(needed.next_multiple_of(ALIGN).max(BLOCK)));
                }
            }
        }
    }

    thread_local! {
        static STACK: UnsafeCell<Stack> = const {
            UnsafeCell::new(Stack { blocks: Vec::new(), block: 0, used: 0 })
        };
    }

    pub struct Mark {
        block: usize,
        used: usize,
    }

    impl Mark {
        #[inline]
        pub fn new() -> Self {
            STACK.with(|stack| {
                let stack = unsafe { &*stack.get() };
                Mark { block: stack.block, used: stack.used }
            })
        }
    }

    impl Drop for Mark {
        #[inline]
        fn drop(&mut self) {
            STACK.with(|stack| {
                let stack = unsafe { &mut *stack.get() };
                stack.block = self.block;
                stack.used = self.used;
            })
        }
    }

    #[inline]
    pub fn alloca(size: usize, align: usize) -> *mut std::ffi::c_void {
        STACK.with(|stack| unsafe { (*stack.get()).allocate(size, align.max(1).next_power_of_two()) }).cast()
    }
}
