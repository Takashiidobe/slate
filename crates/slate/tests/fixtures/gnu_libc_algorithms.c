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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut optarg: *mut i8;
// CHECK-NEXT:     static mut optind: i32;
// CHECK-NEXT:     static mut opterr: i32;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn getopt_long(
// CHECK-NEXT:         ___argc: i32,
// CHECK-NEXT:         ___argv: *const *mut i8,
// CHECK-NEXT:         __shortopts: *const i8,
// CHECK-NEXT:         __longopts: *const option,
// CHECK-NEXT:         __longind: *mut i32,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn argp_parse(
// CHECK-NEXT:         __argp: *const argp,
// CHECK-NEXT:         __argc: i32,
// CHECK-NEXT:         __argv: *mut *mut i8,
// CHECK-NEXT:         __flags: u32,
// CHECK-NEXT:         __arg_index: *mut i32,
// CHECK-NEXT:         __input: *mut (),
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn hsearch_r(
// CHECK-NEXT:         __item: entry,
// CHECK-NEXT:         __action: u32,
// CHECK-NEXT:         __retval: *mut *mut entry,
// CHECK-NEXT:         __htab: *mut hsearch_data,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn hcreate_r(__nel: u64, __htab: *mut hsearch_data) -> i32;
// CHECK-NEXT:     fn hdestroy_r(__htab: *mut hsearch_data);
// CHECK-NEXT:     fn tsearch(
// CHECK-NEXT:         __key: *const (),
// CHECK-NEXT:         __rootp: *mut *mut (),
// CHECK-NEXT:         __compar: Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT:     ) -> *mut ();
// CHECK-NEXT:     fn tfind(
// CHECK-NEXT:         __key: *const (),
// CHECK-NEXT:         __rootp: *const *mut (),
// CHECK-NEXT:         __compar: Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT:     ) -> *mut ();
// CHECK-NEXT:     fn tdestroy(
// CHECK-NEXT:         __root: *mut (),
// CHECK-NEXT:         __freefct: Option<unsafe extern "C-unwind" fn(*mut ())>,
// CHECK-NEXT:     );
// CHECK-NEXT:     fn atoi(__nptr: *const i8) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT:     fn qsort_r(
// CHECK-NEXT:         __base: *mut (),
// CHECK-NEXT:         __nmemb: u64,
// CHECK-NEXT:         __size: u64,
// CHECK-NEXT:         __compar: Option<
// CHECK-NEXT:             unsafe extern "C-unwind" fn(*const (), *const (), *mut ()) -> i32,
// CHECK-NEXT:         >,
// CHECK-NEXT:         __arg: *mut (),
// CHECK-NEXT:     );
// CHECK-NEXT:     fn strcmp(__s1: *const i8, __s2: *const i8) -> i32;
// CHECK-NEXT:     fn index(__s: *const i8, __c: i32) -> *mut i8;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_FILE {
// CHECK-NEXT:     _flags: i32,
// CHECK-NEXT:     _IO_read_ptr: *mut i8,
// CHECK-NEXT:     _IO_read_end: *mut i8,
// CHECK-NEXT:     _IO_read_base: *mut i8,
// CHECK-NEXT:     _IO_write_base: *mut i8,
// CHECK-NEXT:     _IO_write_ptr: *mut i8,
// CHECK-NEXT:     _IO_write_end: *mut i8,
// CHECK-NEXT:     _IO_buf_base: *mut i8,
// CHECK-NEXT:     _IO_buf_end: *mut i8,
// CHECK-NEXT:     _IO_save_base: *mut i8,
// CHECK-NEXT:     _IO_backup_base: *mut i8,
// CHECK-NEXT:     _IO_save_end: *mut i8,
// CHECK-NEXT:     _markers: *mut _IO_marker,
// CHECK-NEXT:     _chain: *mut _IO_FILE,
// CHECK-NEXT:     _fileno: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT:     _short_backupbuf: [i8; 1],
// CHECK-NEXT:     _old_offset: i64,
// CHECK-NEXT:     _cur_column: u16,
// CHECK-NEXT:     _vtable_offset: i8,
// CHECK-NEXT:     _shortbuf: [i8; 1],
// CHECK-NEXT:     _lock: *mut (),
// CHECK-NEXT:     _offset: i64,
// CHECK-NEXT:     _codecvt: *mut _IO_codecvt,
// CHECK-NEXT:     _wide_data: *mut _IO_wide_data,
// CHECK-NEXT:     _freeres_list: *mut _IO_FILE,
// CHECK-NEXT:     _freeres_buf: *mut (),
// CHECK-NEXT:     _prevchain: *mut *mut _IO_FILE,
// CHECK-NEXT:     _mode: i32,
// CHECK-NEXT:     _unused3: i32,
// CHECK-NEXT:     _total_written: u64,
// CHECK-NEXT:     _unused2: [i8; 8],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_marker {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_codecvt {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_wide_data {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct option {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     has_arg: i32,
// CHECK-NEXT:     flag: *mut i32,
// CHECK-NEXT:     val: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct argp_option {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     key: i32,
// CHECK-NEXT:     arg: *const i8,
// CHECK-NEXT:     flags: i32,
// CHECK-NEXT:     doc: *const i8,
// CHECK-NEXT:     group: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct argp {
// CHECK-NEXT:     options: *const argp_option,
// CHECK-NEXT:     parser: Option<unsafe extern "C-unwind" fn(i32, *mut i8, *mut argp_state) -> i32>,
// CHECK-NEXT:     args_doc: *const i8,
// CHECK-NEXT:     doc: *const i8,
// CHECK-NEXT:     children: *const argp_child,
// CHECK-NEXT:     help_filter: Option<unsafe extern "C-unwind" fn(i32, *const i8, *mut ()) -> *mut i8>,
// CHECK-NEXT:     argp_domain: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct argp_child {
// CHECK-NEXT:     argp: *const argp,
// CHECK-NEXT:     flags: i32,
// CHECK-NEXT:     header: *const i8,
// CHECK-NEXT:     group: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct argp_state {
// CHECK-NEXT:     root_argp: *const argp,
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut i8,
// CHECK-NEXT:     next: i32,
// CHECK-NEXT:     flags: u32,
// CHECK-NEXT:     arg_num: u32,
// CHECK-NEXT:     quoted: i32,
// CHECK-NEXT:     input: *mut (),
// CHECK-NEXT:     child_inputs: *mut *mut (),
// CHECK-NEXT:     hook: *mut (),
// CHECK-NEXT:     name: *mut i8,
// CHECK-NEXT:     err_stream: *mut _IO_FILE,
// CHECK-NEXT:     out_stream: *mut _IO_FILE,
// CHECK-NEXT:     pstate: *mut (),
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct entry {
// CHECK-NEXT:     key: *mut i8,
// CHECK-NEXT:     data: *mut (),
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _ENTRY {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct hsearch_data {
// CHECK-NEXT:     table: *mut _ENTRY,
// CHECK-NEXT:     size: u32,
// CHECK-NEXT:     filled: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct GNUArguments {
// CHECK-NEXT:     number: i32,
// CHECK-NEXT:     positional: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         [u8;3],
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits4U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_atoi(s: *const i8) -> i32 {
// CHECK-NEXT:     let bytes = unsafe { std::ffi::CStr::from_ptr(s) }.to_bytes();
// CHECK-NEXT:     let n = bytes.len();
// CHECK-NEXT:     let mut i = 0usize;
// CHECK-NEXT:     while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     let start = i;
// CHECK-NEXT:     if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     let digits = i;
// CHECK-NEXT:     while i < n && bytes[i].is_ascii_digit() {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     if i == digits {
// CHECK-NEXT:         return 0i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     return std::str::from_utf8(&bytes[start..i])
// CHECK-NEXT:         .unwrap()
// CHECK-NEXT:         .parse::<i128>()
// CHECK-NEXT:         .unwrap_or(0i128)
// CHECK-NEXT:         .clamp(i32::MIN as i128, i32::MAX as i128) as i32;
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn gnu_compare_with_direction(
// CHECK-NEXT:     left: *const (),
// CHECK-NEXT:     right: *const (),
// CHECK-NEXT:     state: *mut (),
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let direction: i32 = unsafe { *(state as *mut i32) };
// CHECK-NEXT:     let a: i32 = unsafe { *(left as *const i32) };
// CHECK-NEXT:     let b: i32 = unsafe { *(right as *const i32) };
// CHECK-NEXT:     direction * (((a > b) as i32) - ((a < b) as i32))
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn gnu_compare_entries(left: *const (), right: *const ()) -> i32 {
// CHECK-NEXT:     let a: i32 = unsafe { *(left as *const i32) };
// CHECK-NEXT:     let b: i32 = unsafe { *(right as *const i32) };
// CHECK-NEXT:     ((a > b) as i32) - ((a < b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn gnu_free_entry(entry: *mut ()) {
// CHECK-NEXT:     unsafe { free(entry) };
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn gnu_parse_option(
// CHECK-NEXT:     key: i32,
// CHECK-NEXT:     argument: *mut i8,
// CHECK-NEXT:     state: *mut argp_state,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let arguments: *mut GNUArguments = (unsafe { (*state).input }) as *mut GNUArguments;
// CHECK-NEXT:     if key == 110 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*arguments).number = unsafe { __slate_atoi(argument as *const i8) };
// CHECK-NEXT:         }
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     if key == 0 {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe { (*arguments).positional };
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = [[V_0]]
// CHECK-NEXT:             + (((unsafe { strcmp(argument as *const i8, c"item".as_ptr()) }) == 0)
// CHECK-NEXT:                 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*arguments).positional = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     if key == 16777217 || key == 16777219 || key == 16777218 || key == 16777220
// CHECK-NEXT:         || key == 16777223
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_qsort_extension() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut direction: i32 = -1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         qsort_r(
// CHECK-NEXT:             values.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:             Some(gnu_compare_with_direction),
// CHECK-NEXT:             std::ptr::addr_of_mut!(direction) as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     values.0[0] * 1000 + values.0[1] * 100 + values.0[2] * 10 + values.0[3]
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_getopt_extensions() -> i32 {
// CHECK-NEXT:     let mut program: [i8; 6] = [112, 114, 111, 98, 101, 0];
// CHECK-NEXT:     let mut number: [i8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0];
// CHECK-NEXT:     let mut flag: [i8; 3] = [45, 102, 0];
// CHECK-NEXT:     let mut arguments: __SlateAlign16<[*mut i8; 4]> = __SlateAlign16([
// CHECK-NEXT:         program.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         number.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         flag.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         std::ptr::null_mut::<i8>(),
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut options: __SlateAlign16<[option; 3]> = __SlateAlign16([
// CHECK-NEXT:         option {
// CHECK-NEXT:             name: c"number".as_ptr(),
// CHECK-NEXT:             has_arg: 1,
// CHECK-NEXT:             flag: std::ptr::null_mut::<i32>(),
// CHECK-NEXT:             val: 110,
// CHECK-NEXT:         },
// CHECK-NEXT:         option {
// CHECK-NEXT:             name: c"flag".as_ptr(),
// CHECK-NEXT:             has_arg: 0,
// CHECK-NEXT:             flag: std::ptr::null_mut::<i32>(),
// CHECK-NEXT:             val: 102,
// CHECK-NEXT:         },
// CHECK-NEXT:         option {
// CHECK-NEXT:             name: std::ptr::null::<i8>(),
// CHECK-NEXT:             has_arg: 0,
// CHECK-NEXT:             flag: std::ptr::null_mut::<i32>(),
// CHECK-NEXT:             val: 0,
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut number_value: i32 = 0;
// CHECK-NEXT:     let mut flag_value: i32 = 0;
// CHECK-NEXT:     let mut option: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         optind = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         opterr = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             getopt_long(
// CHECK-NEXT:                 3 as i32,
// CHECK-NEXT:                 arguments.0.as_mut_ptr() as *const *mut i8,
// CHECK-NEXT:                 c"fn:".as_ptr(),
// CHECK-NEXT:                 options.0.as_mut_ptr() as *const option,
// CHECK-NEXT:                 std::ptr::null_mut::<i32>(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         option = [[V_2]];
// CHECK-NEXT:         if !([[V_2]] != -1) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         if option == 110 {
// CHECK-NEXT:             number_value = unsafe { __slate_atoi((unsafe { optarg }) as *const i8) };
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if option == 102 {
// CHECK-NEXT:                 flag_value = 1;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     number_value * 10 + flag_value
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_argp_extensions() -> i32 {
// CHECK-NEXT:     let mut options: __SlateAlign16<[argp_option; 2]> = __SlateAlign16([
// CHECK-NEXT:         argp_option {
// CHECK-NEXT:             name: c"number".as_ptr(),
// CHECK-NEXT:             key: 110,
// CHECK-NEXT:             arg: c"VALUE".as_ptr(),
// CHECK-NEXT:             flags: 0,
// CHECK-NEXT:             doc: c"number".as_ptr(),
// CHECK-NEXT:             group: 0,
// CHECK-NEXT:         },
// CHECK-NEXT:         argp_option {
// CHECK-NEXT:             name: std::ptr::null::<i8>(),
// CHECK-NEXT:             key: 0,
// CHECK-NEXT:             arg: std::ptr::null::<i8>(),
// CHECK-NEXT:             flags: 0,
// CHECK-NEXT:             doc: std::ptr::null::<i8>(),
// CHECK-NEXT:             group: 0,
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut parser: argp = argp {
// CHECK-NEXT:         options: options.0.as_mut_ptr() as *const argp_option,
// CHECK-NEXT:         parser: Some(gnu_parse_option),
// CHECK-NEXT:         args_doc: c"ITEM".as_ptr(),
// CHECK-NEXT:         doc: std::ptr::null::<i8>(),
// CHECK-NEXT:         children: std::ptr::null::<argp_child>(),
// CHECK-NEXT:         help_filter: None,
// CHECK-NEXT:         argp_domain: std::ptr::null::<i8>(),
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut parsed: GNUArguments = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: GNUArguments = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut program: [i8; 6] = [112, 114, 111, 98, 101, 0];
// CHECK-NEXT:     let mut option: [i8; 11] = [45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0];
// CHECK-NEXT:     let mut item: [i8; 5] = [105, 116, 101, 109, 0];
// CHECK-NEXT:     let mut arguments: __SlateAlign16<[*mut i8; 4]> = __SlateAlign16([
// CHECK-NEXT:         program.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         option.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         item.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         std::ptr::null_mut::<i8>(),
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let result: i32 = unsafe {
// CHECK-NEXT:         argp_parse(
// CHECK-NEXT:             std::ptr::addr_of_mut!(parser) as *const argp,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             arguments.0.as_mut_ptr() as *mut *mut i8,
// CHECK-NEXT:             ((32 as i32) | (16 as i32)) as u32,
// CHECK-NEXT:             std::ptr::null_mut::<i32>(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(parsed) as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     ((result == 0) as i32) + parsed.number * 10 + parsed.positional
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_search_extensions() -> i32 {
// CHECK-NEXT:     let mut table: hsearch_data = {
// CHECK-NEXT:         let mut [[T_0]]: hsearch_data = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let inserted: entry = entry {
// CHECK-NEXT:         key: c"slate".as_ptr() as *mut i8,
// CHECK-NEXT:         data: b"24\0".as_ptr() as *mut (),
// CHECK-NEXT:     };
// CHECK-NEXT:     let query: entry = entry {
// CHECK-NEXT:         key: c"slate".as_ptr() as *mut i8,
// CHECK-NEXT:         data: std::ptr::null_mut::<()>(),
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut found: *mut entry = std::ptr::null_mut::<entry>();
// CHECK-NEXT:     let mut tree: *mut () = std::ptr::null_mut::<()>();
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_3]]
// CHECK-NEXT:         + (((unsafe { hcreate_r(8 as u64, std::ptr::addr_of_mut!(table)) }) != 0)
// CHECK-NEXT:             as i32);
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_4]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             hsearch_r(
// CHECK-NEXT:                 inserted,
// CHECK-NEXT:                 1 as u32,
// CHECK-NEXT:                 std::ptr::addr_of_mut!(found),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(table),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != 0) as i32);
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_5]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             hsearch_r(
// CHECK-NEXT:                 query,
// CHECK-NEXT:                 0 as u32,
// CHECK-NEXT:                 std::ptr::addr_of_mut!(found),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(table),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != 0) as i32);
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_6]]
// CHECK-NEXT:         + ((found != std::ptr::null_mut::<entry>()
// CHECK-NEXT:             && (unsafe {
// CHECK-NEXT:                 strcmp((unsafe { (*found).data }) as *const i8, c"24".as_ptr())
// CHECK-NEXT:             }) == 0) as i32);
// CHECK-NEXT:     unsafe { hdestroy_r(std::ptr::addr_of_mut!(table)) };
// CHECK-NEXT:     for index in 0 as u64..4 as u64 {
// CHECK-NEXT:         let value: *mut i32 = (unsafe { malloc(4 as u64) }) as *mut i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *value = values.0[index as usize];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             tsearch(
// CHECK-NEXT:                 value as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(tree),
// CHECK-NEXT:                 Some(gnu_compare_entries),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_7]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             tfind(
// CHECK-NEXT:                 (unsafe { (values.0.as_mut_ptr() as *mut i32).offset(2 as isize) })
// CHECK-NEXT:                     as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(tree) as *const *mut (),
// CHECK-NEXT:                 Some(gnu_compare_entries),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != std::ptr::null_mut::<()>()) as i32);
// CHECK-NEXT:     unsafe { tdestroy(tree, Some(gnu_free_entry)) };
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             gnu_qsort_extension(),
// CHECK-NEXT:             gnu_getopt_extensions(),
// CHECK-NEXT:             gnu_argp_extensions(),
// CHECK-NEXT:             gnu_search_extensions(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
