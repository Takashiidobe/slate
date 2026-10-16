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
// CHECK-LABEL: {{^}}fn sum_ints(mut src: *const (), mut n: u64) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]]
// CHECK-NEXT:             + unsafe { *unsafe { (src as *const i32).offset(i as isize) } };
// CHECK-NEXT:         total = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u64 = [[V_2]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK: {{^}}}
// SLATE-FILECHECK-END
