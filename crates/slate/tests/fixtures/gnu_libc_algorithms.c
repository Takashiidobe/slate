#define _GNU_SOURCE
#include <argp.h>
#include <getopt.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct GNUArguments {
  int number;
  int positional;
};

static int gnu_compare_with_direction(const void *left, const void *right,
                                      void *state) {
  int direction = *(int *)state;
  int a         = *(const int *)left;
  int b         = *(const int *)right;
  return direction * ((a > b) - (a < b));
}

static int gnu_compare_entries(const void *left, const void *right) {
  const int a = *(const int *)left;
  const int b = *(const int *)right;
  return (a > b) - (a < b);
}

static void gnu_free_entry(void *entry) { free(entry); }

static error_t gnu_parse_option(int key, char *argument,
                                struct argp_state *state) {
  struct GNUArguments *arguments = state->input;
  if (key == 'n') {
    arguments->number = atoi(argument);
    return 0;
  }
  if (key == ARGP_KEY_ARG) {
    arguments->positional += strcmp(argument, "item") == 0;
    return 0;
  }
  if (key == ARGP_KEY_END || key == ARGP_KEY_INIT || key == ARGP_KEY_NO_ARGS ||
      key == ARGP_KEY_SUCCESS || key == ARGP_KEY_FINI) {
    return 0;
  }
  return ARGP_ERR_UNKNOWN;
}

static int gnu_qsort_extension(void) {
  int values[]  = {4, 1, 3, 2};
  int direction = -1;
  qsort_r(values, 4, sizeof(values[0]), gnu_compare_with_direction, &direction);
  return values[0] * 1000 + values[1] * 100 + values[2] * 10 + values[3];
}

static int gnu_getopt_extensions(void) {
  char          program[]    = "probe";
  char          number[]     = "--number=7";
  char          flag[]       = "-f";
  char         *arguments[]  = {program, number, flag, NULL};
  struct option options[]    = {{"number", required_argument, NULL, 'n'},
                                {"flag", no_argument, NULL, 'f'},
                                {NULL, 0, NULL, 0}};
  int           number_value = 0;
  int           flag_value   = 0;
  int           option;

  optind = 1;
  opterr = 0;
  while ((option = getopt_long(3, arguments, "fn:", options, NULL)) != -1) {
    if (option == 'n') {
      number_value = atoi(optarg);
    } else if (option == 'f') {
      flag_value = 1;
    }
  }
  return number_value * 10 + flag_value;
}

static int gnu_argp_extensions(void) {
  struct argp_option options[] = {{"number", 'n', "VALUE", 0, "number", 0},
                                  {NULL, 0, NULL, 0, NULL, 0}};
  struct argp parser = {options, gnu_parse_option, "ITEM", NULL, NULL, NULL,
                        NULL};
  struct GNUArguments parsed      = {};
  char                program[]   = "probe";
  char                option[]    = "--number=5";
  char                item[]      = "item";
  char               *arguments[] = {program, option, item, NULL};
  int result = argp_parse(&parser, 3, arguments, ARGP_NO_EXIT | ARGP_NO_HELP,
                          NULL, &parsed);
  return (result == 0) + parsed.number * 10 + parsed.positional;
}

static int gnu_search_extensions(void) {
  struct hsearch_data table    = {};
  ENTRY               inserted = {"slate", "24"};
  ENTRY               query    = {"slate", NULL};
  ENTRY              *found    = NULL;
  void               *tree     = NULL;
  int                 values[] = {3, 1, 4, 2};
  int                 total    = 0;

  total += hcreate_r(8, &table) != 0;
  total += hsearch_r(inserted, ENTER, &found, &table) != 0;
  total += hsearch_r(query, FIND, &found, &table) != 0;
  total += found != NULL && strcmp(found->data, "24") == 0;
  hdestroy_r(&table);

  for (size_t index = 0; index < 4; ++index) {
    int *value = malloc(sizeof(*value));
    *value     = values[index];
    tsearch(value, &tree, gnu_compare_entries);
  }
  total += tfind(&values[2], &tree, gnu_compare_entries) != NULL;
  tdestroy(tree, gnu_free_entry);
  return total;
}

