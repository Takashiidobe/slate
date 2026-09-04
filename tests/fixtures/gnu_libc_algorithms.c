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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum ACTION {
// LOWERING-NEXT:     FIND = 0,
// LOWERING-NEXT:     ENTER = 1,
// LOWERING-NEXT: }
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct GNUArguments {
// LOWERING-NEXT:     number: i32,
// LOWERING-NEXT:     positional: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __tab {
// LOWERING-NEXT:     __slate_empty: [u8; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:     __slate_anon_0: *mut core::ffi::c_void,
// LOWERING-NEXT:     __slate_anon_1: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct argp {
// LOWERING-NEXT:     options: *mut argp_option,
// LOWERING-NEXT:     parser: Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// LOWERING-NEXT:     args_doc: *mut i8,
// LOWERING-NEXT:     doc: *mut i8,
// LOWERING-NEXT:     children: *mut argp_child,
// LOWERING-NEXT:     help_filter:
// LOWERING-NEXT:         Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8>,
// LOWERING-NEXT:     argp_domain: *mut i8,
// LOWERING-NEXT: }
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct entry {
// LOWERING-NEXT:     key: *mut i8,
// LOWERING-NEXT:     data: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct hsearch_data {
// LOWERING-NEXT:     __tab: *mut __tab,
// LOWERING-NEXT:     __unused1: u32,
// LOWERING-NEXT:     __unused2: u32,
// LOWERING-NEXT: }
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
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     static mut optarg: *mut i8;
// LOWERING-NEXT:     static mut opterr: i32;
// LOWERING-NEXT:     static mut optind: i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn qsort_r(
// LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-NEXT:         _1: usize,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:         _3: Option<
// LOWERING-NEXT:             unsafe extern "C-unwind" fn(
// LOWERING-NEXT:                 *mut core::ffi::c_void,
// LOWERING-NEXT:                 *mut core::ffi::c_void,
// LOWERING-NEXT:                 *mut core::ffi::c_void,
// LOWERING-NEXT:             ) -> i32,
// LOWERING-NEXT:         >,
// LOWERING-NEXT:         _4: *mut core::ffi::c_void,
// LOWERING-NEXT:     );
// LOWERING-NEXT:     fn getopt_long(
// LOWERING-NEXT:         _0: i32,
// LOWERING-NEXT:         _1: *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         _2: *const core::ffi::c_char,
// LOWERING-NEXT:         _3: *const option,
// LOWERING-NEXT:         _4: *mut i32,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn argp_parse(
// LOWERING-NEXT:         _0: *const argp,
// LOWERING-NEXT:         _1: i32,
// LOWERING-NEXT:         _2: *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         _3: u32,
// LOWERING-NEXT:         _4: *mut i32,
// LOWERING-NEXT:         _5: *mut core::ffi::c_void,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn hcreate_r(_0: usize, _1: *mut hsearch_data) -> i32;
// LOWERING-NEXT:     fn hsearch_r(
// LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-NEXT:         _1: *mut core::ffi::c_void,
// LOWERING-NEXT:         _2: u32,
// LOWERING-NEXT:         _3: *mut *mut entry,
// LOWERING-NEXT:         _4: *mut hsearch_data,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn hdestroy_r(_0: *mut hsearch_data);
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn tsearch(
// LOWERING-NEXT:         _0: *const core::ffi::c_void,
// LOWERING-NEXT:         _1: *mut *mut core::ffi::c_void,
// LOWERING-NEXT:         _2: Option<
// LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// LOWERING-NEXT:         >,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn tfind(
// LOWERING-NEXT:         _0: *const core::ffi::c_void,
// LOWERING-NEXT:         _1: *mut *mut core::ffi::c_void,
// LOWERING-NEXT:         _2: Option<
// LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// LOWERING-NEXT:         >,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn tdestroy(
// LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// LOWERING-NEXT:     );
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn gnu_compare_with_direction(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT: ) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_qsort_extension() -> i32 {
// LOWERING-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut direction: i32 = 0;
// LOWERING-NEXT:     *values = [4, 1, 3, 2];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     direction = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(direction) as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         qsort_r(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:             Some(gnu_compare_with_direction),
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1000;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_getopt_extensions() -> i32 {
// LOWERING-NEXT:     let mut program: [i8; 6] = [0; 6];
// LOWERING-NEXT:     let mut number: [i8; 11] = [0; 11];
// LOWERING-NEXT:     let mut flag: [i8; 3] = [0; 3];
// LOWERING-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> =
// LOWERING-NEXT:         aligned::Aligned([std::ptr::null_mut(); 4]);
// LOWERING-NEXT:     let mut options: aligned::Aligned<aligned::A16, [option; 3]> = aligned::Aligned(
// LOWERING-NEXT:         [option {
// LOWERING-NEXT:             name: std::ptr::null_mut(),
// LOWERING-NEXT:             has_arg: 0,
// LOWERING-NEXT:             flag: std::ptr::null_mut(),
// LOWERING-NEXT:             val: 0,
// LOWERING-NEXT:         }; 3],
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let mut number_value: i32 = 0;
// LOWERING-NEXT:     let mut flag_value: i32 = 0;
// LOWERING-NEXT:     let mut option: i32 = 0;
// LOWERING-NEXT:     program = [112, 114, 111, 98, 101, 0];
// LOWERING-NEXT:     number = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// LOWERING-NEXT:     flag = [45, 102, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = program.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = number.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = unsafe { {{_v[0-9]+}}.add(2) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = flag.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = unsafe { {{_v[0-9]+}}.add(3) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     *options = [
// LOWERING-NEXT:         option {
// LOWERING-NEXT:             name: b"number\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             has_arg: 1,
// LOWERING-NEXT:             flag: std::ptr::null_mut(),
// LOWERING-NEXT:             val: 110,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         option {
// LOWERING-NEXT:             name: b"flag\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             has_arg: 0,
// LOWERING-NEXT:             flag: std::ptr::null_mut(),
// LOWERING-NEXT:             val: 102,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         option {
// LOWERING-NEXT:             name: std::ptr::null_mut(),
// LOWERING-NEXT:             has_arg: 0,
// LOWERING-NEXT:             flag: std::ptr::null_mut(),
// LOWERING-NEXT:             val: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     number_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     flag_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         optind = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         opterr = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"fn:\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut option = options.as_mut_ptr() as *mut option;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:                 getopt_long(
// LOWERING-NEXT:                     {{_v[0-9]+}} as i32,
// LOWERING-NEXT:                     {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                     {{_v[0-9]+}} as *const option,
// LOWERING-NEXT:                     {{_v[0-9]+}} as *mut i32,
// LOWERING-NEXT:                 )
// LOWERING-NEXT:             };
// LOWERING-NEXT:             option = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = option;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 110;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i8 = unsafe { optarg };
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { atoi({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:                         number_value = {{_v[0-9]+}};
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = option;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = 102;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                             if {{_v[0-9]+}} {
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                                 flag_value = {{_v[0-9]+}};
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = number_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = flag_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn gnu_parse_option({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut argp_state) -> i32 {
// LOWERING-NEXT:     let mut key: i32 = 0;
// LOWERING-NEXT:     let mut argument: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut arguments: *mut GNUArguments = std::ptr::null_mut();
// LOWERING-NEXT:     key = {{arg[0-9]+}};
// LOWERING-NEXT:     argument = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{arg[0-9]+}}).input };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut GNUArguments = {{_v[0-9]+}} as *mut GNUArguments;
// LOWERING-NEXT:     arguments = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = key;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 110;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = argument;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { atoi({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut GNUArguments = arguments;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 (*{{_v[0-9]+}}).number = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             return {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = key;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = argument;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"item\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:                 strcmp(
// LOWERING-NEXT:                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                 )
// LOWERING-NEXT:             };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut GNUArguments = arguments;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).positional };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 (*{{_v[0-9]+}}).positional = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             return {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = key;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 16777217;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:             {{_v[0-9]+}}
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = key;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 16777219;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:             {{_v[0-9]+}}
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:             {{_v[0-9]+}}
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = key;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 16777218;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:             {{_v[0-9]+}}
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:             {{_v[0-9]+}}
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = key;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 16777220;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:             {{_v[0-9]+}}
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:             {{_v[0-9]+}}
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = key;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 16777223;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:             {{_v[0-9]+}}
// LOWERING-NEXT:         };
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             return {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_argp_extensions() -> i32 {
// LOWERING-NEXT:     let mut options: aligned::Aligned<aligned::A16, [argp_option; 2]> = aligned::Aligned(
// LOWERING-NEXT:         [argp_option {
// LOWERING-NEXT:             name: std::ptr::null_mut(),
// LOWERING-NEXT:             key: 0,
// LOWERING-NEXT:             arg: std::ptr::null_mut(),
// LOWERING-NEXT:             flags: 0,
// LOWERING-NEXT:             doc: std::ptr::null_mut(),
// LOWERING-NEXT:             group: 0,
// LOWERING-NEXT:         }; 2],
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let mut parser: argp = argp {
// LOWERING-NEXT:         options: std::ptr::null_mut(),
// LOWERING-NEXT:         parser: None,
// LOWERING-NEXT:         args_doc: std::ptr::null_mut(),
// LOWERING-NEXT:         doc: std::ptr::null_mut(),
// LOWERING-NEXT:         children: std::ptr::null_mut(),
// LOWERING-NEXT:         help_filter: None,
// LOWERING-NEXT:         argp_domain: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut parsed: GNUArguments = GNUArguments {
// LOWERING-NEXT:         number: 0,
// LOWERING-NEXT:         positional: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut program: [i8; 6] = [0; 6];
// LOWERING-NEXT:     let mut option: [i8; 11] = [0; 11];
// LOWERING-NEXT:     let mut item: [i8; 5] = [0; 5];
// LOWERING-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> =
// LOWERING-NEXT:         aligned::Aligned([std::ptr::null_mut(); 4]);
// LOWERING-NEXT:     *options = [
// LOWERING-NEXT:         argp_option {
// LOWERING-NEXT:             name: b"number\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             key: 110,
// LOWERING-NEXT:             arg: b"VALUE\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             flags: 0,
// LOWERING-NEXT:             doc: b"number\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             group: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         argp_option {
// LOWERING-NEXT:             name: std::ptr::null_mut(),
// LOWERING-NEXT:             key: 0,
// LOWERING-NEXT:             arg: std::ptr::null_mut(),
// LOWERING-NEXT:             flags: 0,
// LOWERING-NEXT:             doc: std::ptr::null_mut(),
// LOWERING-NEXT:             group: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut argp_option = options.as_mut_ptr() as *mut argp_option;
// LOWERING-NEXT:     parser.options = {{_v[0-9]+}};
// LOWERING-NEXT:     parser.parser = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<
// LOWERING-NEXT:             *const (),
// LOWERING-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// LOWERING-NEXT:         >(gnu_parse_option as *const ())
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ITEM\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     parser.args_doc = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     parser.doc = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut argp_child = std::ptr::null_mut();
// LOWERING-NEXT:     parser.children = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8> =
// LOWERING-NEXT:         None;
// LOWERING-NEXT:     parser.help_filter = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     parser.argp_domain = {{_v[0-9]+}};
// LOWERING-NEXT:     parsed = GNUArguments {
// LOWERING-NEXT:         number: 0,
// LOWERING-NEXT:         positional: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     program = [112, 114, 111, 98, 101, 0];
// LOWERING-NEXT:     option = [45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0];
// LOWERING-NEXT:     item = [105, 116, 101, 109, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = program.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = option.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = unsafe { {{_v[0-9]+}}.add(2) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = item.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = unsafe { {{_v[0-9]+}}.add(3) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(parsed) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         argp_parse(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(parser) as *const argp,
// LOWERING-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as u32,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut i32,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = parsed.number;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = parsed.positional;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn gnu_compare_entries(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT: ) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn gnu_free_entry({{arg[0-9]+}}: *mut core::ffi::c_void) {
// LOWERING-NEXT:     unsafe { free({{arg[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_search_extensions() -> i32 {
// LOWERING-NEXT:     let mut coerce: entry = entry {
// LOWERING-NEXT:         key: std::ptr::null_mut(),
// LOWERING-NEXT:         data: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce2: entry = entry {
// LOWERING-NEXT:         key: std::ptr::null_mut(),
// LOWERING-NEXT:         data: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut table: hsearch_data = hsearch_data {
// LOWERING-NEXT:         __tab: std::ptr::null_mut(),
// LOWERING-NEXT:         __unused1: 0,
// LOWERING-NEXT:         __unused2: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut found: *mut entry = std::ptr::null_mut();
// LOWERING-NEXT:     let mut tree: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     table = hsearch_data {
// LOWERING-NEXT:         __tab: std::ptr::null_mut(),
// LOWERING-NEXT:         __unused1: 0,
// LOWERING-NEXT:         __unused2: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: entry = entry {
// LOWERING-NEXT:         key: b"slate\0".as_ptr() as *mut i8,
// LOWERING-NEXT:         data: b"24\0".as_ptr() as *mut core::ffi::c_void,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: entry = entry {
// LOWERING-NEXT:         key: b"slate\0".as_ptr() as *mut i8,
// LOWERING-NEXT:         data: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut entry = std::ptr::null_mut();
// LOWERING-NEXT:     found = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     tree = {{_v[0-9]+}};
// LOWERING-NEXT:     *values = [3, 1, 4, 2];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         hcreate_r(
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = ACTION::ENTER as u32;
// LOWERING-NEXT:     coerce2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut {{anon_struct[0-9A-Za-z_]*}} = std::ptr::addr_of_mut!(coerce2) as *mut {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{_v[0-9]+}}).__slate_anon_0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{_v[0-9]+}}).__slate_anon_1 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         hsearch_r(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as u32,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(found) as *mut *mut entry,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = ACTION::FIND as u32;
// LOWERING-NEXT:     coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut {{anon_struct[0-9A-Za-z_]*}} = std::ptr::addr_of_mut!(coerce) as *mut {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{_v[0-9]+}}).__slate_anon_0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{_v[0-9]+}}).__slate_anon_1 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         hsearch_r(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as u32,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(found) as *mut *mut entry,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut entry = found;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut entry = {{_v[0-9]+}} as *mut entry;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut entry = found;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{_v[0-9]+}}).data };
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"24\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:             strcmp(
// LOWERING-NEXT:                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe { hdestroy_r(std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: u64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         index = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:                     tsearch(
// LOWERING-NEXT:                         {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:                         std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void,
// LOWERING-NEXT:                         Some(gnu_compare_entries),
// LOWERING-NEXT:                     )
// LOWERING-NEXT:                 };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             index = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(values[({{_v[0-9]+}} as usize)]) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         tfind(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void,
// LOWERING-NEXT:             Some(gnu_compare_entries),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = tree;
// LOWERING-NEXT:     unsafe { tdestroy({{_v[0-9]+}} as *mut core::ffi::c_void, Some(gnu_free_entry)) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_qsort_extension();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_getopt_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_argp_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_search_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering
