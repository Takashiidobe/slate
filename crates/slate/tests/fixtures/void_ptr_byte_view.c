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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn all_bytes_identical(src: *const (), size: u64) -> i32 {
// CHECK-NEXT:     let b: u8 = unsafe { *(src as *const u8).offset(0 as isize) };
// CHECK-NEXT:     for p in 1..size {
// CHECK-NEXT:         if (((unsafe { *(src as *const u8).offset(p as isize) }) as u32) as i32)
// CHECK-NEXT:             != ((b as u32) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK: {{^}}}
// SLATE-FILECHECK-END
