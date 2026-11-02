// { dg-options "-fblocks" }
#include <stdio.h>
static int (^global_double)(int) = ^(int x) { return x * 2; };
int (^exported)(int) = ^(int x) { return x + 1; };
int main(void) {
  static int hits;
  int (^local_static)(int) = ^(int x) { hits++; return x; };
  local_static(1);
  printf("%d %d %d\n", global_double(4), exported(4), hits);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fblocks" }
// CHECK-NEXT: mod __slate_blocks {
// CHECK-NEXT:     use std::ffi::c_void;
// CHECK-NEXT:     use std::mem::size_of;
// CHECK-EMPTY:
// CHECK-NEXT:     #[cfg(not(target_vendor = "apple"))]
// CHECK-NEXT:     use slate_blocks as _;
// CHECK-EMPTY:
// CHECK-NEXT:     unsafe extern "C" {
// CHECK-NEXT:         static _NSConcreteMallocBlock: [*const c_void; 32];
// CHECK-NEXT:         static _NSConcreteGlobalBlock: [*const c_void; 32];
// CHECK-NEXT:         fn malloc(size: usize) -> *mut c_void;
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     const NEEDS_FREE: i32 = 1 << 24;
// CHECK-NEXT:     const REFCOUNT_ONE: i32 = 2;
// CHECK-NEXT:     const IS_GLOBAL: i32 = 1 << 28;
// CHECK-EMPTY:
// CHECK-NEXT:     #[repr(C)]
// CHECK-NEXT:     pub struct Descriptor {
// CHECK-NEXT:         reserved: usize,
// CHECK-NEXT:         size: usize,
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     #[repr(C)]
// CHECK-NEXT:     pub struct Block<C> {
// CHECK-NEXT:         isa: *const c_void,
// CHECK-NEXT:         flags: i32,
// CHECK-NEXT:         reserved: i32,
// CHECK-NEXT:         invoke: *const c_void,
// CHECK-NEXT:         descriptor: &'static Descriptor,
// CHECK-NEXT:         captures: C,
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl<C> Block<C> {
// CHECK-NEXT:         const DESCRIPTOR: Descriptor = Descriptor {
// CHECK-NEXT:             reserved: 0,
// CHECK-NEXT:             size: size_of::<Block<C>>(),
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub unsafe fn new<C>(invoke: *const c_void, captures: C) -> *mut c_void {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let block = malloc(size_of::<Block<C>>()) as *mut Block<C>;
// CHECK-NEXT:             block.write(Block {
// CHECK-NEXT:                 isa: (&raw const _NSConcreteMallocBlock).cast(),
// CHECK-NEXT:                 flags: NEEDS_FREE | REFCOUNT_ONE,
// CHECK-NEXT:                 reserved: 0,
// CHECK-NEXT:                 invoke,
// CHECK-NEXT:                 descriptor: &Block::<C>::DESCRIPTOR,
// CHECK-NEXT:                 captures,
// CHECK-NEXT:             });
// CHECK-NEXT:             block.cast()
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     unsafe impl<C> Sync for Block<C> {}
// CHECK-EMPTY:
// CHECK-NEXT:     pub const fn global(invoke: *const c_void) -> Block<()> {
// CHECK-NEXT:         Block {
// CHECK-NEXT:             isa: (&raw const _NSConcreteGlobalBlock).cast(),
// CHECK-NEXT:             flags: IS_GLOBAL,
// CHECK-NEXT:             reserved: 0,
// CHECK-NEXT:             invoke,
// CHECK-NEXT:             descriptor: &Block::<()>::DESCRIPTOR,
// CHECK-NEXT:             captures: (),
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub fn cell<T>(value: T) -> *mut T {
// CHECK-NEXT:         Box::into_raw(Box::new(value))
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub unsafe fn invoke(block: *mut c_void) -> *const c_void {
// CHECK-NEXT:         unsafe { (*(block as *const Block<()>)).invoke }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub unsafe fn captures<C>(block: *mut c_void) -> C {
// CHECK-NEXT:         unsafe { (&raw const (*(block as *const Block<C>)).captures).read() }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut global_double: *mut std::ffi::c_void = (std::ptr::addr_of!(__slate_block_0_literal)
// CHECK-NEXT:     as *const std::ffi::c_void)
// CHECK-NEXT:     as *mut std::ffi::c_void;
// CHECK-EMPTY:
// CHECK-NEXT: static mut exported: *mut std::ffi::c_void = (std::ptr::addr_of!(__slate_block_1_literal)
// CHECK-NEXT:     as *const std::ffi::c_void)
// CHECK-NEXT:     as *mut std::ffi::c_void;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let local_static: *mut std::ffi::c_void =
// CHECK-NEXT:         unsafe { __slate_blocks::new(__slate_block_2 as *const std::ffi::c_void, ()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = local_static;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], 1i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = global_double;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_1]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_1]], 4i32)
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_2:__t[0-9]+]] = exported;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_2]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_2]], 4i32)
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe { hits }
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut hits: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_0(__block: *mut std::ffi::c_void, [[V_0:__v[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     [[V_0]] * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_0_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_0 as *const std::ffi::c_void);
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_1(__block: *mut std::ffi::c_void, [[V_0]]: i32) -> i32 {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     [[V_0]] + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_1_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_1 as *const std::ffi::c_void);
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_2(__block: *mut std::ffi::c_void, [[V_0]]: i32) -> i32 {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hits = hits + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
