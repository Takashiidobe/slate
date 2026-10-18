#include <stdatomic.h>
#include <stdio.h>

struct Small {
  char a[3];
};

struct Large {
  int values[5];
};

struct Holder {
  char                 head;
  _Atomic struct Small value;
  char                 tail;
};

_Atomic struct Small small;
_Atomic struct Large large;
static struct Holder holder;

int main(void) {
  printf("%zu %zu\n", sizeof(small), _Alignof(_Atomic struct Small));
  printf("%zu %zu\n", sizeof(large), _Alignof(_Atomic struct Large));
  printf("%zu\n", sizeof(holder));

  struct Small loaded = small;
  printf("%d %d %d\n", loaded.a[0], loaded.a[1], loaded.a[2]);
  small = (struct Small){{7, 8, 9}};
  loaded = atomic_load(&small);
  printf("%d %d %d\n", loaded.a[0], loaded.a[1], loaded.a[2]);

  atomic_store_explicit(&large, ((struct Large){{10, 20, 30, 40, 50}}), memory_order_release);
  struct Large big = atomic_load_explicit(&large, memory_order_acquire);
  printf("%d %d\n", big.values[0], big.values[4]);
  big.values[4] = 99;
  large = big;
  struct Large again = large;
  printf("%d\n", again.values[4]);

  holder.head = 'h';
  holder.value = (struct Small){{4, 5, 6}};
  holder.tail = 't';
  struct Small inner = holder.value;
  printf("%c %d %d %d %c\n", holder.head, inner.a[0], inner.a[1], inner.a[2], holder.tail);
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_libatomic {
// CHECK-NEXT:     use std::sync::atomic::Ordering;
// CHECK-NEXT:     #[link(name = "atomic")]
// CHECK-NEXT:     unsafe extern "C" {
// CHECK-NEXT:         pub fn __atomic_load_16(pointer: *const u128, order: i32) -> u128;
// CHECK-NEXT:         pub fn __atomic_store_16(pointer: *mut u128, value: u128, order: i32);
// CHECK-NEXT:         fn __atomic_compare_exchange_16(
// CHECK-NEXT:             pointer: *mut u128,
// CHECK-NEXT:             expected: *mut u128,
// CHECK-NEXT:             desired: u128,
// CHECK-NEXT:             weak: bool,
// CHECK-NEXT:             success: i32,
// CHECK-NEXT:             failure: i32,
// CHECK-NEXT:         ) -> bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     fn order(order: Ordering) -> i32 {
// CHECK-NEXT:         match order {
// CHECK-NEXT:             Ordering::Relaxed => 0,
// CHECK-NEXT:             Ordering::Acquire => 2,
// CHECK-NEXT:             Ordering::Release => 3,
// CHECK-NEXT:             Ordering::AcqRel => 4,
// CHECK-NEXT:             _ => 5,
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     fn failure(success: Ordering) -> Ordering {
// CHECK-NEXT:         match success {
// CHECK-NEXT:             Ordering::Release => Ordering::Relaxed,
// CHECK-NEXT:             Ordering::AcqRel => Ordering::Acquire,
// CHECK-NEXT:             other => other,
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     macro_rules! atomic_int {
// CHECK-NEXT:         ($name:ident, $ty:ty) => {
// CHECK-NEXT:             pub struct $name (* mut u128); impl $name { pub unsafe fn from_ptr(pointer :
// CHECK-NEXT:             * mut $ty) -> Self { Self(pointer.cast()) } pub fn load(& self, ordering :
// CHECK-NEXT:             Ordering) -> $ty { unsafe { __atomic_load_16(self.0, order(ordering)) as $ty
// CHECK-NEXT:             } } fn exchange(& self, current : $ty, new : $ty, weak : bool, success :
// CHECK-NEXT:             Ordering, failure : Ordering,) -> Result <$ty, $ty > { let mut expected =
// CHECK-NEXT:             current as u128; if unsafe { __atomic_compare_exchange_16(self.0, & mut
// CHECK-NEXT:             expected, new as u128, weak, order(success), order(failure),) } { Ok(current)
// CHECK-NEXT:             } else { Err(expected as $ty) } } pub fn compare_exchange(& self, current :
// CHECK-NEXT:             $ty, new : $ty, success : Ordering, failure : Ordering,) -> Result <$ty, $ty
// CHECK-NEXT:             > { self.exchange(current, new, false, success, failure) } pub fn
// CHECK-NEXT:             compare_exchange_weak(& self, current : $ty, new : $ty, success : Ordering,
// CHECK-NEXT:             failure : Ordering,) -> Result <$ty, $ty > { self.exchange(current, new,
// CHECK-NEXT:             true, success, failure) } pub fn fetch_update(& self, set_order : Ordering,
// CHECK-NEXT:             fetch_order : Ordering, mut f : impl FnMut($ty) -> Option <$ty >,) -> Result
// CHECK-NEXT:             <$ty, $ty > { let mut previous = self.load(fetch_order); while let Some(next)
// CHECK-NEXT:             = f(previous) { match self.exchange(previous, next, true, set_order,
// CHECK-NEXT:             fetch_order) { Ok(value) => return Ok(value), Err(observed) => previous =
// CHECK-NEXT:             observed, } } Err(previous) } fn rmw(& self, ordering : Ordering, f : impl
// CHECK-NEXT:             Fn($ty) -> $ty) -> $ty { let mut previous = self.load(Ordering::Relaxed);
// CHECK-NEXT:             loop { match self.exchange(previous, f(previous), true, ordering,
// CHECK-NEXT:             failure(ordering),) { Ok(value) => return value, Err(observed) => previous =
// CHECK-NEXT:             observed, } } } pub fn swap(& self, value : $ty, ordering : Ordering) -> $ty
// CHECK-NEXT:             { self.rmw(ordering, | _ | value) } pub fn fetch_add(& self, value : $ty,
// CHECK-NEXT:             ordering : Ordering) -> $ty { self.rmw(ordering, | old | old
// CHECK-NEXT:             .wrapping_add(value)) } pub fn fetch_sub(& self, value : $ty, ordering :
// CHECK-NEXT:             Ordering) -> $ty { self.rmw(ordering, | old | old.wrapping_sub(value)) } pub
// CHECK-NEXT:             fn fetch_and(& self, value : $ty, ordering : Ordering) -> $ty { self
// CHECK-NEXT:             .rmw(ordering, | old | old & value) } pub fn fetch_nand(& self, value : $ty,
// CHECK-NEXT:             ordering : Ordering) -> $ty { self.rmw(ordering, | old | ! (old & value)) }
// CHECK-NEXT:             pub fn fetch_or(& self, value : $ty, ordering : Ordering) -> $ty { self
// CHECK-NEXT:             .rmw(ordering, | old | old | value) } pub fn fetch_xor(& self, value : $ty,
// CHECK-NEXT:             ordering : Ordering) -> $ty { self.rmw(ordering, | old | old ^ value) } }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     atomic_int!(AtomicI128, i128);
// CHECK-NEXT:     atomic_int!(AtomicU128, u128);
// CHECK-NEXT:     #[link(name = "atomic")]
// CHECK-NEXT:     unsafe extern "C" {
// CHECK-NEXT:         #[link_name = "__atomic_load"]
// CHECK-NEXT:         fn atomic_load(size: usize, pointer: *mut u8, result: *mut u8, order: i32);
// CHECK-NEXT:         #[link_name = "__atomic_store"]
// CHECK-NEXT:         fn atomic_store(size: usize, pointer: *mut u8, value: *mut u8, order: i32);
// CHECK-NEXT:         #[link_name = "__atomic_exchange"]
// CHECK-NEXT:         fn atomic_exchange(
// CHECK-NEXT:             size: usize,
// CHECK-NEXT:             pointer: *mut u8,
// CHECK-NEXT:             value: *mut u8,
// CHECK-NEXT:             result: *mut u8,
// CHECK-NEXT:             order: i32,
// CHECK-NEXT:         );
// CHECK-NEXT:         #[link_name = "__atomic_compare_exchange"]
// CHECK-NEXT:         fn atomic_compare_exchange(
// CHECK-NEXT:             size: usize,
// CHECK-NEXT:             pointer: *mut u8,
// CHECK-NEXT:             expected: *mut u8,
// CHECK-NEXT:             desired: *mut u8,
// CHECK-NEXT:             success: i32,
// CHECK-NEXT:             failure: i32,
// CHECK-NEXT:         ) -> bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     pub struct AtomicGeneric<T>(*mut T);
// CHECK-NEXT:     impl<T: Copy> AtomicGeneric<T> {
// CHECK-NEXT:         const SIZE: usize = std::mem::size_of::<T>();
// CHECK-NEXT:         pub unsafe fn from_ptr(pointer: *mut T) -> Self {
// CHECK-NEXT:             Self(pointer)
// CHECK-NEXT:         }
// CHECK-NEXT:         pub fn load(&self, ordering: Ordering) -> T {
// CHECK-NEXT:             let mut result = std::mem::MaybeUninit::<T>::uninit();
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 atomic_load(
// CHECK-NEXT:                     Self::SIZE,
// CHECK-NEXT:                     self.0.cast(),
// CHECK-NEXT:                     result.as_mut_ptr().cast(),
// CHECK-NEXT:                     order(ordering),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 result.assume_init()
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         pub fn store(&self, mut value: T, ordering: Ordering) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 atomic_store(
// CHECK-NEXT:                     Self::SIZE,
// CHECK-NEXT:                     self.0.cast(),
// CHECK-NEXT:                     (&raw mut value).cast(),
// CHECK-NEXT:                     order(ordering),
// CHECK-NEXT:                 )
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         pub fn swap(&self, mut value: T, ordering: Ordering) -> T {
// CHECK-NEXT:             let mut result = std::mem::MaybeUninit::<T>::uninit();
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 atomic_exchange(
// CHECK-NEXT:                     Self::SIZE,
// CHECK-NEXT:                     self.0.cast(),
// CHECK-NEXT:                     (&raw mut value).cast(),
// CHECK-NEXT:                     result.as_mut_ptr().cast(),
// CHECK-NEXT:                     order(ordering),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 result.assume_init()
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         pub fn compare_exchange(
// CHECK-NEXT:             &self,
// CHECK-NEXT:             current: T,
// CHECK-NEXT:             mut new: T,
// CHECK-NEXT:             success: Ordering,
// CHECK-NEXT:             failure: Ordering,
// CHECK-NEXT:         ) -> Result<T, T> {
// CHECK-NEXT:             let mut expected = current;
// CHECK-NEXT:             if unsafe {
// CHECK-NEXT:                 atomic_compare_exchange(
// CHECK-NEXT:                     Self::SIZE,
// CHECK-NEXT:                     self.0.cast(),
// CHECK-NEXT:                     (&raw mut expected).cast(),
// CHECK-NEXT:                     (&raw mut new).cast(),
// CHECK-NEXT:                     order(success),
// CHECK-NEXT:                     order(failure),
// CHECK-NEXT:                 )
// CHECK-NEXT:             } {
// CHECK-NEXT:                 Ok(current)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 Err(expected)
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         pub fn compare_exchange_weak(
// CHECK-NEXT:             &self,
// CHECK-NEXT:             current: T,
// CHECK-NEXT:             new: T,
// CHECK-NEXT:             success: Ordering,
// CHECK-NEXT:             failure: Ordering,
// CHECK-NEXT:         ) -> Result<T, T> {
// CHECK-NEXT:             self.compare_exchange(current, new, success, failure)
// CHECK-NEXT:         }
// CHECK-NEXT:         pub fn fetch_update(
// CHECK-NEXT:             &self,
// CHECK-NEXT:             set_order: Ordering,
// CHECK-NEXT:             fetch_order: Ordering,
// CHECK-NEXT:             mut f: impl FnMut(T) -> Option<T>,
// CHECK-NEXT:         ) -> Result<T, T> {
// CHECK-NEXT:             let mut previous = self.load(fetch_order);
// CHECK-NEXT:             while let Some(next) = f(previous) {
// CHECK-NEXT:                 match self.compare_exchange(previous, next, set_order, fetch_order) {
// CHECK-NEXT:                     Ok(value) => return Ok(value),
// CHECK-NEXT:                     Err(observed) => previous = observed,
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             Err(previous)
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Small {
// CHECK-NEXT:     a: [u8; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Large {
// CHECK-NEXT:     values: [i32; 5],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Holder {
// CHECK-NEXT:     head: u8,
// CHECK-NEXT:     __slate_pad_1: [u8; 3],
// CHECK-NEXT:     value: Small,
// CHECK-NEXT:     __slate_pad_3: [u8; 1],
// CHECK-NEXT:     tail: u8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: struct __SlateAlign4<T>(T);
// CHECK-NEXT: static mut small: __SlateAlign4<Small> = __SlateAlign4(unsafe { std::mem::zeroed() });
// CHECK-NEXT: static mut large: Large = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut holder: Holder = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: Small = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: Large = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: Small = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 4 as u64, 4 as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 20 as u64, 4 as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu\n".as_ptr(), 12 as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut loaded: Small = unsafe {
// CHECK-NEXT:         __slate_libatomic::AtomicGeneric::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of!(small.0) as *mut Small,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", (loaded.a[0] as u32) as i32, (loaded.a[1] as u32) as i32, (loaded
// CHECK-NEXT:         .a[2] as u32) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_libatomic::AtomicGeneric::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(small.0) as *mut Small,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *{
// CHECK-NEXT:                         [[SLATE_COMPOUND_0]] = Small { a: [7, 8, 9] };
// CHECK-NEXT:                         std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             )
// CHECK-NEXT:     };
// CHECK-NEXT:     loaded = unsafe {
// CHECK-NEXT:         __slate_libatomic::AtomicGeneric::from_ptr(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(small.0) }) as *mut Small,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", (loaded.a[0] as u32) as i32, (loaded.a[1] as u32) as i32, (loaded
// CHECK-NEXT:         .a[2] as u32) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_libatomic::AtomicGeneric::from_ptr(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(large) }) as *mut Large,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *{
// CHECK-NEXT:                         [[SLATE_COMPOUND_1]] = Large {
// CHECK-NEXT:                             values: [10, 20, 30, 40, 50],
// CHECK-NEXT:                         };
// CHECK-NEXT:                         std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// CHECK-NEXT:                 std::sync::atomic::Ordering::Release,
// CHECK-NEXT:             )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut big: Large = unsafe {
// CHECK-NEXT:         __slate_libatomic::AtomicGeneric::from_ptr(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(large) }) as *mut Large,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::Acquire)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {}", big.values[0], big.values[4]);
// CHECK-NEXT:     big.values[4] = 99;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_libatomic::AtomicGeneric::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(large) as *mut Large,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(big, std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let again: Large = unsafe {
// CHECK-NEXT:         __slate_libatomic::AtomicGeneric::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of!(large) as *mut Large,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", again.values[4]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         holder.head = 104;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_libatomic::AtomicGeneric::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(holder.value) as *mut Small,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *{
// CHECK-NEXT:                         [[SLATE_COMPOUND_2]] = Small { a: [4, 5, 6] };
// CHECK-NEXT:                         std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         holder.tail = 116;
// CHECK-NEXT:     }
// CHECK-NEXT:     let inner: Small = unsafe {
// CHECK-NEXT:         __slate_libatomic::AtomicGeneric::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of!(holder.value) as *mut Small,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%c %d %d %d %c\n".as_ptr(),
// CHECK-NEXT:             ((unsafe { holder.head }) as u32) as i32,
// CHECK-NEXT:             (inner.a[0] as u32) as i32,
// CHECK-NEXT:             (inner.a[1] as u32) as i32,
// CHECK-NEXT:             (inner.a[2] as u32) as i32,
// CHECK-NEXT:             ((unsafe { holder.tail }) as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
