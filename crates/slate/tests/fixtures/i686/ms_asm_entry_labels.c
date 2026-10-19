/* { dg-options "--flavor=msvc" } */
int printf(const char *, ...);

static int asm_to_c(int value) {
  __asm {
    mov eax, value
    cmp eax, 10
    jg big
  }
  return value;
big:
  return value + 1000;
}

static int c_to_asm(int value) {
  int result = 0;
  goto inside;
  __asm {
    mov result, 1
  inside:
    mov eax, value
    add result, eax
  }
  return result;
}

static int c_to_asm_loop(int n) {
  int total = 0;
  int i = 0;
again:
  if (i >= n)
    return total;
  goto step;
  __asm {
    add total, 100
  step:
    add total, 1
  }
  i++;
  goto again;
}

static int both(int value) {
  int result = 0;
  if (value > 0)
    goto entry;
  __asm {
    mov result, 5
  entry:
    add result, 7
    cmp value, 50
    jg far_away
  }
  return result;
far_away:
  return -result;
}

int main(void) {
  printf("%d %d %d %d\n", asm_to_c(4), asm_to_c(40), c_to_asm(9), c_to_asm_loop(3));
  printf("%d %d %d\n", both(0), both(7), both(70));
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(asm_goto_with_outputs)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "--flavor=msvc" }
// CHECK-NEXT: fn asm_to_c(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return value + 1000;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return value;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     [[V_1]] = 0;
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:                     core::arch::asm!("mov eax, [{0}]\ncmp eax, 10\njg {1}", in(reg) std::ptr::addr_of!(value), label {
// CHECK-NEXT: __slate_state = 0usize;
// CHECK-NEXT: }, out("ax") [[T_0]]);
// CHECK-NEXT:                     [[V_1]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         return [[V_1]] as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c_to_asm(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut result: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!(result) as *mut i32;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 1usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return *[[SLATE_SLOT_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     [[V_2]] = 0;
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], 0i32);
// CHECK-NEXT:                     __slate_state = 0usize;
// CHECK-NEXT:                     let [[T_0]]: u32;
// CHECK-NEXT:                     core::arch::asm!("jmp 2f\nmov dword ptr [{0}], 1\n2:\nmov eax, [{1}]\nadd [{0}], eax", in(reg) std::ptr::addr_of_mut!(*[[SLATE_SLOT_0]]), in(reg) std::ptr::addr_of!(value), out("ax") [[T_0]]);
// CHECK-NEXT:                     [[V_2]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         return [[V_2]] as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c_to_asm_loop(n: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!(total) as *mut i32;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 4usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     i += 1;
// CHECK-NEXT:                     __slate_state = 3usize;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     __slate_state = 0usize;
// CHECK-NEXT:                     let [[T_0]]: u32;
// CHECK-NEXT:                     core::arch::asm!("jmp 2f\nadd dword ptr [{0}], 100\n2:\nadd dword ptr [{0}], 1", in(reg) std::ptr::addr_of_mut!(*[[SLATE_SLOT_1]]), lateout("ax") [[T_0]]);
// CHECK-NEXT:                     [[V_3]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     return *[[SLATE_SLOT_1]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     if i >= n {
// CHECK-NEXT:                         __slate_state = 2usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 1usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     [[V_3]] = 0;
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_1]], 0i32);
// CHECK-NEXT:                     i = 0;
// CHECK-NEXT:                     __slate_state = 3usize;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         return [[V_3]] as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn both(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut result: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!(result) as *mut i32;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 4usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return -(*[[SLATE_SLOT_2]]);
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return *[[SLATE_SLOT_2]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     let [[T_0]]: u32;
// CHECK-NEXT:                     core::arch::asm!("mov dword ptr [{0}], 5\n2:\nadd dword ptr [{0}], 7\ncmp dword ptr [{1}], 50\njg {2}", in(reg) std::ptr::addr_of_mut!(*[[SLATE_SLOT_2]]), in(reg) std::ptr::addr_of!(value), label {
// CHECK-NEXT: __slate_state = 0usize;
// CHECK-NEXT: }, lateout("ax") [[T_0]]);
// CHECK-NEXT:                     [[V_4]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:                     core::arch::asm!("jmp 2f\nmov dword ptr [{0}], 5\n2:\nadd dword ptr [{0}], 7\ncmp dword ptr [{1}], 50\njg {2}", in(reg) std::ptr::addr_of_mut!(*[[SLATE_SLOT_2]]), in(reg) std::ptr::addr_of!(value), label {
// CHECK-NEXT: __slate_state = 0usize;
// CHECK-NEXT: }, lateout("ax") [[T_1]]);
// CHECK-NEXT:                     [[V_4]] = unsafe { std::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     [[V_4]] = 0;
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_2]], 0i32);
// CHECK-NEXT:                     if value > 0 {
// CHECK-NEXT:                         __slate_state = 3usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 2usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         return [[V_4]] as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             asm_to_c(4i32),
// CHECK-NEXT:             asm_to_c(40i32),
// CHECK-NEXT:             c_to_asm(9i32),
// CHECK-NEXT:             c_to_asm_loop(3i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), both(0i32), both(7i32), both(70i32)) };
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
