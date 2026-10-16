// @check-fn-begin
static int all_bytes_identical(const void *src, unsigned long size) {
  unsigned char b = ((const unsigned char *)src)[0];
  for (unsigned long p = 1; p < size; p++) {
    if (((const unsigned char *)src)[p] != b) {
      return 0;
    }
  }
  return 1;
}
// @check-fn-end

int main(void) {
  unsigned char buf[4] = {7, 7, 7, 7};
  return all_bytes_identical(buf, 4);
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn all_bytes_identical(mut src: *const (), mut size: u64) -> i32 {
// CHECK-NEXT:     let mut b: u8 = unsafe {
// CHECK-NEXT:         *unsafe { (src as *const u8).offset((0 as i32) as isize) }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut p: u64 = ((1 as i32) as i64) as u64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while p < size {
// CHECK-NEXT:         if (((unsafe { *unsafe { (src as *const u8).offset(p as isize) } }) as u32)
// CHECK-NEXT:             as i32) != ((b as u32) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u64 = p;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:         p = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
