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
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_a();
// CHECK-NEXT:     x.__slate_bits_0.__set_a([[V_0]] + (1 as i32));
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_b();
// CHECK-NEXT:     x.__slate_bits_0.__set_b([[V_1]] + (1 as i32));
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_c();
// CHECK-NEXT:     x.__slate_bits_0.__set_c([[V_2]] + (1 as i32));
// CHECK-NEXT:     println!("{} {} {} {}", [[V_0]], [[V_1]], [[V_2]], 4 as u64);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_a();
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:     x.__slate_bits_0.__set_a([[V_4]]);
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_b();
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:     x.__slate_bits_0.__set_b([[V_6]]);
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_c();
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:     x.__slate_bits_0.__set_c([[V_8]]);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", (([[V_4]] as i16) << 6 >> 6) as i32, (([[V_6]] as i16) << 6 >> 6) as i32,
// CHECK-NEXT:         (([[V_8]] as i16) << 6 >> 6) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
