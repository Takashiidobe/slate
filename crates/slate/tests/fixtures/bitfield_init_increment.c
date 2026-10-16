#include <stdio.h>

typedef struct {
  int a : 10;
  int b : 10;
  int c : 10;
} Bits;

// @check-fn-begin
int main(void) {
  Bits x = {1, 2, 3};
  printf("%d %d %d %d\n", x.a++, x.b++, x.c++, sizeof(x));
  printf("%d %d %d\n", ++x.a, ++x.b, ++x.c);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: [[SLATERECORD_0:__SlateRecord[0-9]+]] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_a(1 as i32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_b(2 as i32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c(3 as i32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_a();
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     x.__slate_bits_0.__set_a([[V_1]]);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_b();
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:     x.__slate_bits_0.__set_b([[V_3]]);
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_c();
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:     x.__slate_bits_0.__set_c([[V_5]]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%d %d %d %d\n".as_ptr() as *const i8, [[V_0]], [[V_2]], [[V_4]], 4 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_a();
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:     x.__slate_bits_0.__set_a([[V_7]]);
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_b();
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:     x.__slate_bits_0.__set_b([[V_9]]);
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_c();
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] + (1 as i32);
// CHECK-NEXT:     x.__slate_bits_0.__set_c([[V_11]]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:             (([[V_7]] as i16) << 6 >> 6) as i32,
// CHECK-NEXT:             (([[V_9]] as i16) << 6 >> 6) as i32,
// CHECK-NEXT:             (([[V_11]] as i16) << 6 >> 6) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
