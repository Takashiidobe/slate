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

// REWRITES: fn __slate_atoi(s: *const i8) -> i32 {
// REWRITES: __slate_atoi(argument as *const i8)

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
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum ACTION {
// COMMON-LOWERING-NEXT:     FIND = 0,
// COMMON-LOWERING-NEXT:     ENTER = 1,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum VISIT {
// COMMON-LOWERING-NEXT:     preorder = 0,
// COMMON-LOWERING-NEXT:     postorder = 1,
// COMMON-LOWERING-NEXT:     endorder = 2,
// COMMON-LOWERING-NEXT:     leaf = 3,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct GNUArguments {
// COMMON-LOWERING-NEXT:     number: i32,
// COMMON-LOWERING-NEXT:     positional: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct __tab {
// COMMON-LOWERING-NEXT:     __slate_empty: [u8; 0],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct argp {
// COMMON-LOWERING-NEXT:     options: *mut argp_option,
// COMMON-LOWERING-NEXT:     children: *mut argp_child,
// COMMON-LOWERING-NEXT:     help_filter:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct argp_child {
// COMMON-LOWERING-NEXT:     argp: *mut argp,
// COMMON-LOWERING-NEXT:     flags: i32,
// COMMON-LOWERING-NEXT:     group: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct argp_option {
// COMMON-LOWERING-NEXT:     key: i32,
// COMMON-LOWERING-NEXT:     flags: i32,
// COMMON-LOWERING-NEXT:     group: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct argp_state {
// COMMON-LOWERING-NEXT:     root_argp: *mut argp,
// COMMON-LOWERING-NEXT:     argc: i32,
// COMMON-LOWERING-NEXT:     next: i32,
// COMMON-LOWERING-NEXT:     flags: u32,
// COMMON-LOWERING-NEXT:     arg_num: u32,
// COMMON-LOWERING-NEXT:     quoted: i32,
// COMMON-LOWERING-NEXT:     input: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     child_inputs: *mut *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     hook: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     err_stream: *mut libc::FILE,
// COMMON-LOWERING-NEXT:     out_stream: *mut libc::FILE,
// COMMON-LOWERING-NEXT:     pstate: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct entry {
// COMMON-LOWERING-NEXT:     data: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct hsearch_data {
// COMMON-LOWERING-NEXT:     __tab: *mut __tab,
// COMMON-LOWERING-NEXT:     __unused1: u32,
// COMMON-LOWERING-NEXT:     __unused2: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct option {
// COMMON-LOWERING-NEXT:     has_arg: i32,
// COMMON-LOWERING-NEXT:     flag: *mut i32,
// COMMON-LOWERING-NEXT:     val: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     static mut opterr: i32;
// COMMON-LOWERING-NEXT:     static mut optind: i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn qsort_r(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: usize,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:         _3: Option<
// COMMON-LOWERING-NEXT:             unsafe extern "C-unwind" fn(
// COMMON-LOWERING-NEXT:                 *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                 *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                 *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             ) -> i32,
// COMMON-LOWERING-NEXT:         >,
// COMMON-LOWERING-NEXT:         _4: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     fn getopt_long(
// COMMON-LOWERING-NEXT:         _0: i32,
// COMMON-LOWERING-NEXT:         _1: *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _3: *const option,
// COMMON-LOWERING-NEXT:         _4: *mut i32,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn argp_parse(
// COMMON-LOWERING-NEXT:         _0: *const argp,
// COMMON-LOWERING-NEXT:         _1: i32,
// COMMON-LOWERING-NEXT:         _2: *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _3: u32,
// COMMON-LOWERING-NEXT:         _4: *mut i32,
// COMMON-LOWERING-NEXT:         _5: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn hcreate_r(_0: usize, _1: *mut hsearch_data) -> i32;
// COMMON-LOWERING-NEXT:     fn hsearch_r(_0: entry, _1: u32, _2: *mut *mut entry, _3: *mut hsearch_data) -> i32;
// COMMON-LOWERING-NEXT:     fn hdestroy_r(_0: *mut hsearch_data);
// COMMON-LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn tsearch(
// COMMON-LOWERING-NEXT:         _0: *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: *mut *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _2: Option<
// COMMON-LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-LOWERING-NEXT:         >,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn tfind(
// COMMON-LOWERING-NEXT:         _0: *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: *mut *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _2: Option<
// COMMON-LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-LOWERING-NEXT:         >,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn tdestroy(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_qsort_extension();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_getopt_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_argp_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_search_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_qsort_extension() -> i32 {
// COMMON-LOWERING-NEXT:     let mut direction: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [4, 1, 3, 2];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:     direction = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(direction) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         qsort_r(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             Some(gnu_compare_with_direction),
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1000;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_getopt_extensions() -> i32 {
// COMMON-LOWERING-NEXT:         [option {
// COMMON-LOWERING-NEXT:             name: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             has_arg: 0,
// COMMON-LOWERING-NEXT:             flag: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             val: 0,
// COMMON-LOWERING-NEXT:         }; 3],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let mut number_value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut flag_value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut option: i32 = 0;
// COMMON-LOWERING-NEXT:     program = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     number = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     flag = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [option; 3] = [
// COMMON-LOWERING-NEXT:         option {
// COMMON-LOWERING-NEXT:             has_arg: 1,
// COMMON-LOWERING-NEXT:             flag: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             val: 110,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         option {
// COMMON-LOWERING-NEXT:             has_arg: 0,
// COMMON-LOWERING-NEXT:             flag: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             val: 102,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         option {
// COMMON-LOWERING-NEXT:             name: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             has_arg: 0,
// COMMON-LOWERING-NEXT:             flag: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             val: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     *options = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     number_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     flag_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         optind = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         opterr = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut option = options.as_mut_ptr() as *mut option;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:                 getopt_long(
// COMMON-LOWERING-NEXT:                     {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:                     {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                     {{__v[0-9]+}} as *const option,
// COMMON-LOWERING-NEXT:                     {{__v[0-9]+}} as *mut i32,
// COMMON-LOWERING-NEXT:                 )
// COMMON-LOWERING-NEXT:             };
// COMMON-LOWERING-NEXT:             option = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = option;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 110;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = unsafe { atoi({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:                         number_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     } else {
// COMMON-LOWERING-NEXT:                         {
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = option;
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = 102;
// COMMON-LOWERING-NEXT:                             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                             if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                                 flag_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                             }
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = number_value;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = flag_value;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_argp_extensions() -> i32 {
// COMMON-LOWERING-NEXT:         [argp_option {
// COMMON-LOWERING-NEXT:             name: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             key: 0,
// COMMON-LOWERING-NEXT:             arg: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             flags: 0,
// COMMON-LOWERING-NEXT:             doc: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             group: 0,
// COMMON-LOWERING-NEXT:         }; 2],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let mut parser: argp = argp {
// COMMON-LOWERING-NEXT:         options: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         parser: None,
// COMMON-LOWERING-NEXT:         args_doc: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         doc: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         children: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         help_filter: None,
// COMMON-LOWERING-NEXT:         argp_domain: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut parsed: GNUArguments = GNUArguments {
// COMMON-LOWERING-NEXT:         number: 0,
// COMMON-LOWERING-NEXT:         positional: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [argp_option; 2] = [
// COMMON-LOWERING-NEXT:         argp_option {
// COMMON-LOWERING-NEXT:             key: 110,
// COMMON-LOWERING-NEXT:             flags: 0,
// COMMON-LOWERING-NEXT:             group: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         argp_option {
// COMMON-LOWERING-NEXT:             name: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             key: 0,
// COMMON-LOWERING-NEXT:             arg: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             flags: 0,
// COMMON-LOWERING-NEXT:             doc: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             group: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     *options = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut argp_option = options.as_mut_ptr() as *mut argp_option;
// COMMON-LOWERING-NEXT:     parser.options = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     parser.parser = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<
// COMMON-LOWERING-NEXT:             *const (),
// COMMON-LOWERING-NEXT:         >(gnu_parse_option as *const ())
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     parser.args_doc = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     parser.doc = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut argp_child = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     parser.children = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         None;
// COMMON-LOWERING-NEXT:     parser.help_filter = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     parser.argp_domain = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: GNUArguments = GNUArguments {
// COMMON-LOWERING-NEXT:         number: 0,
// COMMON-LOWERING-NEXT:         positional: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     parsed = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     program = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     option = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     item = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(parsed) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         argp_parse(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(parser) as *const argp,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = parsed.number;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = parsed.positional;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_search_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut table: hsearch_data = hsearch_data {
// COMMON-LOWERING-NEXT:         __tab: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         __unused1: 0,
// COMMON-LOWERING-NEXT:         __unused2: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut found: *mut entry = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut tree: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: hsearch_data = hsearch_data {
// COMMON-LOWERING-NEXT:         __tab: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         __unused1: 0,
// COMMON-LOWERING-NEXT:         __unused2: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     table = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: entry = entry {
// COMMON-LOWERING-NEXT:         data: b"24\0".as_ptr() as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: entry = entry {
// COMMON-LOWERING-NEXT:         data: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut entry = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     found = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     tree = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [3, 1, 4, 2];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         hcreate_r(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = ACTION::ENTER as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         hsearch_r(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as entry,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(found) as *mut *mut entry,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = ACTION::FIND as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         hsearch_r(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as entry,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(found) as *mut *mut entry,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut entry = found;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut entry = {{__v[0-9]+}} as *mut entry;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut entry = found;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{__v[0-9]+}}).data };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:             strcmp(
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe { hdestroy_r(std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut index: u64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:                     tsearch(
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         Some(gnu_compare_entries),
// COMMON-LOWERING-NEXT:                     )
// COMMON-LOWERING-NEXT:                 };
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)]) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         tfind(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             Some(gnu_compare_entries),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = tree;
// COMMON-LOWERING-NEXT:     unsafe { tdestroy({{__v[0-9]+}} as *mut core::ffi::c_void, Some(gnu_free_entry)) };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn gnu_compare_with_direction(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: ) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut key: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut arguments: *mut GNUArguments = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     key = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     argument = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{arg[0-9]+}}).input };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut GNUArguments = {{__v[0-9]+}} as *mut GNUArguments;
// COMMON-LOWERING-NEXT:     arguments = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = key;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 110;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { atoi({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut GNUArguments = arguments;
// COMMON-LOWERING-NEXT:             unsafe {
// COMMON-LOWERING-NEXT:                 (*{{__v[0-9]+}}).number = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = key;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:                 strcmp(
// COMMON-LOWERING-NEXT:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 )
// COMMON-LOWERING-NEXT:             };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut GNUArguments = arguments;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).positional };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             unsafe {
// COMMON-LOWERING-NEXT:                 (*{{__v[0-9]+}}).positional = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = key;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 16777217;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = key;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 16777219;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = key;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 16777218;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = key;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 16777220;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = key;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 16777223;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn gnu_compare_entries(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: ) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn gnu_free_entry({{arg[0-9]+}}: *mut core::ffi::c_void) {
// COMMON-LOWERING-NEXT:     unsafe { free({{arg[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     parser: Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// LOWERING-X86_64-GNU-NEXT:     args_doc: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     doc: *mut i8,
// LOWERING-X86_64-GNU-NEXT:         Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8>,
// LOWERING-X86_64-GNU-NEXT:     argp_domain: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     header: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     name: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     arg: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     doc: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     argv: *mut *mut i8,
// LOWERING-X86_64-GNU-NEXT:     name: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     key: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     name: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     static mut optarg: *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:     *values = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let mut program: [i8; 6] = [0; 6];
// LOWERING-X86_64-GNU-NEXT:     let mut number: [i8; 11] = [0; 11];
// LOWERING-X86_64-GNU-NEXT:     let mut flag: [i8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> =
// LOWERING-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 4]);
// LOWERING-X86_64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A16, [option; 3]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 6] = [112, 114, 111, 98, 101, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 3] = [45, 102, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = program.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = number.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = flag.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:             name: b"number\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             name: b"flag\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"fn:\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut i8 = unsafe { optarg };
// LOWERING-X86_64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A16, [argp_option; 2]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:     let mut program: [i8; 6] = [0; 6];
// LOWERING-X86_64-GNU-NEXT:     let mut option: [i8; 11] = [0; 11];
// LOWERING-X86_64-GNU-NEXT:     let mut item: [i8; 5] = [0; 5];
// LOWERING-X86_64-GNU-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> =
// LOWERING-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 4]);
// LOWERING-X86_64-GNU-NEXT:             name: b"number\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             arg: b"VALUE\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             doc: b"number\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ITEM\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8> =
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 6] = [112, 114, 111, 98, 101, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 5] = [105, 116, 101, 109, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = program.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = option.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = item.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:         key: b"slate\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:         key: b"slate\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:     *values = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"24\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: extern "C-unwind" fn gnu_parse_option({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut argp_state) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut argument: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = argument;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = argument;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"item\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     parser: Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut argp_state) -> i32>,
// LOWERING-AARCH64-GNU-NEXT:     args_doc: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     doc: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:         Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut core::ffi::c_void) -> *mut u8>,
// LOWERING-AARCH64-GNU-NEXT:     argp_domain: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     header: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     name: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     arg: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     doc: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     argv: *mut *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     name: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     key: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     name: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     static mut optarg: *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     values = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let mut program: [u8; 6] = [0; 6];
// LOWERING-AARCH64-GNU-NEXT:     let mut number: [u8; 11] = [0; 11];
// LOWERING-AARCH64-GNU-NEXT:     let mut flag: [u8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let mut arguments: [*mut u8; 4] = [std::ptr::null_mut(); 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A8, [option; 3]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 6] = [112, 114, 111, 98, 101, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [45, 102, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = program.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = number.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = flag.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:             name: b"number\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             name: b"flag\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"fn:\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut u8 = unsafe { optarg };
// LOWERING-AARCH64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A8, [argp_option; 2]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:     let mut program: [u8; 6] = [0; 6];
// LOWERING-AARCH64-GNU-NEXT:     let mut option: [u8; 11] = [0; 11];
// LOWERING-AARCH64-GNU-NEXT:     let mut item: [u8; 5] = [0; 5];
// LOWERING-AARCH64-GNU-NEXT:     let mut arguments: [*mut u8; 4] = [std::ptr::null_mut(); 4];
// LOWERING-AARCH64-GNU-NEXT:             name: b"number\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             arg: b"VALUE\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             doc: b"number\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut argp_state) -> i32>,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ITEM\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut core::ffi::c_void) -> *mut u8> =
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 6] = [112, 114, 111, 98, 101, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 5] = [105, 116, 101, 109, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = program.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = option.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = item.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:         key: b"slate\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:         key: b"slate\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     values = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"24\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT: extern "C-unwind" fn gnu_parse_option({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut argp_state) -> i32 {
// LOWERING-AARCH64-GNU-NEXT:     let mut argument: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = argument;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = argument;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"item\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum ACTION {
// COMMON-REWRITES-NEXT:     FIND = 0,
// COMMON-REWRITES-NEXT:     ENTER = 1,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum VISIT {
// COMMON-REWRITES-NEXT:     preorder = 0,
// COMMON-REWRITES-NEXT:     postorder = 1,
// COMMON-REWRITES-NEXT:     endorder = 2,
// COMMON-REWRITES-NEXT:     leaf = 3,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct GNUArguments {
// COMMON-REWRITES-NEXT:     number: i32,
// COMMON-REWRITES-NEXT:     positional: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct __tab {
// COMMON-REWRITES-NEXT:     __slate_empty: [u8; 0],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct argp {
// COMMON-REWRITES-NEXT:     options: *mut argp_option,
// COMMON-REWRITES-NEXT:     children: *mut argp_child,
// COMMON-REWRITES-NEXT:     help_filter:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct argp_child {
// COMMON-REWRITES-NEXT:     argp: *mut argp,
// COMMON-REWRITES-NEXT:     flags: i32,
// COMMON-REWRITES-NEXT:     group: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct argp_option {
// COMMON-REWRITES-NEXT:     key: i32,
// COMMON-REWRITES-NEXT:     flags: i32,
// COMMON-REWRITES-NEXT:     group: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct argp_state {
// COMMON-REWRITES-NEXT:     root_argp: *mut argp,
// COMMON-REWRITES-NEXT:     argc: i32,
// COMMON-REWRITES-NEXT:     next: i32,
// COMMON-REWRITES-NEXT:     flags: u32,
// COMMON-REWRITES-NEXT:     arg_num: u32,
// COMMON-REWRITES-NEXT:     quoted: i32,
// COMMON-REWRITES-NEXT:     input: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     child_inputs: *mut *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     hook: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     err_stream: *mut libc::FILE,
// COMMON-REWRITES-NEXT:     out_stream: *mut libc::FILE,
// COMMON-REWRITES-NEXT:     pstate: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct entry {
// COMMON-REWRITES-NEXT:     data: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct hsearch_data {
// COMMON-REWRITES-NEXT:     __tab: *mut __tab,
// COMMON-REWRITES-NEXT:     __unused1: u32,
// COMMON-REWRITES-NEXT:     __unused2: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct option {
// COMMON-REWRITES-NEXT:     has_arg: i32,
// COMMON-REWRITES-NEXT:     flag: *mut i32,
// COMMON-REWRITES-NEXT:     val: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     static mut opterr: i32;
// COMMON-REWRITES-NEXT:     static mut optind: i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn qsort_r(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: usize,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:         _3: Option<
// COMMON-REWRITES-NEXT:             unsafe extern "C-unwind" fn(
// COMMON-REWRITES-NEXT:                 *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:                 *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:                 *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             ) -> i32,
// COMMON-REWRITES-NEXT:         >,
// COMMON-REWRITES-NEXT:         _4: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     fn getopt_long(
// COMMON-REWRITES-NEXT:         _0: i32,
// COMMON-REWRITES-NEXT:         _1: *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _3: *const option,
// COMMON-REWRITES-NEXT:         _4: *mut i32,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn argp_parse(
// COMMON-REWRITES-NEXT:         _0: *const argp,
// COMMON-REWRITES-NEXT:         _1: i32,
// COMMON-REWRITES-NEXT:         _2: *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _3: u32,
// COMMON-REWRITES-NEXT:         _4: *mut i32,
// COMMON-REWRITES-NEXT:         _5: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn hcreate_r(_0: usize, _1: *mut hsearch_data) -> i32;
// COMMON-REWRITES-NEXT:     fn hsearch_r(_0: entry, _1: u32, _2: *mut *mut entry, _3: *mut hsearch_data) -> i32;
// COMMON-REWRITES-NEXT:     fn hdestroy_r(_0: *mut hsearch_data);
// COMMON-REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn tsearch(
// COMMON-REWRITES-NEXT:         _0: *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: *mut *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _2: Option<
// COMMON-REWRITES-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-REWRITES-NEXT:         >,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn tfind(
// COMMON-REWRITES-NEXT:         _0: *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: *mut *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _2: Option<
// COMMON-REWRITES-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-REWRITES-NEXT:         >,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn tdestroy(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let bytes = unsafe { std::ffi::CStr::from_ptr(s) }.to_bytes();
// COMMON-REWRITES-NEXT:     let n = bytes.len();
// COMMON-REWRITES-NEXT:     let mut i = 0usize;
// COMMON-REWRITES-NEXT:     while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// COMMON-REWRITES-NEXT:         i += 1usize;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let start = i;
// COMMON-REWRITES-NEXT:     if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// COMMON-REWRITES-NEXT:         i += 1usize;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let digits = i;
// COMMON-REWRITES-NEXT:     while i < n && bytes[i].is_ascii_digit() {
// COMMON-REWRITES-NEXT:         i += 1usize;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     if i == digits {
// COMMON-REWRITES-NEXT:         return 0i32;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return std::str::from_utf8(&bytes[(start..i)])
// COMMON-REWRITES-NEXT:         .unwrap()
// COMMON-REWRITES-NEXT:         .parse()
// COMMON-REWRITES-NEXT:         .unwrap_or(0i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             gnu_qsort_extension(),
// COMMON-REWRITES-NEXT:             gnu_getopt_extensions(),
// COMMON-REWRITES-NEXT:             gnu_argp_extensions(),
// COMMON-REWRITES-NEXT:             gnu_search_extensions(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_qsort_extension() -> i32 {
// COMMON-REWRITES-NEXT:     let mut direction: i32 = 0;
// COMMON-REWRITES-NEXT:     direction = -1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(direction) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         qsort_r(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (4 as u64) as usize,
// COMMON-REWRITES-NEXT:             (4 as u64) as usize,
// COMMON-REWRITES-NEXT:             Some(gnu_compare_with_direction),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     values[0] * 1000 + values[1] * 100 + values[2] * 10 + values[3]
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_getopt_extensions() -> i32 {
// COMMON-REWRITES-NEXT:         [option {
// COMMON-REWRITES-NEXT:             name: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             has_arg: 0,
// COMMON-REWRITES-NEXT:             flag: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             val: 0,
// COMMON-REWRITES-NEXT:         }; 3],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let mut number_value: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut flag_value: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut option: i32 = 0;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     *options = [
// COMMON-REWRITES-NEXT:         option {
// COMMON-REWRITES-NEXT:             has_arg: 1,
// COMMON-REWRITES-NEXT:             flag: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             val: 110,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         option {
// COMMON-REWRITES-NEXT:             has_arg: 0,
// COMMON-REWRITES-NEXT:             flag: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             val: 102,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         option {
// COMMON-REWRITES-NEXT:             name: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             has_arg: 0,
// COMMON-REWRITES-NEXT:             flag: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             val: 0,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         optind = 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         opterr = 0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut option = options.as_mut_ptr() as *mut option;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:             getopt_long(
// COMMON-REWRITES-NEXT:                 3 as i32,
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const option,
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *mut i32,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         option = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != -1) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = option == 110;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             number_value = unsafe {
// COMMON-REWRITES-NEXT:             };
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             if option == 102 {
// COMMON-REWRITES-NEXT:                 flag_value = 1;
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     number_value * 10 + flag_value
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_argp_extensions() -> i32 {
// COMMON-REWRITES-NEXT:         [argp_option {
// COMMON-REWRITES-NEXT:             name: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             key: 0,
// COMMON-REWRITES-NEXT:             arg: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             flags: 0,
// COMMON-REWRITES-NEXT:             doc: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             group: 0,
// COMMON-REWRITES-NEXT:         }; 2],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let mut parser: argp = argp {
// COMMON-REWRITES-NEXT:         options: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         parser: None,
// COMMON-REWRITES-NEXT:         args_doc: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         doc: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         children: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         help_filter: None,
// COMMON-REWRITES-NEXT:         argp_domain: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut parsed: GNUArguments = GNUArguments {
// COMMON-REWRITES-NEXT:         number: 0,
// COMMON-REWRITES-NEXT:         positional: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     *options = [
// COMMON-REWRITES-NEXT:         argp_option {
// COMMON-REWRITES-NEXT:             key: 110,
// COMMON-REWRITES-NEXT:             flags: 0,
// COMMON-REWRITES-NEXT:             group: 0,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         argp_option {
// COMMON-REWRITES-NEXT:             name: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             key: 0,
// COMMON-REWRITES-NEXT:             arg: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             flags: 0,
// COMMON-REWRITES-NEXT:             doc: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             group: 0,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut argp_option = options.as_mut_ptr() as *mut argp_option;
// COMMON-REWRITES-NEXT:     parser.options = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     parser.parser = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<
// COMMON-REWRITES-NEXT:             *const (),
// COMMON-REWRITES-NEXT:         >(gnu_parse_option as *const ())
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     parser.doc = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     parser.children = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     parser.help_filter = None;
// COMMON-REWRITES-NEXT:     parser.argp_domain = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     parsed = GNUArguments {
// COMMON-REWRITES-NEXT:         number: 0,
// COMMON-REWRITES-NEXT:         positional: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     program = [112, 114, 111, 98, 101, 0];
// COMMON-REWRITES-NEXT:     option = [45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0];
// COMMON-REWRITES-NEXT:     item = [105, 116, 101, 109, 0];
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 16;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = (32 | {{__v[0-9]+}}) as u32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(parsed) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         argp_parse(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(parser) as *const argp,
// COMMON-REWRITES-NEXT:             3 as i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as u32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     (({{__v[0-9]+}} == 0) as i32) + parsed.number * 10 + parsed.positional
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_search_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut table: hsearch_data = hsearch_data {
// COMMON-REWRITES-NEXT:         __tab: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         __unused1: 0,
// COMMON-REWRITES-NEXT:         __unused2: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut found: *mut entry = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut tree: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: entry = entry {
// COMMON-REWRITES-NEXT:         data: b"24\0".as_ptr() as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: entry = entry {
// COMMON-REWRITES-NEXT:         data: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         hcreate_r(
// COMMON-REWRITES-NEXT:             (8 as u64) as usize,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = ACTION::ENTER as u32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         hsearch_r(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as entry,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as u32,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(found) as *mut *mut entry,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = ACTION::FIND as u32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         hsearch_r(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as entry,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as u32,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(found) as *mut *mut entry,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = found != ({{__v[0-9]+}} as *mut entry);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:             strcmp(
// COMMON-REWRITES-NEXT:                 c"24".as_ptr(),
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     unsafe { hdestroy_r(std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// COMMON-REWRITES-NEXT:     for index in 0..4 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc((4 as u64) as usize) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *{{__v[0-9]+}} = values[(index as usize)];
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             tsearch(
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:                 std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:                 Some(gnu_compare_entries),
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(values[2]) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-REWRITES-NEXT:         tfind(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             Some(gnu_compare_entries),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     unsafe { tdestroy(tree as *mut core::ffi::c_void, Some(gnu_free_entry)) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn gnu_compare_with_direction(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: ) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *({{arg[0-9]+}} as *mut i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *({{arg[0-9]+}} as *mut i32) };
// COMMON-REWRITES-NEXT:     (unsafe { *({{arg[0-9]+}} as *mut i32) }) * ((({{__v[0-9]+}} > {{__v[0-9]+}}) as i32) - (({{__v[0-9]+}} < {{__v[0-9]+}}) as i32))
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn gnu_parse_option(
// COMMON-REWRITES-NEXT:     mut key: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut argp_state,
// COMMON-REWRITES-NEXT: ) -> i32 {
// COMMON-REWRITES-NEXT:     let mut arguments: *mut GNUArguments = (unsafe { (*{{arg[0-9]+}}).input }) as *mut GNUArguments;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = key == 110;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             (*arguments).number = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         return 0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = key == 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { strcmp(argument as *const core::ffi::c_char, c"item".as_ptr()) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut GNUArguments = arguments;
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             (*{{__v[0-9]+}}).positional = (unsafe { (*{{__v[0-9]+}}).positional }) + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         return 0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if key == 16777217 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = key == 16777219;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = key == 16777218;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = key == 16777220;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = key == 16777223;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         return 0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     7
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn gnu_compare_entries(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: ) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *({{arg[0-9]+}} as *mut i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *({{arg[0-9]+}} as *mut i32) };
// COMMON-REWRITES-NEXT:     (({{__v[0-9]+}} > {{__v[0-9]+}}) as i32) - (({{__v[0-9]+}} < {{__v[0-9]+}}) as i32)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn gnu_free_entry({{arg[0-9]+}}: *mut core::ffi::c_void) {
// COMMON-REWRITES-NEXT:     unsafe { free({{arg[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     parser: Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// REWRITES-X86_64-GNU-NEXT:     args_doc: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     doc: *mut i8,
// REWRITES-X86_64-GNU-NEXT:         Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8>,
// REWRITES-X86_64-GNU-NEXT:     argp_domain: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     header: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     name: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     arg: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     doc: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     argv: *mut *mut i8,
// REWRITES-X86_64-GNU-NEXT:     name: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     key: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     name: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     static mut optarg: *mut i8;
// REWRITES-X86_64-GNU-NEXT: fn __slate_atoi(s: *const i8) -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     *values = [4, 1, 3, 2];
// REWRITES-X86_64-GNU-NEXT:     let mut program: [i8; 6] = [112, 114, 111, 98, 101, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut number: [i8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut flag: [i8; 3] = [45, 102, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> =
// REWRITES-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 4]);
// REWRITES-X86_64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A16, [option; 3]> = aligned::Aligned(
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = program.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = number.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = flag.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-X86_64-GNU-NEXT:             name: c"number".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             name: c"flag".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = c"fn:".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:                 __slate_atoi(((unsafe { optarg }) as *const core::ffi::c_char) as *const i8)
// REWRITES-X86_64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A16, [argp_option; 2]> = aligned::Aligned(
// REWRITES-X86_64-GNU-NEXT:     let mut program: [i8; 6] = [0; 6];
// REWRITES-X86_64-GNU-NEXT:     let mut option: [i8; 11] = [0; 11];
// REWRITES-X86_64-GNU-NEXT:     let mut item: [i8; 5] = [0; 5];
// REWRITES-X86_64-GNU-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> =
// REWRITES-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 4]);
// REWRITES-X86_64-GNU-NEXT:             name: c"number".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             arg: c"VALUE".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             doc: c"number".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// REWRITES-X86_64-GNU-NEXT:     parser.args_doc = c"ITEM".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = program.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = option.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = item.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:         key: c"slate".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:         key: c"slate".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:     *values = [3, 1, 4, 2];
// REWRITES-X86_64-GNU-NEXT:                 ((unsafe { (*found).data }) as *mut i8) as *const core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:     mut argument: *mut i8,
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { __slate_atoi(argument as *const i8) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     parser: Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut argp_state) -> i32>,
// REWRITES-AARCH64-GNU-NEXT:     args_doc: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     doc: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:         Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut core::ffi::c_void) -> *mut u8>,
// REWRITES-AARCH64-GNU-NEXT:     argp_domain: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     header: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     name: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     arg: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     doc: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     argv: *mut *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     name: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     key: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     name: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     static mut optarg: *mut u8;
// REWRITES-AARCH64-GNU-NEXT: fn __slate_atoi(s: *const u8) -> i32 {
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [4, 1, 3, 2];
// REWRITES-AARCH64-GNU-NEXT:     let mut program: [u8; 6] = [112, 114, 111, 98, 101, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut number: [u8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut flag: [u8; 3] = [45, 102, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut arguments: [*mut u8; 4] = [std::ptr::null_mut(); 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A8, [option; 3]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = program.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = number.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = flag.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-AARCH64-GNU-NEXT:             name: c"number".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             name: c"flag".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = c"fn:".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:                 __slate_atoi(((unsafe { optarg }) as *const core::ffi::c_char) as *const u8)
// REWRITES-AARCH64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A8, [argp_option; 2]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-NEXT:     let mut program: [u8; 6] = [0; 6];
// REWRITES-AARCH64-GNU-NEXT:     let mut option: [u8; 11] = [0; 11];
// REWRITES-AARCH64-GNU-NEXT:     let mut item: [u8; 5] = [0; 5];
// REWRITES-AARCH64-GNU-NEXT:     let mut arguments: [*mut u8; 4] = [std::ptr::null_mut(); 4];
// REWRITES-AARCH64-GNU-NEXT:             name: c"number".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             arg: c"VALUE".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             doc: c"number".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut argp_state) -> i32>,
// REWRITES-AARCH64-GNU-NEXT:     parser.args_doc = c"ITEM".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = program.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = option.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = item.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [3, 1, 4, 2];
// REWRITES-AARCH64-GNU-NEXT:         key: c"slate".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:         key: c"slate".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:                 ((unsafe { (*found).data }) as *mut u8) as *const core::ffi::c_char,
// REWRITES-AARCH64-GNU-NEXT:     mut argument: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { __slate_atoi(argument as *const u8) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
