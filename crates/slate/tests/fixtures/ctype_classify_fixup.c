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

// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: unsafe { isalpha(
// REWRITES: {{^}}}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn isalpha(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn isdigit(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn isupper(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn islower(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn isalnum(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn isxdigit(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn ispunct(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn iscntrl(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn isgraph(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn isprint(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn isspace(_0: i32) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     alpha = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     digit = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     space = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     vtab = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     punct = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { isalpha({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { isalpha({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { isdigit({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { isupper({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { islower({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { isalnum({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { isxdigit({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { ispunct({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { iscntrl({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { isgraph({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { isprint({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { isspace({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { isspace({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isalpha({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut alpha: i8 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut digit: i8 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut space: i8 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut vtab: i8 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut punct: i8 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 65;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 53;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 11;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 33;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = alpha;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"alpha-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"alpha-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = digit;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"not-alpha-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"not-alpha-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = digit;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"digit-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"digit-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = alpha;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"upper-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"upper-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = alpha;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"lower-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"lower-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = punct;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"alnum-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"alnum-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = alpha;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"xdigit-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"xdigit-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = punct;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"punct-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"punct-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = vtab;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"cntrl-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"cntrl-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = punct;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"graph-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"graph-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = space;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"print-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"print-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = space;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"space-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"space-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = vtab;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"vtab-space-yes\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"vtab-space-no\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = alpha;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut alpha: u8 = 0;
// LOWERING-AARCH64-GNU-NEXT:     let mut digit: u8 = 0;
// LOWERING-AARCH64-GNU-NEXT:     let mut space: u8 = 0;
// LOWERING-AARCH64-GNU-NEXT:     let mut vtab: u8 = 0;
// LOWERING-AARCH64-GNU-NEXT:     let mut punct: u8 = 0;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 65;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 53;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 11;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 33;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = alpha;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"alpha-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"alpha-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = digit;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"not-alpha-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"not-alpha-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = digit;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"digit-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"digit-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = alpha;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"upper-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"upper-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = alpha;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"lower-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"lower-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = punct;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"alnum-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"alnum-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = alpha;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"xdigit-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"xdigit-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = punct;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"punct-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"punct-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = vtab;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"cntrl-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"cntrl-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = punct;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"graph-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"graph-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = space;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"print-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"print-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = space;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"space-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"space-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = vtab;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"vtab-space-yes\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"vtab-space-no\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = alpha;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn isalpha(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn isdigit(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn isupper(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn islower(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn isalnum(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn isxdigit(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn ispunct(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn iscntrl(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn isgraph(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn isprint(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn isspace(_0: i32) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isalpha(alpha as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"alpha-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"alpha-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isalpha(digit as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"not-alpha-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"not-alpha-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isdigit(digit as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"digit-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"digit-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isupper(alpha as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"upper-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"upper-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { islower(alpha as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"lower-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"lower-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isalnum(punct as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"alnum-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"alnum-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isxdigit(alpha as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"xdigit-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"xdigit-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { ispunct(punct as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"punct-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"punct-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { iscntrl(vtab as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"cntrl-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"cntrl-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isgraph(punct as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"graph-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"graph-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isprint(space as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"print-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"print-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isspace(space as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"space-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"space-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { isspace(vtab as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"vtab-space-yes\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"vtab-space-no\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), unsafe { isalpha(alpha as i32) }) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut alpha: i8 = 65;
// REWRITES-X86_64-GNU-NEXT:     let mut digit: i8 = 53;
// REWRITES-X86_64-GNU-NEXT:     let mut space: i8 = 32;
// REWRITES-X86_64-GNU-NEXT:     let mut vtab: i8 = 11;
// REWRITES-X86_64-GNU-NEXT:     let mut punct: i8 = 33;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut alpha: u8 = 65;
// REWRITES-AARCH64-GNU-NEXT:     let mut digit: u8 = 53;
// REWRITES-AARCH64-GNU-NEXT:     let mut space: u8 = 32;
// REWRITES-AARCH64-GNU-NEXT:     let mut vtab: u8 = 11;
// REWRITES-AARCH64-GNU-NEXT:     let mut punct: u8 = 33;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
