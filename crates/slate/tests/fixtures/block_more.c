// { dg-options "-fblocks" }
#include <stdio.h>

extern void *_Block_copy(const void *);
extern void _Block_release(const void *);

typedef int (^intfn)(int);

struct handler {
  const char *name;
  intfn run;
};

static intfn compose(intfn f, intfn g) {
  return _Block_copy(^(int x) { return f(g(x)); });
}

static intfn make_scaler(int k) {
  return _Block_copy(^(int x) { return k * x; });
}

int main(void) {
  __block intfn fib;
  fib = ^(int n) { return n < 2 ? n : fib(n - 1) + fib(n - 2); };
  printf("%d\n", fib(15));

  intfn handlers[3];
  for (int i = 0; i < 3; i++) {
    handlers[i] = _Block_copy(^(int x) { return x * 10 + i; });
  }
  for (int i = 0; i < 3; i++) printf("%d ", handlers[i](i + 1));
  printf("\n");

  struct handler h = {"neg", ^(int x) { return -x; }};
  printf("%s %d\n", h.name, h.run(7));

  intfn twice = compose(make_scaler(2), make_scaler(3));
  printf("%d\n", twice(5));

  int (^no_capture)(void) = ^{ return 42; };
  void *erased = no_capture;
  int (^restored)(void) = erased;
  printf("%d %d\n", restored(), erased == (void *)no_capture);

  intfn pick = h.run ? h.run : ^(int x) { return x; };
  printf("%d\n", pick(9));

  __block long sum = 0;
  __block int calls = 0;
  void (^visit)(int) = ^(int v) {
    if (v % 2 == 0) goto skip;
    sum += v;
  skip:
    calls++;
  };
  for (int i = 0; i < 6; i++) visit(i);
  printf("%ld %d\n", sum, calls);

  __block struct { int v[3]; } box = {{1, 2, 3}};
  void (^bump)(void) = ^{ for (int i = 0; i < 3; i++) box.v[i] += 10; };
  bump();
  printf("%d %d %d\n", box.v[0], box.v[1], box.v[2]);

  double scale = 1.5;
  double (^mul)(double) = ^(double v) { return v * scale; };
  printf("%.2f\n", mul(4.0));
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
// CHECK-NEXT:     pub unsafe fn byref<T>(slot: *mut MaybeUninit<Byref<T>>, value: T) -> *mut Byref<T> {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let byref = slot.cast::<Byref<T>>();
// CHECK-NEXT:             byref.write(Byref {
// CHECK-NEXT:                 isa: std::ptr::null(),
// CHECK-NEXT:                 forwarding: byref,
// CHECK-NEXT:                 flags: 0,
// CHECK-NEXT:                 size: size_of::<Byref<T>>() as i32,
// CHECK-NEXT:                 value,
// CHECK-NEXT:             });
// CHECK-NEXT:             byref
// CHECK-NEXT:         }
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
// CHECK-NEXT:     pub unsafe fn init<T>(slot: *mut ByrefSlot<T>, value: T) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             if (*slot).live {
// CHECK-NEXT:                 _Block_object_dispose(self::slot(slot) as *const c_void, FIELD_IS_BYREF);
// CHECK-NEXT:             }
// CHECK-NEXT:             byref(&raw mut (*slot).byref, value);
// CHECK-NEXT:             (*slot).live = true;
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
// CHECK-NEXT: struct handler {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     run: *mut std::ffi::c_void,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_block_more_c_59 {
// CHECK-NEXT:     v: [i32; 3],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn compose(f: *mut std::ffi::c_void, g: *mut std::ffi::c_void) -> *mut std::ffi::c_void {
// CHECK-NEXT:     let mut __slate_block_0_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_0_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_0_slot),
// CHECK-NEXT:             __slate_block_0 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_0_descriptor,
// CHECK-NEXT:             __slate_block_0_captures { c0: f, c1: g },
// CHECK-NEXT:         ) as *const ())
// CHECK-NEXT:     }) as *mut std::ffi::c_void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn make_scaler(k: i32) -> *mut std::ffi::c_void {
// CHECK-NEXT:     let mut __slate_block_1_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_1_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_1_slot),
// CHECK-NEXT:             __slate_block_1 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_1_descriptor,
// CHECK-NEXT:             __slate_block_1_captures { c0: k },
// CHECK-NEXT:         ) as *const ())
// CHECK-NEXT:     }) as *mut std::ffi::c_void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut fib_byref: std::mem::MaybeUninit<__slate_blocks::Byref<*mut std::ffi::c_void>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let fib: *mut __slate_blocks::Byref<*mut std::ffi::c_void> =
// CHECK-NEXT:         unsafe { __slate_blocks::byref(std::ptr::addr_of_mut!(fib_byref), std::mem::zeroed()) };
// CHECK-NEXT:     let fib_scope = __slate_blocks::scope(fib);
// CHECK-NEXT:     let mut __slate_block_2_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_2_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *__slate_blocks::value(fib) = __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_2_slot),
// CHECK-NEXT:             __slate_block_2 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_2_descriptor,
// CHECK-NEXT:             __slate_block_2_captures { c0: fib },
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = *__slate_blocks::value(fib);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], 15i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut handlers: __SlateAlign16<[*mut std::ffi::c_void; 3]> =
// CHECK-NEXT:         __SlateAlign16([0 as *mut std::ffi::c_void; 3]);
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         let mut __slate_block_3_slot: std::mem::MaybeUninit<
// CHECK-NEXT:             __slate_blocks::Block<__slate_block_3_captures>,
// CHECK-NEXT:         > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:         handlers.0[i as usize] = (unsafe {
// CHECK-NEXT:             _Block_copy(__slate_blocks::stack(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(__slate_block_3_slot),
// CHECK-NEXT:                 __slate_block_3 as *const std::ffi::c_void,
// CHECK-NEXT:                 &__slate_block_3_descriptor,
// CHECK-NEXT:                 __slate_block_3_captures { c0: i },
// CHECK-NEXT:             ) as *const ())
// CHECK-NEXT:         }) as *mut std::ffi::c_void;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: for i in 0i32..3i32 {
// CHECK-NEXT:         print!("{} ", unsafe {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = handlers.0[i as usize];
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_1]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_1]], i + 1)
// CHECK-NEXT:         });
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     let h: handler = handler {
// CHECK-NEXT:         name: c"neg".as_ptr(),
// CHECK-NEXT:         run: (std::ptr::addr_of!(__slate_block_4_literal) as *const std::ffi::c_void)
// CHECK-NEXT:             as *mut std::ffi::c_void,
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%s %d\n".as_ptr(), h.name, {
// CHECK-NEXT:             let [[T_2:__t[0-9]+]] = h.run;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_2]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_2]], 7i32)
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let twice: *mut std::ffi::c_void = compose(make_scaler(2i32), make_scaler(3i32));
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = twice;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_3]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_3]], 5i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     let no_capture: *mut std::ffi::c_void = (std::ptr::addr_of!(__slate_block_5_literal)
// CHECK-NEXT:         as *const std::ffi::c_void)
// CHECK-NEXT:         as *mut std::ffi::c_void;
// CHECK-NEXT:     let erased: *mut () = no_capture as *mut ();
// CHECK-NEXT:     let restored: *mut std::ffi::c_void = erased as *mut std::ffi::c_void;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_4:__t[0-9]+]] = restored;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_4]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_4]])
// CHECK-NEXT:         },
// CHECK-NEXT:         (erased == (no_capture as *mut ())) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let pick: *mut std::ffi::c_void = if !h.run.is_null() {
// CHECK-NEXT:         h.run
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (std::ptr::addr_of!(__slate_block_6_literal) as *const std::ffi::c_void)
// CHECK-NEXT:             as *mut std::ffi::c_void
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = pick;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_5]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_5]], 9i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut sum_byref: std::mem::MaybeUninit<__slate_blocks::Byref<i64>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let sum: *mut __slate_blocks::Byref<i64> =
// CHECK-NEXT:         unsafe { __slate_blocks::byref(std::ptr::addr_of_mut!(sum_byref), 0i64) };
// CHECK-NEXT:     let sum_scope = __slate_blocks::scope(sum);
// CHECK-NEXT:     let mut calls_byref: std::mem::MaybeUninit<__slate_blocks::Byref<i32>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let calls: *mut __slate_blocks::Byref<i32> =
// CHECK-NEXT:         unsafe { __slate_blocks::byref(std::ptr::addr_of_mut!(calls_byref), 0i32) };
// CHECK-NEXT:     let calls_scope = __slate_blocks::scope(calls);
// CHECK-NEXT:     let mut __slate_block_7_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_7_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let visit: *mut std::ffi::c_void = unsafe {
// CHECK-NEXT:         __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_7_slot),
// CHECK-NEXT:             __slate_block_7 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_7_descriptor,
// CHECK-NEXT:             __slate_block_7_captures { c0: sum, c1: calls },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: for i in 0i32..6i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_6:__t[0-9]+]] = visit;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32)>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_6]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_6]], i)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %d\n".as_ptr(),
// CHECK-NEXT:             *__slate_blocks::value(sum),
// CHECK-NEXT:             *__slate_blocks::value(calls),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut box_byref: std::mem::MaybeUninit<__slate_blocks::Byref<__SlateRecord_block_more_c_59>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let r#box: *mut __slate_blocks::Byref<__SlateRecord_block_more_c_59> = unsafe {
// CHECK-NEXT:         __slate_blocks::byref(
// CHECK-NEXT:             std::ptr::addr_of_mut!(box_byref),
// CHECK-NEXT:             __SlateRecord_block_more_c_59 { v: [1, 2, 3] },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let box_scope = __slate_blocks::scope(r#box);
// CHECK-NEXT:     let mut __slate_block_8_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_8_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let bump: *mut std::ffi::c_void = unsafe {
// CHECK-NEXT:         __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_8_slot),
// CHECK-NEXT:             __slate_block_8 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_8_descriptor,
// CHECK-NEXT:             __slate_block_8_captures { c0: r#box },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = bump;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void)>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_7]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_7]])
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         unsafe { (*__slate_blocks::value(r#box)).v[0] },
// CHECK-NEXT:         unsafe { (*__slate_blocks::value(r#box)).v[1] },
// CHECK-NEXT:         unsafe { (*__slate_blocks::value(r#box)).v[2] }
// CHECK-NEXT:     );
// CHECK-NEXT:     let scale: f64 = 1.5f64;
// CHECK-NEXT:     let mut __slate_block_9_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_9_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let mul: *mut std::ffi::c_void = unsafe {
// CHECK-NEXT:         __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_9_slot),
// CHECK-NEXT:             __slate_block_9 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_9_descriptor,
// CHECK-NEXT:             __slate_block_9_captures { c0: scale },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%.2f\n".as_ptr(), {
// CHECK-NEXT:             let [[T_8:__t[0-9]+]] = mul;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, f64) -> f64>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_8]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_8]], 4.0f64)
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_0_captures {
// CHECK-NEXT:     c0: *mut std::ffi::c_void,
// CHECK-NEXT:     c1: *mut std::ffi::c_void,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_0(__block: *mut std::ffi::c_void, [[V_0:__v[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_0_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_0_captures>(__block) };
// CHECK-NEXT:     let f: *mut std::ffi::c_void = __captures.c0;
// CHECK-NEXT:     let g: *mut std::ffi::c_void = __captures.c1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]] = f;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], {
// CHECK-NEXT:             let [[T_1]] = g;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_1]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_1]], [[V_0]])
// CHECK-NEXT:         })
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_0_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_0_captures>(&[
// CHECK-NEXT:         __slate_blocks::block_field(std::mem::offset_of!(__slate_block_0_captures, c0)),
// CHECK-NEXT:         __slate_blocks::block_field(std::mem::offset_of!(__slate_block_0_captures, c1)),
// CHECK-NEXT:     ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_1_captures {
// CHECK-NEXT:     c0: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_1(__block: *mut std::ffi::c_void, [[V_0]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_1_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_1_captures>(__block) };
// CHECK-NEXT:     let k: i32 = __captures.c0;
// CHECK-NEXT:     k * [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_1_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_1_captures>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_2_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<*mut std::ffi::c_void>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_2(__block: *mut std::ffi::c_void, [[V_0]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_2_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_2_captures>(__block) };
// CHECK-NEXT:     let fib: *mut __slate_blocks::Byref<*mut std::ffi::c_void> = __captures.c0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = if [[V_0]] < 2 {
// CHECK-NEXT:         [[V_0]]
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             let [[T_0]] = *__slate_blocks::value(fib);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_0]], [[V_0]] - 1)
// CHECK-NEXT:         }) + unsafe {
// CHECK-NEXT:             let [[T_1]] = *__slate_blocks::value(fib);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const std::ffi::c_void,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32) -> i32>,
// CHECK-NEXT:                 >(__slate_blocks::invoke([[T_1]]))
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()([[T_1]], [[V_0]] - 2)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_1]]
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
// CHECK-NEXT:     c0: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_3(__block: *mut std::ffi::c_void, [[V_0]]: i32) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_3_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_3_captures>(__block) };
// CHECK-NEXT:     let i: i32 = __captures.c0;
// CHECK-NEXT:     [[V_0]] * 10 + i
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_3_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_3_captures>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_4(__block: *mut std::ffi::c_void, [[V_2:__v[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     -[[V_2]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_4_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_4 as *const std::ffi::c_void);
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_5(__block: *mut std::ffi::c_void) -> i32 {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     42
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_5_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_5 as *const std::ffi::c_void);
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_6(__block: *mut std::ffi::c_void, [[V_2]]: i32) -> i32 {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     [[V_2]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_6_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_6 as *const std::ffi::c_void);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_7_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<i64>,
// CHECK-NEXT:     c1: *mut __slate_blocks::Byref<i32>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_7(__block: *mut std::ffi::c_void, [[V_2]]: i32) {
// CHECK-NEXT:     let __captures: __slate_block_7_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_7_captures>(__block) };
// CHECK-NEXT:     let sum: *mut __slate_blocks::Byref<i64> = __captures.c0;
// CHECK-NEXT:     let calls: *mut __slate_blocks::Byref<i32> = __captures.c1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         if [[V_2]] % 2 != 0 {
// CHECK-NEXT:             let [[V_3:__v[0-9]+]]: i64 = *__slate_blocks::value(sum);
// CHECK-NEXT:             let [[V_4:__v[0-9]+]]: i64 = [[V_3]] + ([[V_2]] as i64);
// CHECK-NEXT:             *__slate_blocks::value(sum) = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: i32 = *__slate_blocks::value(calls);
// CHECK-NEXT:         let [[V_6:__v[0-9]+]]: i32 = [[V_5]] + 1;
// CHECK-NEXT:         *__slate_blocks::value(calls) = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_7_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_7_captures>(&[
// CHECK-NEXT:         __slate_blocks::byref_field(std::mem::offset_of!(__slate_block_7_captures, c0)),
// CHECK-NEXT:         __slate_blocks::byref_field(std::mem::offset_of!(__slate_block_7_captures, c1)),
// CHECK-NEXT:     ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_8_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<__SlateRecord_block_more_c_59>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_8(__block: *mut std::ffi::c_void) {
// CHECK-NEXT:     let __captures: __slate_block_8_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_8_captures>(__block) };
// CHECK-NEXT:     let r#box: *mut __slate_blocks::Byref<__SlateRecord_block_more_c_59> = __captures.c0;
// CHECK-NEXT:     for [[V_6]] in 0i32..3i32 {
// CHECK-NEXT:         let [[V_5]]: *mut i32 = unsafe {
// CHECK-NEXT:             ((*__slate_blocks::value(r#box)).v.as_mut_ptr() as *mut i32).offset([[V_6]] as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_5]] = *[[V_5]] + 10;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_8_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_8_captures>(&[__slate_blocks::byref_field(
// CHECK-NEXT:         std::mem::offset_of!(__slate_block_8_captures, c0),
// CHECK-NEXT:     )]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_9_captures {
// CHECK-NEXT:     c0: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn __slate_block_9(__block: *mut std::ffi::c_void, [[V_6]]: f64) -> f64 {
// CHECK-NEXT:     let __captures: __slate_block_9_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_9_captures>(__block) };
// CHECK-NEXT:     let scale: f64 = __captures.c0;
// CHECK-NEXT:     [[V_6]] * scale
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_9_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_9_captures>(&[]);
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
