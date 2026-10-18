#include <stdio.h>
#include <stdatomic.h>

struct Counters {
    _Atomic int value;
    volatile unsigned short small;
};
static _Atomic int shared = 7;
static int calls;
static int callback(int x) { return x + 5; }
static volatile int *once(volatile int *p) {
    ++calls;
    return p;
}
int main(void) {
    unsigned char a = 231;
    short b = -1234;
    int c = -765432;
    unsigned long long d = 0xfedcba9876543210ULL;
    int *p = &c;
    const int *cp = &c;
    int (*fn)(int) = callback;
    volatile _Bool flag = 1;
    volatile int v = 17;
    volatile unsigned short s = 65000;
    volatile unsigned long long wide = d;
    struct Counters counters = {3, 9};
    int av = __atomic_load_n(&a, __ATOMIC_RELAXED);
    int bv = __atomic_load_n(&b, __ATOMIC_CONSUME);
    int cv = __atomic_load_n(&c, __ATOMIC_ACQUIRE);
    unsigned long long dv = __atomic_load_n(&d, __ATOMIC_SEQ_CST);
    int *pv = __atomic_load_n(&p, __ATOMIC_ACQUIRE);
    const int *cpv = __atomic_load_n(&cp, __ATOMIC_RELAXED);
    __atomic_store_n(&fn, callback, __ATOMIC_RELAXED);
    int (*fnv)(int) = __atomic_load_n(&fn, __ATOMIC_ACQUIRE);
    if (*cpv != c || fnv(7) != 12 || !__atomic_load_n(&flag, __ATOMIC_RELAXED)) return 1;
    int vv = __atomic_load_n(once(&v), __ATOMIC_RELAXED);
    unsigned short sv = __atomic_load_n(&s, __ATOMIC_ACQUIRE);
    unsigned long long wv = __atomic_load_n(&wide, __ATOMIC_SEQ_CST);
    __atomic_load_n(&v, __ATOMIC_RELAXED);
    __atomic_store_n(&v, 29, __ATOMIC_SEQ_CST);
    atomic_store_explicit(&shared, 11, memory_order_release);
    atomic_store_explicit(&counters.value, 13, memory_order_relaxed);
    __atomic_store_n(&counters.small, 19, __ATOMIC_RELEASE);
    __atomic_thread_fence(__ATOMIC_RELAXED);
    __atomic_thread_fence(__ATOMIC_ACQUIRE);
    __atomic_thread_fence(__ATOMIC_RELEASE);
    __atomic_thread_fence(__ATOMIC_ACQ_REL);
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    __atomic_signal_fence(__ATOMIC_ACQUIRE);
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    printf("%d %d %d %llu %d %d %u %llu %d %d %d %u %d\n",
           av, bv, cv, dv, *pv, vv, sv, wv, calls, v,
           atomic_load_explicit(&shared, memory_order_acquire),
           counters.small, atomic_load_explicit(&counters.value, memory_order_seq_cst));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(atomic_volatile)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Counters {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     small: u16,
// CHECK-NEXT: }
// CHECK-NEXT: static mut shared: i32 = 7 as i32;
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: extern "C-unwind" fn callback(x: i32) -> i32 {
// CHECK-NEXT:     x + 5
// CHECK-NEXT: }
// CHECK-NEXT: fn once(p: &i32) -> *mut i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     (p as *const i32) as *mut i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: u8 = ((231 as i32) as i8) as u8;
// CHECK-NEXT:     let mut b: i16 = -1234;
// CHECK-NEXT:     let mut c: i32 = -765432;
// CHECK-NEXT:     let mut d: u64 = 18364758544493064720;
// CHECK-NEXT:     let mut p: *mut i32 = std::ptr::addr_of_mut!(c);
// CHECK-NEXT:     let mut cp: *const i32 = std::ptr::addr_of_mut!(c) as *const i32;
// CHECK-NEXT:     let mut r#fn: Option<unsafe extern "C-unwind" fn(i32) -> i32> = Some(callback);
// CHECK-NEXT:     let mut flag: bool = (1 as i32) != (0 as i32);
// CHECK-NEXT:     let mut v: i32 = 17;
// CHECK-NEXT:     let mut s: u16 = ((65000 as i32) as i16) as u16;
// CHECK-NEXT:     let mut wide: u64 = d;
// CHECK-NEXT:     let mut counters: Counters = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Counters = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].value = 3 as i32;
// CHECK-NEXT:         [[T_0]].small = 9 as u16;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let av: i32 = ((unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(a) as *mut u8)
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::Relaxed)
// CHECK-NEXT:     }) as u32) as i32;
// CHECK-NEXT:     let bv: i32 = (unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI16::from_ptr(std::ptr::addr_of_mut!(b) as *mut i16)
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::Acquire)
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let cv: i32 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c) as *mut i32)
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::Acquire)
// CHECK-NEXT:     };
// CHECK-NEXT:     let dv: u64 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicU64::from_ptr(std::ptr::addr_of_mut!(d) as *mut u64)
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let pv: *mut i32 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(p) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::Acquire) as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let cpv: *const i32 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(cp) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::Relaxed) as *const i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(r#fn) as *mut *mut u8,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:                         *mut u8,
// CHECK-NEXT:                     >(Some(callback))
// CHECK-NEXT:                 },
// CHECK-NEXT:                 std::sync::atomic::Ordering::Relaxed,
// CHECK-NEXT:             )
// CHECK-NEXT:     };
// CHECK-NEXT:     let fnv: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *mut u8,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                         std::ptr::addr_of_mut!(r#fn) as *mut *mut u8,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(std::sync::atomic::Ordering::Acquire),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { *cpv }) != c {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = (unsafe { fnv.unwrap()(7 as i32) }) != 12;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]]
// CHECK-NEXT:         || !unsafe {
// CHECK-NEXT:             std::sync::atomic::AtomicBool::load_volatile(
// CHECK-NEXT:                 std::sync::atomic::AtomicBool::from_ptr_raw(
// CHECK-NEXT:                     std::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:                 ),
// CHECK-NEXT:                 std::sync::atomic::Ordering::Relaxed,
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let vv: i32 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::load_volatile(
// CHECK-NEXT:             std::sync::atomic::AtomicI32::from_ptr_raw(
// CHECK-NEXT:                 once(unsafe { &(*std::ptr::addr_of_mut!(v)) }) as *mut i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:             std::sync::atomic::Ordering::Relaxed,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let sv: u16 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicU16::load_volatile(
// CHECK-NEXT:             std::sync::atomic::AtomicU16::from_ptr_raw(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(s) as *mut u16,
// CHECK-NEXT:             ),
// CHECK-NEXT:             std::sync::atomic::Ordering::Acquire,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let wv: u64 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicU64::load_volatile(
// CHECK-NEXT:             std::sync::atomic::AtomicU64::from_ptr_raw(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(wide) as *mut u64,
// CHECK-NEXT:             ),
// CHECK-NEXT:             std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::load_volatile(
// CHECK-NEXT:             std::sync::atomic::AtomicI32::from_ptr_raw(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:             std::sync::atomic::Ordering::Relaxed,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::store_volatile(
// CHECK-NEXT:             std::sync::atomic::AtomicI32::from_ptr_raw(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:             29 as i32,
// CHECK-NEXT:             std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(shared) }) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(11 as i32, std::sync::atomic::Ordering::Release)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(counters.value) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(13 as i32, std::sync::atomic::Ordering::Relaxed)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicU16::store_volatile(
// CHECK-NEXT:             std::sync::atomic::AtomicU16::from_ptr_raw(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(counters.small) as *mut u16,
// CHECK-NEXT:             ),
// CHECK-NEXT:             19 as u16,
// CHECK-NEXT:             std::sync::atomic::Ordering::Release,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::Acquire);
// CHECK-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::Release);
// CHECK-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::AcqRel);
// CHECK-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     std::sync::atomic::compiler_fence(std::sync::atomic::Ordering::Acquire);
// CHECK-NEXT:     std::sync::atomic::compiler_fence(std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %llu %d %d %u %llu %d %d %d %u %d\n".as_ptr(),
// CHECK-NEXT:             av,
// CHECK-NEXT:             bv,
// CHECK-NEXT:             cv,
// CHECK-NEXT:             dv,
// CHECK-NEXT:             unsafe { *pv },
// CHECK-NEXT:             vv,
// CHECK-NEXT:             (sv as u32) as i32,
// CHECK-NEXT:             wv,
// CHECK-NEXT:             unsafe { calls },
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v)) },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                         (unsafe { std::ptr::addr_of_mut!(shared) }) as *mut i32,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(std::sync::atomic::Ordering::Acquire)
// CHECK-NEXT:             },
// CHECK-NEXT:             ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(counters.small)) })
// CHECK-NEXT:                 as u32) as i32,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                         std::ptr::addr_of_mut!(counters.value) as *mut i32,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             },
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
