// { dg-options "-fblocks" }
#include <malloc.h>
#include <stdio.h>

extern void *_Block_copy(const void *);
extern void _Block_release(const void *);

typedef int (^intfn)(int);

static int apply(intfn f, int v) { return f(v); }
int (*volatile apply_ptr)(intfn, int) = apply;

static intfn chain(intfn inner) {
  return _Block_copy(^(int x) { return inner(x) * 2; });
}

int main(void) {
  int base = 5;
  intfn literal = ^(int x) { return x + base; };
  intfn copy = _Block_copy(literal);
  printf("copy is new object: %d\n", (void *)copy != (void *)literal);
  printf("copy of heap block is same: %d\n", _Block_copy(copy) == (void *)copy);
  _Block_release(copy);

  intfn plain = ^(int x) { return x; };
  printf("captureless copy is same: %d\n", _Block_copy(plain) == (void *)plain);

  intfn b = _Block_copy(^(int x) { return x + base; });
  intfn c = chain(b);
  _Block_release(b);
  printf("chained: %d\n", c(1));
  _Block_release(c);

  size_t before = mallinfo2().uordblks;
  long sum = 0;
  for (int i = 0; i < 100000; i++) {
    int k = i & 7;
    sum += apply_ptr(^(int x) { return x + k; }, i);
  }
  size_t after = mallinfo2().uordblks;
  printf("%ld heap grew: %d\n", sum, after > before + (1 << 20));
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
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn mallinfo2() -> mallinfo2;
// CHECK-NEXT:     fn _Block_copy([[ARG_0:arg[0-9]+]]: *const ()) -> *mut ();
// CHECK-NEXT:     fn _Block_release([[ARG_0]]: *const ());
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mallinfo2 {
// CHECK-NEXT:     arena: u64,
// CHECK-NEXT:     ordblks: u64,
// CHECK-NEXT:     smblks: u64,
// CHECK-NEXT:     hblks: u64,
// CHECK-NEXT:     hblkhd: u64,
// CHECK-NEXT:     usmblks: u64,
// CHECK-NEXT:     fsmblks: u64,
// CHECK-NEXT:     uordblks: u64,
// CHECK-NEXT:     fordblks: u64,
// CHECK-NEXT:     keepcost: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.apply")]
// CHECK-NEXT: extern "C-unwind" fn apply(f: *mut std::ffi::c_void, v: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = f;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], v)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut apply_ptr: Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32> =
// CHECK-NEXT:     Some(apply);
// CHECK-EMPTY:
// CHECK-NEXT: fn chain(inner: *mut std::ffi::c_void) -> *mut std::ffi::c_void {
// CHECK-NEXT:     let mut __slate_block_0_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_0_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_0_slot),
// CHECK-NEXT:             __slate_block_0 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_0_descriptor,
// CHECK-NEXT:             __slate_block_0_captures { c0: inner },
// CHECK-NEXT:         ) as *const ())
// CHECK-NEXT:     }) as *mut std::ffi::c_void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: mallinfo2 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: mallinfo2 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let base: i32 = 5;
// CHECK-NEXT:     let mut __slate_block_1_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_1_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let literal: *mut std::ffi::c_void = unsafe {
// CHECK-NEXT:         __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_1_slot),
// CHECK-NEXT:             __slate_block_1 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_1_descriptor,
// CHECK-NEXT:             __slate_block_1_captures { c0: base },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let copy: *mut std::ffi::c_void =
// CHECK-NEXT:         (unsafe { _Block_copy(literal as *const ()) }) as *mut std::ffi::c_void;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "copy is new object: {}",
// CHECK-NEXT:         ((copy as *mut ()) != (literal as *mut ())) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "copy of heap block is same: {}",
// CHECK-NEXT:         ((unsafe { _Block_copy(copy as *const ()) }) == (copy as *mut ())) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { _Block_release(copy as *const ()) };
// CHECK-NEXT:     let plain: *mut std::ffi::c_void = (std::ptr::addr_of!(__slate_block_2_literal)
// CHECK-NEXT:         as *const std::ffi::c_void) as *mut std::ffi::c_void;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "captureless copy is same: {}",
// CHECK-NEXT:         ((unsafe { _Block_copy(plain as *const ()) }) == (plain as *mut ())) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut __slate_block_3_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_3_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let b: *mut std::ffi::c_void = (unsafe {
// CHECK-NEXT:         _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_3_slot),
// CHECK-NEXT:             __slate_block_3 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_3_descriptor,
// CHECK-NEXT:             __slate_block_3_captures { c0: base },
// CHECK-NEXT:         ) as *const ())
// CHECK-NEXT:     }) as *mut std::ffi::c_void;
// CHECK-NEXT:     let c: *mut std::ffi::c_void = chain(b);
// CHECK-NEXT:     unsafe { _Block_release(b as *const ()) };
// CHECK-NEXT:     println!("chained: {}", unsafe {
// CHECK-NEXT:         let [[T_0]] = c;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], 1i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe { _Block_release(c as *const ()) };
// CHECK-NEXT:     let before: u64 = unsafe {
// CHECK-NEXT:         (*{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = mallinfo2();
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         })
// CHECK-NEXT:         .uordblks
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut sum: i64 = 0;
// CHECK-NEXT:     for i in 0i32..100000i32 {
// CHECK-NEXT:         let k: i32 = i & 7;
// CHECK-NEXT:         let mut __slate_block_4_slot: std::mem::MaybeUninit<
// CHECK-NEXT:             __slate_blocks::Block<__slate_block_4_captures>,
// CHECK-NEXT:         > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(apply_ptr)).unwrap()(
// CHECK-NEXT:                 __slate_blocks::stack(
// CHECK-NEXT:                     std::ptr::addr_of_mut!(__slate_block_4_slot),
// CHECK-NEXT:                     __slate_block_4 as *const std::ffi::c_void,
// CHECK-NEXT:                     &__slate_block_4_descriptor,
// CHECK-NEXT:                     __slate_block_4_captures { c0: k },
// CHECK-NEXT:                 ),
// CHECK-NEXT:                 i,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         sum += [[V_0]] as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let after: u64 = unsafe {
// CHECK-NEXT:         (*{
// CHECK-NEXT:             [[SLATE_COMPOUND_1]] = mallinfo2();
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:         })
// CHECK-NEXT:         .uordblks
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld heap grew: %d\n".as_ptr(),
// CHECK-NEXT:             sum,
// CHECK-NEXT:             (after > before.wrapping_add(1048576u64)) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_0_captures {
// CHECK-NEXT:     c0: *mut std::ffi::c_void,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_0(__block: *mut std::ffi::c_void, [[V_1:__v[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_0_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_0_captures>(__block) };
// CHECK-NEXT:     let inner: *mut std::ffi::c_void = __captures.c0;
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         let [[T_0]] = inner;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], [[V_1]])
// CHECK-NEXT:     }) * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_0_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_0_captures>(&[__slate_blocks::block_field(
// CHECK-NEXT:         std::mem::offset_of!(__slate_block_0_captures, c0),
// CHECK-NEXT:     )]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_1_captures {
// CHECK-NEXT:     c0: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_1(__block: *mut std::ffi::c_void, [[V_1]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_1_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_1_captures>(__block) };
// CHECK-NEXT:     let base: i32 = __captures.c0;
// CHECK-NEXT:     [[V_1]] + base
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_1_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_1_captures>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_2(__block: *mut std::ffi::c_void, [[V_1]]: i32) -> i32 {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     [[V_1]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_2_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_2 as *const std::ffi::c_void);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_3_captures {
// CHECK-NEXT:     c0: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_3(__block: *mut std::ffi::c_void, [[V_1]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_3_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_3_captures>(__block) };
// CHECK-NEXT:     let base: i32 = __captures.c0;
// CHECK-NEXT:     [[V_1]] + base
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_3_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_3_captures>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_4_captures {
// CHECK-NEXT:     c0: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_4(__block: *mut std::ffi::c_void, [[V_2:__v[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_4_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_4_captures>(__block) };
// CHECK-NEXT:     let k: i32 = __captures.c0;
// CHECK-NEXT:     [[V_2]] + k
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_4_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_4_captures>(&[]);
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
