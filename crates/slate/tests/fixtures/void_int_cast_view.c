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
// CHECK-NEXT:     let mut total: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:         total += unsafe { *unsafe { (src as *const i32).offset(i as isize) } };
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:         i = [[V_0]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK: {{^}}}
// SLATE-FILECHECK-END
