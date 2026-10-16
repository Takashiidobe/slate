__extension__ typedef __UINT32_TYPE__ uint32_t;

struct lock_chain {
  uint32_t irq_context : 2, depth : 6, base : 24;
};

__attribute__((noinline, noclone)) struct lock_chain *
foo(struct lock_chain *chain) {
  int i;
  for (i = 0; i < 100; i++) {
    chain[i + 1].base = chain[i].base;
  }
  return chain;
}

struct lock_chain1 {
  char           x;
  unsigned short base;
} __attribute__((packed));

__attribute__((noinline, noclone)) struct lock_chain1 *
bar(struct lock_chain1 *chain) {
  int i;
  for (i = 0; i < 100; i++) {
    chain[i + 1].base = chain[i].base;
  }
  return chain;
}

struct lock_chain  test[101];
struct lock_chain1 test1[101];

int main() {
  foo(test);
  bar(test1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct lock_chain {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut chain: *mut lock_chain) -> *mut lock_chain {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (100 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { chain.offset((i + (1 as i32)) as isize) })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__set_base(
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         (*unsafe { chain.offset(i as isize) })
// CHECK-NEXT:                             .__slate_bits_0
// CHECK-NEXT:                             .__get_base()
// CHECK-NEXT:                     }) as i32) as u32,
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     chain
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub irq_context: u32,
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub depth: u32,
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub base: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct lock_chain1 {
// CHECK-NEXT:     x: i8,
// CHECK-NEXT:     base: u16,
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut chain: *mut lock_chain1) -> *mut lock_chain1 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (100 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe { chain.offset((i + (1 as i32)) as isize) }).base = unsafe {
// CHECK-NEXT:                 (*unsafe { chain.offset(i as isize) }).base
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     chain
// CHECK-NEXT: }
// CHECK-NEXT: static mut test: __SlateAlign16<[lock_chain; 101]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut test1: __SlateAlign16<[lock_chain1; 101]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(unsafe { std::ptr::addr_of_mut!(test.0) as *mut lock_chain });
// CHECK-NEXT:     bar(unsafe { std::ptr::addr_of_mut!(test1.0) as *mut lock_chain1 });
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
