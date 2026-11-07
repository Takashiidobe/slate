// { dg-options "-fblocks" }
#include <malloc.h>
#include <stdio.h>

extern void *_Block_copy(const void *);
extern void _Block_release(const void *);

typedef int (^counter)(void);
typedef int (^unary)(int);

static int self_referential(void) {
  __block unary fib = ^(int n) { return n < 2 ? n : fib(n - 1) + fib(n - 2); };
  __block unary depth = _Block_copy(^(int n) { return n ? depth(n - 1) + 1 : 0; });
  int result = fib(15) * 100 + depth(7);
  _Block_release(depth);
  return result;
}

static counter make_counter(int start) {
  __block int count = start;
  return _Block_copy(^{ return ++count; });
}

struct pair {
  counter up;
  counter peek;
};

static struct pair make_pair(void) {
  __block long total = 100;
  struct pair pair = {
      _Block_copy(^{ return (int)(total += 5); }),
      _Block_copy(^{ return (int)total; }),
  };
  return pair;
}

int main(void) {
  __block int x = 1;
  int *before = &x;
  counter read = _Block_copy(^{ return x; });
  *before = 2;
  printf("block sees %d, x is %d, moved %d\n", read(), x, before != &x);
  x = 7;
  printf("after copy block sees %d\n", read());
  _Block_release(read);

  counter c = make_counter(10);
  printf("%d %d %d\n", c(), c(), c());
  _Block_release(c);

  struct pair pair = make_pair();
  pair.up();
  pair.up();
  printf("shared %d\n", pair.peek());
  _Block_release(pair.up);
  printf("survives one release %d\n", pair.peek());
  _Block_release(pair.peek);

  counter loops[3];
  for (int i = 0; i < 3; i++) {
    __block int mine = i * 10;
    loops[i] = _Block_copy(^{ return mine++; });
  }
  for (int i = 0; i < 3; i++) {
    loops[i]();
    printf("loop %d -> %d\n", i, loops[i]());
    _Block_release(loops[i]);
  }

  printf("self %d\n", self_referential());

  size_t start = mallinfo2().uordblks;
  long sum = 0;
  for (int i = 0; i < 100000; i++) {
    counter made = make_counter(i);
    sum += made();
    _Block_release(made);
  }
  size_t end = mallinfo2().uordblks;
  printf("%ld heap grew: %d\n", sum, end > start + (1 << 20));
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
// CHECK-NEXT: fn self_referential() -> i32 {
// CHECK-NEXT:     let mut fib_byref: std::mem::MaybeUninit<__slate_blocks::Byref<*mut std::ffi::c_void>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let fib: *mut __slate_blocks::Byref<*mut std::ffi::c_void> =
// CHECK-NEXT:         unsafe { __slate_blocks::byref(std::ptr::addr_of_mut!(fib_byref)) };
// CHECK-NEXT:     let mut __slate_block_0_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_0_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_blocks::store(
// CHECK-NEXT:             fib,
// CHECK-NEXT:             __slate_blocks::stack(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(__slate_block_0_slot),
// CHECK-NEXT:                 __slate_block_0 as *const std::ffi::c_void,
// CHECK-NEXT:                 &__slate_block_0_descriptor,
// CHECK-NEXT:                 __slate_block_0_captures { c0: fib },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let fib_scope = __slate_blocks::scope(fib);
// CHECK-NEXT:     let mut depth_byref: std::mem::MaybeUninit<__slate_blocks::Byref<*mut std::ffi::c_void>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let depth: *mut __slate_blocks::Byref<*mut std::ffi::c_void> =
// CHECK-NEXT:         unsafe { __slate_blocks::byref(std::ptr::addr_of_mut!(depth_byref)) };
// CHECK-NEXT:     let mut __slate_block_1_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_1_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_blocks::store(
// CHECK-NEXT:             depth,
// CHECK-NEXT:             _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(__slate_block_1_slot),
// CHECK-NEXT:                 __slate_block_1 as *const std::ffi::c_void,
// CHECK-NEXT:                 &__slate_block_1_descriptor,
// CHECK-NEXT:                 __slate_block_1_captures { c0: depth },
// CHECK-NEXT:             ) as *const ()) as *mut std::ffi::c_void,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let depth_scope = __slate_blocks::scope(depth);
// CHECK-NEXT:     let result: i32 = (unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = *__slate_blocks::value(fib);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], 15i32)
// CHECK-NEXT:     }) * 100
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = *__slate_blocks::value(depth);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_1]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_1]], 7i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     unsafe { _Block_release(*__slate_blocks::value(depth) as *const ()) };
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn make_counter(start: i32) -> *mut std::ffi::c_void {
// CHECK-NEXT:     let mut count_byref: std::mem::MaybeUninit<__slate_blocks::Byref<i32>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let count: *mut __slate_blocks::Byref<i32> =
// CHECK-NEXT:         unsafe { __slate_blocks::byref(std::ptr::addr_of_mut!(count_byref)) };
// CHECK-NEXT:     unsafe { __slate_blocks::store(count, start) };
// CHECK-NEXT:     let count_scope = __slate_blocks::scope(count);
// CHECK-NEXT:     let mut __slate_block_2_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_2_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_2_slot),
// CHECK-NEXT:             __slate_block_2 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_2_descriptor,
// CHECK-NEXT:             __slate_block_2_captures { c0: count },
// CHECK-NEXT:         ) as *const ())
// CHECK-NEXT:     }) as *mut std::ffi::c_void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     up: *mut std::ffi::c_void,
// CHECK-NEXT:     peek: *mut std::ffi::c_void,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn make_pair() -> pair {
// CHECK-NEXT:     let mut total_byref: std::mem::MaybeUninit<__slate_blocks::Byref<i64>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let total: *mut __slate_blocks::Byref<i64> =
// CHECK-NEXT:         unsafe { __slate_blocks::byref(std::ptr::addr_of_mut!(total_byref)) };
// CHECK-NEXT:     unsafe { __slate_blocks::store(total, 100i64) };
// CHECK-NEXT:     let total_scope = __slate_blocks::scope(total);
// CHECK-NEXT:     let mut __slate_block_3_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_3_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let mut __slate_block_4_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_4_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let pair: pair = pair {
// CHECK-NEXT:         up: (unsafe {
// CHECK-NEXT:             _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(__slate_block_3_slot),
// CHECK-NEXT:                 __slate_block_3 as *const std::ffi::c_void,
// CHECK-NEXT:                 &__slate_block_3_descriptor,
// CHECK-NEXT:                 __slate_block_3_captures { c0: total },
// CHECK-NEXT:             ) as *const ())
// CHECK-NEXT:         }) as *mut std::ffi::c_void,
// CHECK-NEXT:         peek: (unsafe {
// CHECK-NEXT:             _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(__slate_block_4_slot),
// CHECK-NEXT:                 __slate_block_4 as *const std::ffi::c_void,
// CHECK-NEXT:                 &__slate_block_4_descriptor,
// CHECK-NEXT:                 __slate_block_4_captures { c0: total },
// CHECK-NEXT:             ) as *const ())
// CHECK-NEXT:         }) as *mut std::ffi::c_void,
// CHECK-NEXT:     };
// CHECK-NEXT:     return pair;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: mallinfo2 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: mallinfo2 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut x_byref: std::mem::MaybeUninit<__slate_blocks::Byref<i32>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let x: *mut __slate_blocks::Byref<i32> =
// CHECK-NEXT:         unsafe { __slate_blocks::byref(std::ptr::addr_of_mut!(x_byref)) };
// CHECK-NEXT:     unsafe { __slate_blocks::store(x, 1i32) };
// CHECK-NEXT:     let x_scope = __slate_blocks::scope(x);
// CHECK-NEXT:     let before: *mut i32 = unsafe { std::ptr::addr_of_mut!(*__slate_blocks::value(x)) };
// CHECK-NEXT:     let mut __slate_block_5_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_5_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let read: *mut std::ffi::c_void = (unsafe {
// CHECK-NEXT:         _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_5_slot),
// CHECK-NEXT:             __slate_block_5 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_5_descriptor,
// CHECK-NEXT:             __slate_block_5_captures { c0: x },
// CHECK-NEXT:         ) as *const ())
// CHECK-NEXT:     }) as *mut std::ffi::c_void;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *before = 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "block sees {}, x is {}, moved {}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_0]] = read;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_0]])
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe { *__slate_blocks::value(x) },
// CHECK-NEXT:         (before != unsafe { std::ptr::addr_of_mut!(*__slate_blocks::value(x)) }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *__slate_blocks::value(x) = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("after copy block sees {}", unsafe {
// CHECK-NEXT:         let [[T_1]] = read;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_1]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_1]])
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe { _Block_release(read as *const ()) };
// CHECK-NEXT:     let c: *mut std::ffi::c_void = make_counter(10i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_2:__t[0-9]+]] = c;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_2]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_2]])
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_3:__t[0-9]+]] = c;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_3]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_3]])
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_4:__t[0-9]+]] = c;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_4]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_4]])
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { _Block_release(c as *const ()) };
// CHECK-NEXT:     let pair: pair = make_pair();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = pair.up;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_5]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_5]])
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_6:__t[0-9]+]] = pair.up;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_6]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_6]])
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("shared {}", unsafe {
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = pair.peek;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_7]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_7]])
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe { _Block_release(pair.up as *const ()) };
// CHECK-NEXT:     println!("survives one release {}", unsafe {
// CHECK-NEXT:         let [[T_8:__t[0-9]+]] = pair.peek;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_8]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_8]])
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe { _Block_release(pair.peek as *const ()) };
// CHECK-NEXT:     let mut loops: __SlateAlign16<[*mut std::ffi::c_void; 3]> =
// CHECK-NEXT:         __SlateAlign16([0 as *mut std::ffi::c_void; 3]);
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         let mut mine_byref: std::mem::MaybeUninit<__slate_blocks::Byref<i32>> =
// CHECK-NEXT:             std::mem::MaybeUninit::uninit();
// CHECK-NEXT:         let mine: *mut __slate_blocks::Byref<i32> =
// CHECK-NEXT:             unsafe { __slate_blocks::byref(std::ptr::addr_of_mut!(mine_byref)) };
// CHECK-NEXT:         unsafe { __slate_blocks::store(mine, i * 10) };
// CHECK-NEXT:         let mine_scope = __slate_blocks::scope(mine);
// CHECK-NEXT:         let mut __slate_block_6_slot: std::mem::MaybeUninit<
// CHECK-NEXT:             __slate_blocks::Block<__slate_block_6_captures>,
// CHECK-NEXT:         > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:         loops.0[i as usize] = (unsafe {
// CHECK-NEXT:             _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(__slate_block_6_slot),
// CHECK-NEXT:                 __slate_block_6 as *const std::ffi::c_void,
// CHECK-NEXT:                 &__slate_block_6_descriptor,
// CHECK-NEXT:                 __slate_block_6_captures { c0: mine },
// CHECK-NEXT:             ) as *const ())
// CHECK-NEXT:         }) as *mut std::ffi::c_void;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: for i in 0i32..3i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_9:__t[0-9]+]] = loops.0[i as usize];
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_9]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_9]])
// CHECK-NEXT:         };
// CHECK-NEXT:         println!("loop {} -> {}", i, unsafe {
// CHECK-NEXT:             let [[T_10:__t[0-9]+]] = loops.0[i as usize];
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_10]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_10]])
// CHECK-NEXT:         });
// CHECK-NEXT:         unsafe { _Block_release(loops.0[i as usize] as *const ()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("self {}", self_referential());
// CHECK-NEXT:     let start: u64 = unsafe {
// CHECK-NEXT:         (*{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = mallinfo2();
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         })
// CHECK-NEXT:         .uordblks
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut sum: i64 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: for i in 0i32..100000i32 {
// CHECK-NEXT:         let made: *mut std::ffi::c_void = make_counter(i);
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             let [[T_11:__t[0-9]+]] = made;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_11]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_11]])
// CHECK-NEXT:         };
// CHECK-NEXT:         sum += [[V_0]] as i64;
// CHECK-NEXT:         unsafe { _Block_release(made as *const ()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let end: u64 = unsafe {
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
// CHECK-NEXT:             (end > start.wrapping_add(1048576u64)) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_0_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<*mut std::ffi::c_void>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_0(__block: *mut std::ffi::c_void, [[V_1:__v[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_0_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_0_captures>(__block) };
// CHECK-NEXT:     let fib: *mut __slate_blocks::Byref<*mut std::ffi::c_void> = __captures.c0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = if [[V_1]] < 2 {
// CHECK-NEXT:         [[V_1]]
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             let [[T_0]] = *__slate_blocks::value(fib);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_0]], [[V_1]] - 1)
// CHECK-NEXT:         }) + unsafe {
// CHECK-NEXT:             let [[T_1]] = *__slate_blocks::value(fib);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_1]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_1]], [[V_1]] - 2)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_2]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_0_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_0_captures>(&[__slate_blocks::byref_field(
// CHECK-NEXT:         std::mem::offset_of!(__slate_block_0_captures, c0),
// CHECK-NEXT:     )]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_1_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<*mut std::ffi::c_void>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_1(__block: *mut std::ffi::c_void, [[V_1]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_1_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_1_captures>(__block) };
// CHECK-NEXT:     let depth: *mut __slate_blocks::Byref<*mut std::ffi::c_void> = __captures.c0;
// CHECK-NEXT:     let [[V_2]]: i32 = if [[V_1]] != 0 {
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             let [[T_0]] = *__slate_blocks::value(depth);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_0]], [[V_1]] - 1)
// CHECK-NEXT:         }) + 1
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_2]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_1_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_1_captures>(&[__slate_blocks::byref_field(
// CHECK-NEXT:         std::mem::offset_of!(__slate_block_1_captures, c0),
// CHECK-NEXT:     )]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_2_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<i32>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_2(__block: *mut std::ffi::c_void) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_2_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_2_captures>(__block) };
// CHECK-NEXT:     let count: *mut __slate_blocks::Byref<i32> = __captures.c0;
// CHECK-NEXT:     let [[V_1]]: i32 = unsafe { *__slate_blocks::value(count) };
// CHECK-NEXT:     let [[V_2]]: i32 = [[V_1]] + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *__slate_blocks::value(count) = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_2]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_2_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_2_captures>(&[__slate_blocks::byref_field(
// CHECK-NEXT:         std::mem::offset_of!(__slate_block_2_captures, c0),
// CHECK-NEXT:     )]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_3_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<i64>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_3(__block: *mut std::ffi::c_void) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_3_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_3_captures>(__block) };
// CHECK-NEXT:     let total: *mut __slate_blocks::Byref<i64> = __captures.c0;
// CHECK-NEXT:     let [[V_1]]: i64 = unsafe { *__slate_blocks::value(total) };
// CHECK-NEXT:     let [[V_2]]: i64 = [[V_1]] + 5;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *__slate_blocks::value(total) = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_2]] as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_3_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_3_captures>(&[__slate_blocks::byref_field(
// CHECK-NEXT:         std::mem::offset_of!(__slate_block_3_captures, c0),
// CHECK-NEXT:     )]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_4_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<i64>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_4(__block: *mut std::ffi::c_void) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_4_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_4_captures>(__block) };
// CHECK-NEXT:     let total: *mut __slate_blocks::Byref<i64> = __captures.c0;
// CHECK-NEXT:     (unsafe { *__slate_blocks::value(total) }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_4_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_4_captures>(&[__slate_blocks::byref_field(
// CHECK-NEXT:         std::mem::offset_of!(__slate_block_4_captures, c0),
// CHECK-NEXT:     )]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_5_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<i32>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_5(__block: *mut std::ffi::c_void) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_5_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_5_captures>(__block) };
// CHECK-NEXT:     let x: *mut __slate_blocks::Byref<i32> = __captures.c0;
// CHECK-NEXT:     unsafe { *__slate_blocks::value(x) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_5_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_5_captures>(&[__slate_blocks::byref_field(
// CHECK-NEXT:         std::mem::offset_of!(__slate_block_5_captures, c0),
// CHECK-NEXT:     )]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_6_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<i32>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_6(__block: *mut std::ffi::c_void) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_6_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_6_captures>(__block) };
// CHECK-NEXT:     let mine: *mut __slate_blocks::Byref<i32> = __captures.c0;
// CHECK-NEXT:     let [[V_1]]: i32 = unsafe { *__slate_blocks::value(mine) };
// CHECK-NEXT:     let [[V_2]]: i32 = [[V_1]] + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *__slate_blocks::value(mine) = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_6_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_6_captures>(&[__slate_blocks::byref_field(
// CHECK-NEXT:         std::mem::offset_of!(__slate_block_6_captures, c0),
// CHECK-NEXT:     )]);
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
