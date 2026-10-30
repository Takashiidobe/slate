union U {
  struct {
    unsigned first  : 1;
    unsigned second : 1;
  } bits;
  unsigned raw;
};

// @check-fn-begin
int main(void) {
  union U value     = {0};
  value.bits.second = 1;
  return value.bits.second != 1;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: U = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].bits = {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: __SlateRecord_bitfield_storage_c_2 = unsafe {
// CHECK-NEXT:                 std::mem::zeroed()
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_1]].__slate_bits_0.__set_first(0u32);
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         value.bits.__slate_bits_0.__set_second(1u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     (((unsafe { value.bits.__slate_bits_0.__get_second() }) as i32) != 1) as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
