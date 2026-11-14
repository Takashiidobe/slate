#include <stdio.h>

struct cursor {
    int index;
    int *items;
};

struct sequence {
    int *items;
};

#define TO_LONG(op) ((long)(op))
#define WRAP(obj) scale(TO_LONG(obj))
#define ITEM(op, i) ((op)->items[i])

static long scale(long value) {
    return value * 10;
}

int main(void) {
    int values[3] = {4, 5, 6};
    struct sequence sequence = {values};
    struct sequence *seq = &sequence;
    struct cursor state = {0, values};
    struct cursor *it = &state;
    long first = WRAP(ITEM(seq, it->index++));
    long second = WRAP(ITEM(seq, it->index++));
    long third = WRAP(it->items[it->index]);
    printf("%ld %ld %ld %d\n", first, second, third, it->index);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_argument_effects.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_argument_effects.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_argument_effects: 11 items, 1 gated, 0 duplicated functions
// CHECK-NEXT: macros: 3 function-like, 2 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   inline argument_reads_macro_local: 12
// CHECK-NEXT:   macro ITEM: 0 lifted, inline: argument_reads_macro_local 4
// CHECK-NEXT:   macro TO_LONG: 2 lifted, inline: argument_reads_macro_local 4
// CHECK-NEXT:   macro WRAP: 2 lifted, inline: argument_reads_macro_local 4
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct cursor {
// CHECK-NEXT:     index: i32,
// CHECK-NEXT:     items: *mut i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct sequence {
// CHECK-NEXT:     items: *mut i32,
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
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: [i32; 3] = [4, 5, 6];
// CHECK-NEXT:     let mut sequence: sequence = sequence {
// CHECK-NEXT:         items: values.as_mut_ptr() as *mut i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     let seq: *mut sequence = std::ptr::addr_of_mut!(sequence);
// CHECK-NEXT:     let mut state: cursor = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: cursor = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].index = 0i32;
// CHECK-NEXT:         [[T_0]].items = values.as_mut_ptr() as *mut i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let it: *mut cursor = std::ptr::addr_of_mut!(state);
// CHECK-NEXT:     let mut first: c_long = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = unsafe { (*it).index };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*it).index = [[V_0]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     first = scale((unsafe { *unsafe { (*seq).items }.offset([[V_0]] as isize) }) as c_long);
// CHECK-NEXT:     let mut second: c_long = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = unsafe { (*it).index };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*it).index = [[V_1]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     second = scale((unsafe { *unsafe { (*seq).items }.offset([[V_1]] as isize) }) as c_long);
// CHECK-NEXT:     let third: c_long = WRAP!(unsafe { *unsafe { (*it).items }.offset((*it).index as isize) });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %ld %ld %d\n".as_ptr(),
// CHECK-NEXT:             first,
// CHECK-NEXT:             second,
// CHECK-NEXT:             third,
// CHECK-NEXT:             (*it).index,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn scale(value: c_long) -> c_long {
// CHECK-NEXT:     value * 10
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TO_LONG {
// CHECK-NEXT:     ($op:expr) => {
// CHECK-NEXT:         ($op) as c_long
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! WRAP {
// CHECK-NEXT:     ($obj:expr) => {
// CHECK-NEXT:         scale(TO_LONG!($obj))
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
