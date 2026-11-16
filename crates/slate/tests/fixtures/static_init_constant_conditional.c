#include <stdio.h>

unsigned char runtime_bcd(unsigned value) { return (unsigned char)value; }
int runtime(void) { return -1; }

#define bin2bcd(x) (__builtin_constant_p((unsigned char)(x)) ? (((x) / 10) << 4) + (x) % 10 : runtime_bcd(x))
#define ilog2(n) (__builtin_constant_p(n) ? ((n) < 2 ? 0 : 63 - __builtin_clzll(n)) : runtime())

struct header {
    const char *name;
    unsigned long name_length;
    const char *search;
    unsigned long search_length;
};

#define HEADER(name, search) {name, sizeof(name) - 1, search, (search) ? sizeof(search) - 1 : 0}

static const unsigned char descriptor[] = {0x12, bin2bcd(7), bin2bcd(42), 0x09};
static const int log_sizes[] = {ilog2(1), ilog2(64), ilog2(4096)};
static const struct header headers[] = {
    HEADER("CSeq", NULL),
    HEADER("From", "sip:"),
};
static int checked = sizeof(long long) == 8 ? 64 : runtime();
static int nested = 0 ? runtime() : sizeof(int) == 4 ? 32 : runtime();
static long elvis = 5 ?: runtime();

int main(void) {
    for (unsigned i = 0; i < sizeof(descriptor); i++)
        printf("descriptor %u %#x\n", i, descriptor[i]);
    for (unsigned i = 0; i < 3; i++)
        printf("log %u %d\n", i, log_sizes[i]);
    for (unsigned i = 0; i < 2; i++)
        printf("header %s %lu %s %lu\n", headers[i].name, headers[i].name_length,
               headers[i].search ? headers[i].search : "-", headers[i].search_length);
    printf("checked %d nested %d elvis %ld\n", checked, nested, elvis);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn runtime_bcd(value: u32) -> u8 {
// CHECK-NEXT:     value as u8
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-NEXT: fn runtime() -> i32 {
// CHECK-NEXT:     -1
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct header {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     name_length: u64,
// CHECK-NEXT:     search: *const i8,
// CHECK-NEXT:     search_length: u64,
// CHECK-NEXT: }
// CHECK-NEXT: static descriptor: [u8; 4] = [18, 7, 66, 9];
// CHECK-NEXT: static log_sizes: [i32; 3] = [0, 6, 12];
// CHECK-NEXT: static headers: __SlateAlign16<[header; 2]> = __SlateAlign16([
// CHECK-NEXT:     header {
// CHECK-NEXT:         name: (b"CSeq\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         name_length: (core::mem::size_of::<[i8; 5]>() as u64).wrapping_sub(1u64),
// CHECK-NEXT:         search: core::ptr::null::<i8>(),
// CHECK-NEXT:         search_length: 0u64,
// CHECK-NEXT:     },
// CHECK-NEXT:     header {
// CHECK-NEXT:         name: (b"From\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         name_length: (core::mem::size_of::<[i8; 5]>() as u64).wrapping_sub(1u64),
// CHECK-NEXT:         search: (b"sip:\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         search_length: (core::mem::size_of::<[i8; 5]>() as u64).wrapping_sub(1u64),
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut checked: i32 = 64;
// CHECK-NEXT: static mut nested: i32 = 32;
// CHECK-NEXT: static mut elvis: i64 = 5;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: u32 = 0;
// CHECK-NEXT:         while (i as u64) < (core::mem::size_of::<[u8; 4]>() as u64) {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 slate_support::printf(
// CHECK-NEXT:                     c"descriptor %u %#x\n".as_ptr(),
// CHECK-NEXT:                     i,
// CHECK-NEXT:                     descriptor[i as usize] as i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0u32..3u32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(c"log %u %d\n".as_ptr(), i, log_sizes[i as usize])
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0u32..2u32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"header %s %lu %s %lu\n".as_ptr(),
// CHECK-NEXT:                 headers.0[i as usize].name,
// CHECK-NEXT:                 headers.0[i as usize].name_length,
// CHECK-NEXT:                 if !unsafe { headers.0[i as usize].search }.is_null() {
// CHECK-NEXT:                     headers.0[i as usize].search
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     c"-".as_ptr()
// CHECK-NEXT:                 },
// CHECK-NEXT:                 headers.0[i as usize].search_length,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"checked %d nested %d elvis %ld\n".as_ptr(),
// CHECK-NEXT:             checked,
// CHECK-NEXT:             nested,
// CHECK-NEXT:             elvis,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
