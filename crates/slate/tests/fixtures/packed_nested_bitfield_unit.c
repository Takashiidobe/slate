#include <stdio.h>
#include <stdlib.h>

struct alt {
    char tag;
    int off;
    union {
        struct {
            unsigned cpuid : 16;
            unsigned flags : 16;
        };
        unsigned ft;
    };
    unsigned char len;
} __attribute__((packed));

static struct alt table[2] = {{1, 2, {{7, 9}}, 3}, {4, 5, {{11, 13}}, 6}};

static void mark(struct alt *a, unsigned bit) {
    a->flags |= bit;
    a->cpuid = a->cpuid + 1;
}

int main(void) {
    struct alt *heap = calloc(3, sizeof(struct alt));
    heap[1] = table[1];
    mark(&heap[1], 2);
    mark(&table[0], 4);
    struct alt local = table[1];
    local.flags = 21;
    printf("%zu %u %u %u %u %u %u %u\n", sizeof(struct alt), heap[1].cpuid, heap[1].flags,
           table[0].cpuid, table[0].flags, local.cpuid, local.flags, heap[1].ft);
    free(heap);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct alt {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     off: i32,
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: __SlateRecord_packed_nested_bitfield_unit_c_7,
// CHECK-NEXT:     len: u8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_packed_nested_bitfield_unit_c_7 {
// CHECK-NEXT:     [[SLATE_ANON_1:__slate_anon_[0-9]+]]: __SlateRecord_packed_nested_bitfield_unit_c_8,
// CHECK-NEXT:     ft: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_packed_nested_bitfield_unit_c_8 {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub cpuid: u32,
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub flags: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut table: __SlateAlign16<[alt; 2]> = __SlateAlign16([
// CHECK-NEXT:     alt {
// CHECK-NEXT:         tag: 1i8,
// CHECK-NEXT:         off: 2i32,
// CHECK-NEXT:         [[SLATE_ANON_0]]: {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: __SlateRecord_packed_nested_bitfield_unit_c_7 = unsafe {
// CHECK-NEXT:                 std::mem::zeroed()
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_0]].[[SLATE_ANON_1]] = {
// CHECK-NEXT:                 let mut [[T_1:__t[0-9]+]]: __SlateRecord_packed_nested_bitfield_unit_c_8 = unsafe {
// CHECK-NEXT:                     std::mem::zeroed()
// CHECK-NEXT:                 };
// CHECK-NEXT:                 [[T_1]].__slate_bits_0.__set_cpuid(7u32);
// CHECK-NEXT:                 [[T_1]].__slate_bits_0.__set_flags(9u32);
// CHECK-NEXT:                 [[T_1]]
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:         len: 3u8,
// CHECK-NEXT:     },
// CHECK-NEXT:     alt {
// CHECK-NEXT:         tag: 4i8,
// CHECK-NEXT:         off: 5i32,
// CHECK-NEXT:         [[SLATE_ANON_0]]: {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: __SlateRecord_packed_nested_bitfield_unit_c_7 = unsafe {
// CHECK-NEXT:                 std::mem::zeroed()
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_2]].[[SLATE_ANON_1]] = {
// CHECK-NEXT:                 let mut [[T_3:__t[0-9]+]]: __SlateRecord_packed_nested_bitfield_unit_c_8 = unsafe {
// CHECK-NEXT:                     std::mem::zeroed()
// CHECK-NEXT:                 };
// CHECK-NEXT:                 [[T_3]].__slate_bits_0.__set_cpuid(11u32);
// CHECK-NEXT:                 [[T_3]].__slate_bits_0.__set_flags(13u32);
// CHECK-NEXT:                 [[T_3]]
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:         len: 6u8,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: use slate_support::calloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: fn mark(a: *mut alt, bit: u32) {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u32 = unsafe {
// CHECK-NEXT:         std::ptr::read_unaligned(
// CHECK-NEXT:                 std::ptr::addr_of!((* a).[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0),
// CHECK-NEXT:             )
// CHECK-NEXT:             .__get_flags()
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[T_0]] = (([[V_0]] as i32) as u32) | bit;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_1]] = std::ptr::read_unaligned(
// CHECK-NEXT:             std::ptr::addr_of_mut!((* a).[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0),
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_1]].__set_flags([[T_0]]);
// CHECK-NEXT:         std::ptr::write_unaligned(
// CHECK-NEXT:             std::ptr::addr_of_mut!((* a).[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0),
// CHECK-NEXT:             [[T_1]],
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[T_2]] = (((unsafe {
// CHECK-NEXT:         std::ptr::read_unaligned(
// CHECK-NEXT:                 std::ptr::addr_of!((* a).[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0),
// CHECK-NEXT:             )
// CHECK-NEXT:             .__get_cpuid()
// CHECK-NEXT:     }) as i32) + 1) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_3]] = std::ptr::read_unaligned(
// CHECK-NEXT:             std::ptr::addr_of_mut!((* a).[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0),
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_3]].__set_cpuid([[T_2]]);
// CHECK-NEXT:         std::ptr::write_unaligned(
// CHECK-NEXT:             std::ptr::addr_of_mut!((* a).[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0),
// CHECK-NEXT:             [[T_3]],
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let heap: *mut alt = (unsafe {
// CHECK-NEXT:         calloc(3u64 as usize, (std::mem::size_of::<alt>() as u64) as usize) as *mut ()
// CHECK-NEXT:     }) as *mut alt;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *heap.add(1) = table.0[1];
// CHECK-NEXT:     }
// CHECK-NEXT:     mark(unsafe { heap.add(1) }, 2u32);
// CHECK-NEXT:     mark(unsafe { (std::ptr::addr_of_mut!(table.0) as *mut alt).add(0) }, 4u32);
// CHECK-NEXT:     let mut local: alt = unsafe { table.0[1] };
// CHECK-NEXT:     let [[T_0]] = 21u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_1]] = std::ptr::read_unaligned(
// CHECK-NEXT:             std::ptr::addr_of_mut!(local.[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0),
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_1]].__set_flags([[T_0]]);
// CHECK-NEXT:         std::ptr::write_unaligned(
// CHECK-NEXT:             std::ptr::addr_of_mut!(local.[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0),
// CHECK-NEXT:             [[T_1]],
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%zu %u %u %u %u %u %u %u\n".as_ptr(),
// CHECK-NEXT:             std::mem::size_of::<alt>() as u64,
// CHECK-NEXT:             std::ptr::read_unaligned(
// CHECK-NEXT:                     std::ptr::addr_of!(
// CHECK-NEXT:                         (* heap.add(1)).[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_cpuid() as i32,
// CHECK-NEXT:             std::ptr::read_unaligned(
// CHECK-NEXT:                     std::ptr::addr_of!(
// CHECK-NEXT:                         (* heap.add(1)).[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_flags() as i32,
// CHECK-NEXT:             std::ptr::read_unaligned(
// CHECK-NEXT:                     std::ptr::addr_of!(
// CHECK-NEXT:                         * std::ptr::addr_of!(table.0[0].[[SLATE_ANON_0]].[[SLATE_ANON_1]]
// CHECK-NEXT:                         .__slate_bits_0)
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_cpuid() as i32,
// CHECK-NEXT:             std::ptr::read_unaligned(
// CHECK-NEXT:                     std::ptr::addr_of!(
// CHECK-NEXT:                         * std::ptr::addr_of!(table.0[0].[[SLATE_ANON_0]].[[SLATE_ANON_1]]
// CHECK-NEXT:                         .__slate_bits_0)
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_flags() as i32,
// CHECK-NEXT:             std::ptr::read_unaligned(
// CHECK-NEXT:                     std::ptr::addr_of!(
// CHECK-NEXT:                         local.[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_cpuid() as i32,
// CHECK-NEXT:             std::ptr::read_unaligned(
// CHECK-NEXT:                     std::ptr::addr_of!(
// CHECK-NEXT:                         local.[[SLATE_ANON_0]].[[SLATE_ANON_1]].__slate_bits_0
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_flags() as i32,
// CHECK-NEXT:             (*heap.add(1)).[[SLATE_ANON_0]].ft,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { free((heap as *mut ()) as *mut core::ffi::c_void) };
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
