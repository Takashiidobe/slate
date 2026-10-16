#include <stdio.h>

struct __attribute__((packed)) MixedBits {
  unsigned char tag;
  unsigned int  low       : 3;
  signed int    delta     : 6;
  unsigned int            : 0;
  unsigned long long wide : 35;
  unsigned int       tail : 17;
};

// @check-fn-begin
int main(void) {
  struct MixedBits bits = {0};
  bits.tag              = 0xa5;
  bits.low              = 7;
  bits.delta            = -17;
  bits.wide             = 0x712345678ULL;
  bits.tail             = 0x1abcd;
  printf("%u %u %d %llu %u %zu\n", bits.tag, bits.low, bits.delta, bits.wide,
         bits.tail, sizeof(bits));
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut bits: MixedBits = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: MixedBits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].tag = ((0 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     bits.tag = ((165 as i32) as i8) as u8;
// CHECK-NEXT:     bits.__slate_bits_0.__set_low((7 as i32) as u32);
// CHECK-NEXT:     bits.__slate_bits_0.__set_delta(-(17 as i32));
// CHECK-NEXT:     bits.__slate_bits_1.__set_wide(30370190968 as u64);
// CHECK-NEXT:     bits.__slate_bits_1.__set_tail((109517 as i32) as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %d %llu %u %zu\n".as_ptr() as *const i8,
// CHECK-NEXT:             (bits.tag as u32) as i32,
// CHECK-NEXT:             bits.__slate_bits_0.__get_low() as i32,
// CHECK-NEXT:             bits.__slate_bits_0.__get_delta(),
// CHECK-NEXT:             bits.__slate_bits_1.__get_wide(),
// CHECK-NEXT:             bits.__slate_bits_1.__get_tail() as i32,
// CHECK-NEXT:             11 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
