#include <stdlib.h>
#include <string.h>

char **buildargv(char *input) {
  static char *arglist[256];
  int          numargs = 0;

  while (1) {
    while (*input == ' ')
      input++;
    if (*input == 0)
      break;
    arglist[numargs++] = input;
    while (*input != ' ' && *input != 0)
      input++;
    if (*input == 0)
      break;
    *(input++) = 0;
  }
  arglist[numargs] = NULL;
  return arglist;
}

int main() {
  char **args;
  char   input[256];
  int    i;

  strcpy(input, " a b");
  args = buildargv(input);

  if (strcmp(args[0], "a"))
    abort();
  if (strcmp(args[1], "b"))
    abort();
  if (args[2] != NULL)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT:     fn strcpy(__dest: *mut i8, __src: *const i8) -> *mut i8;
// CHECK-NEXT:     fn strcmp(__s1: *const i8, __s2: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn buildargv(mut input: *mut i8) -> *mut *mut i8 {
// CHECK-NEXT:     let mut numargs: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while ((unsafe { *input }) as i32) == (32 as i32) {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: *mut i8 = input;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: *mut i8 = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:             input = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if ((unsafe { *input }) as i32) == (0 as i32) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = numargs;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         numargs = [[V_3]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(arglist.0) as *mut *mut i8 }
// CHECK-NEXT:                     .offset([[V_2]] as isize)
// CHECK-NEXT:             } = input;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while ((unsafe { *input }) as i32) != (32 as i32)
// CHECK-NEXT:             && ((unsafe { *input }) as i32) != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: *mut i8 = input;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: *mut i8 = unsafe { [[V_4]].offset((1 as i32) as isize) };
// CHECK-NEXT:             input = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if ((unsafe { *input }) as i32) == (0 as i32) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut i8 = input;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: *mut i8 = unsafe { [[V_6]].offset((1 as i32) as isize) };
// CHECK-NEXT:         input = [[V_7]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_6]] = (0 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(arglist.0) as *mut *mut i8 }
// CHECK-NEXT:                 .offset(numargs as isize)
// CHECK-NEXT:         } = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(arglist.0) as *mut *mut i8 }
// CHECK-NEXT: }
// CHECK-NEXT: static mut arglist: __SlateAlign16<[*mut i8; 256]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut args: *mut *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut input: __SlateAlign16<[i8; 256]> = __SlateAlign16([0 as i8; 256]);
// CHECK-NEXT:     unsafe { strcpy(input.0.as_mut_ptr() as *mut i8, c" a b".as_ptr() as *const i8) };
// CHECK-NEXT:     args = buildargv(input.0.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strcmp(
// CHECK-NEXT:             (unsafe { *unsafe { args.offset((0 as i32) as isize) } }) as *const i8,
// CHECK-NEXT:             c"a".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strcmp(
// CHECK-NEXT:             (unsafe { *unsafe { args.offset((1 as i32) as isize) } }) as *const i8,
// CHECK-NEXT:             c"b".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { args.offset((2 as i32) as isize) } })
// CHECK-NEXT:         != std::ptr::null_mut::<i8>()
// CHECK-NEXT:     {
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
