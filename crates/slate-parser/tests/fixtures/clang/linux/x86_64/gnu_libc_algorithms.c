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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 __uint64_t = u64;
// DEFAULT-NEXT:     type @type2 __off_t = i64;
// DEFAULT-NEXT:     type @type3 __off64_t = i64;
// DEFAULT-NEXT:     type @type4 _IO_FILE = struct {
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
// DEFAULT-NEXT:         field12 _markers: ptr<@type6>;
// DEFAULT-NEXT:         field13 _chain: ptr<@type4>;
// DEFAULT-NEXT:         field14 _fileno: i32;
// DEFAULT-NEXT:         field15 _flags2: i32 : 24;
// DEFAULT-NEXT:         field16 _short_backupbuf: array<i8, 1>;
// DEFAULT-NEXT:         field17 _old_offset: i64;
// DEFAULT-NEXT:         field18 _cur_column: u16;
// DEFAULT-NEXT:         field19 _vtable_offset: i8;
// DEFAULT-NEXT:         field20 _shortbuf: array<i8, 1>;
// DEFAULT-NEXT:         field21 _lock: ptr<void>;
// DEFAULT-NEXT:         field22 _offset: i64;
// DEFAULT-NEXT:         field23 _codecvt: ptr<@type7>;
// DEFAULT-NEXT:         field24 _wide_data: ptr<@type8>;
// DEFAULT-NEXT:         field25 _freeres_list: ptr<@type4>;
// DEFAULT-NEXT:         field26 _freeres_buf: ptr<void>;
// DEFAULT-NEXT:         field27 _prevchain: ptr<ptr<@type4>>;
// DEFAULT-NEXT:         field28 _mode: i32;
// DEFAULT-NEXT:         field29 _unused3: i32;
// DEFAULT-NEXT:         field30 _total_written: u64;
// DEFAULT-NEXT:         field31 _unused2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=216, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 116, 119, 120, 128, 130, 131, 136, 144, 152, 160, 168, 176, 184, 192, 196, 200, 208], bit_offsets=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(928), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], bit_units=[(116, 3)], field_units=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(0), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]];
// DEFAULT-NEXT:     type @type5 FILE = @type4;
// DEFAULT-NEXT:     type @type6 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type7 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type8 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     type @type9 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type10 option = struct {
// DEFAULT-NEXT:         field0 name: ptr<const i8>;
// DEFAULT-NEXT:         field1 has_arg: i32;
// DEFAULT-NEXT:         field2 flag: ptr<i32>;
// DEFAULT-NEXT:         field3 val: i32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type11 error_t = i32;
// DEFAULT-NEXT:     type @type12 argp_option = struct {
// DEFAULT-NEXT:         field0 name: ptr<const i8>;
// DEFAULT-NEXT:         field1 key: i32;
// DEFAULT-NEXT:         field2 arg: ptr<const i8>;
// DEFAULT-NEXT:         field3 flags: i32;
// DEFAULT-NEXT:         field4 doc: ptr<const i8>;
// DEFAULT-NEXT:         field5 group: i32;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 8, 16, 24, 32, 40]];
// DEFAULT-NEXT:     type @type13 argp = struct {
// DEFAULT-NEXT:         field0 options: ptr<const @type12>;
// DEFAULT-NEXT:         field1 parser: ptr<fn(i32, ptr<i8>, ptr<@type14>) -> i32>;
// DEFAULT-NEXT:         field2 args_doc: ptr<const i8>;
// DEFAULT-NEXT:         field3 doc: ptr<const i8>;
// DEFAULT-NEXT:         field4 children: ptr<const @type15>;
// DEFAULT-NEXT:         field5 help_filter: ptr<fn(i32, ptr<const i8>, ptr<void>) -> ptr<i8>>;
// DEFAULT-NEXT:         field6 argp_domain: ptr<const i8>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 8, 16, 24, 32, 40, 48]];
// DEFAULT-NEXT:     type @type14 argp_state = struct {
// DEFAULT-NEXT:         field0 root_argp: ptr<const @type13>;
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
// DEFAULT-NEXT:         field11 err_stream: ptr<@type4>;
// DEFAULT-NEXT:         field12 out_stream: ptr<@type4>;
// DEFAULT-NEXT:         field13 pstate: ptr<void>;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0, 8, 16, 24, 28, 32, 36, 40, 48, 56, 64, 72, 80, 88]];
// DEFAULT-NEXT:     type @type15 argp_child = struct {
// DEFAULT-NEXT:         field0 argp: ptr<const @type13>;
// DEFAULT-NEXT:         field1 flags: i32;
// DEFAULT-NEXT:         field2 header: ptr<const i8>;
// DEFAULT-NEXT:         field3 group: i32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type16 argp_parser_t = ptr<fn(i32, ptr<i8>, ptr<@type14>) -> i32>;
// DEFAULT-NEXT:     type @type17 __compar_fn_t = ptr<fn(ptr<const void>, ptr<const void>) -> i32>;
// DEFAULT-NEXT:     type @type18 = enum : u32 {
// DEFAULT-NEXT:         %0 FIND = const<i32>(0);
// DEFAULT-NEXT:         %1 ENTER = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type19 ACTION = @type18;
// DEFAULT-NEXT:     type @type20 entry = struct {
// DEFAULT-NEXT:         field0 key: ptr<i8>;
// DEFAULT-NEXT:         field1 data: ptr<void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type21 ENTRY = @type20;
// DEFAULT-NEXT:     type @type22 _ENTRY = struct incomplete;
// DEFAULT-NEXT:     type @type23 hsearch_data = struct {
// DEFAULT-NEXT:         field0 table: ptr<@type22>;
// DEFAULT-NEXT:         field1 size: u32;
// DEFAULT-NEXT:         field2 filled: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type24 __free_fn_t = ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:     type @type25 __compar_d_fn_t = ptr<fn(ptr<const void>, ptr<const void>, ptr<void>) -> i32>;
// DEFAULT-NEXT:     type @type26 GNUArguments = struct {
// DEFAULT-NEXT:         field0 number: i32;
// DEFAULT-NEXT:         field1 positional: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     extern %12 optarg: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %13 optind: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %14 opterr: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %183 .str183: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([105, 116, 101, 109, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %184 .str184: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([110, 117, 109, 98, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %185 .str185: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 108, 97, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %187 .str187: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 110, 58, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %188 .str188: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([110, 117, 109, 98, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %189 .str189: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %190 .str190: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([110, 117, 109, 98, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %191 .str191: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([73, 84, 69, 77, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %192 .str192: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %193 .str193: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([50, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %194 .str194: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %195 .str195: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([50, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %197 .str197: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %11 @printf(%144 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @getopt_long(%145 ___argc: i32, %146 ___argv: ptr<const ptr<i8>>, %147 __shortopts: ptr<const i8>, %148 __longopts: ptr<const @type10>, %149 __longind: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %40 @argp_parse(%150 __argp: ptr<const @type13> [restrict], %151 __argc: i32, %152 __argv: ptr<ptr<i8>> [restrict], %153 __flags: u32, %154 __arg_index: ptr<i32> [restrict], %155 __input: ptr<void> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %54 @hsearch_r(%156 __item: @type20, %157 __action: @type18, %158 __retval: ptr<ptr<@type20>>, %159 __htab: ptr<@type23>) -> i32 [linkage=external] [abi=sysv64(native_c, scalar, scalar, scalar) -> scalar];
// DEFAULT-NEXT:     fn %57 @hcreate_r(%160 __nel: u64, %161 __htab: ptr<@type23>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %59 @hdestroy_r(%162 __htab: ptr<@type23>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %63 @tsearch(%163 __key: ptr<const void>, %164 __rootp: ptr<ptr<void>>, %165 __compar: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %67 @tfind(%166 __key: ptr<const void>, %167 __rootp: ptr<const ptr<void>>, %168 __compar: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %72 @tdestroy(%169 __root: ptr<void>, %170 __freefct: ptr<fn(ptr<void>) -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %74 @atoi(%171 __nptr: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %76 @malloc(%172 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %78 @free(%173 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %85 @qsort_r(%174 __base: ptr<void>, %175 __nmemb: u64, %176 __size: u64, %177 __compar: ptr<fn(ptr<const void>, ptr<const void>, ptr<void>) -> i32>, %178 __arg: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %88 @strcmp(%179 __s1: ptr<const i8>, %180 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %91 @index(%181 __s: ptr<const i8>, %182 __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %93 @gnu_compare_with_direction(%94 left: ptr<const void>, %95 right: ptr<const void>, %96 state: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %97 direction: i32 [storage=automatic] = read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%96))));
// DEFAULT-NEXT:         let %98 a: i32 [storage=automatic] = read<i32>(deref(pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%94))));
// DEFAULT-NEXT:         let %99 b: i32 [storage=automatic] = read<i32>(deref(pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%95))));
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%97), sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%98), read<i32>(%99))), from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%98), read<i32>(%99)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @gnu_compare_entries(%101 left: ptr<const void>, %102 right: ptr<const void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %103 a: i32 [storage=automatic] [const] = read<i32>(deref(pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%101))));
// DEFAULT-NEXT:         let %104 b: i32 [storage=automatic] [const] = read<i32>(deref(pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%102))));
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%103), read<i32>(%104))), from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%103), read<i32>(%104))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %105 @gnu_free_entry(%106 entry: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%78, read<ptr<void>>(%106));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %107 @gnu_parse_option(%108 key: i32, %109 argument: ptr<i8>, %110 state: ptr<@type14>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %111 arguments: ptr<@type26> [storage=automatic] = pointer_cast<ptr<@type26>, reason=assign>(read<ptr<void>>(field7(deref(read<ptr<@type14>>(%110)))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%108), const<i32>(110))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(field0(deref(read<ptr<@type26>>(%111))), call<i32, signature=fn(ptr<const i8>) -> i32>(%74, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%109))));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%108), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %198: ptr<@type26> [synthetic] = read<ptr<@type26>>(%111);
// DEFAULT-NEXT:                 let %199: i32 [synthetic] = read<i32>(field1(deref(read<ptr<@type26>>(%198))));
// DEFAULT-NEXT:                 let %200: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%199), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%88, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%109)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%183))), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(field1(deref(read<ptr<@type26>>(%198))), read<i32>(%200));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(eq<i32>(read<i32>(%108), const<i32>(16777217)), eq<i32>(read<i32>(%108), const<i32>(16777219))), eq<i32>(read<i32>(%108), const<i32>(16777218))), eq<i32>(read<i32>(%108), const<i32>(16777220))), eq<i32>(read<i32>(%108), const<i32>(16777223)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %112 @gnu_qsort_extension() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %113 values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(1), index2 = const<i32>(3), index3 = const<i32>(2));
// DEFAULT-NEXT:         let %114 direction: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>, ptr<void>) -> i32>, ptr<void>) -> void>(%85, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%113)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), const<u64>(4), function_decay<ptr<fn(ptr<const void>, ptr<const void>, ptr<void>) -> i32>>(%93), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%114)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%113), const<i32>(0)))), const<i32>(1000)), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%113), const<i32>(1)))), const<i32>(100))), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%113), const<i32>(2)))), const<i32>(10))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%113), const<i32>(3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %115 @gnu_getopt_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %116 program: array<i8, 6> [storage=automatic] = code_units<array<i8, 6>>([112, 114, 111, 98, 101, 0]);
// DEFAULT-NEXT:         let %117 number: array<i8, 11> [storage=automatic] = code_units<array<i8, 11>>([45, 45, 110, 117, 109, 98, 101, 114, 61, 55, 0]);
// DEFAULT-NEXT:         let %118 flag: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([45, 102, 0]);
// DEFAULT-NEXT:         let %119 arguments: array<ptr<i8>, 4> [storage=automatic] [align=16] = aggregate<array<ptr<i8>, 4>, zero_fill=false>(index0 = array_decay<ptr<i8>, length=Some(6)>(%116), index1 = array_decay<ptr<i8>, length=Some(11)>(%117), index2 = array_decay<ptr<i8>, length=Some(3)>(%118), index3 = null<ptr<i8>>);
// DEFAULT-NEXT:         let %120 options: array<@type10, 3> [storage=automatic] [align=16] = aggregate<array<@type10, 3>, zero_fill=false>(index0 = aggregate<@type10, zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%184)), field1 = const<i32>(1), field2 = null<ptr<i32>>, field3 = const<i32>(110)), index1 = aggregate<@type10, zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%185)), field1 = const<i32>(0), field2 = null<ptr<i32>>, field3 = const<i32>(102)), index2 = aggregate<@type10, zero_fill=false>(field0 = null<ptr<const i8>>, field1 = const<i32>(0), field2 = null<ptr<i32>>, field3 = const<i32>(0)));
// DEFAULT-NEXT:         let %121 number_value: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %122 flag_value: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %123 option: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%13, const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:         while %186 {
// DEFAULT-NEXT:             write<i32>(%123, call<i32, signature=fn(i32, ptr<const ptr<i8>>, ptr<const i8>, ptr<const @type10>, ptr<i32>) -> i32>(%21, const<i32>(3), pointer_cast<ptr<const ptr<i8>>, reason=arg>(array_decay<ptr<ptr<i8>>, length=Some(4)>(%119)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%187)), pointer_cast<ptr<const @type10>, reason=arg>(array_decay<ptr<@type10>, length=Some(3)>(%120)), null<ptr<i32>>));
// DEFAULT-NEXT:             yield ne<i32>(call<i32, signature=fn(i32, ptr<const ptr<i8>>, ptr<const i8>, ptr<const @type10>, ptr<i32>) -> i32>(%21, const<i32>(3), pointer_cast<ptr<const ptr<i8>>, reason=arg>(array_decay<ptr<ptr<i8>>, length=Some(4)>(%119)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%187)), pointer_cast<ptr<const @type10>, reason=arg>(array_decay<ptr<@type10>, length=Some(3)>(%120)), null<ptr<i32>>), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%123), const<i32>(110))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%121, call<i32, signature=fn(ptr<const i8>) -> i32>(%74, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%12))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%123), const<i32>(102))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%122, const<i32>(1));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%121), const<i32>(10)), read<i32>(%122));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %124 @gnu_argp_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %125 options: array<@type12, 2> [storage=automatic] [align=16] = aggregate<array<@type12, 2>, zero_fill=false>(index0 = aggregate<@type12, zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%188)), field1 = const<i32>(110), field2 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%189)), field3 = const<i32>(0), field4 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%190)), field5 = const<i32>(0)), index1 = aggregate<@type12, zero_fill=false>(field0 = null<ptr<const i8>>, field1 = const<i32>(0), field2 = null<ptr<const i8>>, field3 = const<i32>(0), field4 = null<ptr<const i8>>, field5 = const<i32>(0)));
// DEFAULT-NEXT:         let %126 parser: @type13 [storage=automatic] = aggregate<@type13, zero_fill=false>(field0 = pointer_cast<ptr<const @type12>, reason=assign>(array_decay<ptr<@type12>, length=Some(2)>(%125)), field1 = function_decay<ptr<fn(i32, ptr<i8>, ptr<@type14>) -> i32>>(%107), field2 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%191)), field3 = null<ptr<const i8>>, field4 = null<ptr<const @type15>>, field5 = null<ptr<fn(i32, ptr<const i8>, ptr<void>) -> ptr<i8>>>, field6 = null<ptr<const i8>>);
// DEFAULT-NEXT:         let %127 parsed: @type26 [storage=automatic] = aggregate<@type26, zero_fill=true>();
// DEFAULT-NEXT:         let %128 program: array<i8, 6> [storage=automatic] = code_units<array<i8, 6>>([112, 114, 111, 98, 101, 0]);
// DEFAULT-NEXT:         let %129 option: array<i8, 11> [storage=automatic] = code_units<array<i8, 11>>([45, 45, 110, 117, 109, 98, 101, 114, 61, 53, 0]);
// DEFAULT-NEXT:         let %130 item: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([105, 116, 101, 109, 0]);
// DEFAULT-NEXT:         let %131 arguments: array<ptr<i8>, 4> [storage=automatic] [align=16] = aggregate<array<ptr<i8>, 4>, zero_fill=false>(index0 = array_decay<ptr<i8>, length=Some(6)>(%128), index1 = array_decay<ptr<i8>, length=Some(11)>(%129), index2 = array_decay<ptr<i8>, length=Some(5)>(%130), index3 = null<ptr<i8>>);
// DEFAULT-NEXT:         let %132 result: i32 [storage=automatic] = call<i32, signature=fn(ptr<const @type13>, i32, ptr<ptr<i8>>, u32, ptr<i32>, ptr<void>) -> i32>(%40, pointer_cast<ptr<const @type13>, reason=arg>(addr_of<ptr<@type13>>(%126)), const<i32>(3), array_decay<ptr<ptr<i8>>, length=Some(4)>(%131), reinterpret<u32, reason=arg, fits=unknown>(or<i32>(const<i32>(32), const<i32>(16))), null<ptr<i32>>, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type26>>(%127)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%132), const<i32>(0))), mul<i32, overflow=ub>(read<i32>(field0(%127)), const<i32>(10))), read<i32>(field1(%127)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %133 @gnu_search_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %134 table: @type23 [storage=automatic] = aggregate<@type23, zero_fill=true>();
// DEFAULT-NEXT:         let %135 inserted: @type20 [storage=automatic] = aggregate<@type20, zero_fill=false>(field0 = array_decay<ptr<i8>, length=Some(6)>(%192), field1 = pointer_cast<ptr<void>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%193)));
// DEFAULT-NEXT:         let %136 query: @type20 [storage=automatic] = aggregate<@type20, zero_fill=false>(field0 = array_decay<ptr<i8>, length=Some(6)>(%194), field1 = null<ptr<void>>);
// DEFAULT-NEXT:         let %137 found: ptr<@type20> [storage=automatic] = null<ptr<@type20>>;
// DEFAULT-NEXT:         let %138 tree: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         let %139 values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(1), index2 = const<i32>(4), index3 = const<i32>(2));
// DEFAULT-NEXT:         let %140 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %201: i32 [synthetic] = read<i32>(%140);
// DEFAULT-NEXT:         let %202: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%201), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(u64, ptr<@type23>) -> i32>(%57, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), addr_of<ptr<@type23>>(%134)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%140, read<i32>(%202));
// DEFAULT-NEXT:         let %203: i32 [synthetic] = read<i32>(%140);
// DEFAULT-NEXT:         let %204: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%203), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(@type20, @type18, ptr<ptr<@type20>>, ptr<@type23>) -> i32, abi=sysv64(native_c, scalar, scalar, scalar) -> scalar>(%54, copy<@type20, reason=arg>(read<@type20>(%135)), int_to_enum<@type18, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), addr_of<ptr<ptr<@type20>>>(%137), addr_of<ptr<@type23>>(%134)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%140, read<i32>(%204));
// DEFAULT-NEXT:         let %205: i32 [synthetic] = read<i32>(%140);
// DEFAULT-NEXT:         let %206: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%205), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(@type20, @type18, ptr<ptr<@type20>>, ptr<@type23>) -> i32, abi=sysv64(native_c, scalar, scalar, scalar) -> scalar>(%54, copy<@type20, reason=arg>(read<@type20>(%136)), int_to_enum<@type18, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<ptr<@type20>>>(%137), addr_of<ptr<@type23>>(%134)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%140, read<i32>(%206));
// DEFAULT-NEXT:         let %207: i32 [synthetic] = read<i32>(%140);
// DEFAULT-NEXT:         let %208: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%207), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<@type20>>(read<ptr<@type20>>(%137), null<ptr<@type20>>), eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%88, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(field1(deref(read<ptr<@type20>>(%137))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%195))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%140, read<i32>(%208));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type23>) -> void>(%59, addr_of<ptr<@type23>>(%134));
// DEFAULT-NEXT:         for %196
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %141 index: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%141), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %209: u64 [synthetic] = read<u64>(%141);
// DEFAULT-NEXT:                 let %210: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%209), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%141, read<u64>(%210));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %142 value: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%76, const<u64>(4)));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%142)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%139), read<u64>(%141)))));
// DEFAULT-NEXT:                     call<ptr<void>, signature=fn(ptr<const void>, ptr<ptr<void>>, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void>>(%63, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%142)), addr_of<ptr<ptr<void>>>(%138), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%100));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %211: i32 [synthetic] = read<i32>(%140);
// DEFAULT-NEXT:         let %212: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%211), from_bool<i32, reason=promotion>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, ptr<const ptr<void>>, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void>>(%67, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%139), const<i32>(2))))), pointer_cast<ptr<const ptr<void>>, reason=arg>(addr_of<ptr<ptr<void>>>(%138)), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%100)), null<ptr<void>>)));
// DEFAULT-NEXT:         write<i32>(%140, read<i32>(%212));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<fn(ptr<void>) -> void>) -> void>(%72, read<ptr<void>>(%138), function_decay<ptr<fn(ptr<void>) -> void>>(%105));
// DEFAULT-NEXT:         return read<i32>(%140);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %143 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%197)), call<i32, signature=fn() -> i32>(%112), call<i32, signature=fn() -> i32>(%115), call<i32, signature=fn() -> i32>(%124), call<i32, signature=fn() -> i32>(%133));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
