#include <stdio.h>

struct Slice {
  const char   *data;
  unsigned long length;
};

static struct Slice *make_slice(struct Slice *result, const char *data,
                                unsigned long length) {
  result->data   = data;
  result->length = length;
  return result;
}

#define SLICE(text) (*make_slice(&(struct Slice){}, text, sizeof(text) - 1))

// @check-fn-begin
int main(void) {
  struct Slice first  = SLICE("slate");
  struct Slice second = SLICE("translation");
  printf("%c %lu %c %lu\n", first.data[0], first.length, second.data[0],
         second.length);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: Slice = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: Slice = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut first: Slice = unsafe {
// CHECK-NEXT:         *make_slice(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::ptr::addr_of_mut!(
// CHECK-NEXT:                     * { [[SLATE_COMPOUND_0]] = { let mut [[T_0:__t[0-9]+]] : Slice = unsafe {
// CHECK-NEXT:                     std::mem::zeroed() }; [[T_0]] };
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]]) }
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             c"slate".as_ptr() as *const i8,
// CHECK-NEXT:             (6 as u64).wrapping_sub(((1 as i32) as i64) as u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut second: Slice = unsafe {
// CHECK-NEXT:         *make_slice(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::ptr::addr_of_mut!(
// CHECK-NEXT:                     * { [[SLATE_COMPOUND_1]] = { let mut [[T_1:__t[0-9]+]] : Slice = unsafe {
// CHECK-NEXT:                     std::mem::zeroed() }; [[T_1]] };
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]]) }
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             c"translation".as_ptr() as *const i8,
// CHECK-NEXT:             (12 as u64).wrapping_sub(((1 as i32) as i64) as u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%c %lu %c %lu\n".as_ptr() as *const i8,
// CHECK-NEXT:             (unsafe { *unsafe { first.data.offset((0 as i32) as isize) } }) as i32,
// CHECK-NEXT:             first.length,
// CHECK-NEXT:             (unsafe { *unsafe { second.data.offset((0 as i32) as isize) } }) as i32,
// CHECK-NEXT:             second.length,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
