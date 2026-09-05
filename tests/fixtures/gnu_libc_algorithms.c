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
// LOWERING-NEXT: struct argp {
// LOWERING-NEXT:     options: *mut argp_option,
// LOWERING-X86_64-GNU-NEXT:     parser: Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// LOWERING-X86_64-GNU-NEXT:     args_doc: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     doc: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     parser: Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut argp_state) -> i32>,
// LOWERING-AARCH64-GNU-NEXT:     args_doc: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     doc: *mut u8,
// LOWERING-NEXT:     children: *mut argp_child,
// LOWERING-NEXT:     help_filter:
// LOWERING-X86_64-GNU-NEXT:         Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8>,
// LOWERING-X86_64-GNU-NEXT:     argp_domain: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:         Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut core::ffi::c_void) -> *mut u8>,
// LOWERING-AARCH64-GNU-NEXT:     argp_domain: *mut u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct argp_child {
// LOWERING-NEXT:     argp: *mut argp,
// LOWERING-NEXT:     flags: i32,
// LOWERING-X86_64-GNU-NEXT:     header: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     header: *mut u8,
// LOWERING-NEXT:     group: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct argp_option {
// LOWERING-X86_64-GNU-NEXT:     name: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     name: *mut u8,
// LOWERING-NEXT:     key: i32,
// LOWERING-X86_64-GNU-NEXT:     arg: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     arg: *mut u8,
// LOWERING-NEXT:     flags: i32,
// LOWERING-X86_64-GNU-NEXT:     doc: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     doc: *mut u8,
// LOWERING-NEXT:     group: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct argp_state {
// LOWERING-NEXT:     root_argp: *mut argp,
// LOWERING-NEXT:     argc: i32,
// LOWERING-X86_64-GNU-NEXT:     argv: *mut *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     argv: *mut *mut u8,
// LOWERING-NEXT:     next: i32,
// LOWERING-NEXT:     flags: u32,
// LOWERING-NEXT:     arg_num: u32,
// LOWERING-NEXT:     quoted: i32,
// LOWERING-NEXT:     input: *mut core::ffi::c_void,
// LOWERING-NEXT:     child_inputs: *mut *mut core::ffi::c_void,
// LOWERING-NEXT:     hook: *mut core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:     name: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     name: *mut u8,
// LOWERING-NEXT:     err_stream: *mut libc::FILE,
// LOWERING-NEXT:     out_stream: *mut libc::FILE,
// LOWERING-NEXT:     pstate: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct entry {
// LOWERING-X86_64-GNU-NEXT:     key: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     key: *mut u8,
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
// LOWERING-X86_64-GNU-NEXT:     name: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     name: *mut u8,
// LOWERING-NEXT:     has_arg: i32,
// LOWERING-NEXT:     flag: *mut i32,
// LOWERING-NEXT:     val: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     static mut optarg: *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     static mut optarg: *mut u8;
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
// LOWERING-NEXT:     fn argp_parse(
// LOWERING-NEXT:         _0: *const argp,
// LOWERING-NEXT:         _1: i32,
// LOWERING-NEXT:         _2: *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         _3: u32,
// LOWERING-NEXT:         _4: *mut i32,
// LOWERING-NEXT:         _5: *mut core::ffi::c_void,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn hcreate_r(_0: usize, _1: *mut hsearch_data) -> i32;
// LOWERING-NEXT:     fn hsearch_r(_0: entry, _1: u32, _2: *mut *mut entry, _3: *mut hsearch_data) -> i32;
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
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_qsort_extension();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_getopt_extensions();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_argp_extensions();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_search_extensions();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_qsort_extension() -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [0; 4];
// LOWERING-NEXT:     let mut direction: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [4, 1, 3, 2];
// LOWERING-X86_64-GNU-NEXT:     *values = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     values = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     direction = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(direction) as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         qsort_r(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             Some(gnu_compare_with_direction),
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1000;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_getopt_extensions() -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut program: [i8; 6] = [0; 6];
// LOWERING-X86_64-GNU-NEXT:     let mut number: [i8; 11] = [0; 11];
// LOWERING-X86_64-GNU-NEXT:     let mut flag: [i8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> =
// LOWERING-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 4]);
// LOWERING-X86_64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A16, [option; 3]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:     let mut program: [u8; 6] = [0; 6];
// LOWERING-AARCH64-GNU-NEXT:     let mut number: [u8; 11] = [0; 11];
// LOWERING-AARCH64-GNU-NEXT:     let mut flag: [u8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let mut arguments: [*mut u8; 4] = [std::ptr::null_mut(); 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A8, [option; 3]> = aligned::Aligned(
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
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 6] = [112, 114, 111, 98, 101, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 6] = [112, 114, 111, 98, 101, 0];
// LOWERING-NEXT:     program = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// LOWERING-NEXT:     number = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 3] = [45, 102, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [45, 102, 0];
// LOWERING-NEXT:     flag = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = program.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = program.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = number.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = number.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = flag.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = flag.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: [option; 3] = [
// LOWERING-NEXT:         option {
// LOWERING-X86_64-GNU-NEXT:             name: b"number\0".as_ptr() as *mut i8,
// LOWERING-AARCH64-GNU-NEXT:             name: b"number\0".as_ptr() as *mut u8,
// LOWERING-NEXT:             has_arg: 1,
// LOWERING-NEXT:             flag: std::ptr::null_mut(),
// LOWERING-NEXT:             val: 110,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         option {
// LOWERING-X86_64-GNU-NEXT:             name: b"flag\0".as_ptr() as *mut i8,
// LOWERING-AARCH64-GNU-NEXT:             name: b"flag\0".as_ptr() as *mut u8,
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
// LOWERING-NEXT:     *options = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     number_value = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     flag_value = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         optind = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         opterr = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"fn:\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"fn:\0".as_ptr() as *mut u8;
// LOWERING-NEXT:             let {{__v[0-9]+}}: *mut option = options.as_mut_ptr() as *mut option;
// LOWERING-NEXT:             let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:                 getopt_long(
// LOWERING-NEXT:                     {{__v[0-9]+}} as i32,
// LOWERING-NEXT:                     {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                     {{__v[0-9]+}} as *const option,
// LOWERING-NEXT:                     {{__v[0-9]+}} as *mut i32,
// LOWERING-NEXT:                 )
// LOWERING-NEXT:             };
// LOWERING-NEXT:             option = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = option;
// LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 110;
// LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:                     if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut i8 = unsafe { optarg };
// LOWERING-AARCH64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut u8 = unsafe { optarg };
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = unsafe { atoi({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:                         number_value = {{__v[0-9]+}};
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = option;
// LOWERING-NEXT:                             let {{__v[0-9]+}}: i32 = 102;
// LOWERING-NEXT:                             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:                             if {{__v[0-9]+}} {
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                                 flag_value = {{__v[0-9]+}};
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = number_value;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = flag_value;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_argp_extensions() -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A16, [argp_option; 2]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A8, [argp_option; 2]> = aligned::Aligned(
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
// LOWERING-X86_64-GNU-NEXT:     let mut program: [i8; 6] = [0; 6];
// LOWERING-X86_64-GNU-NEXT:     let mut option: [i8; 11] = [0; 11];
// LOWERING-X86_64-GNU-NEXT:     let mut item: [i8; 5] = [0; 5];
// LOWERING-X86_64-GNU-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> =
// LOWERING-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 4]);
// LOWERING-AARCH64-GNU-NEXT:     let mut program: [u8; 6] = [0; 6];
// LOWERING-AARCH64-GNU-NEXT:     let mut option: [u8; 11] = [0; 11];
// LOWERING-AARCH64-GNU-NEXT:     let mut item: [u8; 5] = [0; 5];
// LOWERING-AARCH64-GNU-NEXT:     let mut arguments: [*mut u8; 4] = [std::ptr::null_mut(); 4];
// LOWERING-NEXT:     let {{__v[0-9]+}}: [argp_option; 2] = [
// LOWERING-NEXT:         argp_option {
// LOWERING-X86_64-GNU-NEXT:             name: b"number\0".as_ptr() as *mut i8,
// LOWERING-AARCH64-GNU-NEXT:             name: b"number\0".as_ptr() as *mut u8,
// LOWERING-NEXT:             key: 110,
// LOWERING-X86_64-GNU-NEXT:             arg: b"VALUE\0".as_ptr() as *mut i8,
// LOWERING-AARCH64-GNU-NEXT:             arg: b"VALUE\0".as_ptr() as *mut u8,
// LOWERING-NEXT:             flags: 0,
// LOWERING-X86_64-GNU-NEXT:             doc: b"number\0".as_ptr() as *mut i8,
// LOWERING-AARCH64-GNU-NEXT:             doc: b"number\0".as_ptr() as *mut u8,
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
// LOWERING-NEXT:     *options = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut argp_option = options.as_mut_ptr() as *mut argp_option;
// LOWERING-NEXT:     parser.options = {{__v[0-9]+}};
// LOWERING-NEXT:     parser.parser = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<
// LOWERING-NEXT:             *const (),
// LOWERING-X86_64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// LOWERING-AARCH64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut argp_state) -> i32>,
// LOWERING-NEXT:         >(gnu_parse_option as *const ())
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ITEM\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ITEM\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     parser.args_doc = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     parser.doc = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut argp_child = std::ptr::null_mut();
// LOWERING-NEXT:     parser.children = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8> =
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut core::ffi::c_void) -> *mut u8> =
// LOWERING-NEXT:         None;
// LOWERING-NEXT:     parser.help_filter = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     parser.argp_domain = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: GNUArguments = GNUArguments {
// LOWERING-NEXT:         number: 0,
// LOWERING-NEXT:         positional: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     parsed = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 6] = [112, 114, 111, 98, 101, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 6] = [112, 114, 111, 98, 101, 0];
// LOWERING-NEXT:     program = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0];
// LOWERING-NEXT:     option = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 5] = [105, 116, 101, 109, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 5] = [105, 116, 101, 109, 0];
// LOWERING-NEXT:     item = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = program.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = program.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = option.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = option.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = item.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = item.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 16;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(parsed) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         argp_parse(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(parser) as *const argp,
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = parsed.number;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = parsed.positional;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_search_extensions() -> i32 {
// LOWERING-NEXT:     let mut table: hsearch_data = hsearch_data {
// LOWERING-NEXT:         __tab: std::ptr::null_mut(),
// LOWERING-NEXT:         __unused1: 0,
// LOWERING-NEXT:         __unused2: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut found: *mut entry = std::ptr::null_mut();
// LOWERING-NEXT:     let mut tree: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [0; 4];
// LOWERING-NEXT:     let {{__v[0-9]+}}: hsearch_data = hsearch_data {
// LOWERING-NEXT:         __tab: std::ptr::null_mut(),
// LOWERING-NEXT:         __unused1: 0,
// LOWERING-NEXT:         __unused2: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     table = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: entry = entry {
// LOWERING-X86_64-GNU-NEXT:         key: b"slate\0".as_ptr() as *mut i8,
// LOWERING-AARCH64-GNU-NEXT:         key: b"slate\0".as_ptr() as *mut u8,
// LOWERING-NEXT:         data: b"24\0".as_ptr() as *mut core::ffi::c_void,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: entry = entry {
// LOWERING-X86_64-GNU-NEXT:         key: b"slate\0".as_ptr() as *mut i8,
// LOWERING-AARCH64-GNU-NEXT:         key: b"slate\0".as_ptr() as *mut u8,
// LOWERING-NEXT:         data: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut entry = std::ptr::null_mut();
// LOWERING-NEXT:     found = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     tree = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [3, 1, 4, 2];
// LOWERING-X86_64-GNU-NEXT:     *values = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     values = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         hcreate_r(
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = ACTION::ENTER as u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         hsearch_r(
// LOWERING-NEXT:             {{__v[0-9]+}} as entry,
// LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(found) as *mut *mut entry,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = ACTION::FIND as u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         hsearch_r(
// LOWERING-NEXT:             {{__v[0-9]+}} as entry,
// LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(found) as *mut *mut entry,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut entry = found;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut entry = {{__v[0-9]+}} as *mut entry;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut entry = found;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{__v[0-9]+}}).data };
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"24\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"24\0".as_ptr() as *mut u8;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:             strcmp(
// LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe { hdestroy_r(std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: u64 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         index = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:                     tsearch(
// LOWERING-NEXT:                         {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:                         std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void,
// LOWERING-NEXT:                         Some(gnu_compare_entries),
// LOWERING-NEXT:                     )
// LOWERING-NEXT:                 };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index;
// LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             index = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)]) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         tfind(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void,
// LOWERING-NEXT:             Some(gnu_compare_entries),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = tree;
// LOWERING-NEXT:     unsafe { tdestroy({{__v[0-9]+}} as *mut core::ffi::c_void, Some(gnu_free_entry)) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn gnu_compare_with_direction(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT: ) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: extern "C-unwind" fn gnu_parse_option({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut argp_state) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: extern "C-unwind" fn gnu_parse_option({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut argp_state) -> i32 {
// LOWERING-NEXT:     let mut key: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut argument: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut argument: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut arguments: *mut GNUArguments = std::ptr::null_mut();
// LOWERING-NEXT:     key = {{arg[0-9]+}};
// LOWERING-NEXT:     argument = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{arg[0-9]+}}).input };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut GNUArguments = {{__v[0-9]+}} as *mut GNUArguments;
// LOWERING-NEXT:     arguments = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = key;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 110;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = argument;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = argument;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { atoi({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:             let {{__v[0-9]+}}: *mut GNUArguments = arguments;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 (*{{__v[0-9]+}}).number = {{__v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             return {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = key;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = argument;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"item\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = argument;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"item\0".as_ptr() as *mut u8;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:                 strcmp(
// LOWERING-NEXT:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                 )
// LOWERING-NEXT:             };
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{__v[0-9]+}}: *mut GNUArguments = arguments;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).positional };
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 (*{{__v[0-9]+}}).positional = {{__v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             return {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = key;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 16777217;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = true;
// LOWERING-NEXT:             {{__v[0-9]+}}
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = key;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 16777219;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:             {{__v[0-9]+}}
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = true;
// LOWERING-NEXT:             {{__v[0-9]+}}
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = key;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 16777218;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:             {{__v[0-9]+}}
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = true;
// LOWERING-NEXT:             {{__v[0-9]+}}
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = key;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 16777220;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:             {{__v[0-9]+}}
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = true;
// LOWERING-NEXT:             {{__v[0-9]+}}
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = key;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 16777223;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:             {{__v[0-9]+}}
// LOWERING-NEXT:         };
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             return {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn gnu_compare_entries(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT: ) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn gnu_free_entry({{arg[0-9]+}}: *mut core::ffi::c_void) {
// LOWERING-NEXT:     unsafe { free({{arg[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum ACTION {
// REWRITES-NEXT:     FIND = 0,
// REWRITES-NEXT:     ENTER = 1,
// REWRITES-NEXT: }
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct GNUArguments {
// REWRITES-NEXT:     number: i32,
// REWRITES-NEXT:     positional: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __tab {
// REWRITES-NEXT:     __slate_empty: [u8; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct argp {
// REWRITES-NEXT:     options: *mut argp_option,
// REWRITES-X86_64-GNU-NEXT:     parser: Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// REWRITES-X86_64-GNU-NEXT:     args_doc: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     doc: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     parser: Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut argp_state) -> i32>,
// REWRITES-AARCH64-GNU-NEXT:     args_doc: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     doc: *mut u8,
// REWRITES-NEXT:     children: *mut argp_child,
// REWRITES-NEXT:     help_filter:
// REWRITES-X86_64-GNU-NEXT:         Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut core::ffi::c_void) -> *mut i8>,
// REWRITES-X86_64-GNU-NEXT:     argp_domain: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:         Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut core::ffi::c_void) -> *mut u8>,
// REWRITES-AARCH64-GNU-NEXT:     argp_domain: *mut u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct argp_child {
// REWRITES-NEXT:     argp: *mut argp,
// REWRITES-NEXT:     flags: i32,
// REWRITES-X86_64-GNU-NEXT:     header: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     header: *mut u8,
// REWRITES-NEXT:     group: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct argp_option {
// REWRITES-X86_64-GNU-NEXT:     name: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     name: *mut u8,
// REWRITES-NEXT:     key: i32,
// REWRITES-X86_64-GNU-NEXT:     arg: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     arg: *mut u8,
// REWRITES-NEXT:     flags: i32,
// REWRITES-X86_64-GNU-NEXT:     doc: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     doc: *mut u8,
// REWRITES-NEXT:     group: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct argp_state {
// REWRITES-NEXT:     root_argp: *mut argp,
// REWRITES-NEXT:     argc: i32,
// REWRITES-X86_64-GNU-NEXT:     argv: *mut *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     argv: *mut *mut u8,
// REWRITES-NEXT:     next: i32,
// REWRITES-NEXT:     flags: u32,
// REWRITES-NEXT:     arg_num: u32,
// REWRITES-NEXT:     quoted: i32,
// REWRITES-NEXT:     input: *mut core::ffi::c_void,
// REWRITES-NEXT:     child_inputs: *mut *mut core::ffi::c_void,
// REWRITES-NEXT:     hook: *mut core::ffi::c_void,
// REWRITES-X86_64-GNU-NEXT:     name: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     name: *mut u8,
// REWRITES-NEXT:     err_stream: *mut libc::FILE,
// REWRITES-NEXT:     out_stream: *mut libc::FILE,
// REWRITES-NEXT:     pstate: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct entry {
// REWRITES-X86_64-GNU-NEXT:     key: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     key: *mut u8,
// REWRITES-NEXT:     data: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct hsearch_data {
// REWRITES-NEXT:     __tab: *mut __tab,
// REWRITES-NEXT:     __unused1: u32,
// REWRITES-NEXT:     __unused2: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct option {
// REWRITES-X86_64-GNU-NEXT:     name: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     name: *mut u8,
// REWRITES-NEXT:     has_arg: i32,
// REWRITES-NEXT:     flag: *mut i32,
// REWRITES-NEXT:     val: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     static mut optarg: *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     static mut optarg: *mut u8;
// REWRITES-NEXT:     static mut opterr: i32;
// REWRITES-NEXT:     static mut optind: i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn qsort_r(
// REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-NEXT:         _1: usize,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:         _3: Option<
// REWRITES-NEXT:             unsafe extern "C-unwind" fn(
// REWRITES-NEXT:                 *mut core::ffi::c_void,
// REWRITES-NEXT:                 *mut core::ffi::c_void,
// REWRITES-NEXT:                 *mut core::ffi::c_void,
// REWRITES-NEXT:             ) -> i32,
// REWRITES-NEXT:         >,
// REWRITES-NEXT:         _4: *mut core::ffi::c_void,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     fn getopt_long(
// REWRITES-NEXT:         _0: i32,
// REWRITES-NEXT:         _1: *mut *mut core::ffi::c_char,
// REWRITES-NEXT:         _2: *const core::ffi::c_char,
// REWRITES-NEXT:         _3: *const option,
// REWRITES-NEXT:         _4: *mut i32,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn argp_parse(
// REWRITES-NEXT:         _0: *const argp,
// REWRITES-NEXT:         _1: i32,
// REWRITES-NEXT:         _2: *mut *mut core::ffi::c_char,
// REWRITES-NEXT:         _3: u32,
// REWRITES-NEXT:         _4: *mut i32,
// REWRITES-NEXT:         _5: *mut core::ffi::c_void,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn hcreate_r(_0: usize, _1: *mut hsearch_data) -> i32;
// REWRITES-NEXT:     fn hsearch_r(_0: entry, _1: u32, _2: *mut *mut entry, _3: *mut hsearch_data) -> i32;
// REWRITES-NEXT:     fn hdestroy_r(_0: *mut hsearch_data);
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn tsearch(
// REWRITES-NEXT:         _0: *const core::ffi::c_void,
// REWRITES-NEXT:         _1: *mut *mut core::ffi::c_void,
// REWRITES-NEXT:         _2: Option<
// REWRITES-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// REWRITES-NEXT:         >,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn tfind(
// REWRITES-NEXT:         _0: *const core::ffi::c_void,
// REWRITES-NEXT:         _1: *mut *mut core::ffi::c_void,
// REWRITES-NEXT:         _2: Option<
// REWRITES-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// REWRITES-NEXT:         >,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn tdestroy(
// REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn __slate_atoi(s: *const i8) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn __slate_atoi(s: *const u8) -> i32 {
// REWRITES-NEXT:     let bytes = unsafe { std::ffi::CStr::from_ptr(s) }.to_bytes();
// REWRITES-NEXT:     let n = bytes.len();
// REWRITES-NEXT:     let mut i = 0usize;
// REWRITES-NEXT:     while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// REWRITES-NEXT:         i += 1usize;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let start = i;
// REWRITES-NEXT:     if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// REWRITES-NEXT:         i += 1usize;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let digits = i;
// REWRITES-NEXT:     while i < n && bytes[i].is_ascii_digit() {
// REWRITES-NEXT:         i += 1usize;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     if i == digits {
// REWRITES-NEXT:         return 0i32;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return std::str::from_utf8(&bytes[(start..i)])
// REWRITES-NEXT:         .unwrap()
// REWRITES-NEXT:         .parse()
// REWRITES-NEXT:         .unwrap_or(0i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             gnu_qsort_extension(),
// REWRITES-NEXT:             gnu_getopt_extensions(),
// REWRITES-NEXT:             gnu_argp_extensions(),
// REWRITES-NEXT:             gnu_search_extensions(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_qsort_extension() -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [4, 1, 3, 2];
// REWRITES-NEXT:     let mut direction: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     *values = [4, 1, 3, 2];
// REWRITES-NEXT:     direction = -1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(direction) as *mut core::ffi::c_void;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         qsort_r(
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:             (4 as u64) as usize,
// REWRITES-NEXT:             (4 as u64) as usize,
// REWRITES-NEXT:             Some(gnu_compare_with_direction),
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     values[0] * 1000 + values[1] * 100 + values[2] * 10 + values[3]
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_getopt_extensions() -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let mut program: [i8; 6] = [112, 114, 111, 98, 101, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut number: [i8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut flag: [i8; 3] = [45, 102, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> =
// REWRITES-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 4]);
// REWRITES-X86_64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A16, [option; 3]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-NEXT:     let mut program: [u8; 6] = [112, 114, 111, 98, 101, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut number: [u8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut flag: [u8; 3] = [45, 102, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut arguments: [*mut u8; 4] = [std::ptr::null_mut(); 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A8, [option; 3]> = aligned::Aligned(
// REWRITES-NEXT:         [option {
// REWRITES-NEXT:             name: std::ptr::null_mut(),
// REWRITES-NEXT:             has_arg: 0,
// REWRITES-NEXT:             flag: std::ptr::null_mut(),
// REWRITES-NEXT:             val: 0,
// REWRITES-NEXT:         }; 3],
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let mut number_value: i32 = 0;
// REWRITES-NEXT:     let mut flag_value: i32 = 0;
// REWRITES-NEXT:     let mut option: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = program.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = program.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = number.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = number.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = flag.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = flag.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = std::ptr::null_mut();
// REWRITES-NEXT:     }
// REWRITES-NEXT:     *options = [
// REWRITES-NEXT:         option {
// REWRITES-X86_64-GNU-NEXT:             name: c"number".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-NEXT:             name: c"number".as_ptr() as *mut u8,
// REWRITES-NEXT:             has_arg: 1,
// REWRITES-NEXT:             flag: std::ptr::null_mut(),
// REWRITES-NEXT:             val: 110,
// REWRITES-NEXT:         },
// REWRITES-NEXT:         option {
// REWRITES-X86_64-GNU-NEXT:             name: c"flag".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-NEXT:             name: c"flag".as_ptr() as *mut u8,
// REWRITES-NEXT:             has_arg: 0,
// REWRITES-NEXT:             flag: std::ptr::null_mut(),
// REWRITES-NEXT:             val: 102,
// REWRITES-NEXT:         },
// REWRITES-NEXT:         option {
// REWRITES-NEXT:             name: std::ptr::null_mut(),
// REWRITES-NEXT:             has_arg: 0,
// REWRITES-NEXT:             flag: std::ptr::null_mut(),
// REWRITES-NEXT:             val: 0,
// REWRITES-NEXT:         },
// REWRITES-NEXT:     ];
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         optind = 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         opterr = 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = c"fn:".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = c"fn:".as_ptr() as *mut u8;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut option = options.as_mut_ptr() as *mut option;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:             getopt_long(
// REWRITES-NEXT:                 3 as i32,
// REWRITES-NEXT:                 {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:                 {{__v[0-9]+}} as *const option,
// REWRITES-NEXT:                 {{__v[0-9]+}} as *mut i32,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:         option = {{__v[0-9]+}};
// REWRITES-NEXT:         if !({{__v[0-9]+}} != -1) {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = option == 110;
// REWRITES-NEXT:         if {{__v[0-9]+}} {
// REWRITES-NEXT:             number_value = unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_atoi(((unsafe { optarg }) as *const core::ffi::c_char) as *const i8)
// REWRITES-AARCH64-GNU-NEXT:                 __slate_atoi(((unsafe { optarg }) as *const core::ffi::c_char) as *const u8)
// REWRITES-NEXT:             };
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             if option == 102 {
// REWRITES-NEXT:                 flag_value = 1;
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     number_value * 10 + flag_value
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_argp_extensions() -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A16, [argp_option; 2]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-NEXT:     let mut options: aligned::Aligned<aligned::A8, [argp_option; 2]> = aligned::Aligned(
// REWRITES-NEXT:         [argp_option {
// REWRITES-NEXT:             name: std::ptr::null_mut(),
// REWRITES-NEXT:             key: 0,
// REWRITES-NEXT:             arg: std::ptr::null_mut(),
// REWRITES-NEXT:             flags: 0,
// REWRITES-NEXT:             doc: std::ptr::null_mut(),
// REWRITES-NEXT:             group: 0,
// REWRITES-NEXT:         }; 2],
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let mut parser: argp = argp {
// REWRITES-NEXT:         options: std::ptr::null_mut(),
// REWRITES-NEXT:         parser: None,
// REWRITES-NEXT:         args_doc: std::ptr::null_mut(),
// REWRITES-NEXT:         doc: std::ptr::null_mut(),
// REWRITES-NEXT:         children: std::ptr::null_mut(),
// REWRITES-NEXT:         help_filter: None,
// REWRITES-NEXT:         argp_domain: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut parsed: GNUArguments = GNUArguments {
// REWRITES-NEXT:         number: 0,
// REWRITES-NEXT:         positional: 0,
// REWRITES-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let mut program: [i8; 6] = [0; 6];
// REWRITES-X86_64-GNU-NEXT:     let mut option: [i8; 11] = [0; 11];
// REWRITES-X86_64-GNU-NEXT:     let mut item: [i8; 5] = [0; 5];
// REWRITES-X86_64-GNU-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 4]> =
// REWRITES-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 4]);
// REWRITES-AARCH64-GNU-NEXT:     let mut program: [u8; 6] = [0; 6];
// REWRITES-AARCH64-GNU-NEXT:     let mut option: [u8; 11] = [0; 11];
// REWRITES-AARCH64-GNU-NEXT:     let mut item: [u8; 5] = [0; 5];
// REWRITES-AARCH64-GNU-NEXT:     let mut arguments: [*mut u8; 4] = [std::ptr::null_mut(); 4];
// REWRITES-NEXT:     *options = [
// REWRITES-NEXT:         argp_option {
// REWRITES-X86_64-GNU-NEXT:             name: c"number".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-NEXT:             name: c"number".as_ptr() as *mut u8,
// REWRITES-NEXT:             key: 110,
// REWRITES-X86_64-GNU-NEXT:             arg: c"VALUE".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-NEXT:             arg: c"VALUE".as_ptr() as *mut u8,
// REWRITES-NEXT:             flags: 0,
// REWRITES-X86_64-GNU-NEXT:             doc: c"number".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-NEXT:             doc: c"number".as_ptr() as *mut u8,
// REWRITES-NEXT:             group: 0,
// REWRITES-NEXT:         },
// REWRITES-NEXT:         argp_option {
// REWRITES-NEXT:             name: std::ptr::null_mut(),
// REWRITES-NEXT:             key: 0,
// REWRITES-NEXT:             arg: std::ptr::null_mut(),
// REWRITES-NEXT:             flags: 0,
// REWRITES-NEXT:             doc: std::ptr::null_mut(),
// REWRITES-NEXT:             group: 0,
// REWRITES-NEXT:         },
// REWRITES-NEXT:     ];
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut argp_option = options.as_mut_ptr() as *mut argp_option;
// REWRITES-NEXT:     parser.options = {{__v[0-9]+}};
// REWRITES-NEXT:     parser.parser = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<
// REWRITES-NEXT:             *const (),
// REWRITES-X86_64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// REWRITES-AARCH64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut u8, *mut argp_state) -> i32>,
// REWRITES-NEXT:         >(gnu_parse_option as *const ())
// REWRITES-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     parser.args_doc = c"ITEM".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     parser.args_doc = c"ITEM".as_ptr() as *mut u8;
// REWRITES-NEXT:     parser.doc = std::ptr::null_mut();
// REWRITES-NEXT:     parser.children = std::ptr::null_mut();
// REWRITES-NEXT:     parser.help_filter = None;
// REWRITES-NEXT:     parser.argp_domain = std::ptr::null_mut();
// REWRITES-NEXT:     parsed = GNUArguments {
// REWRITES-NEXT:         number: 0,
// REWRITES-NEXT:         positional: 0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     program = [112, 114, 111, 98, 101, 0];
// REWRITES-NEXT:     option = [45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0];
// REWRITES-NEXT:     item = [105, 116, 101, 109, 0];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = program.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = program.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = option.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = option.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = item.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = item.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = std::ptr::null_mut();
// REWRITES-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 16;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = (32 | {{__v[0-9]+}}) as u32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(parsed) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         argp_parse(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(parser) as *const argp,
// REWRITES-NEXT:             3 as i32,
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}} as u32,
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut i32,
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     (({{__v[0-9]+}} == 0) as i32) + parsed.number * 10 + parsed.positional
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_search_extensions() -> i32 {
// REWRITES-NEXT:     let mut table: hsearch_data = hsearch_data {
// REWRITES-NEXT:         __tab: std::ptr::null_mut(),
// REWRITES-NEXT:         __unused1: 0,
// REWRITES-NEXT:         __unused2: 0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut found: *mut entry = std::ptr::null_mut();
// REWRITES-NEXT:     let mut tree: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [i32; 4] = [3, 1, 4, 2];
// REWRITES-NEXT:     let {{__v[0-9]+}}: entry = entry {
// REWRITES-X86_64-GNU-NEXT:         key: c"slate".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-NEXT:         key: c"slate".as_ptr() as *mut u8,
// REWRITES-NEXT:         data: b"24\0".as_ptr() as *mut core::ffi::c_void,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: entry = entry {
// REWRITES-X86_64-GNU-NEXT:         key: c"slate".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-NEXT:         key: c"slate".as_ptr() as *mut u8,
// REWRITES-NEXT:         data: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     *values = [3, 1, 4, 2];
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         hcreate_r(
// REWRITES-NEXT:             (8 as u64) as usize,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = ACTION::ENTER as u32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         hsearch_r(
// REWRITES-NEXT:             {{__v[0-9]+}} as entry,
// REWRITES-NEXT:             {{__v[0-9]+}} as u32,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(found) as *mut *mut entry,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = ACTION::FIND as u32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         hsearch_r(
// REWRITES-NEXT:             {{__v[0-9]+}} as entry,
// REWRITES-NEXT:             {{__v[0-9]+}} as u32,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(found) as *mut *mut entry,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(table) as *mut hsearch_data,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = found != ({{__v[0-9]+}} as *mut entry);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:             strcmp(
// REWRITES-X86_64-GNU-NEXT:                 ((unsafe { (*found).data }) as *mut i8) as *const core::ffi::c_char,
// REWRITES-AARCH64-GNU-NEXT:                 ((unsafe { (*found).data }) as *mut u8) as *const core::ffi::c_char,
// REWRITES-NEXT:                 c"24".as_ptr(),
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == 0;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// REWRITES-NEXT:     unsafe { hdestroy_r(std::ptr::addr_of_mut!(table) as *mut hsearch_data) };
// REWRITES-NEXT:     for index in 0..4 {
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc((4 as u64) as usize) };
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *{{__v[0-9]+}} = values[(index as usize)];
// REWRITES-NEXT:         }
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             tsearch(
// REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-NEXT:                 std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void,
// REWRITES-NEXT:                 Some(gnu_compare_entries),
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(values[2]) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// REWRITES-NEXT:         tfind(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(tree) as *mut *mut core::ffi::c_void,
// REWRITES-NEXT:             Some(gnu_compare_entries),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// REWRITES-NEXT:     unsafe { tdestroy(tree as *mut core::ffi::c_void, Some(gnu_free_entry)) };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn gnu_compare_with_direction(
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-NEXT: ) -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *({{arg[0-9]+}} as *mut i32) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *({{arg[0-9]+}} as *mut i32) };
// REWRITES-NEXT:     (unsafe { *({{arg[0-9]+}} as *mut i32) }) * ((({{__v[0-9]+}} > {{__v[0-9]+}}) as i32) - (({{__v[0-9]+}} < {{__v[0-9]+}}) as i32))
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn gnu_parse_option(
// REWRITES-NEXT:     mut key: i32,
// REWRITES-X86_64-GNU-NEXT:     mut argument: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     mut argument: *mut u8,
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut argp_state,
// REWRITES-NEXT: ) -> i32 {
// REWRITES-NEXT:     let mut arguments: *mut GNUArguments = (unsafe { (*{{arg[0-9]+}}).input }) as *mut GNUArguments;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = key == 110;
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { __slate_atoi(argument as *const i8) };
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { __slate_atoi(argument as *const u8) };
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             (*arguments).number = {{__v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:         return 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = key == 0;
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { strcmp(argument as *const core::ffi::c_char, c"item".as_ptr()) };
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut GNUArguments = arguments;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             (*{{__v[0-9]+}}).positional = (unsafe { (*{{__v[0-9]+}}).positional }) + (({{__v[0-9]+}} == 0) as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT:         return 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if key == 16777217 {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = key == 16777219;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = key == 16777218;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = key == 16777220;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = key == 16777223;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         return 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     7
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn gnu_compare_entries(
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-NEXT: ) -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *({{arg[0-9]+}} as *mut i32) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *({{arg[0-9]+}} as *mut i32) };
// REWRITES-NEXT:     (({{__v[0-9]+}} > {{__v[0-9]+}}) as i32) - (({{__v[0-9]+}} < {{__v[0-9]+}}) as i32)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn gnu_free_entry({{arg[0-9]+}}: *mut core::ffi::c_void) {
// REWRITES-NEXT:     unsafe { free({{arg[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
