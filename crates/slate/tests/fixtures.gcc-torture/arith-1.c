void abort(void);
void exit(int);

unsigned sat_add(unsigned i) {
  unsigned ret = i + 1;
  if (ret < i)
    ret = i;
  return ret;
}

unsigned sat_add2(unsigned i) {
  unsigned ret = i + 1;
  if (ret > i)
    return ret;
  return i;
}

unsigned sat_add3(unsigned i) {
  unsigned ret = i - 1;
  if (ret > i)
    ret = i;
  return ret;
}

unsigned sat_add4(unsigned i) {
  unsigned ret = i - 1;
  if (ret < i)
    return ret;
  return i;
}

int main(void) {
  if (sat_add(~0U) != ~0U)
    abort();
  if (sat_add2(~0U) != ~0U)
    abort();
  if (sat_add3(0U) != 0U)
    abort();
  if (sat_add4(0U) != 0U)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_add(mut i: u32) -> u32 {
// CHECK-NEXT:     let mut ret: u32 = i.wrapping_add((1 as i32) as u32);
// CHECK-NEXT:     if ret < i {
// CHECK-NEXT:         ret = i;
// CHECK-NEXT:     }
// CHECK-NEXT:     ret
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_add2(mut i: u32) -> u32 {
// CHECK-NEXT:     let mut ret: u32 = i.wrapping_add((1 as i32) as u32);
// CHECK-NEXT:     if ret > i {
// CHECK-NEXT:         return ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     i
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_add3(mut i: u32) -> u32 {
// CHECK-NEXT:     let mut ret: u32 = i.wrapping_sub((1 as i32) as u32);
// CHECK-NEXT:     if ret > i {
// CHECK-NEXT:         ret = i;
// CHECK-NEXT:     }
// CHECK-NEXT:     ret
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_add4(mut i: u32) -> u32 {
// CHECK-NEXT:     let mut ret: u32 = i.wrapping_sub((1 as i32) as u32);
// CHECK-NEXT:     if ret < i {
// CHECK-NEXT:         return ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     i
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if sat_add(!(0 as u32)) != !(0 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if sat_add2(!(0 as u32)) != !(0 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if sat_add3(0 as u32) != (0 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if sat_add4(0 as u32) != (0 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
