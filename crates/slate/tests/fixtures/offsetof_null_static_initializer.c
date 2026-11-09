#include <stddef.h>
#include <stdio.h>

#define NULL_OFFSETOF(type, field) ((size_t) & ((type *)0)->field)

typedef struct State {
  int            mode;
  unsigned char  warn_all;
  unsigned char  warn_error;
  struct {
    short depth;
    char  flags[3];
  } nested;
  long long      tail;
} State;

typedef struct FlagDef {
  unsigned short offset;
  unsigned short flags;
  const char    *name;
} FlagDef;

static const FlagDef options[] = {
    {NULL_OFFSETOF(State, warn_all), 1, "all"},
    {NULL_OFFSETOF(State, warn_error), 0, "error"},
    {NULL_OFFSETOF(State, nested.flags[2]), 0, "nested"},
    {NULL_OFFSETOF(State, tail), 2, "tail"},
};

static const size_t tail_offset = NULL_OFFSETOF(State, tail);
static unsigned int narrow_offset = (unsigned int)NULL_OFFSETOF(State, nested.depth);

int main(void) {
  State state = {0};
  for (size_t i = 0; i < sizeof options / sizeof options[0]; i++) {
    unsigned char *field = (unsigned char *)&state + options[i].offset;
    *field = (unsigned char)(i + 1);
    printf("%s %u %u\n", options[i].name, options[i].offset, options[i].flags);
  }
  printf("%u %u %u %d\n", state.warn_all, state.warn_error,
         (unsigned)state.nested.flags[2], (int)(state.tail & 0xff));
  unsigned char span = NULL_OFFSETOF(State, tail) - NULL_OFFSETOF(State, warn_all);
  printf("%zu %u %u\n", tail_offset, narrow_offset, span);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! NULL_OFFSETOF {
// CHECK-NEXT:     () => {
// CHECK-NEXT:         16usize as u64
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! NULL_OFFSETOF__2 {
// CHECK-NEXT:     () => {
// CHECK-NEXT:         4usize as u64
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct State {
// CHECK-NEXT:     mode: i32,
// CHECK-NEXT:     warn_all: u8,
// CHECK-NEXT:     warn_error: u8,
// CHECK-NEXT:     nested: __SlateRecord_offsetof_null_static_initializer_c_15,
// CHECK-NEXT:     tail: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_offsetof_null_static_initializer_c_15 {
// CHECK-NEXT:     depth: i16,
// CHECK-NEXT:     flags: [i8; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct FlagDef {
// CHECK-NEXT:     offset: u16,
// CHECK-NEXT:     flags: u16,
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut options: __SlateAlign16<[FlagDef; 4]> = __SlateAlign16([
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: FlagDef = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].offset = (4usize as u64) as u16;
// CHECK-NEXT:         [[T_0]].flags = 1u16;
// CHECK-NEXT:         [[T_0]].name = (b"all\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: FlagDef = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].offset = (5usize as u64) as u16;
// CHECK-NEXT:         [[T_1]].flags = 0u16;
// CHECK-NEXT:         [[T_1]].name = (b"error\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: FlagDef = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].offset = (10usize as u64) as u16;
// CHECK-NEXT:         [[T_2]].flags = 0u16;
// CHECK-NEXT:         [[T_2]].name = (b"nested\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_3:__t[0-9]+]]: FlagDef = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]].offset = (16usize as u64) as u16;
// CHECK-NEXT:         [[T_3]].flags = 2u16;
// CHECK-NEXT:         [[T_3]].name = (b"tail\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut tail_offset: u64 = 16usize as u64;
// CHECK-NEXT: static mut narrow_offset: u32 = (6usize as u64) as u32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut state: State = {
// CHECK-NEXT:         let mut [[T_0]]: State = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].mode = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0..((std::mem::size_of::<[FlagDef; 4]>() as u64)
// CHECK-NEXT:         / (std::mem::size_of::<FlagDef>() as u64))
// CHECK-NEXT:     {
// CHECK-NEXT:         let field: *mut u8 = unsafe {
// CHECK-NEXT:             (std::ptr::addr_of_mut!(state) as *mut u8)
// CHECK-NEXT:                 .offset((options.0[i as usize].offset as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *field = i.wrapping_add(1) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%s %u %u\n".as_ptr(),
// CHECK-NEXT:                 options.0[i as usize].name,
// CHECK-NEXT:                 options.0[i as usize].offset as i32,
// CHECK-NEXT:                 options.0[i as usize].flags as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%u %u %u %d\n".as_ptr(),
// CHECK-NEXT:             state.warn_all as i32,
// CHECK-NEXT:             state.warn_error as i32,
// CHECK-NEXT:             state.nested.flags[2] as u32,
// CHECK-NEXT:             (state.tail & 255) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let span: u8 = NULL_OFFSETOF!().wrapping_sub(NULL_OFFSETOF__2!()) as u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%zu %u %u\n".as_ptr(),
// CHECK-NEXT:             tail_offset,
// CHECK-NEXT:             narrow_offset,
// CHECK-NEXT:             span as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
