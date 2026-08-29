#include <stdio.h>

static int classify(_BitInt(128) c) {
  switch (c) {
  case 1:
  case 2:
    break;
  case 99999999999999999999999999999wb:
    goto low;
  case 4:
    goto high;
  default:
    return -1;
  }
  for (;;) {
    switch (c) {
    case 5:
    low:
      return 100;
    case 6:
    high:
      return 200;
    default:
      return -2;
    }
  }
}

int main() {
  printf("%d\n", classify(1));
  printf("%d\n", classify(99999999999999999999999999999wb));
  printf("%d\n", classify(4));
  printf("%d\n", classify(5));
  printf("%d\n", classify(6));
  printf("%d\n", classify(9));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn classify(arg0: i128) -> i32 {
// LOWERING-NEXT:     let mut c: i128 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT: let mut _v0: i128 = 0;
// LOWERING-NEXT: let mut _v1: i128 = 0;
// LOWERING-NEXT:     let mut __state0: i32 = 0;
// LOWERING-NEXT:     '__dispatch0: loop {
// LOWERING-NEXT:         match __state0 {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 c = arg0;
// LOWERING-NEXT:                 __state0 = 1;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 _v0 = c;
// LOWERING-NEXT:                 __state0 = 2;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 match _v0 {
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         __state0 = 4;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         __state0 = 6;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     99999999999999999999999999999 => {
// LOWERING-NEXT:                         __state0 = 7;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     4 => {
// LOWERING-NEXT:                         __state0 = 8;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         __state0 = 9;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             3 => {
// LOWERING-NEXT:                 __state0 = 4;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             4 => {
// LOWERING-NEXT:                 __state0 = 5;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             5 => {
// LOWERING-NEXT:                 __state0 = 6;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             6 => {
// LOWERING-NEXT:                 __state0 = 10;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             7 => {
// LOWERING-NEXT:                 __state0 = 20;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             8 => {
// LOWERING-NEXT:                 __state0 = 22;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             9 => {
// LOWERING-NEXT:                 let _v2: i32 = -1;
// LOWERING-NEXT:                 __retval = _v2;
// LOWERING-NEXT:                 let _v3: i32 = __retval;
// LOWERING-NEXT:                 return _v3;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             10 => {
// LOWERING-NEXT:                 __state0 = 11;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             11 => {
// LOWERING-NEXT:                 __state0 = 12;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             12 => {
// LOWERING-NEXT:                 __state0 = 13;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             13 => {
// LOWERING-NEXT:                 let _v4: bool = true;
// LOWERING-NEXT:                 if _v4 {
// LOWERING-NEXT:                     __state0 = 14;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     __state0 = 28;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             14 => {
// LOWERING-NEXT:                 __state0 = 15;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             15 => {
// LOWERING-NEXT:                 __state0 = 16;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             16 => {
// LOWERING-NEXT:                 _v1 = c;
// LOWERING-NEXT:                 __state0 = 17;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             17 => {
// LOWERING-NEXT:                 match _v1 {
// LOWERING-NEXT:                     5 => {
// LOWERING-NEXT:                         __state0 = 19;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     6 => {
// LOWERING-NEXT:                         __state0 = 21;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         __state0 = 23;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             18 => {
// LOWERING-NEXT:                 __state0 = 19;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             19 => {
// LOWERING-NEXT:                 __state0 = 20;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             20 => {
// LOWERING-NEXT:                 let _v5: i32 = 100;
// LOWERING-NEXT:                 __retval = _v5;
// LOWERING-NEXT:                 let _v6: i32 = __retval;
// LOWERING-NEXT:                 return _v6;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             21 => {
// LOWERING-NEXT:                 __state0 = 22;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             22 => {
// LOWERING-NEXT:                 let _v7: i32 = 200;
// LOWERING-NEXT:                 __retval = _v7;
// LOWERING-NEXT:                 let _v8: i32 = __retval;
// LOWERING-NEXT:                 return _v8;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             23 => {
// LOWERING-NEXT:                 let _v9: i32 = -2;
// LOWERING-NEXT:                 __retval = _v9;
// LOWERING-NEXT:                 let _v10: i32 = __retval;
// LOWERING-NEXT:                 return _v10;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             24 => {
// LOWERING-NEXT:                 __state0 = 25;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             25 => {
// LOWERING-NEXT:                 __state0 = 26;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             26 => {
// LOWERING-NEXT:                 __state0 = 27;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             27 => {
// LOWERING-NEXT:                 __state0 = 13;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             28 => {
// LOWERING-NEXT:                 __state0 = 29;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             29 => {
// LOWERING-NEXT:                 let _v11: i32 = __retval;
// LOWERING-NEXT:                 return _v11;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 unreachable!();
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i128 = 1;
// LOWERING-NEXT:     let _v3: i32 = classify(_v2);
// LOWERING-NEXT:     let _v4: i32 = unsafe { printf(_v1 as *const i8, _v3) };
// LOWERING-NEXT:     let _v5: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i128 = 99999999999999999999999999999i128;
// LOWERING-NEXT:     let _v7: i32 = classify(_v6);
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v5 as *const i8, _v7) };
// LOWERING-NEXT:     let _v9: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: i128 = 4;
// LOWERING-NEXT:     let _v11: i32 = classify(_v10);
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v9 as *const i8, _v11) };
// LOWERING-NEXT:     let _v13: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: i128 = 5;
// LOWERING-NEXT:     let _v15: i32 = classify(_v14);
// LOWERING-NEXT:     let _v16: i32 = unsafe { printf(_v13 as *const i8, _v15) };
// LOWERING-NEXT:     let _v17: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v18: i128 = 6;
// LOWERING-NEXT:     let _v19: i32 = classify(_v18);
// LOWERING-NEXT:     let _v20: i32 = unsafe { printf(_v17 as *const i8, _v19) };
// LOWERING-NEXT:     let _v21: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v22: i128 = 9;
// LOWERING-NEXT:     let _v23: i32 = classify(_v22);
// LOWERING-NEXT:     let _v24: i32 = unsafe { printf(_v21 as *const i8, _v23) };
// LOWERING-NEXT:     let _v25: i32 = 0;
// LOWERING-NEXT:     __retval = _v25;
// LOWERING-NEXT:     let _v26: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v26 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn classify(arg0: i128) -> i32 {
// REWRITES-NEXT: let mut c: i128 = 0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut _v0: i128 = 0;
// REWRITES-NEXT: let mut _v1: i128 = 0;
// REWRITES-NEXT: let mut __state0: i32 = 0;
// REWRITES-NEXT: '__dispatch0: loop {
// REWRITES-NEXT:         match __state0 {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         c = arg0;
// REWRITES-NEXT:                         __state0 = 1;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         _v0 = c;
// REWRITES-NEXT:                         __state0 = 2;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             2 => {
// REWRITES-NEXT:                         match _v0 {
// REWRITES-NEXT:                             1 => {
// REWRITES-NEXT:                                             __state0 = 4;
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             2 => {
// REWRITES-NEXT:                                             __state0 = 6;
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             99999999999999999999999999999 => {
// REWRITES-NEXT:                                             __state0 = 7;
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             4 => {
// REWRITES-NEXT:                                             __state0 = 8;
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             _ => {
// REWRITES-NEXT:                                             __state0 = 9;
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             3 => {
// REWRITES-NEXT:                         __state0 = 4;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             4 => {
// REWRITES-NEXT:                         __state0 = 5;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             5 => {
// REWRITES-NEXT:                         __state0 = 6;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             6 => {
// REWRITES-NEXT:                         __state0 = 10;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             7 => {
// REWRITES-NEXT:                         __state0 = 20;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             8 => {
// REWRITES-NEXT:                         __state0 = 22;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             9 => {
// REWRITES-NEXT:                         __retval = -1;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             10 => {
// REWRITES-NEXT:                         __state0 = 11;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             11 => {
// REWRITES-NEXT:                         __state0 = 12;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             12 => {
// REWRITES-NEXT:                         __state0 = 13;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             13 => {
// REWRITES-NEXT:                         if true {
// REWRITES-NEXT:                                         __state0 = 14;
// REWRITES-NEXT:                         } else {
// REWRITES-NEXT:                                         __state0 = 28;
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             14 => {
// REWRITES-NEXT:                         __state0 = 15;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             15 => {
// REWRITES-NEXT:                         __state0 = 16;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             16 => {
// REWRITES-NEXT:                         _v1 = c;
// REWRITES-NEXT:                         __state0 = 17;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             17 => {
// REWRITES-NEXT:                         match _v1 {
// REWRITES-NEXT:                             5 => {
// REWRITES-NEXT:                                             __state0 = 19;
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             6 => {
// REWRITES-NEXT:                                             __state0 = 21;
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             _ => {
// REWRITES-NEXT:                                             __state0 = 23;
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             18 => {
// REWRITES-NEXT:                         __state0 = 19;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             19 => {
// REWRITES-NEXT:                         __state0 = 20;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             20 => {
// REWRITES-NEXT:                         __retval = 100;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             21 => {
// REWRITES-NEXT:                         __state0 = 22;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             22 => {
// REWRITES-NEXT:                         __retval = 200;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             23 => {
// REWRITES-NEXT:                         __retval = -2;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             24 => {
// REWRITES-NEXT:                         __state0 = 25;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             25 => {
// REWRITES-NEXT:                         __state0 = 26;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             26 => {
// REWRITES-NEXT:                         __state0 = 27;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             27 => {
// REWRITES-NEXT:                         __state0 = 13;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             28 => {
// REWRITES-NEXT:                         __state0 = 29;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             29 => {
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         unreachable!();
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i128 = 1;
// REWRITES-NEXT: let _v3: i32 = classify(_v2);
// REWRITES-NEXT: let _v4: i32 = unsafe { printf(_v1 as *const i8, _v3) };
// REWRITES-NEXT: let _v5: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: i128 = 99999999999999999999999999999i128;
// REWRITES-NEXT: let _v7: i32 = classify(_v6);
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v5 as *const i8, _v7) };
// REWRITES-NEXT: let _v9: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: i128 = 4;
// REWRITES-NEXT: let _v11: i32 = classify(_v10);
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v9 as *const i8, _v11) };
// REWRITES-NEXT: let _v13: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: i128 = 5;
// REWRITES-NEXT: let _v15: i32 = classify(_v14);
// REWRITES-NEXT: let _v16: i32 = unsafe { printf(_v13 as *const i8, _v15) };
// REWRITES-NEXT: let _v17: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v18: i128 = 6;
// REWRITES-NEXT: let _v19: i32 = classify(_v18);
// REWRITES-NEXT: let _v20: i32 = unsafe { printf(_v17 as *const i8, _v19) };
// REWRITES-NEXT: let _v21: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v22: i128 = 9;
// REWRITES-NEXT: let _v23: i32 = classify(_v22);
// REWRITES-NEXT: let _v24: i32 = unsafe { printf(_v21 as *const i8, _v23) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
