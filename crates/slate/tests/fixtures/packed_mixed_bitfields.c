#include <stdio.h>

struct __attribute__((packed)) MixedBits {
  unsigned char tag;
  unsigned int  low       : 3;
  signed int    delta     : 6;
  unsigned int            : 0;
  unsigned long long wide : 35;
  unsigned int       tail : 17;
};

int main(void) {
  struct MixedBits bits = {0};
  bits.tag              = 0xa5;
  // @lowering-begin
  // @rewrite-begin
  bits.low              = 7;
  bits.delta            = -17;
  bits.wide             = 0x712345678ULL;
  bits.tail             = 0x1abcd;
  // @rewrite-end
  // @lowering-end
  // @lowering-begin
  // @rewrite-begin
  printf("%u %u %d %llu %u %zu\n", bits.tag, bits.low, bits.delta, bits.wide,
         bits.tail, sizeof(bits));
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u32 = 7;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 29 >> 29;
// COMMON-LOWERING-DAG: let mut {{__v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) };
// COMMON-LOWERING-DAG: {{__v[0-9]+}}.set_low(({{__v[0-9]+}} as u32) << 29 >> 29);
// COMMON-LOWERING-DAG: unsafe {
// COMMON-LOWERING-DAG:     std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_1), {{__v[0-9]+}});
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = -17;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 26 >> 26;
// COMMON-LOWERING-DAG: let mut {{__v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) };
// COMMON-LOWERING-DAG: {{__v[0-9]+}}.set_delta(({{__v[0-9]+}} as i32) << 26 >> 26);
// COMMON-LOWERING-DAG: unsafe {
// COMMON-LOWERING-DAG:     std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_1), {{__v[0-9]+}});
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = 30370190968u64;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = ({{__v[0-9]+}} as u64) << 29 >> 29;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u32 = 109517;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 15 >> 15;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u8 = bits.__bitfield_0;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u32 = (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) }
// COMMON-LOWERING-DAG:     .low() as u32)
// COMMON-LOWERING-DAG:     << 29
// COMMON-LOWERING-DAG:     >> 29;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) }
// COMMON-LOWERING-DAG:     .delta() as i32)
// COMMON-LOWERING-DAG:     << 26
// COMMON-LOWERING-DAG:     >> 26;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:     printf(
// COMMON-LOWERING-DAG:         {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:     )
// COMMON-LOWERING-DAG: };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: bits.__bitfield_4.set_wide(({{__v[0-9]+}} as u64) << 29 >> 29);
// LOWERING-X86_64-GNU-DAG: bits.__bitfield_4.set_tail(({{__v[0-9]+}} as u32) << 15 >> 15);
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%u %u %d %llu %u %zu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u64 = (bits.__bitfield_4.wide() as u64) << 29 >> 29;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32 = (bits.__bitfield_4.tail() as u32) << 15 >> 15;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u64 = 11;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let mut {{__v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_4)) };
// LOWERING-AARCH64-GNU-DAG: {{__v[0-9]+}}.set_wide(({{__v[0-9]+}} as u64) << 29 >> 29);
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_4), {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG: }
// LOWERING-AARCH64-GNU-DAG: let mut {{__v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_4)) };
// LOWERING-AARCH64-GNU-DAG: {{__v[0-9]+}}.set_tail(({{__v[0-9]+}} as u32) << 15 >> 15);
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_4), {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG: }
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%u %u %d %llu %u %zu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u64 = (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_4)) }
// LOWERING-AARCH64-GNU-DAG:     .wide() as u64)
// LOWERING-AARCH64-GNU-DAG:     << 29
// LOWERING-AARCH64-GNU-DAG:     >> 29;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u32 = (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_4)) }
// LOWERING-AARCH64-GNU-DAG:     .tail() as u32)
// LOWERING-AARCH64-GNU-DAG:     << 15
// LOWERING-AARCH64-GNU-DAG:     >> 15;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u64 = 12;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: let mut {{__v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) };
// COMMON-REWRITES-DAG: {{__v[0-9]+}}.set_low((7 as u32) << 29 >> 29);
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_1), {{__v[0-9]+}});
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: let mut {{__v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) };
// COMMON-REWRITES-DAG: {{__v[0-9]+}}.set_delta((-17 as i32) << 26 >> 26);
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_1), {{__v[0-9]+}});
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: u64 = 30370190968u64;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = bits.__bitfield_0 as i32;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: u32 = (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) }
// COMMON-REWRITES-DAG:     .low() as u32)
// COMMON-REWRITES-DAG:     << 29
// COMMON-REWRITES-DAG:     >> 29;
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     printf(
// COMMON-REWRITES-DAG:         {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-DAG:         {{__v[0-9]+}},
// COMMON-REWRITES-DAG:         {{__v[0-9]+}} as i32,
// COMMON-REWRITES-DAG:         (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) }.delta()
// COMMON-REWRITES-DAG:             as i32)
// COMMON-REWRITES-DAG:             << 26
// COMMON-REWRITES-DAG:             >> 26,
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: bits.__bitfield_4.set_wide(({{__v[0-9]+}} as u64) << 29 >> 29);
// REWRITES-X86_64-GNU-DAG: bits.__bitfield_4.set_tail((109517 as u32) << 15 >> 15);
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = c"%u %u %d %llu %u %zu\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG:         (bits.__bitfield_4.wide() as u64) << 29 >> 29,
// REWRITES-X86_64-GNU-DAG:         ((bits.__bitfield_4.tail() as u32) << 15 >> 15) as i32,
// REWRITES-X86_64-GNU-DAG:         11 as u64,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: let mut {{__v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_4)) };
// REWRITES-AARCH64-GNU-DAG: {{__v[0-9]+}}.set_wide(({{__v[0-9]+}} as u64) << 29 >> 29);
// REWRITES-AARCH64-GNU-DAG:     std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_4), {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG: }
// REWRITES-AARCH64-GNU-DAG: let mut {{__v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_4)) };
// REWRITES-AARCH64-GNU-DAG: {{__v[0-9]+}}.set_tail((109517 as u32) << 15 >> 15);
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_4), {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG: }
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = c"%u %u %d %llu %u %zu\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:         (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_4)) }.wide()
// REWRITES-AARCH64-GNU-DAG:             as u64)
// REWRITES-AARCH64-GNU-DAG:             << 29
// REWRITES-AARCH64-GNU-DAG:             >> 29,
// REWRITES-AARCH64-GNU-DAG:         ((unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_4)) }.tail()
// REWRITES-AARCH64-GNU-DAG:             as u32)
// REWRITES-AARCH64-GNU-DAG:             << 15
// REWRITES-AARCH64-GNU-DAG:             >> 15) as i32,
// REWRITES-AARCH64-GNU-DAG:         12 as u64,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
