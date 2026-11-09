/* { dg-options "/Zc:preprocessor" } */
#if defined(_M_IX86) || defined(_M_X64)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#ifdef _M_IX86
#include <intrin.h>
#endif

static BYTE bytes[16];
static WORD words[8];
static DWORD dwords[8];

static UINT count(UINT n) {
    return n;
}

int main(void) {
    BYTE out[16];
    UINT n = count(16);
    SIZE_T half = count(8);

    for (UINT i = 0; i < 16; i++)
        bytes[i] = (BYTE)(i + 1);
    __movsb(out, bytes, n);
    for (UINT i = 0; i < 16; i++)
        if (out[i] != bytes[i])
            abort();

    __movsb(bytes + 2, bytes, count(10));
    if (bytes[2] != 1 || bytes[3] != 2 || bytes[10] != 1 || bytes[11] != 2 || bytes[12] != 13)
        abort();

    __stosb(out, 0xab, half);
    for (UINT i = 0; i < 8; i++)
        if (out[i] != 0xab)
            abort();
    if (out[8] != 9)
        abort();

    for (UINT i = 0; i < 8; i++)
        words[i] = (WORD)(0x1000 + i);
    __movsw(words + 1, words, count(6));
    if (words[1] != 0x1000 || words[6] != 0x1000 || words[7] != 0x1007)
        abort();
    __stosw(words, 0xbeef, count(2));
    if (words[0] != 0xbeef || words[1] != 0xbeef || words[2] != 0x1000)
        abort();

    for (UINT i = 0; i < 8; i++)
        dwords[i] = 0x10000000ul + i;
    __movsd(dwords, dwords + 2, count(5));
    if (dwords[0] != 0x10000002ul || dwords[4] != 0x10000006ul || dwords[5] != 0x10000005ul)
        abort();
    __stosd(dwords + 6, 0xdeadbeeful, count(2));
    if (dwords[5] != 0x10000005ul || dwords[6] != 0xdeadbeeful || dwords[7] != 0xdeadbeeful)
        abort();

#ifdef _M_X64
    DWORD64 qwords[6] = {1, 2, 3, 4, 5, 6};
    __movsq(qwords + 1, qwords, count(4));
    if (qwords[0] != 1 || qwords[1] != 1 || qwords[4] != 1 || qwords[5] != 6)
        abort();
    __stosq(qwords + 2, 0x0123456789abcdefull, count(3));
    if (qwords[1] != 1 || qwords[2] != 0x0123456789abcdefull || qwords[4] != 0x0123456789abcdefull)
        abort();
#endif
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
// X86_64-PC-WINDOWS-MSVC-NEXT: #[repr(C, align(16))]
// X86_64-PC-WINDOWS-MSVC-NEXT: struct __SlateAlign16<T>(T);
// I686-PC-WINDOWS-MSVC-NEXT: static mut bytes: [u8; 16] = unsafe { std::mem::zeroed() };
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: static mut bytes: __SlateAlign16<[u8; 16]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// I686-PC-WINDOWS-MSVC-NEXT: static mut words: [u16; 8] = unsafe { std::mem::zeroed() };
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: static mut words: __SlateAlign16<[u16; 8]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: static mut dwords: __SlateAlign16<[u32; 8]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// I686-PC-WINDOWS-MSVC-NEXT: static mut dwords: [u32; 8] = unsafe { std::mem::zeroed() };
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__movsb;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::__movsb;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__movsd;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::__movsd;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__movsq;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__movsw;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::__movsw;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__stosb;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::__stosb;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__stosd;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::__stosd;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__stosq;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__stosw;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::__stosw;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn count(n: u32) -> u32 {
// I686-PC-WINDOWS-MSVC-NEXT: fn count(n: u32) -> u32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     n
// I686-PC-WINDOWS-MSVC-NEXT:     n
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut out: __SlateAlign16<[u8; 16]> = __SlateAlign16([0u8; 16]);
// I686-PC-WINDOWS-MSVC-NEXT:     let mut out: [u8; 16] = [0u8; 16];
// X86_64-PC-WINDOWS-MSVC-NEXT:     let n: u32 = count(16u32);
// I686-PC-WINDOWS-MSVC-NEXT:     let n: u32 = count(16u32);
// X86_64-PC-WINDOWS-MSVC-NEXT:     let half: u64 = count(8u32) as u64;
// I686-PC-WINDOWS-MSVC-NEXT:     let half: u32 = count(8u32);
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0u32..16u32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0u32..16u32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             bytes.0[i as usize] = i.wrapping_add(1) as u8;
// I686-PC-WINDOWS-MSVC-NEXT:             bytes[i as usize] = i.wrapping_add(1) as u8;
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsb(
// I686-PC-WINDOWS-MSVC-NEXT:         __movsb(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (out.0.as_mut_ptr() as *mut u8) as *mut u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes.0) as *mut u8) as *const u8) as *const u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             (n as u64) as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (out.as_mut_ptr() as *mut u8) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes) as *mut u8) as *const u8) as *const u8,
// I686-PC-WINDOWS-MSVC-NEXT:             n as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0u32..16u32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0u32..16u32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         if (out.0[i as usize] as i32) != ((unsafe { bytes.0[i as usize] }) as i32) {
// I686-PC-WINDOWS-MSVC-NEXT:         if (out[i as usize] as i32) != ((unsafe { bytes[i as usize] }) as i32) {
// X86_64-PC-WINDOWS-MSVC-NEXT:             unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:             unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsb(
// I686-PC-WINDOWS-MSVC-NEXT:         __movsb(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(bytes.0) as *mut u8).add(2) as *mut u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes.0) as *mut u8) as *const u8) as *const u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             (count(10u32) as u64) as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(bytes) as *mut u8).add(2) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes) as *mut u8) as *const u8) as *const u8,
// I686-PC-WINDOWS-MSVC-NEXT:             count(10u32) as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if ((unsafe { bytes.0[2] }) as i32) != 1
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { bytes.0[3] }) as i32) != 2
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { bytes.0[10] }) as i32) != 1
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { bytes.0[11] }) as i32) != 2
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { bytes.0[12] }) as i32) != 13
// I686-PC-WINDOWS-MSVC-NEXT:     if ((unsafe { bytes[2] }) as i32) != 1
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { bytes[3] }) as i32) != 2
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { bytes[10] }) as i32) != 1
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { bytes[11] }) as i32) != 2
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { bytes[12] }) as i32) != 13
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __stosb(
// I686-PC-WINDOWS-MSVC-NEXT:         __stosb(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (out.0.as_mut_ptr() as *mut u8) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:             (out.as_mut_ptr() as *mut u8) as *mut u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             171u8 as u8,
// I686-PC-WINDOWS-MSVC-NEXT:             171u8 as u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             half as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             half as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0u32..8u32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0u32..8u32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         if (out.0[i as usize] as i32) != 171 {
// I686-PC-WINDOWS-MSVC-NEXT:         if (out[i as usize] as i32) != 171 {
// X86_64-PC-WINDOWS-MSVC-NEXT:             unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:             unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (out.0[8] as i32) != 9 {
// I686-PC-WINDOWS-MSVC-NEXT:     if (out[8] as i32) != 9 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0u32..8u32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0u32..8u32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             words.0[i as usize] = 4096u32.wrapping_add(i) as u16;
// I686-PC-WINDOWS-MSVC-NEXT:             words[i as usize] = 4096u32.wrapping_add(i) as u16;
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsw(
// I686-PC-WINDOWS-MSVC-NEXT:         __movsw(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(words.0) as *mut u16).add(1) as *mut u16,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(words.0) as *mut u16) as *const u16) as *const u16,
// X86_64-PC-WINDOWS-MSVC-NEXT:             (count(6u32) as u64) as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(words) as *mut u16).add(1) as *mut u16,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(words) as *mut u16) as *const u16) as *const u16,
// I686-PC-WINDOWS-MSVC-NEXT:             count(6u32) as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if ((unsafe { words.0[1] }) as i32) != 4096
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words.0[6] }) as i32) != 4096
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words.0[7] }) as i32) != 4103
// I686-PC-WINDOWS-MSVC-NEXT:     if ((unsafe { words[1] }) as i32) != 4096
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words[6] }) as i32) != 4096
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words[7] }) as i32) != 4103
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __stosw(
// I686-PC-WINDOWS-MSVC-NEXT:         __stosw(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(words.0) as *mut u16) as *mut u16,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(words) as *mut u16) as *mut u16,
// X86_64-PC-WINDOWS-MSVC-NEXT:             48879u16 as u16,
// I686-PC-WINDOWS-MSVC-NEXT:             48879u16 as u16,
// X86_64-PC-WINDOWS-MSVC-NEXT:             (count(2u32) as u64) as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             count(2u32) as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if ((unsafe { words.0[0] }) as i32) != 48879
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words.0[1] }) as i32) != 48879
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words.0[2] }) as i32) != 4096
// I686-PC-WINDOWS-MSVC-NEXT:     if ((unsafe { words[0] }) as i32) != 48879
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words[1] }) as i32) != 48879
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words[2] }) as i32) != 4096
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0u32..8u32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0u32..8u32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             dwords.0[i as usize] = 268435456u32.wrapping_add(i);
// I686-PC-WINDOWS-MSVC-NEXT:             dwords[i as usize] = 268435456u32.wrapping_add(i);
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsd(
// I686-PC-WINDOWS-MSVC-NEXT:         __movsd(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(dwords.0) as *mut u32) as *mut u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(dwords.0) as *mut u32).add(2) as *const u32) as *const u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             (count(5u32) as u64) as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(dwords) as *mut u32) as *mut u32,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(dwords) as *mut u32).add(2) as *const u32) as *const u32,
// I686-PC-WINDOWS-MSVC-NEXT:             count(5u32) as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe { dwords.0[0] }) != 268435458
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[4] }) != 268435462
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[5] }) != 268435461
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe { dwords[0] }) != 268435458
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[4] }) != 268435462
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[5] }) != 268435461
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __stosd(
// I686-PC-WINDOWS-MSVC-NEXT:         __stosd(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(dwords.0) as *mut u32).add(6) as *mut u32,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(dwords) as *mut u32).add(6) as *mut u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             3735928559u32 as u32,
// I686-PC-WINDOWS-MSVC-NEXT:             3735928559u32 as u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             (count(2u32) as u64) as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             count(2u32) as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe { dwords.0[5] }) != 268435461
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[6] }) != 3735928559
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[7] }) != 3735928559
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe { dwords[5] }) != 268435461
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[6] }) != 3735928559
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[7] }) != 3735928559
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut qwords: __SlateAlign16<[u64; 6]> = __SlateAlign16([1u64, 2u64, 3u64, 4u64, 5u64, 6u64]);
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsq(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (qwords.0.as_mut_ptr() as *mut u64).add(1) as *mut u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             (qwords.0.as_mut_ptr() as *const u64) as *const u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             (count(4u32) as u64) as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if qwords.0[0] != 1 || qwords.0[1] != 1 || qwords.0[4] != 1 || qwords.0[5] != 6 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __stosq(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (qwords.0.as_mut_ptr() as *mut u64).add(2) as *mut u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             81985529216486895u64 as u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             (count(3u32) as u64) as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if qwords.0[1] != 1 || qwords.0[2] != 81985529216486895 || qwords.0[4] != 81985529216486895 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
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
