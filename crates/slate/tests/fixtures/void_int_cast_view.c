// @check-fn-begin
static int sum_ints(const void *src, unsigned long n) {
  int total = 0;
  for (unsigned long i = 0; i < n; i++) {
    total += ((const int *)src)[i];
  }
  return total;
}
// @check-fn-end

int main(void) {
  int buf[4] = {1, 2, 3, 4};
  return sum_ints(buf, 4);
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn sum_ints(src: *const (), n: u64) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         total += unsafe { *unsafe { (src as *const i32).offset(i as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK: {{^}}}
// SLATE-FILECHECK-END
