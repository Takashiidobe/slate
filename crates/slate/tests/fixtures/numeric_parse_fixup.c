#include <stdio.h>
#include <stdlib.h>

// @check-fn-begin
int main(void) {
  char  whole[]          = "42";
  char  whole_long[]     = "-12345";
  char  whole_unsigned[] = "77";
  char  leading[]        = "  -17tail";
  char  empty[]          = "";
  char  large[]          = "999999999999999999999999999999";
  char  flt[]            = "  -3.5e2rest";
  char  end_source[]     = "12tail";
  char *end              = 0;

  printf("%d %ld %lu %ld %ld %lu %.1f\n", atoi(whole),
         strtol(whole_long, 0, 10), strtoul(whole_unsigned, 0, 10),
         atol(leading), strtol(large, 0, 10), strtoul(empty, 0, 10),
         strtod(flt, 0));

  long raw = strtol(end_source, &end, 10);
  printf("%ld %c\n", raw, *end);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut whole: [i8; 3] = [52, 50, 0];
// CHECK-NEXT:     let mut whole_long: [i8; 7] = [45, 49, 50, 51, 52, 53, 0];
// CHECK-NEXT:     let mut whole_unsigned: [i8; 3] = [55, 55, 0];
// CHECK-NEXT:     let mut leading: [i8; 10] = [32, 32, 45, 49, 55, 116, 97, 105, 108, 0];
// CHECK-NEXT:     let mut empty: [i8; 1] = [0];
// CHECK-NEXT:     let mut large: __SlateAlign16<[i8; 31]> = __SlateAlign16([
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut flt: [i8; 13] = [32, 32, 45, 51, 46, 53, 101, 50, 114, 101, 115, 116, 0];
// CHECK-NEXT:     let mut end_source: [i8; 7] = [49, 50, 116, 97, 105, 108, 0];
// CHECK-NEXT:     let mut end: *mut i8 = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %ld %lu %ld %ld %lu %.1f\n".as_ptr(),
// CHECK-NEXT:             atoi(whole.as_mut_ptr() as *const i8),
// CHECK-NEXT:             strtol(
// CHECK-NEXT:                 whole_long.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i8>(),
// CHECK-NEXT:                 10 as i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:             strtoul(
// CHECK-NEXT:                 whole_unsigned.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i8>(),
// CHECK-NEXT:                 10 as i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:             atol(leading.as_mut_ptr() as *const i8),
// CHECK-NEXT:             strtol(
// CHECK-NEXT:                 large.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i8>(),
// CHECK-NEXT:                 10 as i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:             strtoul(
// CHECK-NEXT:                 empty.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 std::ptr::null_mut::<*mut i8>(),
// CHECK-NEXT:                 10 as i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:             strtod(flt.as_mut_ptr() as *const i8, std::ptr::null_mut::<*mut i8>()),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let raw: i64 = unsafe {
// CHECK-NEXT:         strtol(
// CHECK-NEXT:             end_source.as_mut_ptr() as *const i8,
// CHECK-NEXT:             std::ptr::addr_of_mut!(end),
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld %c\n".as_ptr(), raw, *end as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
