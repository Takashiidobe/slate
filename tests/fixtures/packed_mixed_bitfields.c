#include <stdio.h>

struct __attribute__((packed)) MixedBits {
  unsigned char tag;
  unsigned int low : 3;
  signed int delta : 6;
  unsigned int : 0;
  unsigned long long wide : 35;
  unsigned int tail : 17;
};

int main(void) {
  struct MixedBits bits = {0};
  bits.tag = 0xa5;
  // @lowering-begin
  // @rewrite-begin
  bits.low = 7;
  bits.delta = -17;
  bits.wide = 0x712345678ULL;
  bits.tail = 0x1abcd;
  // @rewrite-end
  // @lowering-end
  // @lowering-begin
  // @rewrite-begin
  printf("%u %u %d %llu %u %zu\n", bits.tag, bits.low, bits.delta,
         bits.wide, bits.tail, sizeof(bits));
  // @rewrite-end
  // @lowering-end
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 7;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 29 >> 29;
// LOWERING-DAG: let mut {{_v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) };
// LOWERING-DAG: {{_v[0-9]+}}.set_low(({{_v[0-9]+}} as u32) << 29 >> 29);
// LOWERING-DAG: unsafe {
// LOWERING-DAG: std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_1), {{_v[0-9]+}});
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = -17;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 26 >> 26;
// LOWERING-DAG: let mut {{_v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) };
// LOWERING-DAG: {{_v[0-9]+}}.set_delta(({{_v[0-9]+}} as i32) << 26 >> 26);
// LOWERING-DAG: unsafe {
// LOWERING-DAG: std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_1), {{_v[0-9]+}});
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 30370190968u64;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = ({{_v[0-9]+}} as u64) << 29 >> 29;
// LOWERING-DAG: bits.__bitfield_3.set_wide(({{_v[0-9]+}} as u64) << 29 >> 29);
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 109517;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 15 >> 15;
// LOWERING-DAG: bits.__bitfield_3.set_tail(({{_v[0-9]+}} as u32) << 15 >> 15);
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%u %u %d %llu %u %zu\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u8 = bits.__bitfield_0;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) }.low() as u32) << 29 >> 29;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) }.delta() as i32) << 26 >> 26;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = (bits.__bitfield_3.wide() as u64) << 29 >> 29;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (bits.__bitfield_3.tail() as u32) << 15 >> 15;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 11;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = 7;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 29 >> 29;
// REWRITES-DAG: let mut {{_v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) };
// REWRITES-DAG: {{_v[0-9]+}}.set_low(({{_v[0-9]+}} as u32) << 29 >> 29);
// REWRITES-DAG: unsafe {
// REWRITES-DAG: std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_1), {{_v[0-9]+}});
// REWRITES-DAG: }
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = -17;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 26 >> 26;
// REWRITES-DAG: let mut {{_v[0-9]+}} = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) };
// REWRITES-DAG: {{_v[0-9]+}}.set_delta(({{_v[0-9]+}} as i32) << 26 >> 26);
// REWRITES-DAG: unsafe {
// REWRITES-DAG: std::ptr::write_unaligned(std::ptr::addr_of_mut!(bits.__bitfield_1), {{_v[0-9]+}});
// REWRITES-DAG: }
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 30370190968u64;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = ({{_v[0-9]+}} as u64) << 29 >> 29;
// REWRITES-DAG: bits.__bitfield_3.set_wide(({{_v[0-9]+}} as u64) << 29 >> 29);
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = 109517;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 15 >> 15;
// REWRITES-DAG: bits.__bitfield_3.set_tail(({{_v[0-9]+}} as u32) << 15 >> 15);
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%u %u %d %llu %u %zu\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = bits.__bitfield_0 as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) }.low() as u32) << 29 >> 29;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(bits.__bitfield_1)) }.delta() as i32) << 26 >> 26;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = (bits.__bitfield_3.wide() as u64) << 29 >> 29;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = (bits.__bitfield_3.tail() as u32) << 15 >> 15;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 11;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}} as i32, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
