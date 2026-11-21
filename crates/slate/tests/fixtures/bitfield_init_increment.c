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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: __SlateRecord_bitfield_init_increment_c_7 = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: __SlateRecord_bitfield_init_increment_c_7 = unsafe {
// CHECK-NEXT:             core::mem::zeroed()
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_a(1i32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_b(2i32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c(3i32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_a();
// CHECK-NEXT:     let [[T_1:__t[0-9]+]] = [[V_0]] + 1;
// CHECK-NEXT:     x.__slate_bits_0.__set_a([[T_1]]);
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_b();
// CHECK-NEXT:     let [[T_2:__t[0-9]+]] = [[V_1]] + 1;
// CHECK-NEXT:     x.__slate_bits_0.__set_b([[T_2]]);
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_c();
// CHECK-NEXT:     let [[T_3:__t[0-9]+]] = [[V_2]] + 1;
// CHECK-NEXT:     x.__slate_bits_0.__set_c([[T_3]]);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}",
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:         [[V_1]],
// CHECK-NEXT:         [[V_2]],
// CHECK-NEXT:         core::mem::size_of::<__SlateRecord_bitfield_init_increment_c_7>() as u64
// CHECK-NEXT:     );
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_a() + 1;
// CHECK-NEXT:     x.__slate_bits_0.__set_a([[V_3]]);
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_b() + 1;
// CHECK-NEXT:     x.__slate_bits_0.__set_b([[V_4]]);
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = x.__slate_bits_0.__get_c() + 1;
// CHECK-NEXT:     x.__slate_bits_0.__set_c([[V_5]]);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         (([[V_3]] as i16) << 6 >> 6) as i32,
// CHECK-NEXT:         (([[V_4]] as i16) << 6 >> 6) as i32,
// CHECK-NEXT:         (([[V_5]] as i16) << 6 >> 6) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
