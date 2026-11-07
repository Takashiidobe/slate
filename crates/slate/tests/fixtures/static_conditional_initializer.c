#include <stdint.h>
#include <stdio.h>

#define R(a, b) ((uint32_t)((a == 0x80 ? 0x40u - b : -a) << 23))
#define C(x) (x < 2 ? -1 : (R(0x80, 0xc0) | x))
#define E(x) ((x == 0 ? R(0xa0, 0xc0) : x == 0xd ? R(0x80, 0xa0) : R(0x80, 0xc0)) | (R(0x80, 0xc0) >> 6) | x)

const uint32_t bittab[] = {C(0x1), C(0x2), C(0xf), E(0x0), E(0x1), E(0xd)};

struct addr {
    unsigned short family;
    unsigned short port;
    uint32_t       host;
};

static const struct addr lo = {.family = 2, .port = 65535, .host = __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ ? 0x7f000001 : 0x0100007f};

static const short values[] = {[0] = sizeof(long) > 4 ? -1 : 1, [3] = (-256 | 2), [5] = 100 > 50 ? 100 : 50};

static const double scales[] = {1 ? 0.5 : 2.0, 0 ? 1.0 : 3.0f};

int main(void) {
    for (unsigned i = 0; i < sizeof bittab / sizeof bittab[0]; i++)
        printf("%u %08x\n", i, bittab[i]);
    printf("%u %u %08x\n", lo.family, lo.port, lo.host);
    for (unsigned i = 0; i < sizeof values / sizeof values[0]; i++)
        printf("v%u %d\n", i, values[i]);
    printf("%.2f %.2f\n", scales[0], scales[1]);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut bittab: __SlateAlign16<[u32; 6]> = __SlateAlign16([
// CHECK-NEXT:     if 1i32 < 2i32 {
// CHECK-NEXT:         4294967295u32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (if 128i32 == 128i32 { 4294967168u32 } else { 4294967168u32 }) << 23i32 | 1u32
// CHECK-NEXT:     },
// CHECK-NEXT:     if 2i32 < 2i32 {
// CHECK-NEXT:         4294967295u32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (if 128i32 == 128i32 { 4294967168u32 } else { 4294967168u32 }) << 23i32 | 2u32
// CHECK-NEXT:     },
// CHECK-NEXT:     if 15i32 < 2i32 {
// CHECK-NEXT:         4294967295u32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (if 128i32 == 128i32 { 4294967168u32 } else { 4294967168u32 }) << 23i32 | 15u32
// CHECK-NEXT:     },
// CHECK-NEXT:     (if 0i32 == 0i32 {
// CHECK-NEXT:         (if 160i32 == 128i32 { 4294967168u32 } else { 4294967136u32 }) << 23i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if 0i32 == 13i32 {
// CHECK-NEXT:             (if 128i32 == 128i32 { 4294967200u32 } else { 4294967168u32 }) << 23i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             (if 128i32 == 128i32 { 4294967168u32 } else { 4294967168u32 }) << 23i32
// CHECK-NEXT:         }
// CHECK-NEXT:     }) | (if 128i32 == 128i32 { 4294967168u32 } else { 4294967168u32 }) << 23i32 >> 6i32
// CHECK-NEXT:         | 0u32,
// CHECK-NEXT:     (if 1i32 == 0i32 {
// CHECK-NEXT:         (if 160i32 == 128i32 { 4294967168u32 } else { 4294967136u32 }) << 23i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if 1i32 == 13i32 {
// CHECK-NEXT:             (if 128i32 == 128i32 { 4294967200u32 } else { 4294967168u32 }) << 23i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             (if 128i32 == 128i32 { 4294967168u32 } else { 4294967168u32 }) << 23i32
// CHECK-NEXT:         }
// CHECK-NEXT:     }) | (if 128i32 == 128i32 { 4294967168u32 } else { 4294967168u32 }) << 23i32 >> 6i32
// CHECK-NEXT:         | 1u32,
// CHECK-NEXT:     (if 13i32 == 0i32 {
// CHECK-NEXT:         (if 160i32 == 128i32 { 4294967168u32 } else { 4294967136u32 }) << 23i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if 13i32 == 13i32 {
// CHECK-NEXT:             (if 128i32 == 128i32 { 4294967200u32 } else { 4294967168u32 }) << 23i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             (if 128i32 == 128i32 { 4294967168u32 } else { 4294967168u32 }) << 23i32
// CHECK-NEXT:         }
// CHECK-NEXT:     }) | (if 128i32 == 128i32 { 4294967168u32 } else { 4294967168u32 }) << 23i32 >> 6i32
// CHECK-NEXT:         | 13u32,
// CHECK-NEXT: ]);
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct addr {
// CHECK-NEXT:     family: u16,
// CHECK-NEXT:     port: u16,
// CHECK-NEXT:     host: u32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut lo: addr = addr {
// CHECK-NEXT:     family: 2u16,
// CHECK-NEXT:     port: 65535u16,
// CHECK-NEXT:     host: (if 1234i32 == 4321i32 { 2130706433i32 } else { 16777343i32 }) as u32,
// CHECK-NEXT: };
// CHECK-NEXT: static mut values: [i16; 6] = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [i16; 6] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (if (std::mem::size_of::<i64>() as u64) > 4u64 { -1i32 } else { 1i32 })
// CHECK-NEXT:         as i16;
// CHECK-NEXT:     [[T_0]][3] = -254i16;
// CHECK-NEXT:     [[T_0]][5] = (if 100i32 > 50i32 { 100i32 } else { 50i32 }) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut scales: __SlateAlign16<[f64; 2]> = __SlateAlign16([
// CHECK-NEXT:     if 1i32 != 0i32 { 0.5f64 } else { 2.0f64 },
// CHECK-NEXT:     if 0i32 != 0i32 { 1.0f64 } else { 3.0f32 as f64 },
// CHECK-NEXT: ]);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: u32 = 0;
// CHECK-NEXT:         while (i as u64)
// CHECK-NEXT:             < (std::mem::size_of::<[u32; 6]>() as u64)
// CHECK-NEXT:                 / (std::mem::size_of::<u32>() as u64)
// CHECK-NEXT:         {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 slate_support::printf(c"%u %08x\n".as_ptr(), i, bittab.0[i as usize])
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%u %u %08x\n".as_ptr(),
// CHECK-NEXT:             lo.family as i32,
// CHECK-NEXT:             lo.port as i32,
// CHECK-NEXT:             lo.host,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64)
// CHECK-NEXT:         < (std::mem::size_of::<[i16; 6]>() as u64) / (std::mem::size_of::<i16>() as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(c"v%u %d\n".as_ptr(), i, values[i as usize] as i32)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%.2f %.2f\n".as_ptr(), scales.0[0], scales.0[1]) };
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
