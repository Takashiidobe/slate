#include <stdio.h>
#include <string.h>

typedef unsigned int u32;
typedef unsigned char u8;

typedef struct {
    u8 b[16];
} guid_t;
typedef guid_t efi_guid_t __attribute__((aligned(__alignof__(u32))));

#define EFI_GUID(a, b, c, d...)                                                \
    ((efi_guid_t){{(a) & 0xff, ((a) >> 8) & 0xff, ((a) >> 16) & 0xff,          \
                   ((a) >> 24) & 0xff, (b) & 0xff, ((b) >> 8) & 0xff,          \
                   (c) & 0xff, ((c) >> 8) & 0xff, d}})

typedef struct msi_data {
    union {
        struct {
            u32 vector : 8, delivery_mode : 3, dest_mode_logical : 1,
                reserved : 2, active_low : 1, is_level : 1;
        };
        u32 subhandle;
    };
} __attribute__((packed)) msi_data_t;

struct msi_msg {
    union {
        u32 address_lo;
    };
    union {
        u32 data;
        msi_data_t arch_data;
    };
};

struct unaligned_msg {
    u8 tag;
    msi_data_t data;
} __attribute__((packed));

static int guid_equal(efi_guid_t a, efi_guid_t b) {
    return memcmp(&a, &b, sizeof(a)) == 0;
}

static void compose(struct msi_msg *msg, u32 vector) {
    msg->arch_data.vector = vector;
    msg->arch_data.delivery_mode = 5;
    msg->arch_data.is_level = 1;
}

