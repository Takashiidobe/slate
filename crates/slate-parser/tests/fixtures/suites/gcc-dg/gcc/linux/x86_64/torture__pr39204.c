/* { dg-do compile } */
/* { dg-options "-fpermissive -w" } */

__extension__ typedef __SIZE_TYPE__ size_t;
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;
__extension__ typedef long long int __quad_t;
__extension__ typedef unsigned long long int __u_quad_t;
__extension__ typedef __u_quad_t __dev_t;
__extension__ typedef unsigned int __uid_t;
__extension__ typedef unsigned int __gid_t;
__extension__ typedef unsigned long int __ino_t;
__extension__ typedef __u_quad_t __ino64_t;
__extension__ typedef unsigned int __mode_t;
__extension__ typedef unsigned int __nlink_t;
__extension__ typedef long int __off_t;
__extension__ typedef __quad_t __off64_t;
__extension__ typedef int __pid_t;
__extension__ typedef struct {
    int __val[2];
} __fsid_t;
__extension__ typedef long int __clock_t;
__extension__ typedef unsigned int __id_t;
typedef struct _IO_FILE FILE;
typedef struct _IO_FILE __FILE;
typedef struct {
    int __count;
    union   {
	unsigned int __wch;
	char __wchb[4];
    }
    __value;
} __mbstate_t;
typedef struct {
    __off_t __pos;
    __mbstate_t __state;
} _G_fpos_t;
typedef struct {
    __off64_t __pos;
    __mbstate_t __state;
} _G_fpos64_t;
typedef unsigned int _G_uint16_t __attribute__ ((__mode__ (__HI__)));
typedef struct _IO_FILE _IO_FILE;
extern struct _IO_FILE_plus _IO_2_1_stderr_;
extern int fputs (__const char *__restrict __s, FILE * __restrict __stream);
extern char *strstr (__const char *__haystack, __const char *__needle)
     __attribute__ ((__nonnull__ (1, 2)));
     extern char *rindex (__const char *__s, int __c)   __attribute__ ((__nonnull__ (1)));
extern size_t strlen (__const char *__s) __attribute__ ((__nothrow__))
     __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));
     char *mystrlwr (char *string);
     char *m_replace_filename (const char *path, const char *filename);
     typedef struct LINE {
	 char *text;
	 struct LINE *next;
	 int flags;
     } LINE;
