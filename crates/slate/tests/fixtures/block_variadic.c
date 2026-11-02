// { dg-options "-fblocks" }
#include <stdarg.h>
#include <stdio.h>

extern void *_Block_copy(const void *);
extern void _Block_release(const void *);

typedef int (^sumfn)(int, ...);

static int call_it(sumfn f) { return f(3, 1, 2, 3); }

int main(void) {
  int base = 100;
  sumfn sum = ^(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int s = base;
    for (int i = 0; i < n; i++) s += va_arg(ap, int);
    va_end(ap);
    return s;
  };
  printf("%d\n", sum(2, 5, 6));
  printf("%d\n", call_it(sum));
  void (^pr)(const char *, ...) = ^(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
  };
  pr("%d %s %f\n", 7, "x", 1.5);
  sumfn h = _Block_copy(sum);
  printf("%d\n", h(1, 9));
  _Block_release(h);
  int (^nocap)(int, ...) = ^(int n, ...) { va_list ap; va_start(ap, n); int r = va_arg(ap, int) * n; va_end(ap); return r; };
  printf("%d\n", nocap(4, 5));
  __block double total = 0;
  void (^acc)(int, ...) = ^(int n, ...) {
    va_list ap;
    va_start(ap, n);
    for (int i = 0; i < n; i++) total += va_arg(ap, double);
    va_end(ap);
  };
  char c = 'A';
  short s = -3;
  float f = 0.25f;
  acc(3, 1.5, f, (double)s);
  printf("%.2f\n", total);
  int (^chars)(int, ...) = ^(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int r = 0;
    for (int i = 0; i < n; i++) r = r * 1000 + va_arg(ap, int);
    va_end(ap);
    return r;
  };
  printf("%d\n", chars(2, c, s));
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
// CHECK-NEXT:     fn vprintf([[ARG_0:arg[0-9]+]]: *const i8, [[ARG_1:arg[0-9]+]]: core::ffi::VaList<'static>) -> i32;
// CHECK-NEXT:     fn _Block_copy([[ARG_0]]: *const ()) -> *mut ();
// CHECK-NEXT:     fn _Block_release([[ARG_0]]: *const ());
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn call_it(f: *mut std::ffi::c_void) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = f;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32, ...) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], 3i32, 1i32, 2i32, 3i32)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let base: i32 = 100;
// CHECK-NEXT:     let mut __slate_block_0_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_0_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let sum: *mut std::ffi::c_void = unsafe {
// CHECK-NEXT:         __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_0_slot),
// CHECK-NEXT:             __slate_block_0 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_0_descriptor,
// CHECK-NEXT:             __slate_block_0_captures { c0: base },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         let [[T_0]] = sum;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32, ...) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_0]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_0]], 2i32, 5i32, 6i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("{}", call_it(sum));
// CHECK-NEXT:     let pr: *mut std::ffi::c_void = (std::ptr::addr_of!(__slate_block_1_literal)
// CHECK-NEXT:         as *const std::ffi::c_void) as *mut std::ffi::c_void;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = pr;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, *const i8, ...)>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_1]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()(
// CHECK-NEXT:             [[T_1]],
// CHECK-NEXT:             c"%d %s %f\n".as_ptr(),
// CHECK-NEXT:             7i32,
// CHECK-NEXT:             c"x".as_ptr() as *mut i8,
// CHECK-NEXT:             1.5f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let h: *mut std::ffi::c_void =
// CHECK-NEXT:         (unsafe { _Block_copy(sum as *const ()) }) as *mut std::ffi::c_void;
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = h;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32, ...) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_2]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_2]], 1i32, 9i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe { _Block_release(h as *const ()) };
// CHECK-NEXT:     let nocap: *mut std::ffi::c_void = (std::ptr::addr_of!(__slate_block_2_literal)
// CHECK-NEXT:         as *const std::ffi::c_void) as *mut std::ffi::c_void;
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = nocap;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32, ...) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_3]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_3]], 4i32, 5i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut total_byref: std::mem::MaybeUninit<__slate_blocks::Byref<f64>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let total: *mut __slate_blocks::Byref<f64> =
// CHECK-NEXT:         unsafe { __slate_blocks::byref(std::ptr::addr_of_mut!(total_byref)) };
// CHECK-NEXT:     unsafe { __slate_blocks::store(total, 0i32 as f64) };
// CHECK-NEXT:     let total_scope = __slate_blocks::scope(total);
// CHECK-NEXT:     let mut __slate_block_3_slot: std::mem::MaybeUninit<
// CHECK-NEXT:         __slate_blocks::Block<__slate_block_3_captures>,
// CHECK-NEXT:     > = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let acc: *mut std::ffi::c_void = unsafe {
// CHECK-NEXT:         __slate_blocks::stack(
// CHECK-NEXT:             std::ptr::addr_of_mut!(__slate_block_3_slot),
// CHECK-NEXT:             __slate_block_3 as *const std::ffi::c_void,
// CHECK-NEXT:             &__slate_block_3_descriptor,
// CHECK-NEXT:             __slate_block_3_captures { c0: total },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let c: i8 = 65;
// CHECK-NEXT:     let s: i16 = -3;
// CHECK-NEXT:     let f: f32 = 0.25f32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = acc;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32, ...)>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_4]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_4]], 3i32, 1.5f64, f as f64, s as f64)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%.2f\n".as_ptr(), *__slate_blocks::value(total)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let chars: *mut std::ffi::c_void = (std::ptr::addr_of!(__slate_block_4_literal)
// CHECK-NEXT:         as *const std::ffi::c_void) as *mut std::ffi::c_void;
// CHECK-NEXT:     println!("{}", unsafe {
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = chars;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const std::ffi::c_void,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(*mut std::ffi::c_void, i32, ...) -> i32>,
// CHECK-NEXT:             >(__slate_blocks::invoke([[T_5]]))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()([[T_5]], 2i32, c as i32, s as i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_0_captures {
// CHECK-NEXT:     c0: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn __slate_block_0(
// CHECK-NEXT:     __block: *mut std::ffi::c_void,
// CHECK-NEXT:     [[V_0:__v[0-9]+]]: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let __captures: __slate_block_0_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_0_captures>(__block) };
// CHECK-NEXT:     let base: i32 = __captures.c0;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[V_1]] = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::ffi::VaList<'_>, core::ffi::VaList<'static>>(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = base;
// CHECK-NEXT:     for [[V_3:__v[0-9]+]] in 0..[[V_0]] {
// CHECK-NEXT:         let [[V_4:__v[0-9]+]]: i32 = unsafe { [[V_1]].next_arg::<i32>() };
// CHECK-NEXT:         [[V_2]] += [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_2]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_0_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_0_captures>(&[]);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn __slate_block_1(
// CHECK-NEXT:     __block: *mut std::ffi::c_void,
// CHECK-NEXT:     [[V_0]]: *const i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     let mut [[V_1]]: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[V_1]] = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::ffi::VaList<'_>, core::ffi::VaList<'static>>(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { vprintf([[V_0]], [[V_1]].clone()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_1_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_1 as *const std::ffi::c_void);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn __slate_block_2(
// CHECK-NEXT:     __block: *mut std::ffi::c_void,
// CHECK-NEXT:     [[V_0]]: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     let mut [[V_1]]: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[V_1]] = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::ffi::VaList<'_>, core::ffi::VaList<'static>>(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_2]]: i32 = (unsafe { [[V_1]].next_arg::<i32>() }) * [[V_0]];
// CHECK-NEXT:     [[V_2]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_2_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_2 as *const std::ffi::c_void);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_block_3_captures {
// CHECK-NEXT:     c0: *mut __slate_blocks::Byref<f64>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn __slate_block_3(
// CHECK-NEXT:     __block: *mut std::ffi::c_void,
// CHECK-NEXT:     [[V_0]]: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let __captures: __slate_block_3_captures =
// CHECK-NEXT:         unsafe { __slate_blocks::captures::<__slate_block_3_captures>(__block) };
// CHECK-NEXT:     let total: *mut __slate_blocks::Byref<f64> = __captures.c0;
// CHECK-NEXT:     let mut [[V_1]]: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[V_1]] = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::ffi::VaList<'_>, core::ffi::VaList<'static>>(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     for [[V_2]] in 0..[[V_0]] {
// CHECK-NEXT:         let [[V_3]]: f64 = unsafe { [[V_1]].next_arg::<f64>() };
// CHECK-NEXT:         let [[V_4]]: f64 = unsafe { *__slate_blocks::value(total) };
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: f64 = [[V_4]] + [[V_3]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *__slate_blocks::value(total) = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_3_descriptor: __slate_blocks::Descriptor =
// CHECK-NEXT:     __slate_blocks::descriptor::<__slate_block_3_captures>(&[__slate_blocks::byref_field(
// CHECK-NEXT:         std::mem::offset_of!(__slate_block_3_captures, c0),
// CHECK-NEXT:     )]);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn __slate_block_4(
// CHECK-NEXT:     __block: *mut std::ffi::c_void,
// CHECK-NEXT:     [[V_0]]: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_blocks::captures::<()>(__block) };
// CHECK-NEXT:     let mut [[V_1]]: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[V_1]] = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::ffi::VaList<'_>, core::ffi::VaList<'static>>(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_2]]: i32 = 0;
// CHECK-NEXT:     for [[V_3]] in 0..[[V_0]] {
// CHECK-NEXT:         [[V_2]] = [[V_2]] * 1000 + unsafe { [[V_1]].next_arg::<i32>() };
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_2]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static __slate_block_4_literal: __slate_blocks::Block<()> =
// CHECK-NEXT:     __slate_blocks::global(__slate_block_4 as *const std::ffi::c_void);
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
