void abort(void);
void exit(int);

extern unsigned long aa[], bb[];

int seqgt(unsigned long a, unsigned short win, unsigned long b);

int seqgt2(unsigned long a, unsigned short win, unsigned long b);

int main(void) {
  if (!seqgt(*aa, 0x1000, *bb) || !seqgt2(*aa, 0x1000, *bb))
    abort();

  exit(0);
}

int seqgt(unsigned long a, unsigned short win, unsigned long b) {
  return (long)((a + win) - b) > 0;
}

int seqgt2(unsigned long a, unsigned short win, unsigned long b) {
  long l = ((a + win) - b);
  return l > 0;
}

unsigned long aa[] = {(1UL << (sizeof(long) * 8 - 1)) - 0xfff};
unsigned long bb[] = {(1UL << (sizeof(long) * 8 - 1)) - 0xfff};

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if !(seqgt(
// CHECK-NEXT:         unsafe { *unsafe { std::ptr::addr_of_mut!(aa) as *mut u64 } },
// CHECK-NEXT:         ((4096 as i32) as i16) as u16,
// CHECK-NEXT:         unsafe { *unsafe { std::ptr::addr_of_mut!(bb) as *mut u64 } },
// CHECK-NEXT:     ) != (0 as i32))
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = !(seqgt2(
// CHECK-NEXT:             unsafe { *unsafe { std::ptr::addr_of_mut!(aa) as *mut u64 } },
// CHECK-NEXT:             ((4096 as i32) as i16) as u16,
// CHECK-NEXT:             unsafe { *unsafe { std::ptr::addr_of_mut!(bb) as *mut u64 } },
// CHECK-NEXT:         ) != (0 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn seqgt(mut a: u64, mut win: u16, mut b: u64) -> i32 {
// CHECK-NEXT:     ((a.wrapping_add((((win as u32) as i32) as i64) as u64).wrapping_sub(b) as i64)
// CHECK-NEXT:         > ((0 as i32) as i64)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn seqgt2(mut a: u64, mut win: u16, mut b: u64) -> i32 {
// CHECK-NEXT:     let mut l: i64 = a
// CHECK-NEXT:         .wrapping_add((((win as u32) as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_sub(b) as i64;
// CHECK-NEXT:     (l > ((0 as i32) as i64)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut aa: [u64; 1] = [
// CHECK-NEXT:     ((1 as u64)
// CHECK-NEXT:         << (8 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((1 as i32) as i64) as u64))
// CHECK-NEXT:         .wrapping_sub(((4095 as i32) as i64) as u64),
// CHECK-NEXT: ];
// CHECK-NEXT: static mut bb: [u64; 1] = [
// CHECK-NEXT:     ((1 as u64)
// CHECK-NEXT:         << (8 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((1 as i32) as i64) as u64))
// CHECK-NEXT:         .wrapping_sub(((4095 as i32) as i64) as u64),
// CHECK-NEXT: ];
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