int main(void) {
  printf("%d %d %d %d\n", gnu_qsort_extension(), gnu_getopt_extensions(),
         gnu_argp_extensions(), gnu_search_extensions());
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum ACTION {
// LOWERING-NEXT:     FIND = 0,
// LOWERING-NEXT:     ENTER = 1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum VISIT {
// LOWERING-NEXT:     preorder = 0,
// LOWERING-NEXT:     postorder = 1,
// LOWERING-NEXT:     endorder = 2,
// LOWERING-NEXT:     leaf = 3,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct GNUArguments {
// LOWERING-NEXT:     number: i32,
// LOWERING-NEXT:     positional: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __tab {
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct anon_struct {
// LOWERING-NEXT:     __slate_anon_0: *mut core::ffi::c_void,
// LOWERING-NEXT:     __slate_anon_1: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct argp {
// LOWERING-NEXT:     options: *mut argp_option,
// LOWERING-NEXT:     parser: Option<unsafe extern "C" fn(i32, *mut i8, *mut argp_state) -> i32>,
// LOWERING-NEXT:     args_doc: *mut i8,
// LOWERING-NEXT:     doc: *mut i8,
// LOWERING-NEXT:     children: *mut argp_child,
// LOWERING-NEXT:     help_filter: Option<unsafe extern "C" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8>,
// LOWERING-NEXT:     argp_domain: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct argp_child {
// LOWERING-NEXT:     argp: *mut argp,
// LOWERING-NEXT:     flags: i32,
// LOWERING-NEXT:     header: *mut i8,
// LOWERING-NEXT:     group: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct argp_option {
// LOWERING-NEXT:     name: *mut i8,
// LOWERING-NEXT:     key: i32,
// LOWERING-NEXT:     arg: *mut i8,
// LOWERING-NEXT:     flags: i32,
// LOWERING-NEXT:     doc: *mut i8,
// LOWERING-NEXT:     group: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct argp_state {
// LOWERING-NEXT:     root_argp: *mut argp,
// LOWERING-NEXT:     argc: i32,
// LOWERING-NEXT:     argv: *mut *mut i8,
// LOWERING-NEXT:     next: i32,
// LOWERING-NEXT:     flags: u32,
// LOWERING-NEXT:     arg_num: u32,
// LOWERING-NEXT:     quoted: i32,
// LOWERING-NEXT:     input: *mut core::ffi::c_void,
// LOWERING-NEXT:     child_inputs: *mut *mut core::ffi::c_void,
// LOWERING-NEXT:     hook: *mut core::ffi::c_void,
// LOWERING-NEXT:     name: *mut i8,
// LOWERING-NEXT:     err_stream: *mut libc::FILE,
// LOWERING-NEXT:     out_stream: *mut libc::FILE,
// LOWERING-NEXT:     pstate: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct entry {
// LOWERING-NEXT:     key: *mut i8,
// LOWERING-NEXT:     data: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct hsearch_data {
// LOWERING-NEXT:     __tab: *mut __tab,
// LOWERING-NEXT:     __unused1: u32,
// LOWERING-NEXT:     __unused2: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct option {
// LOWERING-NEXT:     name: *mut i8,
// LOWERING-NEXT:     has_arg: i32,
// LOWERING-NEXT:     flag: *mut i32,
// LOWERING-NEXT:     val: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     static mut optarg: *mut i8;
// LOWERING-NEXT:     static mut opterr: i32;
// LOWERING-NEXT:     static mut optind: i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn qsort_r(_0: *mut core::ffi::c_void, _1: usize, _2: usize, _3: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut core::ffi::c_void, *mut core::ffi::c_void) -> i32>, _4: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn getopt_long(_0: i32, _1: *mut *mut i8, _2: *const i8, _3: *const option, _4: *mut i32) -> i32;
// LOWERING-NEXT:     fn atoi(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// LOWERING-NEXT:     fn argp_parse(_0: *const argp, _1: i32, _2: *mut *mut i8, _3: u32, _4: *mut i32, _5: *mut core::ffi::c_void) -> i32;
// LOWERING-NEXT:     fn hcreate_r(_0: usize, _1: *mut hsearch_data) -> i32;
// LOWERING-NEXT:     fn hsearch_r(_0: *mut core::ffi::c_void, _1: *mut core::ffi::c_void, _2: u32, _3: *mut *mut entry, _4: *mut hsearch_data) -> i32;
// LOWERING-NEXT:     fn hdestroy_r(_0: *mut hsearch_data);
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn tsearch(_0: *const core::ffi::c_void, _1: *mut *mut core::ffi::c_void, _2: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32>) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn tfind(_0: *const core::ffi::c_void, _1: *mut *mut core::ffi::c_void, _2: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32>) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn tdestroy(_0: *mut core::ffi::c_void, _1: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>);
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn gnu_compare_with_direction(arg6: *mut core::ffi::c_void, arg7: *mut core::ffi::c_void, arg8: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let mut left: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut right: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut state: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut direction: i32 = 0;
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     left = arg6;
// LOWERING-NEXT:     right = arg7;
// LOWERING-NEXT:     state = arg8;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = state;
// LOWERING-NEXT:     let _v1: *mut i32 = _v0 as *mut i32;
// LOWERING-NEXT:     let _v2: i32 = unsafe { *_v1 };
// LOWERING-NEXT:     direction = _v2;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = left;
// LOWERING-NEXT:     let _v4: *mut i32 = _v3 as *mut i32;
// LOWERING-NEXT:     let _v5: i32 = unsafe { *_v4 };
// LOWERING-NEXT:     a = _v5;
// LOWERING-NEXT:     let _v6: *mut core::ffi::c_void = right;
// LOWERING-NEXT:     let _v7: *mut i32 = _v6 as *mut i32;
// LOWERING-NEXT:     let _v8: i32 = unsafe { *_v7 };
// LOWERING-NEXT:     b = _v8;
// LOWERING-NEXT:     let _v9: i32 = direction;
// LOWERING-NEXT:     let _v10: i32 = a;
// LOWERING-NEXT:     let _v11: i32 = b;
// LOWERING-NEXT:     let _v12: bool = _v10 > _v11;
// LOWERING-NEXT:     let _v13: i32 = _v12 as i32;
// LOWERING-NEXT:     let _v14: i32 = a;
// LOWERING-NEXT:     let _v15: i32 = b;
// LOWERING-NEXT:     let _v16: bool = _v14 < _v15;
// LOWERING-NEXT:     let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:     let _v18: i32 = _v13 - _v17;
// LOWERING-NEXT:     let _v19: i32 = _v9 * _v18;
// LOWERING-NEXT:     __retval = _v19;
// LOWERING-NEXT:     let _v20: i32 = __retval;
// LOWERING-NEXT:     return _v20;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_qsort_extension() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut direction: i32 = 0;
// LOWERING-NEXT:     *values = [4, 1, 3, 2];
// LOWERING-NEXT:     let _v0: i32 = -1;
// LOWERING-NEXT:     direction = _v0;
// LOWERING-NEXT:     let _v1: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v3: u64 = 4;
// LOWERING-NEXT:     let _v4: u64 = 4;
// LOWERING-NEXT:     let _v5: *mut core::ffi::c_void = std::ptr::addr_of_mut!(direction) as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { qsort_r(_v2 as *mut core::ffi::c_void, _v3 as usize, _v4 as usize, Some(gnu_compare_with_direction), _v5 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v6: i64 = 0;
// LOWERING-NEXT:     let _v7: i32 = values[(_v6 as usize)];
// LOWERING-NEXT:     let _v8: i32 = 1000;
// LOWERING-NEXT:     let _v9: i32 = _v7 * _v8;
// LOWERING-NEXT:     let _v10: i64 = 1;
// LOWERING-NEXT:     let _v11: i32 = values[(_v10 as usize)];
// LOWERING-NEXT:     let _v12: i32 = 100;
// LOWERING-NEXT:     let _v13: i32 = _v11 * _v12;
// LOWERING-NEXT:     let _v14: i32 = _v9 + _v13;
// LOWERING-NEXT:     let _v15: i64 = 2;
// LOWERING-NEXT:     let _v16: i32 = values[(_v15 as usize)];
// LOWERING-NEXT:     let _v17: i32 = 10;
// LOWERING-NEXT:     let _v18: i32 = _v16 * _v17;
// LOWERING-NEXT:     let _v19: i32 = _v14 + _v18;
// LOWERING-NEXT:     let _v20: i64 = 3;
// LOWERING-NEXT:     let _v21: i32 = values[(_v20 as usize)];
// LOWERING-NEXT:     let _v22: i32 = _v19 + _v21;
// LOWERING-NEXT:     __retval = _v22;
// LOWERING-NEXT:     let _v23: i32 = __retval;
// LOWERING-NEXT:     return _v23;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_getopt_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut program: [i8; 6] = [0; 6];
// LOWERING-NEXT:     let mut number: [i8; 11] = [0; 11];
// LOWERING-NEXT:     let mut flag: [i8; 3] = [0; 3];
// LOWERING-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> = aligned::Aligned([std::ptr::null_mut(); 4]);
// LOWERING-NEXT:     let mut options: aligned::Aligned<aligned::A16, [option; 3]> = aligned::Aligned([option { name: std::ptr::null_mut(), has_arg: 0, flag: std::ptr::null_mut(), val: 0 }; 3]);
// LOWERING-NEXT:     let mut number_value: i32 = 0;
// LOWERING-NEXT:     let mut flag_value: i32 = 0;
// LOWERING-NEXT:     let mut option: i32 = 0;
// LOWERING-NEXT:     program = [112, 114, 111, 98, 101, 0];
// LOWERING-NEXT:     number = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// LOWERING-NEXT:     flag = [45, 102, 0];
// LOWERING-NEXT:     let _v0: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:     let _v1: *mut i8 = program.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v0 = _v1;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v2: i64 = 1;
// LOWERING-NEXT:     let _v3: *mut *mut i8 = unsafe { _v0.add(1) };
// LOWERING-NEXT:     let _v4: *mut i8 = number.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v3 = _v4;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v5: i64 = 2;
// LOWERING-NEXT:     let _v6: *mut *mut i8 = unsafe { _v0.add(2) };
// LOWERING-NEXT:     let _v7: *mut i8 = flag.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v6 = _v7;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v8: i64 = 3;
// LOWERING-NEXT:     let _v9: *mut *mut i8 = unsafe { _v0.add(3) };
// LOWERING-NEXT:     let _v10: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v9 = _v10;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     *options = [option { name: b"number\0".as_ptr() as *mut i8, has_arg: 1, flag: std::ptr::null_mut(), val: 110 }, option { name: b"flag\0".as_ptr() as *mut i8, has_arg: 0, flag: std::ptr::null_mut(), val: 102 }, option { name: std::ptr::null_mut(), has_arg: 0, flag: std::ptr::null_mut(), val: 0 }];
// LOWERING-NEXT:     let _v11: i32 = 0;
// LOWERING-NEXT:     number_value = _v11;
// LOWERING-NEXT:     let _v12: i32 = 0;
// LOWERING-NEXT:     flag_value = _v12;
// LOWERING-NEXT:     let _v13: i32 = 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         optind = _v13;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v14: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         opterr = _v14;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v15: i32 = 3;
// LOWERING-NEXT:             let _v16: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:             let _v17: *mut i8 = b"fn:\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v18: *mut option = options.as_mut_ptr() as *mut option;
// LOWERING-NEXT:             let _v19: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:             let _v20: i32 = unsafe { getopt_long(_v15 as i32, _v16 as *mut *mut i8, _v17 as *const i8, _v18 as *const option, _v19 as *mut i32) };
// LOWERING-NEXT:             option = _v20;
// LOWERING-NEXT:             let _v21: i32 = -1;
// LOWERING-NEXT:             let _v22: bool = _v20 != _v21;
// LOWERING-NEXT:             if !_v22 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v23: i32 = option;
// LOWERING-NEXT:                     let _v24: i32 = 110;
// LOWERING-NEXT:                     let _v25: bool = _v23 == _v24;
// LOWERING-NEXT:                     if _v25 {
// LOWERING-NEXT:                         let _v26: *mut i8 = unsafe { optarg };
// LOWERING-NEXT:                         let _v27: i32 = unsafe { atoi(_v26 as *const i8) };
// LOWERING-NEXT:                         number_value = _v27;
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let _v28: i32 = option;
// LOWERING-NEXT:                             let _v29: i32 = 102;
// LOWERING-NEXT:                             let _v30: bool = _v28 == _v29;
// LOWERING-NEXT:                             if _v30 {
// LOWERING-NEXT:                                 let _v31: i32 = 1;
// LOWERING-NEXT:                                 flag_value = _v31;
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v32: i32 = number_value;
// LOWERING-NEXT:     let _v33: i32 = 10;
// LOWERING-NEXT:     let _v34: i32 = _v32 * _v33;
// LOWERING-NEXT:     let _v35: i32 = flag_value;
// LOWERING-NEXT:     let _v36: i32 = _v34 + _v35;
// LOWERING-NEXT:     __retval = _v36;
// LOWERING-NEXT:     let _v37: i32 = __retval;
// LOWERING-NEXT:     return _v37;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn gnu_parse_option(arg3: i32, arg4: *mut i8, arg5: *mut argp_state) -> i32 {
// LOWERING-NEXT:     let mut key: i32 = 0;
// LOWERING-NEXT:     let mut argument: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut state: *mut argp_state = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut arguments: *mut GNUArguments = std::ptr::null_mut();
// LOWERING-NEXT:     key = arg3;
// LOWERING-NEXT:     argument = arg4;
// LOWERING-NEXT:     state = arg5;
// LOWERING-NEXT:     let _v0: *mut argp_state = state;
// LOWERING-NEXT:     let _v1: *mut core::ffi::c_void = unsafe { (*_v0).input };
// LOWERING-NEXT:     let _v2: *mut GNUArguments = _v1 as *mut GNUArguments;
// LOWERING-NEXT:     arguments = _v2;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v3: i32 = key;
// LOWERING-NEXT:         let _v4: i32 = 110;
// LOWERING-NEXT:         let _v5: bool = _v3 == _v4;
// LOWERING-NEXT:         if _v5 {
// LOWERING-NEXT:             let _v6: *mut i8 = argument;
// LOWERING-NEXT:             let _v7: i32 = unsafe { atoi(_v6 as *const i8) };
// LOWERING-NEXT:             let _v8: *mut GNUArguments = arguments;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 (*_v8).number = _v7;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v9: i32 = 0;
// LOWERING-NEXT:             __retval = _v9;
// LOWERING-NEXT:             let _v10: i32 = __retval;
// LOWERING-NEXT:             return _v10;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v11: i32 = key;
// LOWERING-NEXT:         let _v12: i32 = 0;
// LOWERING-NEXT:         let _v13: bool = _v11 == _v12;
// LOWERING-NEXT:         if _v13 {
// LOWERING-NEXT:             let _v14: *mut i8 = argument;
// LOWERING-NEXT:             let _v15: *mut i8 = b"item\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v16: i32 = unsafe { strcmp(_v14 as *const i8, _v15 as *const i8) };
// LOWERING-NEXT:             let _v17: i32 = 0;
// LOWERING-NEXT:             let _v18: bool = _v16 == _v17;
// LOWERING-NEXT:             let _v19: i32 = _v18 as i32;
// LOWERING-NEXT:             let _v20: *mut GNUArguments = arguments;
// LOWERING-NEXT:             let _v21: i32 = unsafe { (*_v20).positional };
// LOWERING-NEXT:             let _v22: i32 = _v21 + _v19;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 (*_v20).positional = _v22;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v23: i32 = 0;
// LOWERING-NEXT:             __retval = _v23;
// LOWERING-NEXT:             let _v24: i32 = __retval;
// LOWERING-NEXT:             return _v24;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v25: i32 = key;
// LOWERING-NEXT:         let _v26: i32 = 16777217;
// LOWERING-NEXT:         let _v27: bool = _v25 == _v26;
// LOWERING-NEXT:         let _v28: bool = if _v27 {
// LOWERING-NEXT:             let _v29: bool = true;
// LOWERING-NEXT:             _v29
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let _v30: i32 = key;
// LOWERING-NEXT:             let _v31: i32 = 16777219;
// LOWERING-NEXT:             let _v32: bool = _v30 == _v31;
// LOWERING-NEXT:             _v32
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let _v33: bool = if _v28 {
// LOWERING-NEXT:             let _v34: bool = true;
// LOWERING-NEXT:             _v34
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let _v35: i32 = key;
// LOWERING-NEXT:             let _v36: i32 = 16777218;
// LOWERING-NEXT:             let _v37: bool = _v35 == _v36;
// LOWERING-NEXT:             _v37
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let _v38: bool = if _v33 {
// LOWERING-NEXT:             let _v39: bool = true;
// LOWERING-NEXT:             _v39
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let _v40: i32 = key;
// LOWERING-NEXT:             let _v41: i32 = 16777220;
// LOWERING-NEXT:             let _v42: bool = _v40 == _v41;
// LOWERING-NEXT:             _v42
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let _v43: bool = if _v38 {
// LOWERING-NEXT:             let _v44: bool = true;
// LOWERING-NEXT:             _v44
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let _v45: i32 = key;
// LOWERING-NEXT:             let _v46: i32 = 16777223;
// LOWERING-NEXT:             let _v47: bool = _v45 == _v46;
// LOWERING-NEXT:             _v47
// LOWERING-NEXT:         };
// LOWERING-NEXT:         if _v43 {
// LOWERING-NEXT:             let _v48: i32 = 0;
// LOWERING-NEXT:             __retval = _v48;
// LOWERING-NEXT:             let _v49: i32 = __retval;
// LOWERING-NEXT:             return _v49;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v50: i32 = 7;
// LOWERING-NEXT:     __retval = _v50;
// LOWERING-NEXT:     let _v51: i32 = __retval;
// LOWERING-NEXT:     return _v51;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_argp_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut options: aligned::Aligned<aligned::A16, [argp_option; 2]> = aligned::Aligned([argp_option { name: std::ptr::null_mut(), key: 0, arg: std::ptr::null_mut(), flags: 0, doc: std::ptr::null_mut(), group: 0 }; 2]);
// LOWERING-NEXT:     let mut parser: argp = argp { options: std::ptr::null_mut(), parser: None, args_doc: std::ptr::null_mut(), doc: std::ptr::null_mut(), children: std::ptr::null_mut(), help_filter: None, argp_domain: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut parsed: GNUArguments = GNUArguments { number: 0, positional: 0 };
// LOWERING-NEXT:     let mut program: [i8; 6] = [0; 6];
// LOWERING-NEXT:     let mut option: [i8; 11] = [0; 11];
// LOWERING-NEXT:     let mut item: [i8; 5] = [0; 5];
// LOWERING-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> = aligned::Aligned([std::ptr::null_mut(); 4]);
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     *options = [argp_option { name: b"number\0".as_ptr() as *mut i8, key: 110, arg: b"VALUE\0".as_ptr() as *mut i8, flags: 0, doc: b"number\0".as_ptr() as *mut i8, group: 0 }, argp_option { name: std::ptr::null_mut(), key: 0, arg: std::ptr::null_mut(), flags: 0, doc: std::ptr::null_mut(), group: 0 }];
// LOWERING-NEXT:     let _v0: *mut argp_option = options.as_mut_ptr() as *mut argp_option;
// LOWERING-NEXT:     parser.options = _v0;
// LOWERING-NEXT:     parser.parser = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32, *mut i8, *mut argp_state) -> i32>>(gnu_parse_option as *const ()) };
// LOWERING-NEXT:     let _v1: *mut i8 = b"ITEM\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     parser.args_doc = _v1;
// LOWERING-NEXT:     let _v2: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     parser.doc = _v2;
// LOWERING-NEXT:     let _v3: *mut argp_child = std::ptr::null_mut();
// LOWERING-NEXT:     parser.children = _v3;
// LOWERING-NEXT:     let _v4: Option<unsafe extern "C" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8> = None;
// LOWERING-NEXT:     parser.help_filter = _v4;
// LOWERING-NEXT:     let _v5: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     parser.argp_domain = _v5;
// LOWERING-NEXT:     parsed = GNUArguments { number: 0, positional: 0 };
// LOWERING-NEXT:     program = [112, 114, 111, 98, 101, 0];
// LOWERING-NEXT:     option = [45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0];
// LOWERING-NEXT:     item = [105, 116, 101, 109, 0];
// LOWERING-NEXT:     let _v6: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:     let _v7: *mut i8 = program.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v6 = _v7;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v8: i64 = 1;
// LOWERING-NEXT:     let _v9: *mut *mut i8 = unsafe { _v6.add(1) };
// LOWERING-NEXT:     let _v10: *mut i8 = option.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v9 = _v10;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v11: i64 = 2;
// LOWERING-NEXT:     let _v12: *mut *mut i8 = unsafe { _v6.add(2) };
// LOWERING-NEXT:     let _v13: *mut i8 = item.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v12 = _v13;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v14: i64 = 3;
// LOWERING-NEXT:     let _v15: *mut *mut i8 = unsafe { _v6.add(3) };
// LOWERING-NEXT:     let _v16: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v15 = _v16;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v17: i32 = 3;
// LOWERING-NEXT:     let _v18: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:     let _v19: i32 = 32;
// LOWERING-NEXT:     let _v20: i32 = 16;
// LOWERING-NEXT:     let _v21: i32 = _v19 | _v20;
// LOWERING-NEXT:     let _v22: u32 = _v21 as u32;
// LOWERING-NEXT:     let _v23: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v24: *mut core::ffi::c_void = std::ptr::addr_of_mut!(parsed) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v25: i32 = unsafe { argp_parse(std::ptr::addr_of_mut!(parser) as *const argp, _v17 as i32, _v18 as *mut *mut i8, _v22 as u32, _v23 as *mut i32, _v24 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     result = _v25;
// LOWERING-NEXT:     let _v26: i32 = result;
// LOWERING-NEXT:     let _v27: i32 = 0;
// LOWERING-NEXT:     let _v28: bool = _v26 == _v27;
// LOWERING-NEXT:     let _v29: i32 = _v28 as i32;
// LOWERING-NEXT:     let _v30: i32 = parsed.number;
// LOWERING-NEXT:     let _v31: i32 = 10;
// LOWERING-NEXT:     let _v32: i32 = _v30 * _v31;
// LOWERING-NEXT:     let _v33: i32 = _v29 + _v32;
// LOWERING-NEXT:     let _v34: i32 = parsed.positional;
// LOWERING-NEXT:     let _v35: i32 = _v33 + _v34;
// LOWERING-NEXT:     __retval = _v35;
// LOWERING-NEXT:     let _v36: i32 = __retval;
// LOWERING-NEXT:     return _v36;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn gnu_compare_entries(arg1: *mut core::ffi::c_void, arg2: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let mut left: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut right: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     left = arg1;
// LOWERING-NEXT:     right = arg2;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = left;
// LOWERING-NEXT:     let _v1: *mut i32 = _v0 as *mut i32;
// LOWERING-NEXT:     let _v2: i32 = unsafe { *_v1 };
// LOWERING-NEXT:     a = _v2;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = right;
// LOWERING-NEXT:     let _v4: *mut i32 = _v3 as *mut i32;
// LOWERING-NEXT:     let _v5: i32 = unsafe { *_v4 };
// LOWERING-NEXT:     b = _v5;
// LOWERING-NEXT:     let _v6: i32 = a;
// LOWERING-NEXT:     let _v7: i32 = b;
// LOWERING-NEXT:     let _v8: bool = _v6 > _v7;
// LOWERING-NEXT:     let _v9: i32 = _v8 as i32;
// LOWERING-NEXT:     let _v10: i32 = a;
// LOWERING-NEXT:     let _v11: i32 = b;
// LOWERING-NEXT:     let _v12: bool = _v10 < _v11;
// LOWERING-NEXT:     let _v13: i32 = _v12 as i32;
// LOWERING-NEXT:     let _v14: i32 = _v9 - _v13;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: i32 = __retval;
// LOWERING-NEXT:     return _v15;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn gnu_free_entry(arg0: *mut core::ffi::c_void) {
// LOWERING-NEXT:     let mut entry: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     entry = arg0;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = entry;
// LOWERING-NEXT:     unsafe { free(_v0 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_search_extensions() -> i32 {
// LOWERING-NEXT:     let mut coerce: entry = entry { key: std::ptr::null_mut(), data: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut coerce2: entry = entry { key: std::ptr::null_mut(), data: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut table: hsearch_data = hsearch_data { __tab: std::ptr::null_mut(), __unused1: 0, __unused2: 0 };
// LOWERING-NEXT:     let mut inserted: entry = entry { key: std::ptr::null_mut(), data: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut query: entry = entry { key: std::ptr::null_mut(), data: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut found: *mut entry = std::ptr::null_mut();
// LOWERING-NEXT:     let mut tree: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     table = hsearch_data { __tab: std::ptr::null_mut(), __unused1: 0, __unused2: 0 };
// LOWERING-NEXT:     inserted = entry { key: b"slate\0".as_ptr() as *mut i8, data: b"24\0".as_ptr() as *mut core::ffi::c_void };
// LOWERING-NEXT:     query = entry { key: b"slate\0".as_ptr() as *mut i8, data: std::ptr::null_mut() };
// LOWERING-NEXT:     let _v0: *mut entry = std::ptr::null_mut();
// LOWERING-NEXT:     found = _v0;
// LOWERING-NEXT:     let _v1: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     tree = _v1;
// LOWERING-NEXT:     *values = [3, 1, 4, 2];
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     total = _v2;
// LOWERING-NEXT:     let _v3: u64 = 8;
// LOWERING-NEXT:     let _v4: i32 = unsafe { hcreate_r(_v3 as usize, std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// LOWERING-NEXT:     let _v5: i32 = 0;
// LOWERING-NEXT:     let _v6: bool = _v4 != _v5;
// LOWERING-NEXT:     let _v7: i32 = _v6 as i32;
// LOWERING-NEXT:     let _v8: i32 = total;
// LOWERING-NEXT:     let _v9: i32 = _v8 + _v7;
// LOWERING-NEXT:     total = _v9;
// LOWERING-NEXT:     let _v10: u32 = ACTION::ENTER as u32;
// LOWERING-NEXT:     let _v11: entry = inserted;
// LOWERING-NEXT:     coerce2 = _v11;
// LOWERING-NEXT:     let _v12: *mut anon_struct = std::ptr::addr_of_mut!(coerce2) as *mut anon_struct;
// LOWERING-NEXT:     let _v13: *mut core::ffi::c_void = unsafe { (*_v12).__slate_anon_0 };
// LOWERING-NEXT:     let _v14: *mut core::ffi::c_void = unsafe { (*_v12).__slate_anon_1 };
// LOWERING-NEXT:     let _v15: i32 = unsafe { hsearch_r(_v13 as *mut core::ffi::c_void, _v14 as *mut core::ffi::c_void, _v10 as u32, std::ptr::addr_of_mut!(found) as *mut *mut entry, std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// LOWERING-NEXT:     let _v16: i32 = 0;
// LOWERING-NEXT:     let _v17: bool = _v15 != _v16;
// LOWERING-NEXT:     let _v18: i32 = _v17 as i32;
// LOWERING-NEXT:     let _v19: i32 = total;
// LOWERING-NEXT:     let _v20: i32 = _v19 + _v18;
// LOWERING-NEXT:     total = _v20;
// LOWERING-NEXT:     let _v21: u32 = ACTION::FIND as u32;
// LOWERING-NEXT:     let _v22: entry = query;
// LOWERING-NEXT:     coerce = _v22;
// LOWERING-NEXT:     let _v23: *mut anon_struct = std::ptr::addr_of_mut!(coerce) as *mut anon_struct;
// LOWERING-NEXT:     let _v24: *mut core::ffi::c_void = unsafe { (*_v23).__slate_anon_0 };
// LOWERING-NEXT:     let _v25: *mut core::ffi::c_void = unsafe { (*_v23).__slate_anon_1 };
// LOWERING-NEXT:     let _v26: i32 = unsafe { hsearch_r(_v24 as *mut core::ffi::c_void, _v25 as *mut core::ffi::c_void, _v21 as u32, std::ptr::addr_of_mut!(found) as *mut *mut entry, std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// LOWERING-NEXT:     let _v27: i32 = 0;
// LOWERING-NEXT:     let _v28: bool = _v26 != _v27;
// LOWERING-NEXT:     let _v29: i32 = _v28 as i32;
// LOWERING-NEXT:     let _v30: i32 = total;
// LOWERING-NEXT:     let _v31: i32 = _v30 + _v29;
// LOWERING-NEXT:     total = _v31;
// LOWERING-NEXT:     let _v32: *mut entry = found;
// LOWERING-NEXT:     let _v33: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v34: *mut entry = _v33 as *mut entry;
// LOWERING-NEXT:     let _v35: bool = _v32 != _v34;
// LOWERING-NEXT:     let _v36: bool = if _v35 {
// LOWERING-NEXT:         let _v37: *mut entry = found;
// LOWERING-NEXT:         let _v38: *mut core::ffi::c_void = unsafe { (*_v37).data };
// LOWERING-NEXT:         let _v39: *mut i8 = _v38 as *mut i8;
// LOWERING-NEXT:         let _v40: *mut i8 = b"24\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let _v41: i32 = unsafe { strcmp(_v39 as *const i8, _v40 as *const i8) };
// LOWERING-NEXT:         let _v42: i32 = 0;
// LOWERING-NEXT:         let _v43: bool = _v41 == _v42;
// LOWERING-NEXT:         _v43
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v44: bool = false;
// LOWERING-NEXT:         _v44
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v45: i32 = _v36 as i32;
// LOWERING-NEXT:     let _v46: i32 = total;
// LOWERING-NEXT:     let _v47: i32 = _v46 + _v45;
// LOWERING-NEXT:     total = _v47;
// LOWERING-NEXT:     unsafe { hdestroy_r(std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: u64 = 0;
// LOWERING-NEXT:         let _v48: u64 = 0;
// LOWERING-NEXT:         index = _v48;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v49: u64 = index;
// LOWERING-NEXT:             let _v50: u64 = 4;
// LOWERING-NEXT:             let _v51: bool = _v49 < _v50;
// LOWERING-NEXT:             if !_v51 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let mut value: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:                 let _v52: u64 = 4;
// LOWERING-NEXT:                 let _v53: *mut core::ffi::c_void = unsafe { malloc(_v52 as usize) };
// LOWERING-NEXT:                 let _v54: *mut i32 = _v53 as *mut i32;
// LOWERING-NEXT:                 value = _v54;
// LOWERING-NEXT:                 let _v55: u64 = index;
// LOWERING-NEXT:                 let _v56: i32 = values[(_v55 as usize)];
// LOWERING-NEXT:                 let _v57: *mut i32 = value;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *_v57 = _v56;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v58: *mut i32 = value;
// LOWERING-NEXT:                 let _v59: *mut core::ffi::c_void = _v58 as *mut core::ffi::c_void;
// LOWERING-NEXT:                 let _v60: *mut core::ffi::c_void = unsafe { tsearch(_v59 as *const core::ffi::c_void, std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void, Some(gnu_compare_entries)) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v61: u64 = index;
// LOWERING-NEXT:             let _v62: u64 = _v61 + 1;
// LOWERING-NEXT:             index = _v62;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v63: i64 = 2;
// LOWERING-NEXT:     let _v64: *mut core::ffi::c_void = std::ptr::addr_of_mut!(values[(_v63 as usize)]) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v65: *mut core::ffi::c_void = unsafe { tfind(_v64 as *const core::ffi::c_void, std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void, Some(gnu_compare_entries)) };
// LOWERING-NEXT:     let _v66: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v67: bool = _v65 != _v66;
// LOWERING-NEXT:     let _v68: i32 = _v67 as i32;
// LOWERING-NEXT:     let _v69: i32 = total;
// LOWERING-NEXT:     let _v70: i32 = _v69 + _v68;
// LOWERING-NEXT:     total = _v70;
// LOWERING-NEXT:     let _v71: *mut core::ffi::c_void = tree;
// LOWERING-NEXT:     unsafe { tdestroy(_v71 as *mut core::ffi::c_void, Some(gnu_free_entry)) };
// LOWERING-NEXT:     let _v72: i32 = total;
// LOWERING-NEXT:     __retval = _v72;
// LOWERING-NEXT:     let _v73: i32 = __retval;
// LOWERING-NEXT:     return _v73;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = gnu_qsort_extension();
// LOWERING-NEXT:     let _v3: i32 = gnu_getopt_extensions();
// LOWERING-NEXT:     let _v4: i32 = gnu_argp_extensions();
// LOWERING-NEXT:     let _v5: i32 = gnu_search_extensions();
// LOWERING-NEXT:     let _v6: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5) };
// LOWERING-NEXT:     let _v7: i32 = 0;
// LOWERING-NEXT:     __retval = _v7;
// LOWERING-NEXT:     let _v8: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v8 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum ACTION {
// REWRITES-NEXT:     FIND = 0,
// REWRITES-NEXT:     ENTER = 1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum VISIT {
// REWRITES-NEXT:     preorder = 0,
// REWRITES-NEXT:     postorder = 1,
// REWRITES-NEXT:     endorder = 2,
// REWRITES-NEXT:     leaf = 3,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct GNUArguments {
// REWRITES-NEXT:     number: i32,
// REWRITES-NEXT:     positional: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __tab {
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct anon_struct {
// REWRITES-NEXT:     __slate_anon_0: *mut core::ffi::c_void,
// REWRITES-NEXT:     __slate_anon_1: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct argp {
// REWRITES-NEXT:     options: *mut argp_option,
// REWRITES-NEXT:     parser: Option<unsafe extern "C" fn(i32, *mut i8, *mut argp_state) -> i32>,
// REWRITES-NEXT:     args_doc: *mut i8,
// REWRITES-NEXT:     doc: *mut i8,
// REWRITES-NEXT:     children: *mut argp_child,
// REWRITES-NEXT:     help_filter: Option<unsafe extern "C" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8>,
// REWRITES-NEXT:     argp_domain: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct argp_child {
// REWRITES-NEXT:     argp: *mut argp,
// REWRITES-NEXT:     flags: i32,
// REWRITES-NEXT:     header: *mut i8,
// REWRITES-NEXT:     group: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct argp_option {
// REWRITES-NEXT:     name: *mut i8,
// REWRITES-NEXT:     key: i32,
// REWRITES-NEXT:     arg: *mut i8,
// REWRITES-NEXT:     flags: i32,
// REWRITES-NEXT:     doc: *mut i8,
// REWRITES-NEXT:     group: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct argp_state {
// REWRITES-NEXT:     root_argp: *mut argp,
// REWRITES-NEXT:     argc: i32,
// REWRITES-NEXT:     argv: *mut *mut i8,
// REWRITES-NEXT:     next: i32,
// REWRITES-NEXT:     flags: u32,
// REWRITES-NEXT:     arg_num: u32,
// REWRITES-NEXT:     quoted: i32,
// REWRITES-NEXT:     input: *mut core::ffi::c_void,
// REWRITES-NEXT:     child_inputs: *mut *mut core::ffi::c_void,
// REWRITES-NEXT:     hook: *mut core::ffi::c_void,
// REWRITES-NEXT:     name: *mut i8,
// REWRITES-NEXT:     err_stream: *mut libc::FILE,
// REWRITES-NEXT:     out_stream: *mut libc::FILE,
// REWRITES-NEXT:     pstate: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct entry {
// REWRITES-NEXT:     key: *mut i8,
// REWRITES-NEXT:     data: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct hsearch_data {
// REWRITES-NEXT:     __tab: *mut __tab,
// REWRITES-NEXT:     __unused1: u32,
// REWRITES-NEXT:     __unused2: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct option {
// REWRITES-NEXT:     name: *mut i8,
// REWRITES-NEXT:     has_arg: i32,
// REWRITES-NEXT:     flag: *mut i32,
// REWRITES-NEXT:     val: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     static mut optarg: *mut i8;
// REWRITES-NEXT:     static mut opterr: i32;
// REWRITES-NEXT:     static mut optind: i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn qsort_r(_0: *mut core::ffi::c_void, _1: usize, _2: usize, _3: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut core::ffi::c_void, *mut core::ffi::c_void) -> i32>, _4: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn getopt_long(_0: i32, _1: *mut *mut i8, _2: *const i8, _3: *const option, _4: *mut i32) -> i32;
// REWRITES-NEXT:     fn atoi(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// REWRITES-NEXT:     fn argp_parse(_0: *const argp, _1: i32, _2: *mut *mut i8, _3: u32, _4: *mut i32, _5: *mut core::ffi::c_void) -> i32;
// REWRITES-NEXT:     fn hcreate_r(_0: usize, _1: *mut hsearch_data) -> i32;
// REWRITES-NEXT:     fn hsearch_r(_0: *mut core::ffi::c_void, _1: *mut core::ffi::c_void, _2: u32, _3: *mut *mut entry, _4: *mut hsearch_data) -> i32;
// REWRITES-NEXT:     fn hdestroy_r(_0: *mut hsearch_data);
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn tsearch(_0: *const core::ffi::c_void, _1: *mut *mut core::ffi::c_void, _2: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32>) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn tfind(_0: *const core::ffi::c_void, _1: *mut *mut core::ffi::c_void, _2: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32>) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn tdestroy(_0: *mut core::ffi::c_void, _1: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>);
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn gnu_compare_with_direction(arg6: *mut core::ffi::c_void, arg7: *mut core::ffi::c_void, arg8: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT: let mut left: *mut core::ffi::c_void = arg6;
// REWRITES-NEXT: let mut right: *mut core::ffi::c_void = arg7;
// REWRITES-NEXT: let mut state: *mut core::ffi::c_void = arg8;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut direction: i32 = 0;
// REWRITES-NEXT: let mut a: i32 = 0;
// REWRITES-NEXT: let mut b: i32 = 0;
// REWRITES-NEXT: direction = unsafe { *(state as *mut i32) };
// REWRITES-NEXT: a = unsafe { *(left as *mut i32) };
// REWRITES-NEXT: b = unsafe { *(right as *mut i32) };
// REWRITES-NEXT: __retval = direction * (((a > b) as i32) - ((a < b) as i32));
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_qsort_extension() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut direction: i32 = 0;
// REWRITES-NEXT: *values = [4, 1, 3, 2];
// REWRITES-NEXT: direction = -1;
// REWRITES-NEXT: let _v1: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v3: u64 = 4;
// REWRITES-NEXT: let _v4: u64 = 4;
// REWRITES-NEXT: let _v5: *mut core::ffi::c_void = std::ptr::addr_of_mut!(direction) as *mut core::ffi::c_void;
// REWRITES-NEXT: unsafe { qsort_r(_v2 as *mut core::ffi::c_void, _v3 as usize, _v4 as usize, Some(gnu_compare_with_direction), _v5 as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v6: i64 = 0;
// REWRITES-NEXT: let _v8: i32 = 1000;
// REWRITES-NEXT: let _v10: i64 = 1;
// REWRITES-NEXT: let _v12: i32 = 100;
// REWRITES-NEXT: let _v15: i64 = 2;
// REWRITES-NEXT: let _v17: i32 = 10;
// REWRITES-NEXT: let _v20: i64 = 3;
// REWRITES-NEXT: __retval = values[(_v6 as usize)] * _v8 + values[(_v10 as usize)] * _v12 + values[(_v15 as usize)] * _v17 + values[(_v20 as usize)];
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_getopt_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut program: [i8; 6] = [112, 114, 111, 98, 101, 0];
// REWRITES-NEXT: let mut number: [i8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// REWRITES-NEXT: let mut flag: [i8; 3] = [45, 102, 0];
// REWRITES-NEXT: let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> = aligned::Aligned([std::ptr::null_mut(); 4]);
// REWRITES-NEXT: let mut options: aligned::Aligned<aligned::A16, [option; 3]> = aligned::Aligned([option { name: std::ptr::null_mut(), has_arg: 0, flag: std::ptr::null_mut(), val: 0 }; 3]);
// REWRITES-NEXT: let mut number_value: i32 = 0;
// REWRITES-NEXT: let mut flag_value: i32 = 0;
// REWRITES-NEXT: let mut option: i32 = 0;
// REWRITES-NEXT: let _v0: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-NEXT: let _v1: *mut i8 = program.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v0 = _v1;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v2: i64 = 1;
// REWRITES-NEXT: let _v3: *mut *mut i8 = unsafe { _v0.add(1) };
// REWRITES-NEXT: let _v4: *mut i8 = number.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v3 = _v4;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v5: i64 = 2;
// REWRITES-NEXT: let _v6: *mut *mut i8 = unsafe { _v0.add(2) };
// REWRITES-NEXT: let _v7: *mut i8 = flag.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v6 = _v7;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v8: i64 = 3;
// REWRITES-NEXT: let _v9: *mut *mut i8 = unsafe { _v0.add(3) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v9 = std::ptr::null_mut();
// REWRITES-NEXT: }
// REWRITES-NEXT: *options = [option { name: b"number\0".as_ptr() as *mut i8, has_arg: 1, flag: std::ptr::null_mut(), val: 110 }, option { name: b"flag\0".as_ptr() as *mut i8, has_arg: 0, flag: std::ptr::null_mut(), val: 102 }, option { name: std::ptr::null_mut(), has_arg: 0, flag: std::ptr::null_mut(), val: 0 }];
// REWRITES-NEXT: number_value = 0;
// REWRITES-NEXT: flag_value = 0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         optind = 1;
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         opterr = 0;
// REWRITES-NEXT: }
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let _v15: i32 = 3;
// REWRITES-NEXT:         let _v16: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-NEXT:         let _v17: *mut i8 = b"fn:\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let _v18: *mut option = options.as_mut_ptr() as *mut option;
// REWRITES-NEXT:         let _v19: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT:         let _v20: i32 = unsafe { getopt_long(_v15 as i32, _v16 as *mut *mut i8, _v17 as *const i8, _v18 as *const option, _v19 as *mut i32) };
// REWRITES-NEXT:         option = _v20;
// REWRITES-NEXT:         let _v21: i32 = -1;
// REWRITES-NEXT:         if !(_v20 != _v21) {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v24: i32 = 110;
// REWRITES-NEXT:                                     let _v25: bool = option == _v24;
// REWRITES-NEXT:                                     if _v25 {
// REWRITES-NEXT:                                                         number_value = unsafe { atoi((unsafe { optarg }) as *const i8) };
// REWRITES-NEXT:                                     } else {
// REWRITES-NEXT:                                                         {
// REWRITES-NEXT:                                                                                 let _v29: i32 = 102;
// REWRITES-NEXT:                                                                                 if option == _v29 {
// REWRITES-NEXT:                                                                                                             flag_value = 1;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v33: i32 = 10;
// REWRITES-NEXT: __retval = number_value * _v33 + flag_value;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn gnu_parse_option(arg3: i32, arg4: *mut i8, arg5: *mut argp_state) -> i32 {
// REWRITES-NEXT: let mut key: i32 = arg3;
// REWRITES-NEXT: let mut argument: *mut i8 = arg4;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut arguments: *mut GNUArguments = std::ptr::null_mut();
// REWRITES-NEXT: arguments = (unsafe { (*arg5).input }) as *mut GNUArguments;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v4: i32 = 110;
// REWRITES-NEXT:         let _v5: bool = key == _v4;
// REWRITES-NEXT:         if _v5 {
// REWRITES-NEXT:                     let _v7: i32 = unsafe { atoi(argument as *const i8) };
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     (*arguments).number = _v7;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     return __retval;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v12: i32 = 0;
// REWRITES-NEXT:         let _v13: bool = key == _v12;
// REWRITES-NEXT:         if _v13 {
// REWRITES-NEXT:                     let _v14: *mut i8 = argument;
// REWRITES-NEXT:                     let _v15: *mut i8 = b"item\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v16: i32 = unsafe { strcmp(_v14 as *const i8, _v15 as *const i8) };
// REWRITES-NEXT:                     let _v17: i32 = 0;
// REWRITES-NEXT:                     let _v20: *mut GNUArguments = arguments;
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     (*_v20).positional = (unsafe { (*_v20).positional }) + ((_v16 == _v17) as i32);
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     return __retval;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v26: i32 = 16777217;
// REWRITES-NEXT:         let _v28: bool = if key == _v26 {
// REWRITES-NEXT:                     let _v29: bool = true;
// REWRITES-NEXT:             _v29
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:                     let _v31: i32 = 16777219;
// REWRITES-NEXT:                     let _v32: bool = key == _v31;
// REWRITES-NEXT:             _v32
// REWRITES-NEXT:         };
// REWRITES-NEXT:         let _v33: bool = if _v28 {
// REWRITES-NEXT:                     let _v34: bool = true;
// REWRITES-NEXT:             _v34
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:                     let _v36: i32 = 16777218;
// REWRITES-NEXT:                     let _v37: bool = key == _v36;
// REWRITES-NEXT:             _v37
// REWRITES-NEXT:         };
// REWRITES-NEXT:         let _v38: bool = if _v33 {
// REWRITES-NEXT:                     let _v39: bool = true;
// REWRITES-NEXT:             _v39
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:                     let _v41: i32 = 16777220;
// REWRITES-NEXT:                     let _v42: bool = key == _v41;
// REWRITES-NEXT:             _v42
// REWRITES-NEXT:         };
// REWRITES-NEXT:         let _v43: bool = if _v38 {
// REWRITES-NEXT:                     let _v44: bool = true;
// REWRITES-NEXT:             _v44
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:                     let _v46: i32 = 16777223;
// REWRITES-NEXT:                     let _v47: bool = key == _v46;
// REWRITES-NEXT:             _v47
// REWRITES-NEXT:         };
// REWRITES-NEXT:         if _v43 {
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     return __retval;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = 7;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_argp_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut options: aligned::Aligned<aligned::A16, [argp_option; 2]> = aligned::Aligned([argp_option { name: std::ptr::null_mut(), key: 0, arg: std::ptr::null_mut(), flags: 0, doc: std::ptr::null_mut(), group: 0 }; 2]);
// REWRITES-NEXT: let mut parser: argp = argp { options: std::ptr::null_mut(), parser: None, args_doc: std::ptr::null_mut(), doc: std::ptr::null_mut(), children: std::ptr::null_mut(), help_filter: None, argp_domain: std::ptr::null_mut() };
// REWRITES-NEXT: let mut parsed: GNUArguments = GNUArguments { number: 0, positional: 0 };
// REWRITES-NEXT: let mut program: [i8; 6] = [0; 6];
// REWRITES-NEXT: let mut option: [i8; 11] = [0; 11];
// REWRITES-NEXT: let mut item: [i8; 5] = [0; 5];
// REWRITES-NEXT: let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> = aligned::Aligned([std::ptr::null_mut(); 4]);
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: *options = [argp_option { name: b"number\0".as_ptr() as *mut i8, key: 110, arg: b"VALUE\0".as_ptr() as *mut i8, flags: 0, doc: b"number\0".as_ptr() as *mut i8, group: 0 }, argp_option { name: std::ptr::null_mut(), key: 0, arg: std::ptr::null_mut(), flags: 0, doc: std::ptr::null_mut(), group: 0 }];
// REWRITES-NEXT: let _v0: *mut argp_option = options.as_mut_ptr() as *mut argp_option;
// REWRITES-NEXT: parser.options = _v0;
// REWRITES-NEXT: parser.parser = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32, *mut i8, *mut argp_state) -> i32>>(gnu_parse_option as *const ()) };
// REWRITES-NEXT: let _v1: *mut i8 = b"ITEM\0".as_ptr() as *mut i8;
// REWRITES-NEXT: parser.args_doc = _v1;
// REWRITES-NEXT: parser.doc = std::ptr::null_mut();
// REWRITES-NEXT: parser.children = std::ptr::null_mut();
// REWRITES-NEXT: parser.help_filter = None;
// REWRITES-NEXT: parser.argp_domain = std::ptr::null_mut();
// REWRITES-NEXT: parsed = GNUArguments { number: 0, positional: 0 };
// REWRITES-NEXT: program = [112, 114, 111, 98, 101, 0];
// REWRITES-NEXT: option = [45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0];
// REWRITES-NEXT: item = [105, 116, 101, 109, 0];
// REWRITES-NEXT: let _v6: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-NEXT: let _v7: *mut i8 = program.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v6 = _v7;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v8: i64 = 1;
// REWRITES-NEXT: let _v9: *mut *mut i8 = unsafe { _v6.add(1) };
// REWRITES-NEXT: let _v10: *mut i8 = option.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v9 = _v10;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v11: i64 = 2;
// REWRITES-NEXT: let _v12: *mut *mut i8 = unsafe { _v6.add(2) };
// REWRITES-NEXT: let _v13: *mut i8 = item.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v12 = _v13;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v14: i64 = 3;
// REWRITES-NEXT: let _v15: *mut *mut i8 = unsafe { _v6.add(3) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v15 = std::ptr::null_mut();
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v17: i32 = 3;
// REWRITES-NEXT: let _v18: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-NEXT: let _v19: i32 = 32;
// REWRITES-NEXT: let _v20: i32 = 16;
// REWRITES-NEXT: let _v22: u32 = (_v19 | _v20) as u32;
// REWRITES-NEXT: let _v23: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let _v24: *mut core::ffi::c_void = std::ptr::addr_of_mut!(parsed) as *mut core::ffi::c_void;
// REWRITES-NEXT: result = unsafe { argp_parse(std::ptr::addr_of_mut!(parser) as *const argp, _v17 as i32, _v18 as *mut *mut i8, _v22 as u32, _v23 as *mut i32, _v24 as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v27: i32 = 0;
// REWRITES-NEXT: let _v31: i32 = 10;
// REWRITES-NEXT: __retval = ((result == _v27) as i32) + parsed.number * _v31 + parsed.positional;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn gnu_compare_entries(arg1: *mut core::ffi::c_void, arg2: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT: let mut left: *mut core::ffi::c_void = arg1;
// REWRITES-NEXT: let mut right: *mut core::ffi::c_void = arg2;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: i32 = 0;
// REWRITES-NEXT: let mut b: i32 = 0;
// REWRITES-NEXT: a = unsafe { *(left as *mut i32) };
// REWRITES-NEXT: b = unsafe { *(right as *mut i32) };
// REWRITES-NEXT: __retval = ((a > b) as i32) - ((a < b) as i32);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn gnu_free_entry(arg0: *mut core::ffi::c_void) {
// REWRITES-NEXT: let mut entry: *mut core::ffi::c_void = arg0;
// REWRITES-NEXT: unsafe { free(entry as *mut core::ffi::c_void) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_search_extensions() -> i32 {
// REWRITES-NEXT: let mut coerce: entry = entry { key: std::ptr::null_mut(), data: std::ptr::null_mut() };
// REWRITES-NEXT: let mut coerce2: entry = entry { key: std::ptr::null_mut(), data: std::ptr::null_mut() };
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut table: hsearch_data = hsearch_data { __tab: std::ptr::null_mut(), __unused1: 0, __unused2: 0 };
// REWRITES-NEXT: let mut inserted: entry = entry { key: b"slate\0".as_ptr() as *mut i8, data: b"24\0".as_ptr() as *mut core::ffi::c_void };
// REWRITES-NEXT: let mut query: entry = entry { key: b"slate\0".as_ptr() as *mut i8, data: std::ptr::null_mut() };
// REWRITES-NEXT: let mut found: *mut entry = std::ptr::null_mut();
// REWRITES-NEXT: let mut tree: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: found = std::ptr::null_mut();
// REWRITES-NEXT: tree = std::ptr::null_mut();
// REWRITES-NEXT: *values = [3, 1, 4, 2];
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: let _v3: u64 = 8;
// REWRITES-NEXT: let _v4: i32 = unsafe { hcreate_r(_v3 as usize, std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// REWRITES-NEXT: let _v5: i32 = 0;
// REWRITES-NEXT: total = total + ((_v4 != _v5) as i32);
// REWRITES-NEXT: let _v10: u32 = ACTION::ENTER as u32;
// REWRITES-NEXT: coerce2 = inserted;
// REWRITES-NEXT: let _v12: *mut anon_struct = std::ptr::addr_of_mut!(coerce2) as *mut anon_struct;
// REWRITES-NEXT: let _v13: *mut core::ffi::c_void = unsafe { (*_v12).__slate_anon_0 };
// REWRITES-NEXT: let _v14: *mut core::ffi::c_void = unsafe { (*_v12).__slate_anon_1 };
// REWRITES-NEXT: let _v15: i32 = unsafe { hsearch_r(_v13 as *mut core::ffi::c_void, _v14 as *mut core::ffi::c_void, _v10 as u32, std::ptr::addr_of_mut!(found) as *mut *mut entry, std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// REWRITES-NEXT: let _v16: i32 = 0;
// REWRITES-NEXT: total = total + ((_v15 != _v16) as i32);
// REWRITES-NEXT: let _v21: u32 = ACTION::FIND as u32;
// REWRITES-NEXT: coerce = query;
// REWRITES-NEXT: let _v23: *mut anon_struct = std::ptr::addr_of_mut!(coerce) as *mut anon_struct;
// REWRITES-NEXT: let _v24: *mut core::ffi::c_void = unsafe { (*_v23).__slate_anon_0 };
// REWRITES-NEXT: let _v25: *mut core::ffi::c_void = unsafe { (*_v23).__slate_anon_1 };
// REWRITES-NEXT: let _v26: i32 = unsafe { hsearch_r(_v24 as *mut core::ffi::c_void, _v25 as *mut core::ffi::c_void, _v21 as u32, std::ptr::addr_of_mut!(found) as *mut *mut entry, std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// REWRITES-NEXT: let _v27: i32 = 0;
// REWRITES-NEXT: total = total + ((_v26 != _v27) as i32);
// REWRITES-NEXT: let _v33: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v35: bool = found != (_v33 as *mut entry);
// REWRITES-NEXT: let _v36: bool = if _v35 {
// REWRITES-NEXT:         let _v39: *mut i8 = (unsafe { (*found).data }) as *mut i8;
// REWRITES-NEXT:         let _v40: *mut i8 = b"24\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let _v41: i32 = unsafe { strcmp(_v39 as *const i8, _v40 as *const i8) };
// REWRITES-NEXT:         let _v42: i32 = 0;
// REWRITES-NEXT:         let _v43: bool = _v41 == _v42;
// REWRITES-NEXT:     _v43
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v44: bool = false;
// REWRITES-NEXT:     _v44
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v36 as i32);
// REWRITES-NEXT: unsafe { hdestroy_r(std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut index: u64 = 0;
// REWRITES-NEXT:         index = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v50: u64 = 4;
// REWRITES-NEXT:                     if !(index < _v50) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let mut value: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT:                                     let _v52: u64 = 4;
// REWRITES-NEXT:                                     let _v53: *mut core::ffi::c_void = unsafe { malloc(_v52 as usize) };
// REWRITES-NEXT:                                     value = _v53 as *mut i32;
// REWRITES-NEXT:                                     unsafe {
// REWRITES-NEXT:                                                         *value = values[(index as usize)];
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     let _v60: *mut core::ffi::c_void = unsafe { tsearch((value as *mut core::ffi::c_void) as *const core::ffi::c_void, std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void, Some(gnu_compare_entries)) };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     index = index + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v63: i64 = 2;
// REWRITES-NEXT: let _v64: *mut core::ffi::c_void = std::ptr::addr_of_mut!(values[(_v63 as usize)]) as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v65: *mut core::ffi::c_void = unsafe { tfind(_v64 as *const core::ffi::c_void, std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void, Some(gnu_compare_entries)) };
// REWRITES-NEXT: let _v66: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v65 != _v66) as i32);
// REWRITES-NEXT: unsafe { tdestroy(tree as *mut core::ffi::c_void, Some(gnu_free_entry)) };
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = gnu_qsort_extension();
// REWRITES-NEXT: let _v3: i32 = gnu_getopt_extensions();
// REWRITES-NEXT: let _v4: i32 = gnu_argp_extensions();
// REWRITES-NEXT: let _v5: i32 = gnu_search_extensions();
// REWRITES-NEXT: let _v6: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
