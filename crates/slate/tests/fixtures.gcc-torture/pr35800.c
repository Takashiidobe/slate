extern void abort(void);

int stab_xcoff_builtin_type(int typenum) {
  const char *name;
  if (typenum >= 0 || typenum < -34) {
    return 0;
  }
  switch (-typenum) {
  case 1:
    name = "int";
    break;
  case 2:
    name = "char";
  case 3:
    name = "short";
    break;
  case 4:
    name = "long";
  case 5:
    name = "unsigned char";
  case 6:
    name = "signed char";
  case 7:
    name = "unsigned short";
  case 8:
    name = "unsigned int";
  case 9:
    name = "unsigned";
  case 10:
    name = "unsigned long";
  case 11:
    name = "void";
  case 12:
    name = "float";
  case 13:
    name = "double";
  case 14:
    name = "long double";
  case 15:
    name = "integer";
  case 16:
    name = "boolean";
  case 17:
    name = "short real";
  case 18:
    name = "real";
  case 19:
    name = "stringptr";
  case 20:
    name = "character";
  case 21:
    name = "logical*1";
  case 22:
    name = "logical*2";
  case 23:
    name = "logical*4";
  case 24:
    name = "logical";
  case 25:
    name = "complex";
  case 26:
    name = "double complex";
  case 27:
    name = "integer*1";
  case 28:
    name = "integer*2";
  case 29:
    name = "integer*4";
  case 30:
    name = "wchar";
  case 31:
    name = "long long";
  case 32:
    name = "unsigned long long";
  case 33:
    name = "logical*8";
  case 34:
    name = "integer*8";
  }
  return name[0];
}

int main() {
  int i;
  if (stab_xcoff_builtin_type(0) != 0)
    abort();
  if (stab_xcoff_builtin_type(-1) != 'i')
    abort();
  if (stab_xcoff_builtin_type(-2) != 's')
    abort();
  if (stab_xcoff_builtin_type(-3) != 's')
    abort();
  for (i = -4; i >= -34; --i)
    if (stab_xcoff_builtin_type(i) != 'i')
      abort();
  if (stab_xcoff_builtin_type(-35) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn stab_xcoff_builtin_type(mut typenum: i32) -> i32 {
// CHECK-NEXT:     let mut name: *const i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if typenum >= (0 as i32) || typenum < -(34 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: i64 = match -typenum {
// CHECK-NEXT:         1 => 0,
// CHECK-NEXT:         2 => 1,
// CHECK-NEXT:         3 => 2,
// CHECK-NEXT:         4 => 3,
// CHECK-NEXT:         5 => 4,
// CHECK-NEXT:         6 => 5,
// CHECK-NEXT:         7 => 6,
// CHECK-NEXT:         8 => 7,
// CHECK-NEXT:         9 => 8,
// CHECK-NEXT:         10 => 9,
// CHECK-NEXT:         11 => 10,
// CHECK-NEXT:         12 => 11,
// CHECK-NEXT:         13 => 12,
// CHECK-NEXT:         14 => 13,
// CHECK-NEXT:         15 => 14,
// CHECK-NEXT:         16 => 15,
// CHECK-NEXT:         17 => 16,
// CHECK-NEXT:         18 => 17,
// CHECK-NEXT:         19 => 18,
// CHECK-NEXT:         20 => 19,
// CHECK-NEXT:         21 => 20,
// CHECK-NEXT:         22 => 21,
// CHECK-NEXT:         23 => 22,
// CHECK-NEXT:         24 => 23,
// CHECK-NEXT:         25 => 24,
// CHECK-NEXT:         26 => 25,
// CHECK-NEXT:         27 => 26,
// CHECK-NEXT:         28 => 27,
// CHECK-NEXT:         29 => 28,
// CHECK-NEXT:         30 => 29,
// CHECK-NEXT:         31 => 30,
// CHECK-NEXT:         32 => 31,
// CHECK-NEXT:         33 => 32,
// CHECK-NEXT:         34 => 33,
// CHECK-NEXT:         _ => -1,
// CHECK-NEXT:     };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         match [[T_0]] {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 name = c"int".as_ptr() as *const i8;
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 name = c"char".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 2;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 name = c"short".as_ptr() as *const i8;
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
// CHECK-NEXT:                 name = c"long".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 4;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 name = c"unsigned char".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 5;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             5 => {
// CHECK-NEXT:                 name = c"signed char".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 6;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             6 => {
// CHECK-NEXT:                 name = c"unsigned short".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 7;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             7 => {
// CHECK-NEXT:                 name = c"unsigned int".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 8;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             8 => {
// CHECK-NEXT:                 name = c"unsigned".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 9;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             9 => {
// CHECK-NEXT:                 name = c"unsigned long".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 10;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             10 => {
// CHECK-NEXT:                 name = c"void".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 11;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             11 => {
// CHECK-NEXT:                 name = c"float".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 12;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             12 => {
// CHECK-NEXT:                 name = c"double".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 13;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             13 => {
// CHECK-NEXT:                 name = c"long double".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 14;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             14 => {
// CHECK-NEXT:                 name = c"integer".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 15;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             15 => {
// CHECK-NEXT:                 name = c"boolean".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 16;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             16 => {
// CHECK-NEXT:                 name = c"short real".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 17;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             17 => {
// CHECK-NEXT:                 name = c"real".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 18;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             18 => {
// CHECK-NEXT:                 name = c"stringptr".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 19;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             19 => {
// CHECK-NEXT:                 name = c"character".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 20;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             20 => {
// CHECK-NEXT:                 name = c"logical*1".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 21;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             21 => {
// CHECK-NEXT:                 name = c"logical*2".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 22;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             22 => {
// CHECK-NEXT:                 name = c"logical*4".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 23;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             23 => {
// CHECK-NEXT:                 name = c"logical".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 24;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             24 => {
// CHECK-NEXT:                 name = c"complex".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 25;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             25 => {
// CHECK-NEXT:                 name = c"double complex".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 26;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             26 => {
// CHECK-NEXT:                 name = c"integer*1".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 27;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             27 => {
// CHECK-NEXT:                 name = c"integer*2".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 28;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             28 => {
// CHECK-NEXT:                 name = c"integer*4".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 29;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             29 => {
// CHECK-NEXT:                 name = c"wchar".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 30;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             30 => {
// CHECK-NEXT:                 name = c"long long".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 31;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             31 => {
// CHECK-NEXT:                 name = c"unsigned long long".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 32;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             32 => {
// CHECK-NEXT:                 name = c"logical*8".as_ptr() as *const i8;
// CHECK-NEXT:                 [[T_0]] = 33;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             33 => {
// CHECK-NEXT:                 name = c"integer*8".as_ptr() as *const i8;
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { *unsafe { name.offset((0 as i32) as isize) } }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = -(4 as i32);
// CHECK-NEXT:     if stab_xcoff_builtin_type(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if stab_xcoff_builtin_type(-(1 as i32)) != (105 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if stab_xcoff_builtin_type(-(2 as i32)) != (115 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if stab_xcoff_builtin_type(-(3 as i32)) != (115 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i >= -(34 as i32) {
// CHECK-NEXT:         if stab_xcoff_builtin_type(i) != (105 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if stab_xcoff_builtin_type(-(35 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
