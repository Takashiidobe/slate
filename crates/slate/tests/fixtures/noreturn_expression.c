_Noreturn void _Exit(int);

static _Noreturn int die(void) { _Exit(0); }

int (*die_handler)(void) = die;

int pick(int ok) {
  if (ok < 0)
    return die_handler();
  return ok ? 42 : 1 + die();
}

int main(void) {
  if (pick(1) != 42)
    return 1;
  return pick(0);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn _Exit([[ARG_0:arg[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.die")]
// CHECK-NEXT: extern "C-unwind" fn die() -> i32 {
// CHECK-NEXT:     unsafe { _Exit(0i32) };
// CHECK-NEXT:     unsafe { core::hint::unreachable_unchecked() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut die_handler: Option<unsafe extern "C-unwind" fn() -> i32> = Some(die);
// CHECK-EMPTY:
// CHECK-NEXT: fn pick(ok: i32) -> i32 {
// CHECK-NEXT:     if ok < 0 {
// CHECK-NEXT:         return unsafe { die_handler.unwrap()() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = if ok != 0 {
// CHECK-NEXT:         42
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1
// CHECK-NEXT:             + {
// CHECK-NEXT:                 let [[T_0:__t[0-9]+]] = die();
// CHECK-NEXT:                 unsafe { core::hint::unreachable_unchecked() };
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             }
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if pick(1i32) != 42 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     pick(0i32)
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
