/* { dg-options "/Zc:preprocessor" } */
void abort(void);

static void check(int got, int want) {
  if (got != want)
    abort();
}

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
  check(asm_to_c(4), 4);
  check(asm_to_c(40), 1040);
  check(c_to_asm(9), 9);
  check(c_to_asm_loop(3), 3);
  check(both(0), 12);
  check(both(7), 7);
  check(both(70), -7);
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(asm_goto_with_outputs)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor" }
// CHECK-NEXT: fn check(got: i32, want: i32) {
// CHECK-NEXT:     if got != want {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn asm_to_c(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         [[V_0]] = 0;
// CHECK-NEXT:         let mut __slate_asm_target: bool = false;
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov eax, [{0}]\ncmp eax, 10\njg {1}",
// CHECK-NEXT:             in(reg) core::ptr::addr_of!(value),
// CHECK-NEXT:             label {
// CHECK-NEXT:                 __slate_asm_target = true;
// CHECK-NEXT:             },
// CHECK-NEXT:             out("ax") [[T_0]],
// CHECK-NEXT:         );
// CHECK-NEXT:         [[V_0]] = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:         if __slate_asm_target {
// CHECK-NEXT:             return value + 1000;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return value;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c_to_asm(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut result: core::mem::MaybeUninit<i32> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = core::ptr::addr_of_mut!(result) as *mut i32;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         [[V_1]] = 0;
// CHECK-NEXT:         core::ptr::write([[SLATE_SLOT_0]], 0i32);
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "jmp 2f\nmov dword ptr [{0}], 1\n2:\nmov eax, [{1}]\nadd [{0}], eax",
// CHECK-NEXT:             in(reg) core::ptr::addr_of_mut!(*[[SLATE_SLOT_0]]),
// CHECK-NEXT:             in(reg) core::ptr::addr_of!(value),
// CHECK-NEXT:             out("ax") [[T_0]],
// CHECK-NEXT:         );
// CHECK-NEXT:         [[V_1]] = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:         return *[[SLATE_SLOT_0]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c_to_asm_loop(n: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut total: core::mem::MaybeUninit<i32> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = core::ptr::addr_of_mut!(total) as *mut i32;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         [[V_2]] = 0;
// CHECK-NEXT:         core::ptr::write([[SLATE_SLOT_1]], 0i32);
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if i >= n {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 let [[T_0]]: u32;
// CHECK-NEXT:                 core::arch::asm!(
// CHECK-NEXT:                     "jmp 2f\nadd dword ptr [{0}], 100\n2:\nadd dword ptr [{0}], 1",
// CHECK-NEXT:                     in(reg) core::ptr::addr_of_mut!(*[[SLATE_SLOT_1]]),
// CHECK-NEXT:                     lateout("ax") [[T_0]],
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[V_2]] = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:                 i += 1;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return *[[SLATE_SLOT_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn both(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut result: core::mem::MaybeUninit<i32> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0]]: *mut i32 = core::ptr::addr_of_mut!(result) as *mut i32;
// CHECK-NEXT:     let mut [[V_1]]: u32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             [[V_1]] = 0;
// CHECK-NEXT:             core::ptr::write([[SLATE_SLOT_0]], 0i32);
// CHECK-NEXT:             if value > 0 {
// CHECK-NEXT:                 let mut __slate_asm_target: bool = false;
// CHECK-NEXT:                 let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:                 core::arch::asm!(
// CHECK-NEXT:                     "jmp 2f\nmov dword ptr [{0}], 5\n2:\nadd dword ptr [{0}], 7\ncmp dword ptr [{1}], 50\njg {2}",
// CHECK-NEXT:                     in(reg) core::ptr::addr_of_mut!(*[[SLATE_SLOT_0]]),
// CHECK-NEXT:                     in(reg) core::ptr::addr_of!(value),
// CHECK-NEXT:                     label {
// CHECK-NEXT:                         __slate_asm_target = true;
// CHECK-NEXT:                     },
// CHECK-NEXT:                     lateout("ax") [[T_1]],
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[V_1]] = unsafe { core::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:                 if __slate_asm_target {
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 let mut __slate_asm_target: bool = false;
// CHECK-NEXT:                 let [[T_0]]: u32;
// CHECK-NEXT:                 core::arch::asm!(
// CHECK-NEXT:                     "mov dword ptr [{0}], 5\n2:\nadd dword ptr [{0}], 7\ncmp dword ptr [{1}], 50\njg {2}",
// CHECK-NEXT:                     in(reg) core::ptr::addr_of_mut!(*[[SLATE_SLOT_0]]),
// CHECK-NEXT:                     in(reg) core::ptr::addr_of!(value),
// CHECK-NEXT:                     label {
// CHECK-NEXT:                         __slate_asm_target = true;
// CHECK-NEXT:                     },
// CHECK-NEXT:                     lateout("ax") [[T_0]],
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[V_1]] = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:                 if __slate_asm_target {
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             return *[[SLATE_SLOT_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         return -(*[[SLATE_SLOT_0]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     check(asm_to_c(4i32), 4i32);
// CHECK-NEXT:     check(asm_to_c(40i32), 1040i32);
// CHECK-NEXT:     check(c_to_asm(9i32), 9i32);
// CHECK-NEXT:     check(c_to_asm_loop(3i32), 3i32);
// CHECK-NEXT:     check(both(0i32), 12i32);
// CHECK-NEXT:     check(both(7i32), 7i32);
// CHECK-NEXT:     check(both(70i32), -7i32);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