typedef struct TOC {
    char *text;
    char *alt;
    struct TOC *next;
    int root;
    int texinfoable;
    int htmlable;
    int otherfile;
} TOC;
static int _empty_count;
extern char *_word_substitution[256];
static void _output_sorted_nested_toc (TOC ** list, unsigned int num_items);
static char *_do_text_substitution (char *input);
static int _toc_scmp (const void *e1, const void *e2)
{
  TOC *t1 = *((TOC **) e1);
  TOC *t2 = *((TOC **) e2);
  return mystricmp (t1->text, t2->text);
}
static void _output_buffered_text (void) { if (_empty_count) ; }
void _post_process_filename (char *filename)
{
  int code_scanning = 0;
  char *new_filename, *p;
  char *line;
  FILE *f1 = 0, *f2 = 0;
  if (!new_filename || strlen (new_filename) < 2)
    new_filename[strlen (filename) - 1] = 'x';
  if (!f1 || !f2)
    while ((line = m_fgets (f1)))
      {
	line = _do_text_substitution (line);
	fputs (line, f2);
      }
  if (remove (filename))
    cancel:
	if (f1)
	  if (f2)
	    if (new_filename)
	      ;
}
static void _close_html_file (FILE * file)
{
  int f;
  for (f = 0; _word_substitution[f]; f++)
    ;
}
static __attribute__((always_inline))
char * _do_text_substitution (char *input)
{
  int start, end, middle, f;
  char *temp, *found, *reader;
  for (f = 0; _word_substitution[f]; f++)
    {
      reader = input;
      while ((found = strstr (reader, _word_substitution[f])))
	{
	  start = found - input;
	  input = temp;
	  reader = temp + start + middle;
	}
    }
  return input;
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE___u_char:[0-9]+]] __u_char = u8;
// DEFAULT-NEXT:     type @type[[TYPE___u_short:[0-9]+]] __u_short = u16;
// DEFAULT-NEXT:     type @type[[TYPE___u_int:[0-9]+]] __u_int = u32;
// DEFAULT-NEXT:     type @type[[TYPE___u_long:[0-9]+]] __u_long = u64;
// DEFAULT-NEXT:     type @type[[TYPE___int8_t:[0-9]+]] __int8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE___uint8_t:[0-9]+]] __uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE___int16_t:[0-9]+]] __int16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE___uint16_t:[0-9]+]] __uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___int32_t:[0-9]+]] __int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___int64_t:[0-9]+]] __int64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___quad_t:[0-9]+]] __quad_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___u_quad_t:[0-9]+]] __u_quad_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___dev_t:[0-9]+]] __dev_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___uid_t:[0-9]+]] __uid_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___gid_t:[0-9]+]] __gid_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___ino_t:[0-9]+]] __ino_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___ino64_t:[0-9]+]] __ino64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___mode_t:[0-9]+]] __mode_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___nlink_t:[0-9]+]] __nlink_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___off_t:[0-9]+]] __off_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___off64_t:[0-9]+]] __off64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___pid_t:[0-9]+]] __pid_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __val: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE___fsid_t:[0-9]+]] __fsid_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE___clock_t:[0-9]+]] __clock_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___id_t:[0-9]+]] __id_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE__IO_FILE:[0-9]+]] _IO_FILE = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = @type[[TYPE__IO_FILE]];
// DEFAULT-NEXT:     type @type[[TYPE___FILE:[0-9]+]] __FILE = @type[[TYPE__IO_FILE]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __count: i32;
// DEFAULT-NEXT:         field1 __value: @type[[TYPE2:[0-9]+]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE2]] = union {
// DEFAULT-NEXT:         field0 __wch: u32;
// DEFAULT-NEXT:         field1 __wchb: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE___mbstate_t:[0-9]+]] __mbstate_t = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __pos: i64;
// DEFAULT-NEXT:         field1 __state: @type[[TYPE1]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE__G_fpos_t:[0-9]+]] _G_fpos_t = @type[[TYPE3]];
// DEFAULT-NEXT:     type @type[[TYPE4:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __pos: i64;
// DEFAULT-NEXT:         field1 __state: @type[[TYPE1]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE__G_fpos64_t:[0-9]+]] _G_fpos64_t = @type[[TYPE4]];
// DEFAULT-NEXT:     type @type[[TYPE__G_uint16_t:[0-9]+]] _G_uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE__IO_FILE_2:[0-9]+]] _IO_FILE = @type[[TYPE__IO_FILE]];
// DEFAULT-NEXT:     type @type[[TYPE__IO_FILE_plus:[0-9]+]] _IO_FILE_plus = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_LINE:[0-9]+]] LINE = struct {
// DEFAULT-NEXT:         field0 text: ptr<i8>;
// DEFAULT-NEXT:         field1 next: ptr<@type[[TYPE_LINE]]>;
// DEFAULT-NEXT:         field2 flags: i32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_LINE_2:[0-9]+]] LINE = @type[[TYPE_LINE]];
// DEFAULT-NEXT:     type @type[[TYPE_TOC:[0-9]+]] TOC = struct {
// DEFAULT-NEXT:         field0 text: ptr<i8>;
// DEFAULT-NEXT:         field1 alt: ptr<i8>;
// DEFAULT-NEXT:         field2 next: ptr<@type[[TYPE_TOC]]>;
// DEFAULT-NEXT:         field3 root: i32;
// DEFAULT-NEXT:         field4 texinfoable: i32;
// DEFAULT-NEXT:         field5 htmlable: i32;
// DEFAULT-NEXT:         field6 otherfile: i32;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24, 28, 32, 36]];
// DEFAULT-NEXT:     type @type[[TYPE_TOC_2:[0-9]+]] TOC = @type[[TYPE_TOC]];
// DEFAULT-NEXT:     extern %[[VALUE__IO_2_1_stderr_:[0-9]+]] _IO_2_1_stderr_: @type[[TYPE__IO_FILE_plus]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE__empty_count:[0-9]+]] _empty_count: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE__word_substitution:[0-9]+]] _word_substitution: array<ptr<i8>, 256> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fputs:[0-9]+]] @fputs(%[[VALUE___s:[0-9]+]] __s: ptr<const i8> [restrict], %[[VALUE___stream:[0-9]+]] __stream: ptr<@type[[TYPE__IO_FILE]]> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strstr:[0-9]+]] @strstr(%[[VALUE___haystack:[0-9]+]] __haystack: ptr<const i8>, %[[VALUE___needle:[0-9]+]] __needle: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_rindex:[0-9]+]] @rindex(%[[VALUE___s_2:[0-9]+]] __s: ptr<const i8>, %[[VALUE___c:[0-9]+]] __c: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE___s_3:[0-9]+]] __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_mystrlwr:[0-9]+]] @mystrlwr(%[[VALUE_string:[0-9]+]] string: ptr<i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_m_replace_filename:[0-9]+]] @m_replace_filename(%[[VALUE_path:[0-9]+]] path: ptr<const i8>, %[[VALUE_filename:[0-9]+]] filename: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__output_sorted_nested_toc:[0-9]+]] @_output_sorted_nested_toc(%[[VALUE_list:[0-9]+]] list: ptr<ptr<@type[[TYPE_TOC]]>>, %[[VALUE_num_items:[0-9]+]] num_items: u32) -> void [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE__do_text_substitution:[0-9]+]] @_do_text_substitution(%[[VALUE_input:[0-9]+]] input: ptr<i8>) -> ptr<i8> [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_start:[0-9]+]] start: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_end:[0-9]+]] end: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_middle:[0-9]+]] middle: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_temp:[0-9]+]] temp: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_found:[0-9]+]] found: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_reader:[0-9]+]] reader: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_f]], const<i32>(0));
// DEFAULT-NEXT:             condition: ne<ptr<i8>>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(256)>(%[[VALUE__word_substitution]]), read<i32>(%[[VALUE_f]])))), null<ptr<i8>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_f]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_f]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_reader]], read<ptr<i8>>(%[[VALUE_input]]));
// DEFAULT-NEXT:                     while %[[VALUE3:[0-9]+]] {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strstr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_reader]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(256)>(%[[VALUE__word_substitution]]), read<i32>(%[[VALUE_f]]))))));
// DEFAULT-NEXT:                         write<ptr<i8>>(%[[VALUE_found]], read<ptr<i8>>(%[[VALUE4]]));
// DEFAULT-NEXT:                         yield ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE4]]), null<ptr<i8>>);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_start]], truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%[[VALUE_found]]), read<ptr<i8>>(%[[VALUE_input]]))));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_input]], read<ptr<i8>>(%[[VALUE_temp]]));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_reader]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_temp]]), read<i32>(%[[VALUE_start]])), read<i32>(%[[VALUE_middle]])));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_input]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mystricmp:[0-9]+]] @mystricmp(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__toc_scmp:[0-9]+]] @_toc_scmp(%[[VALUE_e1:[0-9]+]] e1: ptr<const void>, %[[VALUE_e2:[0-9]+]] e2: ptr<const void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t1:[0-9]+]] t1: ptr<@type[[TYPE_TOC]]> [storage=automatic] = read<ptr<@type[[TYPE_TOC]]>>(deref(pointer_cast<ptr<ptr<@type[[TYPE_TOC]]>>, reason=explicit>(read<ptr<const void>>(%[[VALUE_e1]]))));
// DEFAULT-NEXT:         let %[[VALUE_t2:[0-9]+]] t2: ptr<@type[[TYPE_TOC]]> [storage=automatic] = read<ptr<@type[[TYPE_TOC]]>>(deref(pointer_cast<ptr<ptr<@type[[TYPE_TOC]]>>, reason=explicit>(read<ptr<const void>>(%[[VALUE_e2]]))));
// DEFAULT-NEXT:         return call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_mystricmp]], read<ptr<i8>>(field0(deref(read<ptr<@type[[TYPE_TOC]]>>(%[[VALUE_t1]])))), read<ptr<i8>>(field0(deref(read<ptr<@type[[TYPE_TOC]]>>(%[[VALUE_t2]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE__output_buffered_text:[0-9]+]] @_output_buffered_text() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE__empty_count]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_m_fgets:[0-9]+]] @m_fgets(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remove:[0-9]+]] @remove(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__post_process_filename:[0-9]+]] @_post_process_filename(%[[VALUE_filename_2:[0-9]+]] filename: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_code_scanning:[0-9]+]] code_scanning: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_new_filename:[0-9]+]] new_filename: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_line:[0-9]+]] line: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f1:[0-9]+]] f1: ptr<@type[[TYPE__IO_FILE]]> [storage=automatic] = null<ptr<@type[[TYPE__IO_FILE]]>>;
// DEFAULT-NEXT:         let %[[VALUE_f2:[0-9]+]] f2: ptr<@type[[TYPE__IO_FILE]]> [storage=automatic] = null<ptr<@type[[TYPE__IO_FILE]]>>;
// DEFAULT-NEXT:         if logical_or<bool>(not<bool>(ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_new_filename]]), null<ptr<i8>>)), lt<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_new_filename]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_new_filename]]), sub<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_filename_2]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), truncate<i8, reason=assign, fits=always>(const<i32>(120)));
// DEFAULT-NEXT:         if logical_or<bool>(not<bool>(ne<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_f1]]), null<ptr<@type[[TYPE__IO_FILE]]>>)), not<bool>(ne<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_f2]]), null<ptr<@type[[TYPE__IO_FILE]]>>)))
// DEFAULT-NEXT:             while %[[VALUE5:[0-9]+]] {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = int_to_ptr<ptr<i8>, reason=assign>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_m_fgets]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_f1]])));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_line]], read<ptr<i8>>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE6]]), null<ptr<i8>>);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_line]], call<ptr<i8>, signature=fn(ptr<i8>) -> ptr<i8>>(%[[VALUE__do_text_substitution]], read<ptr<i8>>(%[[VALUE_line]])));
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ptr<@type[[TYPE__IO_FILE]]>) -> i32>(%[[VALUE_fputs]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_line]])), read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_f2]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_remove]], read<ptr<i8>>(%[[VALUE_filename_2]])), const<i32>(0))
// DEFAULT-NEXT:             label %[[VALUE_cancel:[0-9]+]] cancel:
// DEFAULT-NEXT:                 if ne<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_f1]]), null<ptr<@type[[TYPE__IO_FILE]]>>)
// DEFAULT-NEXT:                     if ne<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_f2]]), null<ptr<@type[[TYPE__IO_FILE]]>>)
// DEFAULT-NEXT:                         if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_new_filename]]), null<ptr<i8>>)
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE__close_html_file:[0-9]+]] @_close_html_file(%[[VALUE_file:[0-9]+]] file: ptr<@type[[TYPE__IO_FILE]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_f_2:[0-9]+]] f: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_f_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: ne<ptr<i8>>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(256)>(%[[VALUE__word_substitution]]), read<i32>(%[[VALUE_f_2]])))), null<ptr<i8>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_f_2]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_f_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
