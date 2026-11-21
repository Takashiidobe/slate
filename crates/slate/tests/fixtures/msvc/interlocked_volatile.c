/* { dg-options "/Zc:preprocessor" } */
#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#include <stdlib.h>

static volatile LONG g_locked = FALSE;
static volatile LONG g_counter;
static PVOID volatile g_shared;
static int g_entered;

static void enter(void) {
    while (InterlockedCompareExchange(&g_locked, TRUE, FALSE) != FALSE)
        abort();
    g_entered++;
}

static void leave(void) { InterlockedExchange(&g_locked, FALSE); }

static PVOID reference_shared(PVOID fresh) {
    PVOID previous = InterlockedCompareExchangePointer(&g_shared, fresh, NULL);
    return previous ? previous : fresh;
}

int main(void) {
    enter();
    if (g_locked != TRUE || InterlockedCompareExchange(&g_locked, TRUE, FALSE) != TRUE)
        abort();
    leave();
    if (g_locked != FALSE || g_entered != 1)
        abort();

    if (InterlockedIncrement(&g_counter) != 1 || InterlockedExchangeAdd(&g_counter, 5) != 1 ||
        InterlockedDecrement(&g_counter) != 5 || InterlockedOr(&g_counter, 0x30) != 5 ||
        InterlockedAnd(&g_counter, 0x31) != 0x35 || InterlockedXor(&g_counter, 0x11) != 0x31 ||
        g_counter != 0x20)
        abort();

    int a = 1, b = 2;
    if (reference_shared(&a) != &a || reference_shared(&b) != &a)
        abort();
    if (InterlockedExchangePointer(&g_shared, &b) != &a || g_shared != &b)
        abort();

    volatile LONG flags = 0x5;
    if (!_interlockedbittestandreset((PLONG)&flags, 2) || flags != 0x1)
        abort();
    if (_interlockedbittestandreset((PLONG)&flags, 2) || flags != 0x1)
        abort();
    if (_interlockedbittestandset((PLONG)&flags, 3) || flags != 0x9)
        abort();
    return 0;
}
#else
int main(void) { return 0; }
#endif

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor" }
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _interlockedbittestandset([[ARG_0:arg[0-9]+]]: *mut i32, [[ARG_1:arg[0-9]+]]: i32) -> u8;
// I686-PC-WINDOWS-MSVC-NEXT:     fn _interlockedbittestandset([[ARG_0:arg[0-9]+]]: *mut i32, [[ARG_1:arg[0-9]+]]: i32) -> u8;
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _interlockedbittestandreset([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> u8;
// I686-PC-WINDOWS-MSVC-NEXT:     fn _interlockedbittestandreset([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> u8;
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedAnd([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> i32;
// I686-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedAnd([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> i32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedAnd([[ARG_0:arg[0-9]+]]: *mut i32, [[ARG_1:arg[0-9]+]]: i32) -> i32;
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedOr([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> i32;
// I686-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedOr([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> i32;
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedXor([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> i32;
// I686-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedXor([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> i32;
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedIncrement([[ARG_0]]: *mut i32) -> i32;
// I686-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedIncrement([[ARG_0]]: *mut i32) -> i32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedCompareExchange([[ARG_0]]: *mut i32, [[ARG_1]]: i32, [[ARG_2:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     fn _InterlockedDecrement([[ARG_0]]: *mut i32) -> i32;
// CHECK-NEXT:     fn _InterlockedExchange([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> i32;
// CHECK-NEXT:     fn _InterlockedExchangeAdd([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> i32;
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedCompareExchange([[ARG_0]]: *mut i32, [[ARG_1]]: i32, [[ARG_2:arg[0-9]+]]: i32) -> i32;
// I686-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedCompareExchange([[ARG_0]]: *mut i32, [[ARG_1]]: i32, [[ARG_2:arg[0-9]+]]: i32) -> i32;
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedCompareExchangePointer(
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[ARG_0]]: *mut *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[ARG_1]]: *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[ARG_2]]: *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:     ) -> *mut ();
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedExchangePointer([[ARG_0]]: *mut *mut (), [[ARG_1]]: *mut ()) -> *mut ();
// I686-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-NEXT: #[inline(always)]
// I686-PC-WINDOWS-MSVC-NEXT: fn _InlineInterlockedExchangePointer(
// I686-PC-WINDOWS-MSVC-NEXT:     Destination: *mut *mut (),
// I686-PC-WINDOWS-MSVC-NEXT:     Value: *mut (),
// I686-PC-WINDOWS-MSVC-NEXT: ) -> *mut () {
// I686-PC-WINDOWS-MSVC-NEXT:     (unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         core::sync::atomic::AtomicI32::from_ptr((Destination as *mut i32) as *mut i32)
// I686-PC-WINDOWS-MSVC-NEXT:             .swap(Value as i32, core::sync::atomic::Ordering::SeqCst)
// I686-PC-WINDOWS-MSVC-NEXT:     }) as *mut ()
// I686-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-NEXT: #[inline(always)]
// I686-PC-WINDOWS-MSVC-NEXT: fn _InlineInterlockedCompareExchangePointer(
// I686-PC-WINDOWS-MSVC-NEXT:     Destination: *mut *mut (),
// I686-PC-WINDOWS-MSVC-NEXT:     ExChange: *mut (),
// I686-PC-WINDOWS-MSVC-NEXT:     Comperand: *mut (),
// I686-PC-WINDOWS-MSVC-NEXT: ) -> *mut () {
// I686-PC-WINDOWS-MSVC-NEXT:     (unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         core::sync::atomic::AtomicI32::from_ptr((Destination as *mut i32) as *mut i32)
// I686-PC-WINDOWS-MSVC-NEXT:             .compare_exchange(
// I686-PC-WINDOWS-MSVC-NEXT:                 Comperand as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 ExChange as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// I686-PC-WINDOWS-MSVC-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// I686-PC-WINDOWS-MSVC-NEXT:             )
// I686-PC-WINDOWS-MSVC-NEXT:             .unwrap_or_else(|[[T_0:__t[0-9]+]]| [[T_0]])
// I686-PC-WINDOWS-MSVC-NEXT:     }) as *mut ()
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedIncrement([[ARG_0]]: *mut i32) -> i32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedOr([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> i32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedXor([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> i32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _interlockedbittestandset([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> u8;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedCompareExchangePointer(
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[ARG_0]]: *mut *mut (),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[ARG_1]]: *mut (),
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[ARG_2]]: *mut (),
// AARCH64-PC-WINDOWS-MSVC-NEXT:     ) -> *mut ();
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _InterlockedExchangePointer([[ARG_0]]: *mut *mut (), [[ARG_1]]: *mut ()) -> *mut ();
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _interlockedbittestandreset([[ARG_0]]: *mut i32, [[ARG_1]]: i32) -> u8;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut g_locked: i32 = 0;
// CHECK-EMPTY:
// CHECK-NEXT: static mut g_counter: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// I686-PC-WINDOWS-MSVC-EMPTY:
// CHECK-NEXT: static mut g_shared: *mut () = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g_entered: i32 = unsafe { core::mem::zeroed() };
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// AARCH64-PC-WINDOWS-MSVC-EMPTY:
// AARCH64-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// CHECK-EMPTY:
// CHECK-NEXT: fn enter() {
// CHECK-NEXT:     while (unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(g_locked) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 0i32,
// CHECK-NEXT:                 1i32,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             )
// X86_64-PC-WINDOWS-MSVC-NEXT:             .unwrap_or_else(|[[T_0:__t[0-9]+]]| [[T_0]])
// I686-PC-WINDOWS-MSVC-NEXT:             .unwrap_or_else(|[[T_0]]| [[T_0]])
// AARCH64-PC-WINDOWS-MSVC-NEXT:             .unwrap_or_else(|[[T_0:__t[0-9]+]]| [[T_0]])
// CHECK-NEXT:     }) != 0 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g_entered = g_entered + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn leave() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(g_locked) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap(0i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn reference_shared(fresh: *mut ()) -> *mut () {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let previous: *mut () = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         core::sync::atomic::AtomicPtr::from_ptr(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 core::ptr::addr_of_mut!(g_shared) as *mut *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             )
// X86_64-PC-WINDOWS-MSVC-NEXT:             .compare_exchange(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 core::ptr::null_mut::<()>() as *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:                 fresh as *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// X86_64-PC-WINDOWS-MSVC-NEXT:             )
// X86_64-PC-WINDOWS-MSVC-NEXT:             .unwrap_or_else(|[[T_0]]| [[T_0]]) as *mut ()
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     let previous: *mut () = _InlineInterlockedCompareExchangePointer(
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { core::ptr::addr_of_mut!(g_shared) },
// I686-PC-WINDOWS-MSVC-NEXT:         fresh,
// I686-PC-WINDOWS-MSVC-NEXT:         core::ptr::null_mut::<()>(),
// I686-PC-WINDOWS-MSVC-NEXT:     );
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let previous: *mut () = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         core::sync::atomic::AtomicPtr::from_ptr(
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 core::ptr::addr_of_mut!(g_shared) as *mut *mut (),
// AARCH64-PC-WINDOWS-MSVC-NEXT:             )
// AARCH64-PC-WINDOWS-MSVC-NEXT:             .compare_exchange(
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 core::ptr::null_mut::<()>() as *mut (),
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 fresh as *mut (),
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// AARCH64-PC-WINDOWS-MSVC-NEXT:             )
// AARCH64-PC-WINDOWS-MSVC-NEXT:             .unwrap_or_else(|[[T_0]]| [[T_0]]) as *mut ()
// AARCH64-PC-WINDOWS-MSVC-NEXT:     };
// CHECK-NEXT:     if !previous.is_null() { previous } else { fresh }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     enter();
// CHECK-NEXT:     if (unsafe { core::ptr::read_volatile(core::ptr::addr_of!(g_locked)) }) != 1
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(g_locked) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .compare_exchange(
// CHECK-NEXT:                     0i32,
// CHECK-NEXT:                     1i32,
// CHECK-NEXT:                     core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                     core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .unwrap_or_else(|[[T_0]]| [[T_0]])
// CHECK-NEXT:         }) != 1 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     leave();
// CHECK-NEXT:     if (unsafe { core::ptr::read_volatile(core::ptr::addr_of!(g_locked)) }) != 0
// CHECK-NEXT:         || (unsafe { g_entered }) != 1 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = (unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(g_counter) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(1i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             .wrapping_add(1)
// CHECK-NEXT:     }) != 1
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(g_counter) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add(5i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }) != 1;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = [[V_0]]
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(g_counter) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_sub(1i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:                 .wrapping_sub(1)
// CHECK-NEXT:         }) != 5;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: bool = [[V_1]]
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(g_counter) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_or(48i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }) != 5;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: bool = [[V_2]]
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(g_counter) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_and(49i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }) != 53;
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: bool = [[V_3]]
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(g_counter) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_xor(17i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }) != 49;
// CHECK-NEXT:     if [[V_4]]
// CHECK-NEXT:         || (unsafe { core::ptr::read_volatile(core::ptr::addr_of!(g_counter)) }) != 32 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut a: i32 = 1;
// CHECK-NEXT:     let mut b: i32 = 2;
// CHECK-NEXT:     if reference_shared(core::ptr::addr_of_mut!(a) as *mut ())
// CHECK-NEXT:         != (core::ptr::addr_of_mut!(a) as *mut ())
// CHECK-NEXT:         || reference_shared(core::ptr::addr_of_mut!(b) as *mut ())
// CHECK-NEXT:             != (core::ptr::addr_of_mut!(a) as *mut ()) {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_5:__v[0-9]+]]: *mut () = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         core::sync::atomic::AtomicPtr::from_ptr(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 core::ptr::addr_of_mut!(g_shared) as *mut *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             )
// X86_64-PC-WINDOWS-MSVC-NEXT:             .swap(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 (core::ptr::addr_of_mut!(b) as *mut ()) as *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ) as *mut ()
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_5]] != (core::ptr::addr_of_mut!(a) as *mut ())
// I686-PC-WINDOWS-MSVC-NEXT:     if _InlineInterlockedExchangePointer(
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { core::ptr::addr_of_mut!(g_shared) },
// I686-PC-WINDOWS-MSVC-NEXT:         core::ptr::addr_of_mut!(b) as *mut (),
// I686-PC-WINDOWS-MSVC-NEXT:     ) != (core::ptr::addr_of_mut!(a) as *mut ())
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_5:__v[0-9]+]]: *mut () = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         core::sync::atomic::AtomicPtr::from_ptr(
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 core::ptr::addr_of_mut!(g_shared) as *mut *mut (),
// AARCH64-PC-WINDOWS-MSVC-NEXT:             )
// AARCH64-PC-WINDOWS-MSVC-NEXT:             .swap(
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 (core::ptr::addr_of_mut!(b) as *mut ()) as *mut (),
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// AARCH64-PC-WINDOWS-MSVC-NEXT:             ) as *mut ()
// AARCH64-PC-WINDOWS-MSVC-NEXT:     };
// AARCH64-PC-WINDOWS-MSVC-NEXT:     if [[V_5]] != (core::ptr::addr_of_mut!(a) as *mut ())
// CHECK-NEXT:         || (unsafe { core::ptr::read_volatile(core::ptr::addr_of!(g_shared)) })
// CHECK-NEXT:             != (core::ptr::addr_of_mut!(b) as *mut ()) {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut flags: i32 = 5;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_6:__v[0-9]+]]: i32 = 2;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_7:__v[0-9]+]]: u8 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_5:__v[0-9]+]]: i32 = 2;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_6:__v[0-9]+]]: u8 = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_6:__v[0-9]+]]: i32 = 2;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_7:__v[0-9]+]]: u8 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU8::from_ptr(
// CHECK-NEXT:                 (((core::ptr::addr_of_mut!(flags) as *mut i32) as *mut i32) as *mut u8)
// X86_64-PC-WINDOWS-MSVC-NEXT:                     .wrapping_offset(([[V_6]] >> 3i32) as isize) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:                     .wrapping_offset(([[V_5]] >> 3i32) as isize) as *mut u8,
// AARCH64-PC-WINDOWS-MSVC-NEXT:                     .wrapping_offset(([[V_6]] >> 3i32) as isize) as *mut u8,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 !(1u8 << (([[V_6]] as u8) & 7)),
// I686-PC-WINDOWS-MSVC-NEXT:                 !(1u8 << (([[V_5]] as u8) & 7)),
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 !(1u8 << (([[V_6]] as u8) & 7)),
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             )
// CHECK-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_7]] >> (([[V_6]] as u8) & 7) & 1 == 0
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_6]] >> (([[V_5]] as u8) & 7) & 1 == 0
// AARCH64-PC-WINDOWS-MSVC-NEXT:     if [[V_7]] >> (([[V_6]] as u8) & 7) & 1 == 0
// CHECK-NEXT:         || (unsafe { core::ptr::read_volatile(core::ptr::addr_of!(flags)) }) != 1 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_8:__v[0-9]+]]: i32 = 2;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_9:__v[0-9]+]]: u8 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_7:__v[0-9]+]]: i32 = 2;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_8:__v[0-9]+]]: u8 = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_8:__v[0-9]+]]: i32 = 2;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_9:__v[0-9]+]]: u8 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU8::from_ptr(
// CHECK-NEXT:                 (((core::ptr::addr_of_mut!(flags) as *mut i32) as *mut i32) as *mut u8)
// X86_64-PC-WINDOWS-MSVC-NEXT:                     .wrapping_offset(([[V_8]] >> 3i32) as isize) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:                     .wrapping_offset(([[V_7]] >> 3i32) as isize) as *mut u8,
// AARCH64-PC-WINDOWS-MSVC-NEXT:                     .wrapping_offset(([[V_8]] >> 3i32) as isize) as *mut u8,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 !(1u8 << (([[V_8]] as u8) & 7)),
// I686-PC-WINDOWS-MSVC-NEXT:                 !(1u8 << (([[V_7]] as u8) & 7)),
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 !(1u8 << (([[V_8]] as u8) & 7)),
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             )
// CHECK-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_9]] >> (([[V_8]] as u8) & 7) & 1 != 0
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_8]] >> (([[V_7]] as u8) & 7) & 1 != 0
// AARCH64-PC-WINDOWS-MSVC-NEXT:     if [[V_9]] >> (([[V_8]] as u8) & 7) & 1 != 0
// CHECK-NEXT:         || (unsafe { core::ptr::read_volatile(core::ptr::addr_of!(flags)) }) != 1 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_10:__v[0-9]+]]: i32 = 3;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_11:__v[0-9]+]]: u8 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_9:__v[0-9]+]]: i32 = 3;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_10:__v[0-9]+]]: u8 = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_10:__v[0-9]+]]: i32 = 3;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_11:__v[0-9]+]]: u8 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU8::from_ptr(
// CHECK-NEXT:                 (((core::ptr::addr_of_mut!(flags) as *mut i32) as *mut i32) as *mut u8)
// X86_64-PC-WINDOWS-MSVC-NEXT:                     .wrapping_offset(([[V_10]] >> 3i32) as isize) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:                     .wrapping_offset(([[V_9]] >> 3i32) as isize) as *mut u8,
// AARCH64-PC-WINDOWS-MSVC-NEXT:                     .wrapping_offset(([[V_10]] >> 3i32) as isize) as *mut u8,
// CHECK-NEXT:             )
// X86_64-PC-WINDOWS-MSVC-NEXT:             .fetch_or(1u8 << (([[V_10]] as u8) & 7), core::sync::atomic::Ordering::SeqCst)
// I686-PC-WINDOWS-MSVC-NEXT:             .fetch_or(1u8 << (([[V_9]] as u8) & 7), core::sync::atomic::Ordering::SeqCst)
// AARCH64-PC-WINDOWS-MSVC-NEXT:             .fetch_or(1u8 << (([[V_10]] as u8) & 7), core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_11]] >> (([[V_10]] as u8) & 7) & 1 != 0
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_10]] >> (([[V_9]] as u8) & 7) & 1 != 0
// AARCH64-PC-WINDOWS-MSVC-NEXT:     if [[V_11]] >> (([[V_10]] as u8) & 7) & 1 != 0
// CHECK-NEXT:         || (unsafe { core::ptr::read_volatile(core::ptr::addr_of!(flags)) }) != 9 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
