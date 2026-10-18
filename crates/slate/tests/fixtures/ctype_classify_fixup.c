#include <ctype.h>
#include <stdio.h>

int main(void) {
  char alpha = 'A';
  char digit = '5';
  char space = ' ';
  char vtab  = '\v';
  char punct = '!';

  if (isalpha(alpha)) {
    printf("alpha-yes\n");
  } else {
    printf("alpha-no\n");
  }
  if (!isalpha(digit)) {
    printf("not-alpha-yes\n");
  } else {
    printf("not-alpha-no\n");
  }
  if (isdigit(digit)) {
    printf("digit-yes\n");
  } else {
    printf("digit-no\n");
  }
  if (isupper(alpha)) {
    printf("upper-yes\n");
  } else {
    printf("upper-no\n");
  }
  if (islower(alpha)) {
    printf("lower-yes\n");
  } else {
    printf("lower-no\n");
  }
  if (isalnum(punct)) {
    printf("alnum-yes\n");
  } else {
    printf("alnum-no\n");
  }
  if (isxdigit(alpha)) {
    printf("xdigit-yes\n");
  } else {
    printf("xdigit-no\n");
  }
  if (ispunct(punct)) {
    printf("punct-yes\n");
  } else {
    printf("punct-no\n");
  }
  if (iscntrl(vtab)) {
    printf("cntrl-yes\n");
  } else {
    printf("cntrl-no\n");
  }
  if (isgraph(punct)) {
    printf("graph-yes\n");
  } else {
    printf("graph-no\n");
  }
  if (isprint(space)) {
    printf("print-yes\n");
  } else {
    printf("print-no\n");
  }
  if (isspace(space)) {
    printf("space-yes\n");
  } else {
    printf("space-no\n");
  }
  if (isspace(vtab)) {
    printf("vtab-space-yes\n");
  } else {
    printf("vtab-space-no\n");
  }

  printf("%d\n", isalpha(alpha));

  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __ctype_b_loc() -> *mut *const u16;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let alpha: i8 = 65;
// CHECK-NEXT:     let digit: i8 = 53;
// CHECK-NEXT:     let space: i8 = 32;
// CHECK-NEXT:     let vtab: i8 = 11;
// CHECK-NEXT:     let punct: i8 = 33;
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((alpha as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & 1024 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("alpha-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("alpha-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((((unsafe { *unsafe { *__ctype_b_loc() }.offset((digit as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & 1024 != 0)
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("not-alpha-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("not-alpha-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((digit as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & 2048 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("digit-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("digit-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((alpha as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & 256 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("upper-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("upper-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((alpha as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & 512 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("lower-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("lower-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((punct as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & 8 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("alnum-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("alnum-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((alpha as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & 4096 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("xdigit-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("xdigit-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((punct as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & 4 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("punct-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("punct-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((vtab as i32) as isize) }) as u32)
// CHECK-NEXT:         as i32) & 2 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("cntrl-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("cntrl-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((punct as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & (((((32768 as i32) as i16) as u16) as u32) as i32) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("graph-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("graph-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((space as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & 16384 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("print-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("print-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((space as i32) as isize) })
// CHECK-NEXT:         as u32) as i32) & 8192 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("space-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("space-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((vtab as i32) as isize) }) as u32)
// CHECK-NEXT:         as i32) & 8192 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("vtab-space-yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("vtab-space-no");
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (((unsafe { * unsafe { * __ctype_b_loc() } .offset((alpha as i32) as isize)
// CHECK-NEXT:         }) as u32) as i32) & 1024
// CHECK-NEXT:     );
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