int main(void) {
    efi_guid_t guid = EFI_GUID(0x12345678, 0x9abc, 0xdef0, 1, 2, 3, 4, 5, 6, 7, 8);
    printf("guid %d %d\n",
           guid_equal(guid, EFI_GUID(0x12345678, 0x9abc, 0xdef0, 1, 2, 3, 4, 5, 6, 7, 8)),
           guid_equal(guid, EFI_GUID(0x12345678, 0, 0, 0)));
    printf("guid bytes %x %x %x\n", guid.b[0], guid.b[5], guid.b[15]);

    struct msi_msg msg = {0};
    compose(&msg, 0x31);
    struct msi_msg copy = msg;
    printf("msg %x %u %u %u\n", msg.data, copy.arch_data.vector,
           copy.arch_data.delivery_mode, copy.arch_data.is_level);
    copy.arch_data.subhandle = 0xabcd;
    printf("subhandle %x %u\n", copy.data, copy.arch_data.vector);

    struct unaligned_msg packed[2];
    memset(packed, 0, sizeof(packed));
    packed[1].data.vector = 0x7f;
    packed[1].data.active_low = 1;
    packed[1].data.vector += 1;
    printf("packed %x %u %u\n", packed[1].data.subhandle, packed[1].data.vector,
           packed[1].data.active_low);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_packed_union_bitfield_and_aligned_literal_c_9 {
// CHECK-NEXT:     b: [u8; 16],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct msi_data {
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: core::mem::MaybeUninit<[u8; 4]>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25 {
// CHECK-NEXT:     [[SLATE_ANON_0]]: __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25_2,
// CHECK-NEXT:     subhandle: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25_2 {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits8U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct msi_msg {
// CHECK-NEXT:     [[SLATE_ANON_0]]: __SlateRecord_packed_union_bitfield_and_aligned_literal_c_28,
// CHECK-NEXT:     [[SLATE_ANON_1:__slate_anon_[0-9]+]]: __SlateRecord_packed_union_bitfield_and_aligned_literal_c_31,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_packed_union_bitfield_and_aligned_literal_c_28 {
// CHECK-NEXT:     address_lo: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_packed_union_bitfield_and_aligned_literal_c_31 {
// CHECK-NEXT:     data: u32,
// CHECK-NEXT:     arch_data: msi_data,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct unaligned_msg {
// CHECK-NEXT:     tag: u8,
// CHECK-NEXT:     data: msi_data,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: struct __SlateAlign4<T>(T);
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits8U0 {
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub vector: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub delivery_mode: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub dest_mode_logical: u32,
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub reserved: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub active_low: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub is_level: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::memset;
// CHECK-NEXT: use slate_support::memcmp;
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn guid_equal(
// CHECK-NEXT:     mut a: __SlateRecord_packed_union_bitfield_and_aligned_literal_c_9,
// CHECK-NEXT:     mut b: __SlateRecord_packed_union_bitfield_and_aligned_literal_c_9,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             (core::ptr::addr_of_mut!(a) as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:             (core::ptr::addr_of_mut!(b) as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:             16u64 as usize,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     }) == 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn compose(msg: &mut msi_msg, vector: u32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = vector;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = core::ptr::addr_of_mut!(
// CHECK-NEXT:             (* (msg as * mut msi_msg)).[[SLATE_ANON_1]].arch_data.[[SLATE_ANON_0]]
// CHECK-NEXT:         ) as *mut __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25;
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]] = [[T_1]].read_unaligned();
// CHECK-NEXT:         [[T_2]].[[SLATE_ANON_0]].__slate_bits_0.__set_vector([[T_0]]);
// CHECK-NEXT:         [[T_1]].write_unaligned([[T_2]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = 5u32;
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = core::ptr::addr_of_mut!(
// CHECK-NEXT:             (* (msg as * mut msi_msg)).[[SLATE_ANON_1]].arch_data.[[SLATE_ANON_0]]
// CHECK-NEXT:         ) as *mut __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25;
// CHECK-NEXT:         let mut [[T_5:__t[0-9]+]] = [[T_4]].read_unaligned();
// CHECK-NEXT:         [[T_5]].[[SLATE_ANON_0]].__slate_bits_0.__set_delivery_mode([[T_3]]);
// CHECK-NEXT:         [[T_4]].write_unaligned([[T_5]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_6:__t[0-9]+]] = 1u32;
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = core::ptr::addr_of_mut!(
// CHECK-NEXT:             (* (msg as * mut msi_msg)).[[SLATE_ANON_1]].arch_data.[[SLATE_ANON_0]]
// CHECK-NEXT:         ) as *mut __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25;
// CHECK-NEXT:         let mut [[T_8:__t[0-9]+]] = [[T_7]].read_unaligned();
// CHECK-NEXT:         [[T_8]].[[SLATE_ANON_0]].__slate_bits_0.__set_is_level([[T_6]]);
// CHECK-NEXT:         [[T_7]].write_unaligned([[T_8]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: __SlateAlign4<
// CHECK-NEXT:         __SlateRecord_packed_union_bitfield_and_aligned_literal_c_9,
// CHECK-NEXT:     > = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: __SlateAlign4<
// CHECK-NEXT:         __SlateRecord_packed_union_bitfield_and_aligned_literal_c_9,
// CHECK-NEXT:     > = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: __SlateAlign4<
// CHECK-NEXT:         __SlateRecord_packed_union_bitfield_and_aligned_literal_c_9,
// CHECK-NEXT:     > = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let guid: __SlateAlign4<
// CHECK-NEXT:         __SlateRecord_packed_union_bitfield_and_aligned_literal_c_9,
// CHECK-NEXT:     > = __SlateAlign4(unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]].0 = __SlateRecord_packed_union_bitfield_and_aligned_literal_c_9 {
// CHECK-NEXT:                 b: [
// CHECK-NEXT:                     120u8, 86u8, 52u8, 18u8, 188u8, 154u8, 240u8, 222u8, 1, 2, 3, 4, 5,
// CHECK-NEXT:                     6, 7, 8,
// CHECK-NEXT:                 ],
// CHECK-NEXT:             };
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]].0)
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "guid {} {}", guid_equal(guid.0, unsafe { * { [[SLATE_COMPOUND_1]].0 =
// CHECK-NEXT:         __SlateRecord_packed_union_bitfield_and_aligned_literal_c_9 { b : [120u8, 86u8,
// CHECK-NEXT:         52u8, 18u8, 188u8, 154u8, 240u8, 222u8, 1, 2, 3, 4, 5, 6, 7, 8] };
// CHECK-NEXT:         core::ptr::addr_of_mut!([[SLATE_COMPOUND_1]].0) } }), guid_equal(guid.0, unsafe { *
// CHECK-NEXT:         { [[SLATE_COMPOUND_2]].0 =
// CHECK-NEXT:         __SlateRecord_packed_union_bitfield_and_aligned_literal_c_9 { b : { let mut [[T_0]]
// CHECK-NEXT:         : [u8; 16] = core::mem::zeroed(); [[T_0]][0] = 120u8; [[T_0]][1] = 86u8; [[T_0]][2] =
// CHECK-NEXT:         52u8; [[T_0]][3] = 18u8; [[T_0]][4] = 0u8; [[T_0]][5] = 0u8; [[T_0]][6] = 0u8; [[T_0]][7] = 0u8;
// CHECK-NEXT:         [[T_0]][8] = 0u8; [[T_0]] } }; core::ptr::addr_of_mut!([[SLATE_COMPOUND_2]].0) } })
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"guid bytes %x %x %x\n".as_ptr(),
// CHECK-NEXT:             guid.0.b[0] as i32,
// CHECK-NEXT:             guid.0.b[5] as i32,
// CHECK-NEXT:             guid.0.b[15] as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut msg: msi_msg = {
// CHECK-NEXT:         let mut [[T_1]]: msi_msg = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].[[SLATE_ANON_0]] = __SlateRecord_packed_union_bitfield_and_aligned_literal_c_28 {
// CHECK-NEXT:             address_lo: 0,
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     compose(unsafe { &mut (*core::ptr::addr_of_mut!(msg)) }, 49u32);
// CHECK-NEXT:     let mut copy: msi_msg = msg;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"msg %x %u %u %u\n".as_ptr(),
// CHECK-NEXT:             msg.[[SLATE_ANON_1]].data,
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_2]] = (core::ptr::addr_of!(
// CHECK-NEXT:                     copy.[[SLATE_ANON_1]].arch_data.[[SLATE_ANON_0]]
// CHECK-NEXT:                 ) as *const __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25)
// CHECK-NEXT:                     .read_unaligned();
// CHECK-NEXT:                 [[T_2]].[[SLATE_ANON_0]].__slate_bits_0.__get_vector()
// CHECK-NEXT:             }) as i32,
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_3]] = (core::ptr::addr_of!(
// CHECK-NEXT:                     copy.[[SLATE_ANON_1]].arch_data.[[SLATE_ANON_0]]
// CHECK-NEXT:                 ) as *const __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25)
// CHECK-NEXT:                     .read_unaligned();
// CHECK-NEXT:                 [[T_3]].[[SLATE_ANON_0]].__slate_bits_0.__get_delivery_mode()
// CHECK-NEXT:             }) as i32,
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_4]] = (core::ptr::addr_of!(
// CHECK-NEXT:                     copy.[[SLATE_ANON_1]].arch_data.[[SLATE_ANON_0]]
// CHECK-NEXT:                 ) as *const __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25)
// CHECK-NEXT:                     .read_unaligned();
// CHECK-NEXT:                 [[T_4]].[[SLATE_ANON_0]].__slate_bits_0.__get_is_level()
// CHECK-NEXT:             }) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_5]] = 43981u32;
// CHECK-NEXT:         let [[T_6]] = core::ptr::addr_of_mut!(copy.[[SLATE_ANON_1]].arch_data.[[SLATE_ANON_0]])
// CHECK-NEXT:             as *mut __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25;
// CHECK-NEXT:         let mut [[T_7]] = [[T_6]].read_unaligned();
// CHECK-NEXT:         [[T_7]].subhandle = [[T_5]];
// CHECK-NEXT:         [[T_6]].write_unaligned([[T_7]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"subhandle %x %u\n".as_ptr(),
// CHECK-NEXT:             copy.[[SLATE_ANON_1]].data,
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_8]] = (core::ptr::addr_of!(
// CHECK-NEXT:                     copy.[[SLATE_ANON_1]].arch_data.[[SLATE_ANON_0]]
// CHECK-NEXT:                 ) as *const __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25)
// CHECK-NEXT:                     .read_unaligned();
// CHECK-NEXT:                 [[T_8]].[[SLATE_ANON_0]].__slate_bits_0.__get_vector()
// CHECK-NEXT:             }) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut packed: [unaligned_msg; 2] = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write_bytes(
// CHECK-NEXT:             ((packed.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (0i32 as i32) as u8,
// CHECK-NEXT:             ((core::mem::size_of::<[unaligned_msg; 2]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_9:__t[0-9]+]] = 127u32;
// CHECK-NEXT:         let [[T_10:__t[0-9]+]] = core::ptr::addr_of_mut!(packed[1].data.[[SLATE_ANON_0]])
// CHECK-NEXT:             as *mut __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25;
// CHECK-NEXT:         let mut [[T_11:__t[0-9]+]] = [[T_10]].read_unaligned();
// CHECK-NEXT:         [[T_11]].[[SLATE_ANON_0]].__slate_bits_0.__set_vector([[T_9]]);
// CHECK-NEXT:         [[T_10]].write_unaligned([[T_11]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_12:__t[0-9]+]] = 1u32;
// CHECK-NEXT:         let [[T_13:__t[0-9]+]] = core::ptr::addr_of_mut!(packed[1].data.[[SLATE_ANON_0]])
// CHECK-NEXT:             as *mut __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25;
// CHECK-NEXT:         let mut [[T_14:__t[0-9]+]] = [[T_13]].read_unaligned();
// CHECK-NEXT:         [[T_14]].[[SLATE_ANON_0]].__slate_bits_0.__set_active_low([[T_12]]);
// CHECK-NEXT:         [[T_13]].write_unaligned([[T_14]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut unaligned_msg = unsafe {
// CHECK-NEXT:         (packed.as_mut_ptr() as *mut unaligned_msg).add(1)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u32 = unsafe {
// CHECK-NEXT:         let [[T_15:__t[0-9]+]] = (core::ptr::addr_of!((* [[V_0]]).data.[[SLATE_ANON_0]])
// CHECK-NEXT:             as *const __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25)
// CHECK-NEXT:             .read_unaligned();
// CHECK-NEXT:         [[T_15]].[[SLATE_ANON_0]].__slate_bits_0.__get_vector()
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: u32 = (([[V_1]] as i32) + 1) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_16:__t[0-9]+]] = [[V_2]];
// CHECK-NEXT:         let [[T_17:__t[0-9]+]] = core::ptr::addr_of_mut!((* [[V_0]]).data.[[SLATE_ANON_0]])
// CHECK-NEXT:             as *mut __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25;
// CHECK-NEXT:         let mut [[T_18:__t[0-9]+]] = [[T_17]].read_unaligned();
// CHECK-NEXT:         [[T_18]].[[SLATE_ANON_0]].__slate_bits_0.__set_vector([[T_16]]);
// CHECK-NEXT:         [[T_17]].write_unaligned([[T_18]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"packed %x %u %u\n".as_ptr(),
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_19:__t[0-9]+]] = (core::ptr::addr_of!(packed[1].data.[[SLATE_ANON_0]])
// CHECK-NEXT:                     as *const __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25)
// CHECK-NEXT:                     .read_unaligned();
// CHECK-NEXT:                 [[T_19]].subhandle
// CHECK-NEXT:             },
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_20:__t[0-9]+]] = (core::ptr::addr_of!(packed[1].data.[[SLATE_ANON_0]])
// CHECK-NEXT:                     as *const __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25)
// CHECK-NEXT:                     .read_unaligned();
// CHECK-NEXT:                 [[T_20]].[[SLATE_ANON_0]].__slate_bits_0.__get_vector()
// CHECK-NEXT:             }) as i32,
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_21:__t[0-9]+]] = (core::ptr::addr_of!(packed[1].data.[[SLATE_ANON_0]])
// CHECK-NEXT:                     as *const __SlateRecord_packed_union_bitfield_and_aligned_literal_c_25)
// CHECK-NEXT:                     .read_unaligned();
// CHECK-NEXT:                 [[T_21]].[[SLATE_ANON_0]].__slate_bits_0.__get_active_low()
// CHECK-NEXT:             }) as i32,
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
