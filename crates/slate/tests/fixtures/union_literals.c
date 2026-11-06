#include <stdio.h>
#include <string.h>

union same_size {
    unsigned short freq;
    unsigned short code;
};

union second_member {
    float real;
    unsigned int bits;
};

union pointer_or_int {
    const char *text;
    char bytes[sizeof(char *)];
};

union array_member {
    char text[4];
    int word;
};

union partial {
    char small;
    long long large;
};

struct padded {
    char tag;
    int value;
};

union record_member {
    struct padded record;
    unsigned char bytes[sizeof(struct padded)];
};

union long_double_member {
    long double wide;
    unsigned char bytes[sizeof(long double)];
};

union bit_field_member {
    unsigned int bits : 3;
    unsigned int word;
};

struct entry {
    union same_size fc;
    union same_size dl;
};

static const struct entry table[3] = {{{12}, {8}}, {{140}, {8}}, {{.code = 76}, {.code = 9}}};
static const union second_member second = {.bits = 0x3f800000u};
static const union pointer_or_int pointer = {"text"};
static const union array_member array = {"abc"};
static const union partial partial_static = {'x'};
static const union record_member record_static = {{'r', 7}};
static const union long_double_member wide_static = {1.5L};
static const union bit_field_member bit_static = {5};

static int zero_after(const void *object, size_t start, size_t size) {
    const unsigned char *bytes = object;
    for (size_t i = start; i < size; i++) {
        if (bytes[i] != 0) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    for (int i = 0; i < 3; i++) {
        printf("%d %d\n", table[i].fc.freq, table[i].dl.code);
    }
    printf("%f %s %s\n", second.real, pointer.text, array.text);
    printf("%d %d\n", zero_after(&partial_static, 1, sizeof partial_static), partial_static.small);
    printf("%d %d\n", record_static.record.tag, record_static.record.value);
    printf("%d %d\n", zero_after(&record_static, 1, 4), (int)wide_static.wide);
    printf("%u\n", bit_static.word);

    int seed = 3;
    union same_size local = {(unsigned short)(seed * 2)};
    union partial local_partial = {(char)seed};
    union array_member local_array = {{'a', (char)('a' + seed)}};
    printf("%d %d %d\n", local.code, zero_after(&local_partial, 1, sizeof local_partial), local_partial.small);
    printf("%s %d\n", local_array.text, zero_after(&local_array, 2, sizeof local_array));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_fma(
// CHECK-NEXT:         __a: LongDouble,
// CHECK-NEXT:         __b: LongDouble,
// CHECK-NEXT:         __c: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// CHECK-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// CHECK-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// CHECK-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// CHECK-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// CHECK-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// CHECK-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// CHECK-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// CHECK-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// CHECK-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// CHECK-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialOrd for LongDouble {
// CHECK-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// CHECK-NEXT:         if __slate_f80_lt(*self, *__other) {
// CHECK-NEXT:             Some(std::cmp::Ordering::Less)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_gt(*self, *__other) {
// CHECK-NEXT:                 Some(std::cmp::Ordering::Greater)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_eq(*self, *__other) {
// CHECK-NEXT:                     Some(std::cmp::Ordering::Equal)
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     None
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union same_size {
// CHECK-NEXT:     freq: u16,
// CHECK-NEXT:     code: u16,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union second_member {
// CHECK-NEXT:     real: f32,
// CHECK-NEXT:     bits: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union pointer_or_int {
// CHECK-NEXT:     text: *const i8,
// CHECK-NEXT:     bytes: [i8; 8],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union array_member {
// CHECK-NEXT:     text: [i8; 4],
// CHECK-NEXT:     word: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union partial {
// CHECK-NEXT:     small: i8,
// CHECK-NEXT:     large: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct padded {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union record_member {
// CHECK-NEXT:     record: padded,
// CHECK-NEXT:     bytes: [u8; 8],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union long_double_member {
// CHECK-NEXT:     wide: LongDouble,
// CHECK-NEXT:     bytes: [u8; 16],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union bit_field_member {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits9U0,
// CHECK-NEXT:     word: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct entry {
// CHECK-NEXT:     fc: same_size,
// CHECK-NEXT:     dl: same_size,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits9U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub bits: u32,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut table: [entry; 3] = [
// CHECK-NEXT:     entry {
// CHECK-NEXT:         fc: same_size { freq: 12u16 },
// CHECK-NEXT:         dl: same_size { freq: 8u16 },
// CHECK-NEXT:     },
// CHECK-NEXT:     entry {
// CHECK-NEXT:         fc: same_size { freq: 140u16 },
// CHECK-NEXT:         dl: same_size { freq: 8u16 },
// CHECK-NEXT:     },
// CHECK-NEXT:     entry {
// CHECK-NEXT:         fc: same_size { code: 76u16 },
// CHECK-NEXT:         dl: same_size { code: 9u16 },
// CHECK-NEXT:     },
// CHECK-NEXT: ];
// CHECK-NEXT: static mut second: second_member = second_member {
// CHECK-NEXT:     bits: 1065353216u32,
// CHECK-NEXT: };
// CHECK-NEXT: static mut pointer: pointer_or_int = pointer_or_int {
// CHECK-NEXT:     text: (b"text\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT: };
// CHECK-NEXT: static mut array: array_member = array_member {
// CHECK-NEXT:     text: [97i8, 98i8, 99i8, 0i8],
// CHECK-NEXT: };
// CHECK-NEXT: static mut partial_static: partial = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: partial = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].small = 120i8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut record_static: record_member = {
// CHECK-NEXT:     let mut [[T_0]]: record_member = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].record = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].tag = 114i8;
// CHECK-NEXT:         [[T_1]].value = 7i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut wide_static: long_double_member = {
// CHECK-NEXT:     let mut [[T_0]]: long_double_member = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].wide = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut bit_static: bit_field_member = {
// CHECK-NEXT:     let mut [[T_0]]: bit_field_member = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_bits(5u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn zero_after(object: *const (), start: u64, size: u64) -> i32 {
// CHECK-NEXT:     let bytes: *const u8 = object as *const u8;
// CHECK-NEXT:     for i in start..size {
// CHECK-NEXT:         if ((unsafe { *bytes.add(i as usize) }) as i32) != 0 {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}", (unsafe { table[i as usize].fc.freq }) as i32, (unsafe { table[i as
// CHECK-NEXT:             usize].dl.code }) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%f %s %s\n".as_ptr(),
// CHECK-NEXT:             second.real as f64,
// CHECK-NEXT:             pointer.text,
// CHECK-NEXT:             std::ptr::addr_of!(array.text) as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", zero_after((unsafe { std::ptr::addr_of!(partial_static) }) as * const
// CHECK-NEXT:         (), 1u64, std::mem::size_of::< partial > () as u64), (unsafe { partial_static
// CHECK-NEXT:         .small }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", (unsafe { record_static.record.tag }) as i32, unsafe { record_static
// CHECK-NEXT:         .record.value }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", zero_after((unsafe { std::ptr::addr_of!(record_static) }) as * const (),
// CHECK-NEXT:         1u64, 4u64), __slate_f80_to_i32(unsafe { wide_static.wide })
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { slate_support::printf(c"%u\n".as_ptr(), bit_static.word) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let seed: i32 = 3;
// CHECK-NEXT:     let local: same_size = same_size {
// CHECK-NEXT:         freq: (seed * 2) as u16,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut local_partial: partial = {
// CHECK-NEXT:         let mut [[T_0]]: partial = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].small = seed as i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut local_array: array_member = array_member {
// CHECK-NEXT:         text: {
// CHECK-NEXT:             let mut [[T_1]]: [i8; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]][0] = 97i8;
// CHECK-NEXT:             [[T_1]][1] = (97 + seed) as i8;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", (unsafe { local.code }) as i32,
// CHECK-NEXT:         zero_after(std::ptr::addr_of_mut!(local_partial) as * const (), 1u64,
// CHECK-NEXT:         std::mem::size_of::< partial > () as u64), (unsafe { local_partial.small }) as
// CHECK-NEXT:         i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s %d\n".as_ptr(),
// CHECK-NEXT:             local_array.text.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             zero_after(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(local_array) as *const (),
// CHECK-NEXT:                 2u64,
// CHECK-NEXT:                 std::mem::size_of::<array_member>() as u64,
// CHECK-NEXT:             ),
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
