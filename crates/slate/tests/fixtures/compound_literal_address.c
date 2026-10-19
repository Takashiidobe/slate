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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: Slice = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: Slice = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut first: Slice = unsafe {
// CHECK-NEXT:         *make_slice(
// CHECK-NEXT:             std::ptr::addr_of_mut!(
// CHECK-NEXT:                 * { [[SLATE_COMPOUND_0]] = { let mut [[T_0:__t[0-9]+]] : Slice = std::mem::zeroed();
// CHECK-NEXT:                 [[T_0]] }; std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]]) }
// CHECK-NEXT:             ),
// CHECK-NEXT:             c"slate".as_ptr(),
// CHECK-NEXT:             5u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut second: Slice = unsafe {
// CHECK-NEXT:         *make_slice(
// CHECK-NEXT:             std::ptr::addr_of_mut!(
// CHECK-NEXT:                 * { [[SLATE_COMPOUND_1]] = { let mut [[T_1:__t[0-9]+]] : Slice = std::mem::zeroed();
// CHECK-NEXT:                 [[T_1]] }; std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]]) }
// CHECK-NEXT:             ),
// CHECK-NEXT:             c"translation".as_ptr(),
// CHECK-NEXT:             11u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%c %lu %c %lu\n".as_ptr(),
// CHECK-NEXT:             *first.data.offset(0 as isize) as i32,
// CHECK-NEXT:             first.length,
// CHECK-NEXT:             *second.data.offset(0 as isize) as i32,
// CHECK-NEXT:             second.length,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
