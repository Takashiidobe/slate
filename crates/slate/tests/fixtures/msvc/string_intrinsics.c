/* { dg-options "/Zc:preprocessor" } */
#if defined(_M_IX86) || defined(_M_X64)
#include <intrin.h>
#include <stdlib.h>
#include <string.h>

static unsigned char bytes[16];
static unsigned short words[8];
static unsigned long dwords[8];

static void fill_bytes(void) {
    for (int i = 0; i < 16; i++)
        bytes[i] = (unsigned char)(i + 1);
}

static int bytes_are(const char *want) {
    for (int i = 0; i < 16; i++)
        if (bytes[i] != (unsigned char)want[i])
            return 0;
    return 1;
}

int main(void) {
    unsigned char out[16];

    fill_bytes();
    __movsb(out, bytes, 16);
    if (memcmp(out, bytes, 16) != 0)
        abort();

    fill_bytes();
    __movsb(bytes, bytes + 3, 10);
    if (!bytes_are("\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0b\x0c\x0d\x0e\x0f\x10"))
        abort();

    fill_bytes();
    __movsb(bytes + 2, bytes, 10);
    if (!bytes_are("\x01\x02\x01\x02\x01\x02\x01\x02\x01\x02\x01\x02\x0d\x0e\x0f\x10"))
        abort();

    fill_bytes();
    __movsb(bytes + 5, bytes + 5, 4);
    __movsb(bytes, bytes + 8, 0);
    if (!bytes_are("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10"))
        abort();

    for (int i = 0; i < 8; i++)
        words[i] = (unsigned short)(0x1000 + i);
    __movsw(words + 1, words, 6);
    for (int i = 1; i < 7; i++)
        if (words[i] != 0x1000)
            abort();
    if (words[0] != 0x1000 || words[7] != 0x1007)
        abort();

    for (int i = 0; i < 8; i++)
        dwords[i] = 0x10000000ul + i;
    __movsd(dwords, dwords + 2, 5);
    if (dwords[0] != 0x10000002ul || dwords[4] != 0x10000006ul || dwords[5] != 0x10000005ul)
        abort();
    __movsd(dwords + 2, dwords, 4);
    if (dwords[2] != 0x10000002ul || dwords[3] != 0x10000003ul || dwords[4] != 0x10000002ul ||
        dwords[5] != 0x10000003ul || dwords[6] != 0x10000006ul)
        abort();

    __stosb(out, 0xab, 7);
    for (int i = 0; i < 7; i++)
        if (out[i] != 0xab)
            abort();
    if (out[7] != bytes[7])
        abort();

    __stosw(words + 2, 0xbeef, 3);
    if (words[1] != 0x1000 || words[2] != 0xbeef || words[4] != 0xbeef || words[5] != 0x1000)
        abort();

    __stosd(dwords + 1, 0xdeadbeeful, 2);
    if (dwords[0] != 0x10000002ul || dwords[1] != 0xdeadbeeful || dwords[2] != 0xdeadbeeful ||
        dwords[3] != 0x10000003ul)
        abort();

#ifdef _M_X64
    unsigned long long qwords[6] = {1, 2, 3, 4, 5, 6};
    __movsq(qwords + 1, qwords, 4);
    if (qwords[0] != 1 || qwords[1] != 1 || qwords[4] != 1 || qwords[5] != 6)
        abort();
    __movsq(qwords, qwords + 4, 2);
    if (qwords[0] != 1 || qwords[1] != 6)
        abort();
    __stosq(qwords + 2, 0x0123456789abcdefull, 3);
    if (qwords[1] != 6 || qwords[2] != 0x0123456789abcdefull || qwords[4] != 0x0123456789abcdefull)
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
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::memcmp;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::memcmp;
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn fill_bytes() {
// I686-PC-WINDOWS-MSVC-NEXT: fn fill_bytes() {
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0i32..16i32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0i32..16i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             bytes.0[i as usize] = (i + 1) as u8;
// I686-PC-WINDOWS-MSVC-NEXT:             bytes[i as usize] = (i + 1) as u8;
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn bytes_are(want: *const i8) -> i32 {
// I686-PC-WINDOWS-MSVC-NEXT: fn bytes_are(want: *const i8) -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0i32..16i32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0i32..16i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         if ((unsafe { bytes.0[i as usize] }) as i32)
// I686-PC-WINDOWS-MSVC-NEXT:         if ((unsafe { bytes[i as usize] }) as i32)
// X86_64-PC-WINDOWS-MSVC-NEXT:             != (((unsafe { *want.offset(i as isize) }) as u8) as i32)
// I686-PC-WINDOWS-MSVC-NEXT:             != (((unsafe { *want.offset(i as isize) }) as u8) as i32)
// X86_64-PC-WINDOWS-MSVC-NEXT:         {
// I686-PC-WINDOWS-MSVC-NEXT:         {
// X86_64-PC-WINDOWS-MSVC-NEXT:             return 0;
// I686-PC-WINDOWS-MSVC-NEXT:             return 0;
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     1
// I686-PC-WINDOWS-MSVC-NEXT:     1
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut out: __SlateAlign16<[u8; 16]> = __SlateAlign16([0u8; 16]);
// I686-PC-WINDOWS-MSVC-NEXT:     let mut out: [u8; 16] = [0u8; 16];
// X86_64-PC-WINDOWS-MSVC-NEXT:     fill_bytes();
// I686-PC-WINDOWS-MSVC-NEXT:     fill_bytes();
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsb(
// I686-PC-WINDOWS-MSVC-NEXT:         __movsb(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (out.0.as_mut_ptr() as *mut u8) as *mut u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes.0) as *mut u8) as *const u8) as *const u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             16u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (out.as_mut_ptr() as *mut u8) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes) as *mut u8) as *const u8) as *const u8,
// I686-PC-WINDOWS-MSVC-NEXT:             16u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         memcmp(
// I686-PC-WINDOWS-MSVC-NEXT:         memcmp(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (out.0.as_mut_ptr() as *const ()) as *const core::ffi::c_void,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes.0) as *mut u8) as *const ()) as *const core::ffi::c_void,
// X86_64-PC-WINDOWS-MSVC-NEXT:             16u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (out.as_mut_ptr() as *const ()) as *const core::ffi::c_void,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes) as *mut u8) as *const ()) as *const core::ffi::c_void,
// I686-PC-WINDOWS-MSVC-NEXT:             16u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) as i32
// I686-PC-WINDOWS-MSVC-NEXT:         ) as i32
// X86_64-PC-WINDOWS-MSVC-NEXT:     }) != 0
// I686-PC-WINDOWS-MSVC-NEXT:     }) != 0
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     fill_bytes();
// I686-PC-WINDOWS-MSVC-NEXT:     fill_bytes();
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsb(
// I686-PC-WINDOWS-MSVC-NEXT:         __movsb(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(bytes.0) as *mut u8) as *mut u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes.0) as *mut u8).add(3) as *const u8) as *const u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             10u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(bytes) as *mut u8) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes) as *mut u8).add(3) as *const u8) as *const u8,
// I686-PC-WINDOWS-MSVC-NEXT:             10u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if bytes_are(c"\x04\x05\x06\x07\x08\t\n\x0b\x0c\r\x0b\x0c\r\x0e\x0f\x10".as_ptr()) == 0 {
// I686-PC-WINDOWS-MSVC-NEXT:     if bytes_are(c"\x04\x05\x06\x07\x08\t\n\x0b\x0c\r\x0b\x0c\r\x0e\x0f\x10".as_ptr()) == 0 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     fill_bytes();
// I686-PC-WINDOWS-MSVC-NEXT:     fill_bytes();
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsb(
// I686-PC-WINDOWS-MSVC-NEXT:         __movsb(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(bytes.0) as *mut u8).add(2) as *mut u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes.0) as *mut u8) as *const u8) as *const u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             10u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(bytes) as *mut u8).add(2) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes) as *mut u8) as *const u8) as *const u8,
// I686-PC-WINDOWS-MSVC-NEXT:             10u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if bytes_are(c"\x01\x02\x01\x02\x01\x02\x01\x02\x01\x02\x01\x02\r\x0e\x0f\x10".as_ptr()) == 0 {
// I686-PC-WINDOWS-MSVC-NEXT:     if bytes_are(c"\x01\x02\x01\x02\x01\x02\x01\x02\x01\x02\x01\x02\r\x0e\x0f\x10".as_ptr()) == 0 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     fill_bytes();
// I686-PC-WINDOWS-MSVC-NEXT:     fill_bytes();
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsb(
// I686-PC-WINDOWS-MSVC-NEXT:         __movsb(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(bytes.0) as *mut u8).add(5) as *mut u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes.0) as *mut u8).add(5) as *const u8) as *const u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             4u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(bytes) as *mut u8).add(5) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes) as *mut u8).add(5) as *const u8) as *const u8,
// I686-PC-WINDOWS-MSVC-NEXT:             4u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsb(
// I686-PC-WINDOWS-MSVC-NEXT:         __movsb(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(bytes.0) as *mut u8) as *mut u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes.0) as *mut u8).add(8) as *const u8) as *const u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(bytes) as *mut u8) as *mut u8,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(bytes) as *mut u8).add(8) as *const u8) as *const u8,
// I686-PC-WINDOWS-MSVC-NEXT:             0u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if bytes_are(c"\x01\x02\x03\x04\x05\x06\x07\x08\t\n\x0b\x0c\r\x0e\x0f\x10".as_ptr()) == 0 {
// I686-PC-WINDOWS-MSVC-NEXT:     if bytes_are(c"\x01\x02\x03\x04\x05\x06\x07\x08\t\n\x0b\x0c\r\x0e\x0f\x10".as_ptr()) == 0 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0i32..8i32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0i32..8i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             words.0[i as usize] = (4096 + i) as u16;
// I686-PC-WINDOWS-MSVC-NEXT:             words[i as usize] = (4096 + i) as u16;
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
// X86_64-PC-WINDOWS-MSVC-NEXT:             6u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(words) as *mut u16).add(1) as *mut u16,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(words) as *mut u16) as *const u16) as *const u16,
// I686-PC-WINDOWS-MSVC-NEXT:             6u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 1i32..7i32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 1i32..7i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         if ((unsafe { words.0[i as usize] }) as i32) != 4096 {
// I686-PC-WINDOWS-MSVC-NEXT:         if ((unsafe { words[i as usize] }) as i32) != 4096 {
// X86_64-PC-WINDOWS-MSVC-NEXT:             unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:             unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     if ((unsafe { words.0[0] }) as i32) != 4096 || ((unsafe { words.0[7] }) as i32) != 4103 {
// I686-PC-WINDOWS-MSVC-NEXT:     if ((unsafe { words[0] }) as i32) != 4096 || ((unsafe { words[7] }) as i32) != 4103 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0i32..8i32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0i32..8i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             dwords.0[i as usize] = 268435456u32.wrapping_add(i as u32);
// I686-PC-WINDOWS-MSVC-NEXT:             dwords[i as usize] = 268435456u32.wrapping_add(i as u32);
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
// X86_64-PC-WINDOWS-MSVC-NEXT:             5u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(dwords) as *mut u32) as *mut u32,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(dwords) as *mut u32).add(2) as *const u32) as *const u32,
// I686-PC-WINDOWS-MSVC-NEXT:             5u32 as usize,
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
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsd(
// I686-PC-WINDOWS-MSVC-NEXT:         __movsd(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(dwords.0) as *mut u32).add(2) as *mut u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(dwords.0) as *mut u32) as *const u32) as *const u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             4u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(dwords) as *mut u32).add(2) as *mut u32,
// I686-PC-WINDOWS-MSVC-NEXT:             ((std::ptr::addr_of_mut!(dwords) as *mut u32) as *const u32) as *const u32,
// I686-PC-WINDOWS-MSVC-NEXT:             4u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe { dwords.0[2] }) != 268435458
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[3] }) != 268435459
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[4] }) != 268435458
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[5] }) != 268435459
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[6] }) != 268435462
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe { dwords[2] }) != 268435458
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[3] }) != 268435459
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[4] }) != 268435458
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[5] }) != 268435459
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[6] }) != 268435462
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
// X86_64-PC-WINDOWS-MSVC-NEXT:             7u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             7u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     for i in 0i32..7i32 {
// I686-PC-WINDOWS-MSVC-NEXT:     for i in 0i32..7i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         if (out.0[i as usize] as i32) != 171 {
// I686-PC-WINDOWS-MSVC-NEXT:         if (out[i as usize] as i32) != 171 {
// X86_64-PC-WINDOWS-MSVC-NEXT:             unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:             unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (out.0[7] as i32) != ((unsafe { bytes.0[7] }) as i32) {
// I686-PC-WINDOWS-MSVC-NEXT:     if (out[7] as i32) != ((unsafe { bytes[7] }) as i32) {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __stosw(
// I686-PC-WINDOWS-MSVC-NEXT:         __stosw(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(words.0) as *mut u16).add(2) as *mut u16,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(words) as *mut u16).add(2) as *mut u16,
// X86_64-PC-WINDOWS-MSVC-NEXT:             48879u16 as u16,
// I686-PC-WINDOWS-MSVC-NEXT:             48879u16 as u16,
// X86_64-PC-WINDOWS-MSVC-NEXT:             3u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             3u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if ((unsafe { words.0[1] }) as i32) != 4096
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words.0[2] }) as i32) != 48879
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words.0[4] }) as i32) != 48879
// X86_64-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words.0[5] }) as i32) != 4096
// I686-PC-WINDOWS-MSVC-NEXT:     if ((unsafe { words[1] }) as i32) != 4096
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words[2] }) as i32) != 48879
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words[4] }) as i32) != 48879
// I686-PC-WINDOWS-MSVC-NEXT:         || ((unsafe { words[5] }) as i32) != 4096
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
// X86_64-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(dwords.0) as *mut u32).add(1) as *mut u32,
// I686-PC-WINDOWS-MSVC-NEXT:             (std::ptr::addr_of_mut!(dwords) as *mut u32).add(1) as *mut u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             3735928559u32 as u32,
// I686-PC-WINDOWS-MSVC-NEXT:             3735928559u32 as u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             2u64 as usize,
// I686-PC-WINDOWS-MSVC-NEXT:             2u32 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe { dwords.0[0] }) != 268435458
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[1] }) != 3735928559
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[2] }) != 3735928559
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords.0[3] }) != 268435459
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe { dwords[0] }) != 268435458
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[1] }) != 3735928559
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[2] }) != 3735928559
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe { dwords[3] }) != 268435459
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut qwords: __SlateAlign16<[u64; 6]> = __SlateAlign16([1u64, 2u64, 3u64, 4u64, 5u64, 6u64]);
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsq(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (qwords.0.as_mut_ptr() as *mut u64).add(1) as *mut u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             (qwords.0.as_mut_ptr() as *const u64) as *const u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             4u64 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if qwords.0[0] != 1 || qwords.0[1] != 1 || qwords.0[4] != 1 || qwords.0[5] != 6 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __movsq(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (qwords.0.as_mut_ptr() as *mut u64) as *mut u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ((qwords.0.as_mut_ptr() as *mut u64).add(4) as *const u64) as *const u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             2u64 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if qwords.0[0] != 1 || qwords.0[1] != 6 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __stosq(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (qwords.0.as_mut_ptr() as *mut u64).add(2) as *mut u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             81985529216486895u64 as u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             3u64 as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if qwords.0[1] != 6 || qwords.0[2] != 81985529216486895 || qwords.0[4] != 81985529216486895 {
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
