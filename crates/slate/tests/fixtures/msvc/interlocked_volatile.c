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
// I686-PC-WINDOWS-MSVC-NEXT: fn _InlineInterlockedExchangePointer(Destination: *mut *mut (), Value: &()) -> *mut () {
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_0:__v[0-9]+]]: i32 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         let [[T_0:__t[0-9]+]] = (Value as *const ()) as i32;
// I686-PC-WINDOWS-MSVC-NEXT:         let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(Destination as *mut i32)
// I686-PC-WINDOWS-MSVC-NEXT:             .swap([[T_0]], std::sync::atomic::Ordering::SeqCst);
// I686-PC-WINDOWS-MSVC-NEXT:         [[T_1]]
// I686-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     [[V_0]] as *mut ()
// I686-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-NEXT: fn _InlineInterlockedCompareExchangePointer(
// I686-PC-WINDOWS-MSVC-NEXT:     Destination: *mut *mut (),
// I686-PC-WINDOWS-MSVC-NEXT:     ExChange: &(),
// I686-PC-WINDOWS-MSVC-NEXT:     Comperand: *mut (),
// I686-PC-WINDOWS-MSVC-NEXT: ) -> *mut () {
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: i32 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         let [[T_1]] = Comperand as i32;
// I686-PC-WINDOWS-MSVC-NEXT:         let [[T_0]] = (ExChange as *const ()) as i32;
// I686-PC-WINDOWS-MSVC-NEXT:         let [[T_2:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(Destination as *mut i32)
// I686-PC-WINDOWS-MSVC-NEXT:             .compare_exchange(
// I686-PC-WINDOWS-MSVC-NEXT:                 [[T_1]],
// I686-PC-WINDOWS-MSVC-NEXT:                 [[T_0]],
// I686-PC-WINDOWS-MSVC-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// I686-PC-WINDOWS-MSVC-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// I686-PC-WINDOWS-MSVC-NEXT:             );
// I686-PC-WINDOWS-MSVC-NEXT:         [[T_2]].unwrap_or_else(|[[T_3:__t[0-9]+]]| [[T_3]])
// I686-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     [[V_0]] as *mut ()
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
// CHECK-NEXT: static mut g_counter: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// I686-PC-WINDOWS-MSVC-EMPTY:
// CHECK-NEXT: static mut g_shared: *mut () = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g_entered: i32 = unsafe { std::mem::zeroed() };
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// AARCH64-PC-WINDOWS-MSVC-EMPTY:
// AARCH64-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// CHECK-EMPTY:
// CHECK-NEXT: fn enter() {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         let [[V_0]]: i32 = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             let [[T_1:__t[0-9]+]] = 0i32;
// I686-PC-WINDOWS-MSVC-NEXT:             let [[T_1]] = 0i32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:             let [[T_1:__t[0-9]+]] = 0i32;
// X86_64-PC-WINDOWS-MSVC-NEXT:             let [[T_0:__t[0-9]+]] = 1i32;
// I686-PC-WINDOWS-MSVC-NEXT:             let [[T_0]] = 1i32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:             let [[T_0:__t[0-9]+]] = 1i32;
// X86_64-PC-WINDOWS-MSVC-NEXT:             let [[T_2:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// I686-PC-WINDOWS-MSVC-NEXT:             let [[T_2]] = std::sync::atomic::AtomicI32::from_ptr(
// AARCH64-PC-WINDOWS-MSVC-NEXT:             let [[T_2:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(g_locked) as *mut i32
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 [[T_1]],
// CHECK-NEXT:                 [[T_0]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             );
// X86_64-PC-WINDOWS-MSVC-NEXT:             [[T_2]].unwrap_or_else(|[[T_3:__t[0-9]+]]| [[T_3]])
// I686-PC-WINDOWS-MSVC-NEXT:             [[T_2]].unwrap_or_else(|[[T_3]]| [[T_3]])
// AARCH64-PC-WINDOWS-MSVC-NEXT:             [[T_2]].unwrap_or_else(|[[T_3:__t[0-9]+]]| [[T_3]])
// CHECK-NEXT:         };
// CHECK-NEXT:         if [[V_0]] == 0 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g_entered = g_entered + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn leave() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]] = 0i32;
// CHECK-NEXT:         let [[T_1]] =
// CHECK-NEXT:             std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(g_locked) as *mut i32)
// CHECK-NEXT:                 .swap([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn reference_shared(fresh: &()) -> *mut () {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: *mut () = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[T_1]] = std::ptr::null_mut::<()>();
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[T_0]] = fresh as *const ();
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[T_2]] = std::sync::atomic::AtomicPtr::from_ptr(
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(g_shared) as *mut *mut ()
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:         .compare_exchange(
// X86_64-PC-WINDOWS-MSVC-NEXT:             [[T_1]] as *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             [[T_0]] as *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::sync::atomic::Ordering::SeqCst,
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::sync::atomic::Ordering::SeqCst,
// X86_64-PC-WINDOWS-MSVC-NEXT:         );
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_2]].unwrap_or_else(|[[T_3]]| [[T_3]]) as *mut ()
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let previous: *mut () = [[V_0]];
// I686-PC-WINDOWS-MSVC-NEXT:     let previous: *mut () = _InlineInterlockedCompareExchangePointer(
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::ptr::addr_of_mut!(g_shared) },
// I686-PC-WINDOWS-MSVC-NEXT:         fresh,
// I686-PC-WINDOWS-MSVC-NEXT:         std::ptr::null_mut::<()>(),
// I686-PC-WINDOWS-MSVC-NEXT:     );
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: *mut () = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[T_1]] = std::ptr::null_mut::<()>();
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[T_0]] = fresh as *const ();
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[T_2]] = std::sync::atomic::AtomicPtr::from_ptr(
// AARCH64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(g_shared) as *mut *mut ()
// AARCH64-PC-WINDOWS-MSVC-NEXT:         )
// AARCH64-PC-WINDOWS-MSVC-NEXT:         .compare_exchange(
// AARCH64-PC-WINDOWS-MSVC-NEXT:             [[T_1]] as *mut (),
// AARCH64-PC-WINDOWS-MSVC-NEXT:             [[T_0]] as *mut (),
// AARCH64-PC-WINDOWS-MSVC-NEXT:             std::sync::atomic::Ordering::SeqCst,
// AARCH64-PC-WINDOWS-MSVC-NEXT:             std::sync::atomic::Ordering::SeqCst,
// AARCH64-PC-WINDOWS-MSVC-NEXT:         );
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_2]].unwrap_or_else(|[[T_3]]| [[T_3]]) as *mut ()
// AARCH64-PC-WINDOWS-MSVC-NEXT:     };
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let previous: *mut () = [[V_0]];
// CHECK-NEXT:     if !previous.is_null() {
// CHECK-NEXT:         previous
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (fresh as *const ()) as *mut ()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     enter();
// CHECK-NEXT:     let [[V_0]]: bool = if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(g_locked)) }) != 1 {
// CHECK-NEXT:         true
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             let [[T_1]] = 0i32;
// CHECK-NEXT:             let [[T_0]] = 1i32;
// CHECK-NEXT:             let [[T_2]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(g_locked) as *mut i32
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 [[T_1]],
// CHECK-NEXT:                 [[T_0]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             );
// CHECK-NEXT:             [[T_2]].unwrap_or_else(|[[T_3]]| [[T_3]])
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_1]] != 1
// CHECK-NEXT:     };
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     leave();
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(g_locked)) }) != 0
// CHECK-NEXT:         || (unsafe { g_entered }) != 1
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = 1i32;
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] =
// CHECK-NEXT:             std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(g_counter) as *mut i32)
// CHECK-NEXT:                 .fetch_add([[T_4]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_5]].wrapping_add([[T_4]])
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: bool = if [[V_2]] != 1 {
// CHECK-NEXT:         true
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_4:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             let [[T_6:__t[0-9]+]] = 5i32;
// CHECK-NEXT:             let [[T_7:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(g_counter) as *mut i32
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_6]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:             [[T_7]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_4]] != 1
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: bool = if [[V_3]] {
// CHECK-NEXT:         true
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_6:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             let [[T_8:__t[0-9]+]] = 1i32;
// CHECK-NEXT:             let [[T_9:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(g_counter) as *mut i32
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_8]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:             [[T_9]].wrapping_sub([[T_8]])
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_6]] != 5
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: bool = if [[V_5]] {
// CHECK-NEXT:         true
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_8:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             let [[T_10:__t[0-9]+]] = 48i32;
// CHECK-NEXT:             let [[T_11:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(g_counter) as *mut i32
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_or([[T_10]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:             [[T_11]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_8]] != 5
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: bool = if [[V_7]] {
// CHECK-NEXT:         true
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_10:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             let [[T_12:__t[0-9]+]] = 49i32;
// CHECK-NEXT:             let [[T_13:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(g_counter) as *mut i32
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and([[T_12]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:             [[T_13]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_10]] != 53
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: bool = if [[V_9]] {
// CHECK-NEXT:         true
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_12:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             let [[T_14:__t[0-9]+]] = 17i32;
// CHECK-NEXT:             let [[T_15:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(g_counter) as *mut i32
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_xor([[T_14]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:             [[T_15]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_12]] != 49
// CHECK-NEXT:     };
// CHECK-NEXT:     if [[V_11]] || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(g_counter)) }) != 32 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut a: i32 = 1;
// CHECK-NEXT:     let mut b: i32 = 2;
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: bool = reference_shared(unsafe { &(*(std::ptr::addr_of_mut!(a) as *mut ())) })
// CHECK-NEXT:         != (std::ptr::addr_of_mut!(a) as *mut ())
// CHECK-NEXT:         || reference_shared(unsafe { &(*(std::ptr::addr_of_mut!(b) as *mut ())) })
// CHECK-NEXT:             != (std::ptr::addr_of_mut!(a) as *mut ());
// CHECK-NEXT:     if [[V_13]] {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_14:__v[0-9]+]]: *mut () = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[T_16:__t[0-9]+]] = std::ptr::addr_of_mut!(b) as *mut ();
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[T_17:__t[0-9]+]] = std::sync::atomic::AtomicPtr::from_ptr(
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(g_shared) as *mut *mut ()
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:         .swap([[T_16]] as *mut (), std::sync::atomic::Ordering::SeqCst);
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_17]] as *mut ()
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_14]] != (std::ptr::addr_of_mut!(a) as *mut ())
// I686-PC-WINDOWS-MSVC-NEXT:     if _InlineInterlockedExchangePointer(unsafe { std::ptr::addr_of_mut!(g_shared) }, unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         &(*(std::ptr::addr_of_mut!(b) as *mut ()))
// I686-PC-WINDOWS-MSVC-NEXT:     }) != (std::ptr::addr_of_mut!(a) as *mut ())
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_14:__v[0-9]+]]: *mut () = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[T_16:__t[0-9]+]] = std::ptr::addr_of_mut!(b) as *mut ();
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[T_17:__t[0-9]+]] = std::sync::atomic::AtomicPtr::from_ptr(
// AARCH64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(g_shared) as *mut *mut ()
// AARCH64-PC-WINDOWS-MSVC-NEXT:         )
// AARCH64-PC-WINDOWS-MSVC-NEXT:         .swap([[T_16]] as *mut (), std::sync::atomic::Ordering::SeqCst);
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_17]] as *mut ()
// AARCH64-PC-WINDOWS-MSVC-NEXT:     };
// AARCH64-PC-WINDOWS-MSVC-NEXT:     if [[V_14]] != (std::ptr::addr_of_mut!(a) as *mut ())
// CHECK-NEXT:         || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(g_shared)) })
// CHECK-NEXT:             != (std::ptr::addr_of_mut!(b) as *mut ())
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut flags: i32 = 5;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_15:__v[0-9]+]]: i32 = 2;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_16:__v[0-9]+]]: u8 = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[T_18:__t[0-9]+]] = !(1u8 << (([[V_15]] as u8) & 7));
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_14:__v[0-9]+]]: i32 = 2;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_15:__v[0-9]+]]: u8 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         let [[T_16:__t[0-9]+]] = !(1u8 << (([[V_14]] as u8) & 7));
// I686-PC-WINDOWS-MSVC-NEXT:         let [[T_17:__t[0-9]+]] = std::sync::atomic::AtomicU8::from_ptr(
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(flags) as *mut i32) as *mut u8)
// I686-PC-WINDOWS-MSVC-NEXT:                 .offset(([[V_14]] >> 3i32) as isize) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         .fetch_and([[T_16]], std::sync::atomic::Ordering::SeqCst);
// I686-PC-WINDOWS-MSVC-NEXT:         [[T_17]]
// I686-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_15]] >> (([[V_14]] as u8) & 7) & 1 == 0
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(flags)) }) != 1
// I686-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_16:__v[0-9]+]]: i32 = 2;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_17:__v[0-9]+]]: u8 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         let [[T_18:__t[0-9]+]] = !(1u8 << (([[V_16]] as u8) & 7));
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_15:__v[0-9]+]]: i32 = 2;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_16:__v[0-9]+]]: u8 = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[T_18:__t[0-9]+]] = !(1u8 << (([[V_15]] as u8) & 7));
// CHECK-NEXT:         let [[T_19:__t[0-9]+]] = std::sync::atomic::AtomicU8::from_ptr(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(flags) as *mut i32) as *mut u8)
// X86_64-PC-WINDOWS-MSVC-NEXT:                 .offset(([[V_15]] >> 3i32) as isize) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:                 .offset(([[V_16]] >> 3i32) as isize) as *mut u8,
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 .offset(([[V_15]] >> 3i32) as isize) as *mut u8,
// CHECK-NEXT:         )
// CHECK-NEXT:         .fetch_and([[T_18]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_19]]
// CHECK-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_16]] >> (([[V_15]] as u8) & 7) & 1 == 0
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_17]] >> (([[V_16]] as u8) & 7) & 1 != 0
// AARCH64-PC-WINDOWS-MSVC-NEXT:     if [[V_16]] >> (([[V_15]] as u8) & 7) & 1 == 0
// CHECK-NEXT:         || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(flags)) }) != 1
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_17:__v[0-9]+]]: i32 = 2;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_18:__v[0-9]+]]: u8 = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[T_20:__t[0-9]+]] = !(1u8 << (([[V_17]] as u8) & 7));
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_18:__v[0-9]+]]: i32 = 3;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_19:__v[0-9]+]]: u8 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         let [[T_20:__t[0-9]+]] = 1u8 << (([[V_18]] as u8) & 7);
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_17:__v[0-9]+]]: i32 = 2;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_18:__v[0-9]+]]: u8 = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[T_20:__t[0-9]+]] = !(1u8 << (([[V_17]] as u8) & 7));
// CHECK-NEXT:         let [[T_21:__t[0-9]+]] = std::sync::atomic::AtomicU8::from_ptr(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(flags) as *mut i32) as *mut u8)
// X86_64-PC-WINDOWS-MSVC-NEXT:                 .offset(([[V_17]] >> 3i32) as isize) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:                 .offset(([[V_18]] >> 3i32) as isize) as *mut u8,
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 .offset(([[V_17]] >> 3i32) as isize) as *mut u8,
// CHECK-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:         .fetch_and([[T_20]], std::sync::atomic::Ordering::SeqCst);
// I686-PC-WINDOWS-MSVC-NEXT:         .fetch_or([[T_20]], std::sync::atomic::Ordering::SeqCst);
// AARCH64-PC-WINDOWS-MSVC-NEXT:         .fetch_and([[T_20]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_21]]
// CHECK-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_18]] >> (([[V_17]] as u8) & 7) & 1 != 0
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(flags)) }) != 1
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_19:__v[0-9]+]]: i32 = 3;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_20:__v[0-9]+]]: u8 = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[T_22:__t[0-9]+]] = 1u8 << (([[V_19]] as u8) & 7);
// X86_64-PC-WINDOWS-MSVC-NEXT:         let [[T_23:__t[0-9]+]] = std::sync::atomic::AtomicU8::from_ptr(
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(flags) as *mut i32) as *mut u8)
// X86_64-PC-WINDOWS-MSVC-NEXT:                 .offset(([[V_19]] >> 3i32) as isize) as *mut u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:         .fetch_or([[T_22]], std::sync::atomic::Ordering::SeqCst);
// X86_64-PC-WINDOWS-MSVC-NEXT:         [[T_23]]
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_20]] >> (([[V_19]] as u8) & 7) & 1 != 0
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_19]] >> (([[V_18]] as u8) & 7) & 1 != 0
// AARCH64-PC-WINDOWS-MSVC-NEXT:     if [[V_18]] >> (([[V_17]] as u8) & 7) & 1 != 0
// AARCH64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(flags)) }) != 1
// AARCH64-PC-WINDOWS-MSVC-NEXT:     {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// AARCH64-PC-WINDOWS-MSVC-NEXT:     }
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_19:__v[0-9]+]]: i32 = 3;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let [[V_20:__v[0-9]+]]: u8 = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[T_22:__t[0-9]+]] = 1u8 << (([[V_19]] as u8) & 7);
// AARCH64-PC-WINDOWS-MSVC-NEXT:         let [[T_23:__t[0-9]+]] = std::sync::atomic::AtomicU8::from_ptr(
// AARCH64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(flags) as *mut i32) as *mut u8)
// AARCH64-PC-WINDOWS-MSVC-NEXT:                 .offset(([[V_19]] >> 3i32) as isize) as *mut u8,
// AARCH64-PC-WINDOWS-MSVC-NEXT:         )
// AARCH64-PC-WINDOWS-MSVC-NEXT:         .fetch_or([[T_22]], std::sync::atomic::Ordering::SeqCst);
// AARCH64-PC-WINDOWS-MSVC-NEXT:         [[T_23]]
// AARCH64-PC-WINDOWS-MSVC-NEXT:     };
// AARCH64-PC-WINDOWS-MSVC-NEXT:     if [[V_20]] >> (([[V_19]] as u8) & 7) & 1 != 0
// CHECK-NEXT:         || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(flags)) }) != 9
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
