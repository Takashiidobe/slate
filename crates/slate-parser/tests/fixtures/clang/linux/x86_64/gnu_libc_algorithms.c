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





// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___off_t:[0-9]+]] __off_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___off64_t:[0-9]+]] __off64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE__IO_FILE:[0-9]+]] _IO_FILE = struct {
// DEFAULT-NEXT:         field0 _flags: i32;
// DEFAULT-NEXT:         field1 _IO_read_ptr: ptr<i8>;
// DEFAULT-NEXT:         field2 _IO_read_end: ptr<i8>;
// DEFAULT-NEXT:         field3 _IO_read_base: ptr<i8>;
// DEFAULT-NEXT:         field4 _IO_write_base: ptr<i8>;
// DEFAULT-NEXT:         field5 _IO_write_ptr: ptr<i8>;
// DEFAULT-NEXT:         field6 _IO_write_end: ptr<i8>;
// DEFAULT-NEXT:         field7 _IO_buf_base: ptr<i8>;
// DEFAULT-NEXT:         field8 _IO_buf_end: ptr<i8>;
// DEFAULT-NEXT:         field9 _IO_save_base: ptr<i8>;
// DEFAULT-NEXT:         field10 _IO_backup_base: ptr<i8>;
// DEFAULT-NEXT:         field11 _IO_save_end: ptr<i8>;
// DEFAULT-NEXT:         field12 _markers: ptr<@type[[TYPE__IO_marker:[0-9]+]]>;
// DEFAULT-NEXT:         field13 _chain: ptr<@type[[TYPE__IO_FILE]]>;
// DEFAULT-NEXT:         field14 _fileno: i32;
// DEFAULT-NEXT:         field15 _flags2: i32 : 24;
// DEFAULT-NEXT:         field16 _short_backupbuf: array<i8, 1>;
// DEFAULT-NEXT:         field17 _old_offset: i64;
// DEFAULT-NEXT:         field18 _cur_column: u16;
// DEFAULT-NEXT:         field19 _vtable_offset: i8;
// DEFAULT-NEXT:         field20 _shortbuf: array<i8, 1>;
// DEFAULT-NEXT:         field21 _lock: ptr<void>;
// DEFAULT-NEXT:         field22 _offset: i64;
// DEFAULT-NEXT:         field23 _codecvt: ptr<@type[[TYPE__IO_codecvt:[0-9]+]]>;
// DEFAULT-NEXT:         field24 _wide_data: ptr<@type[[TYPE__IO_wide_data:[0-9]+]]>;
// DEFAULT-NEXT:         field25 _freeres_list: ptr<@type[[TYPE__IO_FILE]]>;
// DEFAULT-NEXT:         field26 _freeres_buf: ptr<void>;
// DEFAULT-NEXT:         field27 _prevchain: ptr<ptr<@type[[TYPE__IO_FILE]]>>;
// DEFAULT-NEXT:         field28 _mode: i32;
// DEFAULT-NEXT:         field29 _unused3: i32;
// DEFAULT-NEXT:         field30 _total_written: u64;
// DEFAULT-NEXT:         field31 _unused2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=216, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 116, 119, 120, 128, 130, 131, 136, 144, 152, 160, 168, 176, 184, 192, 196, 200, 208], bit_offsets=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(928), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], bit_units=[(116, 3)], field_units=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(0), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = @type[[TYPE__IO_FILE]];
// DEFAULT-NEXT:     type @type[[TYPE__IO_marker]] _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_codecvt]] _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_wide_data]] _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_lock_t:[0-9]+]] _IO_lock_t = void;
// DEFAULT-NEXT:     type @type[[TYPE_option:[0-9]+]] option = struct {
// DEFAULT-NEXT:         field0 name: ptr<const i8>;
// DEFAULT-NEXT:         field1 has_arg: i32;
// DEFAULT-NEXT:         field2 flag: ptr<i32>;
// DEFAULT-NEXT:         field3 val: i32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_error_t:[0-9]+]] error_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_argp_option:[0-9]+]] argp_option = struct {
// DEFAULT-NEXT:         field0 name: ptr<const i8>;
// DEFAULT-NEXT:         field1 key: i32;
// DEFAULT-NEXT:         field2 arg: ptr<const i8>;
// DEFAULT-NEXT:         field3 flags: i32;
// DEFAULT-NEXT:         field4 doc: ptr<const i8>;
// DEFAULT-NEXT:         field5 group: i32;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 8, 16, 24, 32, 40]];
// DEFAULT-NEXT:     type @type[[TYPE_argp:[0-9]+]] argp = struct {
// DEFAULT-NEXT:         field0 options: ptr<const @type[[TYPE_argp_option]]>;
// DEFAULT-NEXT:         field1 parser: ptr<fn(i32, ptr<i8>, ptr<@type[[TYPE_argp_state:[0-9]+]]>) -> i32>;
// DEFAULT-NEXT:         field2 args_doc: ptr<const i8>;
// DEFAULT-NEXT:         field3 doc: ptr<const i8>;
// DEFAULT-NEXT:         field4 children: ptr<const @type[[TYPE_argp_child:[0-9]+]]>;
// DEFAULT-NEXT:         field5 help_filter: ptr<fn(i32, ptr<const i8>, ptr<void>) -> ptr<i8>>;
// DEFAULT-NEXT:         field6 argp_domain: ptr<const i8>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 8, 16, 24, 32, 40, 48]];
// DEFAULT-NEXT:     type @type[[TYPE_argp_state]] argp_state = struct {
// DEFAULT-NEXT:         field0 root_argp: ptr<const @type[[TYPE_argp]]>;
// DEFAULT-NEXT:         field1 argc: i32;
// DEFAULT-NEXT:         field2 argv: ptr<ptr<i8>>;
// DEFAULT-NEXT:         field3 next: i32;
// DEFAULT-NEXT:         field4 flags: u32;
// DEFAULT-NEXT:         field5 arg_num: u32;
// DEFAULT-NEXT:         field6 quoted: i32;
// DEFAULT-NEXT:         field7 input: ptr<void>;
// DEFAULT-NEXT:         field8 child_inputs: ptr<ptr<void>>;
// DEFAULT-NEXT:         field9 hook: ptr<void>;
// DEFAULT-NEXT:         field10 name: ptr<i8>;
// DEFAULT-NEXT:         field11 err_stream: ptr<@type[[TYPE__IO_FILE]]>;
// DEFAULT-NEXT:         field12 out_stream: ptr<@type[[TYPE__IO_FILE]]>;
// DEFAULT-NEXT:         field13 pstate: ptr<void>;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0, 8, 16, 24, 28, 32, 36, 40, 48, 56, 64, 72, 80, 88]];
// DEFAULT-NEXT:     type @type[[TYPE_argp_child]] argp_child = struct {
// DEFAULT-NEXT:         field0 argp: ptr<const @type[[TYPE_argp]]>;
// DEFAULT-NEXT:         field1 flags: i32;
// DEFAULT-NEXT:         field2 header: ptr<const i8>;
// DEFAULT-NEXT:         field3 group: i32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_argp_parser_t:[0-9]+]] argp_parser_t = ptr<fn(i32, ptr<i8>, ptr<@type[[TYPE_argp_state]]>) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE___compar_fn_t:[0-9]+]] __compar_fn_t = ptr<fn(ptr<const void>, ptr<const void>) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_FIND:[0-9]+]] FIND = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_ENTER:[0-9]+]] ENTER = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_ACTION:[0-9]+]] ACTION = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_entry:[0-9]+]] entry = struct {
// DEFAULT-NEXT:         field0 key: ptr<i8>;
// DEFAULT-NEXT:         field1 data: ptr<void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_ENTRY:[0-9]+]] ENTRY = @type[[TYPE_entry]];
// DEFAULT-NEXT:     type @type[[TYPE__ENTRY:[0-9]+]] _ENTRY = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_hsearch_data:[0-9]+]] hsearch_data = struct {
// DEFAULT-NEXT:         field0 table: ptr<@type[[TYPE__ENTRY]]>;
// DEFAULT-NEXT:         field1 size: u32;
// DEFAULT-NEXT:         field2 filled: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE___free_fn_t:[0-9]+]] __free_fn_t = ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:     type @type[[TYPE___compar_d_fn_t:[0-9]+]] __compar_d_fn_t = ptr<fn(ptr<const void>, ptr<const void>, ptr<void>) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_GNUArguments:[0-9]+]] GNUArguments = struct {
// DEFAULT-NEXT:         field0 number: i32;
// DEFAULT-NEXT:         field1 positional: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     extern %[[VALUE_optarg:[0-9]+]] optarg: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_optind:[0-9]+]] optind: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_opterr:[0-9]+]] opterr: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([105, 116, 101, 109, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([110, 117, 109, 98, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 108, 97, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 110, 58, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([110, 117, 109, 98, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([110, 117, 109, 98, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([73, 84, 69, 77, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([50, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([50, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_getopt_long:[0-9]+]] @getopt_long(%[[VALUE____argc:[0-9]+]] ___argc: i32, %[[VALUE____argv:[0-9]+]] ___argv: ptr<const ptr<i8>>, %[[VALUE___shortopts:[0-9]+]] __shortopts: ptr<const i8>, %[[VALUE___longopts:[0-9]+]] __longopts: ptr<const @type[[TYPE_option]]>, %[[VALUE___longind:[0-9]+]] __longind: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_argp_parse:[0-9]+]] @argp_parse(%[[VALUE___argp:[0-9]+]] __argp: ptr<const @type[[TYPE_argp]]> [restrict], %[[VALUE___argc:[0-9]+]] __argc: i32, %[[VALUE___argv:[0-9]+]] __argv: ptr<ptr<i8>> [restrict], %[[VALUE___flags:[0-9]+]] __flags: u32, %[[VALUE___arg_index:[0-9]+]] __arg_index: ptr<i32> [restrict], %[[VALUE___input:[0-9]+]] __input: ptr<void> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_hsearch_r:[0-9]+]] @hsearch_r(%[[VALUE___item:[0-9]+]] __item: @type[[TYPE_entry]], %[[VALUE___action:[0-9]+]] __action: @type[[TYPE0]], %[[VALUE___retval:[0-9]+]] __retval: ptr<ptr<@type[[TYPE_entry]]>>, %[[VALUE___htab:[0-9]+]] __htab: ptr<@type[[TYPE_hsearch_data]]>) -> i32 [linkage=external] [abi=sysv64(native_c, scalar, scalar, scalar) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_hcreate_r:[0-9]+]] @hcreate_r(%[[VALUE___nel:[0-9]+]] __nel: u64, %[[VALUE___htab_2:[0-9]+]] __htab: ptr<@type[[TYPE_hsearch_data]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_hdestroy_r:[0-9]+]] @hdestroy_r(%[[VALUE___htab_3:[0-9]+]] __htab: ptr<@type[[TYPE_hsearch_data]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tsearch:[0-9]+]] @tsearch(%[[VALUE___key:[0-9]+]] __key: ptr<const void>, %[[VALUE___rootp:[0-9]+]] __rootp: ptr<ptr<void>>, %[[VALUE___compar:[0-9]+]] __compar: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tfind:[0-9]+]] @tfind(%[[VALUE___key_2:[0-9]+]] __key: ptr<const void>, %[[VALUE___rootp_2:[0-9]+]] __rootp: ptr<const ptr<void>>, %[[VALUE___compar_2:[0-9]+]] __compar: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tdestroy:[0-9]+]] @tdestroy(%[[VALUE___root:[0-9]+]] __root: ptr<void>, %[[VALUE___freefct:[0-9]+]] __freefct: ptr<fn(ptr<void>) -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atoi:[0-9]+]] @atoi(%[[VALUE___nptr:[0-9]+]] __nptr: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_qsort_r:[0-9]+]] @qsort_r(%[[VALUE___base:[0-9]+]] __base: ptr<void>, %[[VALUE___nmemb:[0-9]+]] __nmemb: u64, %[[VALUE___size_2:[0-9]+]] __size: u64, %[[VALUE___compar_3:[0-9]+]] __compar: ptr<fn(ptr<const void>, ptr<const void>, ptr<void>) -> i32>, %[[VALUE___arg:[0-9]+]] __arg: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_index:[0-9]+]] @index(%[[VALUE___s:[0-9]+]] __s: ptr<const i8>, %[[VALUE___c:[0-9]+]] __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_gnu_compare_with_direction:[0-9]+]] @gnu_compare_with_direction(%[[VALUE_left:[0-9]+]] left: ptr<const void>, %[[VALUE_right:[0-9]+]] right: ptr<const void>, %[[VALUE_state:[0-9]+]] state: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_direction:[0-9]+]] direction: i32 [storage=automatic] = read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_state]]))));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = read<i32>(deref(pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%[[VALUE_left]]))));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = read<i32>(deref(pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%[[VALUE_right]]))));
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%[[VALUE_direction]]), sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]))), from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_compare_entries:[0-9]+]] @gnu_compare_entries(%[[VALUE_left_2:[0-9]+]] left: ptr<const void>, %[[VALUE_right_2:[0-9]+]] right: ptr<const void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic] [const] = read<i32>(deref(pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%[[VALUE_left_2]]))));
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: i32 [storage=automatic] [const] = read<i32>(deref(pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%[[VALUE_right_2]]))));
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]))), from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_free_entry:[0-9]+]] @gnu_free_entry(%[[VALUE_entry:[0-9]+]] entry: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_entry]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_parse_option:[0-9]+]] @gnu_parse_option(%[[VALUE_key:[0-9]+]] key: i32, %[[VALUE_argument:[0-9]+]] argument: ptr<i8>, %[[VALUE_state_2:[0-9]+]] state: ptr<@type[[TYPE_argp_state]]>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_arguments:[0-9]+]] arguments: ptr<@type[[TYPE_GNUArguments]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_GNUArguments]]>, reason=assign>(read<ptr<void>>(field7(deref(read<ptr<@type[[TYPE_argp_state]]>>(%[[VALUE_state_2]])))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_key]]), const<i32>(110))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(field0(deref(read<ptr<@type[[TYPE_GNUArguments]]>>(%[[VALUE_arguments]]))), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_argument]]))));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_key]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE0:[0-9]+]]: ptr<@type[[TYPE_GNUArguments]]> [synthetic] = read<ptr<@type[[TYPE_GNUArguments]]>>(%[[VALUE_arguments]]);
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(field1(deref(read<ptr<@type[[TYPE_GNUArguments]]>>(%[[VALUE0]]))));
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_argument]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]]))), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(field1(deref(read<ptr<@type[[TYPE_GNUArguments]]>>(%[[VALUE0]]))), read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(eq<i32>(read<i32>(%[[VALUE_key]]), const<i32>(16777217)), eq<i32>(read<i32>(%[[VALUE_key]]), const<i32>(16777219))), eq<i32>(read<i32>(%[[VALUE_key]]), const<i32>(16777218))), eq<i32>(read<i32>(%[[VALUE_key]]), const<i32>(16777220))), eq<i32>(read<i32>(%[[VALUE_key]]), const<i32>(16777223)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_qsort_extension:[0-9]+]] @gnu_qsort_extension() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_values:[0-9]+]] values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(1), index2 = const<i32>(3), index3 = const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_direction_2:[0-9]+]] direction: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>, ptr<void>) -> i32>, ptr<void>) -> void>(%[[VALUE_qsort_r]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), const<u64>(4), function_decay<ptr<fn(ptr<const void>, ptr<const void>, ptr<void>) -> i32>>(%[[VALUE_gnu_compare_with_direction]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_direction_2]])));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]]), const<i32>(0)))), const<i32>(1000)), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]]), const<i32>(1)))), const<i32>(100))), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]]), const<i32>(2)))), const<i32>(10))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]]), const<i32>(3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_getopt_extensions:[0-9]+]] @gnu_getopt_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_program:[0-9]+]] program: array<i8, 6> [storage=automatic] = code_units<array<i8, 6>>([112, 114, 111, 98, 101, 0]);
// DEFAULT-NEXT:         let %[[VALUE_number:[0-9]+]] number: array<i8, 11> [storage=automatic] = code_units<array<i8, 11>>([45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0]);
// DEFAULT-NEXT:         let %[[VALUE_flag:[0-9]+]] flag: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([45, 102, 0]);
// DEFAULT-NEXT:         let %[[VALUE_arguments_2:[0-9]+]] arguments: array<ptr<i8>, 4> [storage=automatic] [align=16] = aggregate<array<ptr<i8>, 4>, zero_fill=false>(index0 = array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_program]]), index1 = array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_number]]), index2 = array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_flag]]), index3 = null<ptr<i8>>);
// DEFAULT-NEXT:         let %[[VALUE_options:[0-9]+]] options: array<@type[[TYPE_option]], 3> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_option]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_option]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_2]])), field1 = const<i32>(1), field2 = null<ptr<i32>>, field3 = const<i32>(110)), index1 = aggregate<@type[[TYPE_option]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_3]])), field1 = const<i32>(0), field2 = null<ptr<i32>>, field3 = const<i32>(102)), index2 = aggregate<@type[[TYPE_option]], zero_fill=false>(field0 = null<ptr<const i8>>, field1 = const<i32>(0), field2 = null<ptr<i32>>, field3 = const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_number_value:[0-9]+]] number_value: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_flag_value:[0-9]+]] flag_value: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_option:[0-9]+]] option: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_optind]], const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_opterr]], const<i32>(0));
// DEFAULT-NEXT:         while %[[VALUE3:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = call<i32, signature=fn(i32, ptr<const ptr<i8>>, ptr<const i8>, ptr<const @type[[TYPE_option]]>, ptr<i32>) -> i32>(%[[VALUE_getopt_long]], const<i32>(3), pointer_cast<ptr<const ptr<i8>>, reason=arg>(array_decay<ptr<ptr<i8>>, length=Some(4)>(%[[VALUE_arguments_2]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])), pointer_cast<ptr<const @type[[TYPE_option]]>, reason=arg>(array_decay<ptr<@type[[TYPE_option]]>, length=Some(3)>(%[[VALUE_options]])), null<ptr<i32>>);
// DEFAULT-NEXT:             write<i32>(%[[VALUE_option]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE4]]), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_option]]), const<i32>(110))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_number_value]], call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_optarg]]))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%[[VALUE_option]]), const<i32>(102))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_flag_value]], const<i32>(1));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_number_value]]), const<i32>(10)), read<i32>(%[[VALUE_flag_value]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_argp_extensions:[0-9]+]] @gnu_argp_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_options_2:[0-9]+]] options: array<@type[[TYPE_argp_option]], 2> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_argp_option]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_argp_option]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_5]])), field1 = const<i32>(110), field2 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_6]])), field3 = const<i32>(0), field4 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_7]])), field5 = const<i32>(0)), index1 = aggregate<@type[[TYPE_argp_option]], zero_fill=false>(field0 = null<ptr<const i8>>, field1 = const<i32>(0), field2 = null<ptr<const i8>>, field3 = const<i32>(0), field4 = null<ptr<const i8>>, field5 = const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_parser:[0-9]+]] parser: @type[[TYPE_argp]] [storage=automatic] = aggregate<@type[[TYPE_argp]], zero_fill=false>(field0 = pointer_cast<ptr<const @type[[TYPE_argp_option]]>, reason=assign>(array_decay<ptr<@type[[TYPE_argp_option]]>, length=Some(2)>(%[[VALUE_options_2]])), field1 = function_decay<ptr<fn(i32, ptr<i8>, ptr<@type[[TYPE_argp_state]]>) -> i32>>(%[[VALUE_gnu_parse_option]]), field2 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_8]])), field3 = null<ptr<const i8>>, field4 = null<ptr<const @type[[TYPE_argp_child]]>>, field5 = null<ptr<fn(i32, ptr<const i8>, ptr<void>) -> ptr<i8>>>, field6 = null<ptr<const i8>>);
// DEFAULT-NEXT:         let %[[VALUE_parsed:[0-9]+]] parsed: @type[[TYPE_GNUArguments]] [storage=automatic] = aggregate<@type[[TYPE_GNUArguments]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_program_2:[0-9]+]] program: array<i8, 6> [storage=automatic] = code_units<array<i8, 6>>([112, 114, 111, 98, 101, 0]);
// DEFAULT-NEXT:         let %[[VALUE_option_2:[0-9]+]] option: array<i8, 11> [storage=automatic] = code_units<array<i8, 11>>([45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0]);
// DEFAULT-NEXT:         let %[[VALUE_item:[0-9]+]] item: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([105, 116, 101, 109, 0]);
// DEFAULT-NEXT:         let %[[VALUE_arguments_3:[0-9]+]] arguments: array<ptr<i8>, 4> [storage=automatic] [align=16] = aggregate<array<ptr<i8>, 4>, zero_fill=false>(index0 = array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_program_2]]), index1 = array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_option_2]]), index2 = array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_item]]), index3 = null<ptr<i8>>);
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic] = call<i32, signature=fn(ptr<const @type[[TYPE_argp]]>, i32, ptr<ptr<i8>>, u32, ptr<i32>, ptr<void>) -> i32>(%[[VALUE_argp_parse]], pointer_cast<ptr<const @type[[TYPE_argp]]>, reason=arg>(addr_of<ptr<@type[[TYPE_argp]]>>(%[[VALUE_parser]])), const<i32>(3), array_decay<ptr<ptr<i8>>, length=Some(4)>(%[[VALUE_arguments_3]]), reinterpret<u32, reason=arg, fits=unknown>(or<i32>(const<i32>(32), const<i32>(16))), null<ptr<i32>>, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_GNUArguments]]>>(%[[VALUE_parsed]])));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_result]]), const<i32>(0))), mul<i32, overflow=ub>(read<i32>(field0(%[[VALUE_parsed]])), const<i32>(10))), read<i32>(field1(%[[VALUE_parsed]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_search_extensions:[0-9]+]] @gnu_search_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_table:[0-9]+]] table: @type[[TYPE_hsearch_data]] [storage=automatic] = aggregate<@type[[TYPE_hsearch_data]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_inserted:[0-9]+]] inserted: @type[[TYPE_entry]] [storage=automatic] = aggregate<@type[[TYPE_entry]], zero_fill=false>(field0 = array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_9]]), field1 = pointer_cast<ptr<void>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_10]])));
// DEFAULT-NEXT:         let %[[VALUE_query:[0-9]+]] query: @type[[TYPE_entry]] [storage=automatic] = aggregate<@type[[TYPE_entry]], zero_fill=false>(field0 = array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_11]]), field1 = null<ptr<void>>);
// DEFAULT-NEXT:         let %[[VALUE_found:[0-9]+]] found: ptr<@type[[TYPE_entry]]> [storage=automatic] = null<ptr<@type[[TYPE_entry]]>>;
// DEFAULT-NEXT:         let %[[VALUE_tree:[0-9]+]] tree: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         let %[[VALUE_values_2:[0-9]+]] values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(1), index2 = const<i32>(4), index3 = const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(u64, ptr<@type[[TYPE_hsearch_data]]>) -> i32>(%[[VALUE_hcreate_r]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), addr_of<ptr<@type[[TYPE_hsearch_data]]>>(%[[VALUE_table]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(@type[[TYPE_entry]], @type[[TYPE0]], ptr<ptr<@type[[TYPE_entry]]>>, ptr<@type[[TYPE_hsearch_data]]>) -> i32, abi=sysv64(native_c, scalar, scalar, scalar) -> scalar>(%[[VALUE_hsearch_r]], copy<@type[[TYPE_entry]], reason=arg>(read<@type[[TYPE_entry]]>(%[[VALUE_inserted]])), int_to_enum<@type[[TYPE0]], reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), addr_of<ptr<ptr<@type[[TYPE_entry]]>>>(%[[VALUE_found]]), addr_of<ptr<@type[[TYPE_hsearch_data]]>>(%[[VALUE_table]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(@type[[TYPE_entry]], @type[[TYPE0]], ptr<ptr<@type[[TYPE_entry]]>>, ptr<@type[[TYPE_hsearch_data]]>) -> i32, abi=sysv64(native_c, scalar, scalar, scalar) -> scalar>(%[[VALUE_hsearch_r]], copy<@type[[TYPE_entry]], reason=arg>(read<@type[[TYPE_entry]]>(%[[VALUE_query]])), int_to_enum<@type[[TYPE0]], reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<ptr<@type[[TYPE_entry]]>>>(%[[VALUE_found]]), addr_of<ptr<@type[[TYPE_hsearch_data]]>>(%[[VALUE_table]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<@type[[TYPE_entry]]>>(read<ptr<@type[[TYPE_entry]]>>(%[[VALUE_found]]), null<ptr<@type[[TYPE_entry]]>>), eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(field1(deref(read<ptr<@type[[TYPE_entry]]>>(%[[VALUE_found]]))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_12]]))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_hsearch_data]]>) -> void>(%[[VALUE_hdestroy_r]], addr_of<ptr<@type[[TYPE_hsearch_data]]>>(%[[VALUE_table]]));
// DEFAULT-NEXT:         for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_index_2:[0-9]+]] index: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%[[VALUE_index_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_index_2]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE14]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_index_2]], read<u64>(%[[VALUE15]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_value:[0-9]+]] value: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(4)));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE_value]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values_2]]), read<u64>(%[[VALUE_index_2]])))));
// DEFAULT-NEXT:                     call<ptr<void>, signature=fn(ptr<const void>, ptr<ptr<void>>, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void>>(%[[VALUE_tsearch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_value]])), addr_of<ptr<ptr<void>>>(%[[VALUE_tree]]), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_gnu_compare_entries]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), from_bool<i32, reason=promotion>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, ptr<const ptr<void>>, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void>>(%[[VALUE_tfind]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values_2]]), const<i32>(2))))), pointer_cast<ptr<const ptr<void>>, reason=arg>(addr_of<ptr<ptr<void>>>(%[[VALUE_tree]])), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_gnu_compare_entries]])), null<ptr<void>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<fn(ptr<void>) -> void>) -> void>(%[[VALUE_tdestroy]], read<ptr<void>>(%[[VALUE_tree]]), function_decay<ptr<fn(ptr<void>) -> void>>(%[[VALUE_gnu_free_entry]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_13]])), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_qsort_extension]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_getopt_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_argp_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_search_extensions]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
