int table_sum(const int *values, int count) {
  int sum = 0;
  for (int i = 0; i < count; ++i)
    sum += values[i];
  return sum;
}

void table_scale(int *values, int count, int by) {
  for (int i = 0; i < count; ++i)
    values[i] *= by;
}

int table_first(const int *values) { return values[1] - values[0]; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}
// CHECK-NEXT: pub fn table_sum(values: &[i32]) -> i32 {
// CHECK-NEXT:     let count: i32 = values.len() as i32;
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         sum += values[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn table_scale(values: &mut [i32], by: i32) {
// CHECK-NEXT:     let count: i32 = values.len() as i32;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         values[i as usize] *= by;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn table_first(values: &[i32]) -> i32 {
// CHECK-NEXT:     values[1] - values[0]
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
