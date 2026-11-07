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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fblocks" }
// CHECK-NEXT: mod __slate_blocks {
// CHECK-NEXT:     use std::ffi::c_void;
// CHECK-NEXT:     use std::mem::{MaybeUninit, offset_of, size_of};
// CHECK-EMPTY:
// CHECK-NEXT:     #[cfg(not(target_vendor = "apple"))]
// CHECK-NEXT:     use slate_blocks as _;
// CHECK-EMPTY:
// CHECK-NEXT:     unsafe extern "C" {
// CHECK-NEXT:         static _NSConcreteStackBlock: [*const c_void; 32];
// CHECK-NEXT:         static _NSConcreteGlobalBlock: [*const c_void; 32];
// CHECK-NEXT:         fn _Block_object_assign(destination: *mut c_void, object: *const c_void, flags: i32);
// CHECK-NEXT:         fn _Block_object_dispose(object: *const c_void, flags: i32);
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     const HAS_COPY_DISPOSE: i32 = 1 << 25;
// CHECK-NEXT:     const IS_GLOBAL: i32 = 1 << 28;
// CHECK-NEXT:     const FIELD_IS_BLOCK: i32 = 7;
// CHECK-NEXT:     const FIELD_IS_BYREF: i32 = 8;
// CHECK-EMPTY:
// CHECK-NEXT:     pub const fn block_field(offset: usize) -> (usize, i32) {
// CHECK-NEXT:         (offset, FIELD_IS_BLOCK)
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub const fn byref_field(offset: usize) -> (usize, i32) {
// CHECK-NEXT:         (offset, FIELD_IS_BYREF)
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     #[repr(C)]
// CHECK-NEXT:     pub struct Byref<T> {
// CHECK-NEXT:         isa: *const c_void,
// CHECK-NEXT:         forwarding: *mut Byref<T>,
// CHECK-NEXT:         flags: i32,
// CHECK-NEXT:         size: i32,
// CHECK-NEXT:         value: T,
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub unsafe fn byref<T>(slot: *mut MaybeUninit<Byref<T>>) -> *mut Byref<T> {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let byref = slot.cast::<Byref<T>>();
// CHECK-NEXT:             (&raw mut (*byref).isa).write(std::ptr::null());
// CHECK-NEXT:             (&raw mut (*byref).forwarding).write(byref);
// CHECK-NEXT:             (&raw mut (*byref).flags).write(0);
// CHECK-NEXT:             (&raw mut (*byref).size).write(size_of::<Byref<T>>() as i32);
// CHECK-NEXT:             byref
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub unsafe fn store<T>(byref: *mut Byref<T>, initial: T) {
// CHECK-NEXT:         unsafe { value(byref).write(initial) }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub unsafe fn value<T>(byref: *mut Byref<T>) -> *mut T {
// CHECK-NEXT:         unsafe { &raw mut (*(*byref).forwarding).value }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub struct ByrefSlot<T> {
// CHECK-NEXT:         byref: MaybeUninit<Byref<T>>,
// CHECK-NEXT:         live: bool,
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl<T> ByrefSlot<T> {
// CHECK-NEXT:         pub const fn new() -> Self {
// CHECK-NEXT:             ByrefSlot {
// CHECK-NEXT:                 byref: MaybeUninit::uninit(),
// CHECK-NEXT:                 live: false,
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub unsafe fn slot<T>(slot: *mut ByrefSlot<T>) -> *mut Byref<T> {
// CHECK-NEXT:         unsafe { (&raw mut (*slot).byref).cast() }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub unsafe fn init<T>(slot: *mut ByrefSlot<T>) -> *mut Byref<T> {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             if (*slot).live {
// CHECK-NEXT:                 _Block_object_dispose(self::slot(slot) as *const c_void, FIELD_IS_BYREF);
// CHECK-NEXT:             }
// CHECK-NEXT:             (*slot).live = true;
// CHECK-NEXT:             byref(&raw mut (*slot).byref)
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl<T> Drop for ByrefSlot<T> {
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             if self.live {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     _Block_object_dispose(self.byref.as_ptr() as *const c_void, FIELD_IS_BYREF)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub struct Scope<T>(*mut Byref<T>);
// CHECK-EMPTY:
// CHECK-NEXT:     pub fn scope<T>(byref: *mut Byref<T>) -> Scope<T> {
// CHECK-NEXT:         Scope(byref)
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl<T> Drop for Scope<T> {
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             unsafe { _Block_object_dispose(self.0 as *const c_void, FIELD_IS_BYREF) }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     #[repr(C)]
// CHECK-NEXT:     pub struct Descriptor {
// CHECK-NEXT:         reserved: usize,
// CHECK-NEXT:         size: usize,
// CHECK-NEXT:         copy: unsafe extern "C" fn(*mut c_void, *const c_void),
// CHECK-NEXT:         dispose: unsafe extern "C" fn(*const c_void),
// CHECK-NEXT:         captures_offset: usize,
// CHECK-NEXT:         fields: &'static [(usize, i32)],
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
// CHECK-NEXT:     pub const fn descriptor<C>(fields: &'static [(usize, i32)]) -> Descriptor {
// CHECK-NEXT:         Descriptor {
// CHECK-NEXT:             reserved: 0,
// CHECK-NEXT:             size: size_of::<Block<C>>(),
// CHECK-NEXT:             copy: copy_helper,
// CHECK-NEXT:             dispose: dispose_helper,
// CHECK-NEXT:             captures_offset: offset_of!(Block<C>, captures),
// CHECK-NEXT:             fields,
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     unsafe fn field_addresses(
// CHECK-NEXT:         block: *const c_void,
// CHECK-NEXT:     ) -> impl Iterator<Item = (*mut *const c_void, i32)> {
// CHECK-NEXT:         let descriptor = unsafe { (*(block as *const Block<()>)).descriptor };
// CHECK-NEXT:         descriptor.fields.iter().map(move |&(offset, kind)| {
// CHECK-NEXT:             let address = block.wrapping_byte_add(descriptor.captures_offset + offset);
// CHECK-NEXT:             (address as *mut *const c_void, kind)
// CHECK-NEXT:         })
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     unsafe extern "C" fn copy_helper(destination: *mut c_void, source: *const c_void) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             for (field, kind) in field_addresses(source) {
// CHECK-NEXT:                 let offset = field.byte_offset_from(source);
// CHECK-NEXT:                 _Block_object_assign(destination.byte_offset(offset), *field, kind);
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     unsafe extern "C" fn dispose_helper(block: *const c_void) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             for (field, kind) in field_addresses(block) {
// CHECK-NEXT:                 _Block_object_dispose(*field, kind);
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub unsafe fn stack<C>(
// CHECK-NEXT:         slot: *mut MaybeUninit<Block<C>>,
// CHECK-NEXT:         invoke: *const c_void,
// CHECK-NEXT:         descriptor: &'static Descriptor,
// CHECK-NEXT:         captures: C,
// CHECK-NEXT:     ) -> *mut c_void {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let flags = if descriptor.fields.is_empty() {
// CHECK-NEXT:                 0
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 HAS_COPY_DISPOSE
// CHECK-NEXT:             };
// CHECK-NEXT:             (*slot).write(Block {
// CHECK-NEXT:                 isa: (&raw const _NSConcreteStackBlock).cast(),
// CHECK-NEXT:                 flags,
// CHECK-NEXT:                 reserved: 0,
// CHECK-NEXT:                 invoke,
// CHECK-NEXT:                 descriptor,
// CHECK-NEXT:                 captures,
// CHECK-NEXT:             });
// CHECK-NEXT:             slot.cast()
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     unsafe impl<C> Sync for Block<C> {}
// CHECK-EMPTY:
// CHECK-NEXT:     static GLOBAL_DESCRIPTOR: Descriptor = descriptor::<()>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT:     pub const fn global(invoke: *const c_void) -> Block<()> {
// CHECK-NEXT:         Block {
// CHECK-NEXT:             isa: (&raw const _NSConcreteGlobalBlock).cast(),
// CHECK-NEXT:             flags: IS_GLOBAL,
// CHECK-NEXT:             reserved: 0,
// CHECK-NEXT:             invoke,
// CHECK-NEXT:             descriptor: &GLOBAL_DESCRIPTOR,
// CHECK-NEXT:             captures: (),
// CHECK-NEXT:         }
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
// CHECK-NEXT:     let local_static: *mut std::ffi::c_void = (std::ptr::addr_of!(__slate_block_2_literal)
// CHECK-NEXT:         as *const std::ffi::c_void)
// CHECK-NEXT:         as *mut std::ffi::c_void;
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
// CHECK-NEXT: static __slate_block_2_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_2 as *const std::ffi::c_void);
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
