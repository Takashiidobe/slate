#include <stdint.h>
// LOWERING-DAG: #[repr(C, align(32))]
// LOWERING-LABEL: {{^}}fn main() {
// LOWERING-DAG: aligned::Aligned<aligned::A32, OverAligned>
// LOWERING-DAG: aligned::Aligned<aligned::A64, i32>
// LOWERING-DAG: std::mem::align_of::<OverAligned>() as u64
// LOWERING-DAG: std::ptr::addr_of_mut!(*object) as u64
// LOWERING-DAG: std::ptr::addr_of_mut!(*local) as u64
// LOWERING-NOT: object as u64
// LOWERING-NOT: *local as u64
// LOWERING: {{^}}}

#include <stdio.h>

struct OverAligned {
  _Alignas(32) int value;
};

int main(void) {
  struct OverAligned object = {7};
  _Alignas(64) int   local  = 11;
  printf("%zu %zu %zu %d %d\n", _Alignof(struct OverAligned),
         (uintptr_t)&object % 32U, (uintptr_t)&local % 64U, object.value,
         local);
  return 0;
}
