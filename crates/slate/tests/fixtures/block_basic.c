// { dg-options "-fblocks" }
#include <stdio.h>

extern void *_Block_copy(const void *);
extern void _Block_release(const void *);

typedef int (^binop)(int, int);
typedef struct { int x, y; double w; } point;

static int apply(binop f, int a, int b) { return f(a, b); }

static binop make_adder(int n) {
  return _Block_copy(^(int a, int b) { return a + b + n; });
}

static int (^make_counter(void))(void) {
  __block int count = 0;
  return _Block_copy(^{ return ++count; });
}

static point shift(point (^f)(point, int), point p) { return f(p, 3); }

int main(void) {
  int base = 10;
  binop add = ^(int a, int b) { return a + b + base; };
  base = 99;
  printf("%d\n", apply(add, 1, 2));

  binop bound = make_adder(100);
  printf("%d %d\n", bound(1, 2), apply(bound, 5, 5));
  _Block_release(bound);

  int (^counter)(void) = make_counter();
  counter();
  counter();
  printf("%d\n", counter());

  __block int total = 0;
  void (^accumulate)(int) = ^(int v) { total += v; };
  for (int i = 1; i <= 4; i++) accumulate(i);
  printf("%d\n", total);

  point p = {1, 2, 0.5};
  point (^mover)(point, int) = ^point(point q, int d) {
    q.x += d;
    q.y -= d;
    q.w *= 2;
    return q;
  };
  point r = shift(mover, p);
  printf("%d %d %.1f\n", r.x, r.y, r.w);

  int (^outer)(int) = ^(int a) {
    int (^inner)(int) = ^(int b) { return a * b + base; };
    return inner(a + 1);
  };
  printf("%d\n", outer(4));

  void (^none)(void) = 0;
  printf("%d %d\n", none == 0, add != 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
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
// CHECK-EMPTY:
// CHECK-NEXT:     pub const fn block_field(offset: usize) -> (usize, i32) {
// CHECK-NEXT:         (offset, FIELD_IS_BLOCK)
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
// CHECK-NEXT: unsafe extern "C" {
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
// CHECK-NEXT: struct __SlateRecord_block_basic_c_8 {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT:     w: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn apply(f: *mut std::ffi::c_void, a: i32, b: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = f;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], a, b)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn make_adder(n: i32) -> *mut std::ffi::c_void {
// CHECK-NEXT:     let mut __slate_block_0_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_0_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_0_slot),
// CHECK-NEXT:             __slate_block_0 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_0_descriptor,
// CHECK-NEXT:             __slate_block_0_captures { c0: n },
// CHECK-NEXT:         ) as *const ())
// CHECK-NEXT:     }) as *mut std::ffi::c_void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn make_counter() -> *mut std::ffi::c_void {
// CHECK-NEXT:     let mut __slate_block_1_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_1_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let count: *mut i32 = __slate_blocks::cell(0i32);
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_1_slot),
// CHECK-NEXT:             __slate_block_1 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_1_descriptor,
// CHECK-NEXT:             __slate_block_1_captures { c0: count },
// CHECK-NEXT:         ) as *const ())
// CHECK-NEXT:     }) as *mut std::ffi::c_void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn shift(
// CHECK-NEXT:     f: *mut std::ffi::c_void,
// CHECK-NEXT:     p: __SlateRecord_block_basic_c_8,
// CHECK-NEXT: ) -> __SlateRecord_block_basic_c_8 {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         let [[T_0]] = f;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<
// CHECK-NEXT:                     unsafe extern "C-unwind" fn(
// CHECK-NEXT:                         *mut std::ffi::c_void,
// CHECK-NEXT:                         __SlateRecord_block_basic_c_8,
// CHECK-NEXT:                         i32,
// CHECK-NEXT:                     )
// CHECK-NEXT:                         -> __SlateRecord_block_basic_c_8,
// CHECK-NEXT:                 >,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], p, 3i32)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut __slate_block_2_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_2_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let mut __slate_block_3_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_3_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let mut __slate_block_5_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_5_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let mut base: i32 = 10;
// CHECK-NEXT:     let add: *mut std::ffi::c_void = unsafe {
// CHECK-NEXT:         __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_2_slot),
// CHECK-NEXT:             __slate_block_2 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_2_descriptor,
// CHECK-NEXT:             __slate_block_2_captures { c0: base },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     base = 99;
// CHECK-NEXT:     println!("{}", apply(add, 1i32, 2i32));
// CHECK-NEXT:     let bound: *mut std::ffi::c_void = make_adder(100i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_0]] = bound;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_0]], 1i32, 2i32)
// CHECK-NEXT:         },
// CHECK-NEXT:         apply(bound, 5i32, 5i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { _Block_release(bound as *const ()) };
// CHECK-NEXT:     let counter: *mut std::ffi::c_void = make_counter();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = counter;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_1]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_1]])
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = counter;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_2]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_2]])
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = counter;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_3]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_3]])
// CHECK-NEXT:     });
// CHECK-NEXT:     let total: *mut i32 = __slate_blocks::cell(0i32);
// CHECK-NEXT:     let accumulate: *mut std::ffi::c_void = unsafe {
// CHECK-NEXT:         __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_3_slot),
// CHECK-NEXT:             __slate_block_3 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_3_descriptor,
// CHECK-NEXT:             __slate_block_3_captures { c0: total },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: for i in 1..5i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_4:__t[0-9]+]] = accumulate;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32)>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_4]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_4]], i)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", unsafe { *total });
// CHECK-NEXT:     let p: __SlateRecord_block_basic_c_8 = __SlateRecord_block_basic_c_8 {
// CHECK-NEXT:         x: 1,
// CHECK-NEXT:         y: 2,
// CHECK-NEXT:         w: 0.5f64,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mover: *mut std::ffi::c_void = (std::ptr::addr_of!(__slate_block_4_literal)
// CHECK-NEXT:         as *const std::ffi::c_void) as *mut std::ffi::c_void;
// CHECK-NEXT:     let r: __SlateRecord_block_basic_c_8 = shift(mover, p);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%d %d %.1f\n".as_ptr(), r.x, r.y, r.w) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let outer: *mut std::ffi::c_void = unsafe {
// CHECK-NEXT:         __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_5_slot),
// CHECK-NEXT:             __slate_block_5 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_5_descriptor,
// CHECK-NEXT:             __slate_block_5_captures { c0: base },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = outer;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_5]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_5]], 4i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     let none: *mut std::ffi::c_void = std::ptr::null_mut::<std::ffi::c_void>();
// CHECK-NEXT:     println!("{} {}", none.is_null() as i32, !add.is_null() as i32);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_0_captures {
// CHECK-NEXT:     c0: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_0(__block: *mut std::ffi::c_void, [[V_0:__v[0-9]+]]: i32, [[V_1:__v[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_0_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_0_captures>(__block) };
// CHECK-NEXT:     let n: i32 = __captures.c0;
// CHECK-NEXT:     [[V_0]] + [[V_1]] + n
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_0_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_0_captures>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_1_captures {
// CHECK-NEXT:     c0: *mut i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_1(__block: *mut std::ffi::c_void) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_1_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_1_captures>(__block) };
// CHECK-NEXT:     let count: *mut i32 = __captures.c0;
// CHECK-NEXT:     let [[V_1]]: i32 = (unsafe { *count }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *count = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_1_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_1_captures>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_2_captures {
// CHECK-NEXT:     c0: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_2(__block: *mut std::ffi::c_void, [[V_0]]: i32, [[V_1]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_2_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_2_captures>(__block) };
// CHECK-NEXT:     let base: i32 = __captures.c0;
// CHECK-NEXT:     [[V_0]] + [[V_1]] + base
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_2_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_2_captures>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_3_captures {
// CHECK-NEXT:     c0: *mut i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_3(__block: *mut std::ffi::c_void, [[V_0]]: i32) {
// CHECK-NEXT:     let __captures: __slate_block_3_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_3_captures>(__block) };
// CHECK-NEXT:     let total: *mut i32 = __captures.c0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *total = *total + [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_3_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_3_captures>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_4(
// CHECK-NEXT:     __block: *mut std::ffi::c_void,
// CHECK-NEXT:     mut [[V_1]]: __SlateRecord_block_basic_c_8,
// CHECK-NEXT:     [[V_2:__v[0-9]+]]: i32,
// CHECK-NEXT: ) -> __SlateRecord_block_basic_c_8 {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     [[V_1]].x += [[V_2]];
// CHECK-NEXT:     [[V_1]].y -= [[V_2]];
// CHECK-NEXT:     [[V_1]].w *= 2i32 as f64;
// CHECK-NEXT:     return [[V_1]];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_4_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_4 as *const std::ffi::c_void);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_5_captures {
// CHECK-NEXT:     c0: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_6_captures {
// CHECK-NEXT:     c0: i32,
// CHECK-NEXT:     c1: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_6(__block: *mut std::ffi::c_void, [[V_3:__v[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_6_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_6_captures>(__block) };
// CHECK-NEXT:     let [[V_1]]: i32 = __captures.c0;
// CHECK-NEXT:     let base: i32 = __captures.c1;
// CHECK-NEXT:     [[V_1]] * [[V_3]] + base
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_6_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_6_captures>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_5(__block: *mut std::ffi::c_void, [[V_1]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_5_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_5_captures>(__block) };
// CHECK-NEXT:     let base: i32 = __captures.c0;
// CHECK-NEXT:     let mut __slate_block_6_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_6_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[V_2]]: *mut std::ffi::c_void = unsafe {
// CHECK-NEXT:         __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_6_slot),
// CHECK-NEXT:             __slate_block_6 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_6_descriptor,
// CHECK-NEXT:             __slate_block_6_captures { c0: [[V_1]], c1: base },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]] = [[V_2]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], [[V_1]] + 1)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_5_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_5_captures>(&[]);
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
