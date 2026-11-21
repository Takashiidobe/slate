#include <stdio.h>

union Interrupt { volatile int flag; double padding; };
struct State {
  int before;
  union { volatile int interrupted; double padding; };
  volatile unsigned short counters[3];
  volatile int *pointer;
};
static volatile int global;
static int calls;

static struct State *once(struct State *state) { ++calls; return state; }

int main(void) {
  union Interrupt interrupt = {0};
  struct State state = {0};
  int target = 7;
  state.pointer = &target;
  once(&state)->interrupted = 17;
  state.counters[2] = 100;
  state.counters[2] += 23;
  *state.pointer = 11;
  const volatile union Interrupt *view = &interrupt;
  interrupt.flag = 9;
  global = view->flag + state.interrupted;
  if (state.counters[2] != 123 || target != 11 || global != 26 || calls != 1) return 1;
  volatile struct State *all = &state;
  all->before = 31;
  if (all->before != 31) return 2;
  printf("%d %u %d %d\n", global, state.counters[2], target, all->before);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union Interrupt {
// CHECK-NEXT:     flag: i32,
// CHECK-NEXT:     padding: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct State {
// CHECK-NEXT:     before: i32,
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: __SlateRecord_volatile_record_access_c_6,
// CHECK-NEXT:     counters: [u16; 3],
// CHECK-NEXT:     pointer: *mut i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_volatile_record_access_c_6 {
// CHECK-NEXT:     interrupted: i32,
// CHECK-NEXT:     padding: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut global: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut calls: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn once(state: &mut State) -> *mut State {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     state as *mut State
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut interrupt: Interrupt = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Interrupt = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].flag = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut state: State = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: State = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].before = 0i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut target: i32 = 7;
// CHECK-NEXT:     state.pointer = core::ptr::addr_of_mut!(target) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write_volatile(
// CHECK-NEXT:             core::ptr::addr_of_mut!((*once(&mut state)).[[SLATE_ANON_0]].interrupted),
// CHECK-NEXT:             17i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write_volatile(
// CHECK-NEXT:             (state.counters.as_mut_ptr() as *mut u16).add(2),
// CHECK-NEXT:             100u16,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut u16 = unsafe { (state.counters.as_mut_ptr() as *mut u16).add(2) };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u16 = unsafe { core::ptr::read_volatile([[V_0]]) };
// CHECK-NEXT:     unsafe { core::ptr::write_volatile([[V_0]], (([[V_1]] as i32) + 23) as u16) };
// CHECK-NEXT:     unsafe { core::ptr::write_volatile(state.pointer, 11i32) };
// CHECK-NEXT:     let view: *const Interrupt = core::ptr::addr_of_mut!(interrupt) as *const Interrupt;
// CHECK-NEXT:     unsafe { core::ptr::write_volatile(core::ptr::addr_of_mut!(interrupt.flag), 9i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write_volatile(
// CHECK-NEXT:             core::ptr::addr_of_mut!(global),
// CHECK-NEXT:             core::ptr::read_volatile(core::ptr::addr_of!((*view).flag))
// CHECK-NEXT:                 + core::ptr::read_volatile(
// CHECK-NEXT:                     core::ptr::addr_of!(state.[[SLATE_ANON_0]].interrupted),
// CHECK-NEXT:                 ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         core::ptr::read_volatile((state.counters.as_mut_ptr() as *mut u16).add(2))
// CHECK-NEXT:     }) as i32) != 123 || target != 11
// CHECK-NEXT:         || (unsafe { core::ptr::read_volatile(core::ptr::addr_of!(global)) }) != 26
// CHECK-NEXT:         || (unsafe { calls }) != 1 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let all: *mut State = core::ptr::addr_of_mut!(state) as *mut State;
// CHECK-NEXT:     unsafe { core::ptr::write_volatile(core::ptr::addr_of_mut!((*all).before), 31i32) };
// CHECK-NEXT:     if (unsafe { core::ptr::read_volatile(core::ptr::addr_of!((*all).before)) }) != 31 {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %u %d %d\n".as_ptr(),
// CHECK-NEXT:             core::ptr::read_volatile(core::ptr::addr_of!(global)),
// CHECK-NEXT:             core::ptr::read_volatile((state.counters.as_mut_ptr() as *mut u16).add(2))
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             target,
// CHECK-NEXT:             core::ptr::read_volatile(core::ptr::addr_of!((*all).before)),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
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
