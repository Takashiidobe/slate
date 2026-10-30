/* { dg-options "-ftrivial-auto-var-init=pattern" } */
int printf(const char *, ...);

struct padded {
  char c;
  int i;
};

static int walk(int limit) {
  int round = 0;
  int total = 0;
again:
  {
    struct padded s;
    int n;
    const unsigned char *raw = (const unsigned char *)&s;
    total += raw[0] + raw[1] + (unsigned char)n;
    round++;
    if (round < limit)
      goto again;
  }
  return total;
}

int main(void) {
  printf("%d\n", walk(3));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-ftrivial-auto-var-init=pattern" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct padded {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn walk(limit: i32) -> i32 {
// CHECK-NEXT:     let mut s: std::mem::MaybeUninit<padded> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut padded = std::ptr::addr_of_mut!(s) as *mut padded;
// CHECK-NEXT:     let mut total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut round: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         round = 0;
// CHECK-NEXT:         total = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let __slate_auto_bytes = [[SLATE_SLOT_0]] as *mut u8;
// CHECK-NEXT:             std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 8usize);
// CHECK-NEXT:             let n: i32 = -1431655766i32;
// CHECK-NEXT:             let raw: *const u8 = std::ptr::addr_of_mut!(*[[SLATE_SLOT_0]]) as *const u8;
// CHECK-NEXT:             total += ((*raw.add(0) as u32) as i32)
// CHECK-NEXT:                 + ((*raw.add(1) as u32) as i32)
// CHECK-NEXT:                 + ((((n as i8) as u8) as u32) as i32);
// CHECK-NEXT:             round += 1;
// CHECK-NEXT:             if round >= limit {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return total;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { printf(c"%d\n".as_ptr(), walk(3i32)) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
