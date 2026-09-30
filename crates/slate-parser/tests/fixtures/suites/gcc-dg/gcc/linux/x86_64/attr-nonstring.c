/* PR middle-end/85359 - duplicate -Wstringop-overflow for a strcmp call
   with a nonstring pointer
   { dg-do compile }
   { dg-options "-O2 -Wall" } */

typedef __SIZE_TYPE__       size_t;
typedef __builtin_va_list   va_list;

int printf (const char*, ...);
int puts (const char*);
int puts_unlocked (const char*);
int sprintf (char*, const char*, ...);
int snprintf (char*, size_t, const char*, ...);
int vsprintf (char*, const char*, va_list);
int vsnprintf (char*, size_t, const char*, va_list);

int strcmp (const char*, const char*);
int strncmp (const char*, const char*, size_t);

char* stpcpy (char*, const char*);
char* stpncpy (char*, const char*, size_t);

char* strcat (char*, const char*);
char* strncat (char*, const char*, size_t);

char* strcpy (char*, const char*);
char* strncpy (char*, const char*, size_t);

char* strchr (const char*, int);
char* strrchr (const char*, int);
char* strstr (const char*, const char*);
char* strdup (const char*);
size_t strlen (const char*);
size_t strnlen (const char*, size_t);
char* strndup (const char*, size_t);

#define NONSTRING __attribute__ ((nonstring))

extern char ns5[5] NONSTRING;

int strcmp_nonstring_1 (NONSTRING const char *a, const char *b)
{
  /* dg-warning matches one or more instances of the warning so it's
     no good on its own.  Use dg-regexp instead to verify that just
     one instance of the warning is issued.  See gcc.dg/pr64223-1
     for a different approach.  */
  return strcmp (a, b);  /* { dg-regexp "\[^\n\r\]+: warning: .strcmp. argument 1 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "strcmp" } */
}

int strcmp_nonstring_2 (const char *a, NONSTRING const char *b)
{
  return strcmp (a, b);  /* { dg-regexp "\[^\n\r\]+: warning: .strcmp. argument 2 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "strcmp" } */
}

int strncmp_nonstring_1 (const char *s)
{
  return strncmp (s, ns5, sizeof ns5 + 1);  /* { dg-regexp "\[^\n\r\]+: warning: .strncmp. argument 2 declared attribute .nonstring. \[^\n\r\]+ \\\[-Wstringop-overread\[^\n\r\]*" "strncmp" } */
}

int strncmp_nonstring_2 (const char *s)
{
  return strncmp (ns5, s, sizeof ns5 + 1);  /* { dg-regexp "\[^\n\r\]+: warning: .strncmp. argument 1 declared attribute .nonstring. \[^\n\r\]+ \\\[-Wstringop-overread\[^\n\r\]*" "strncmp" } */
}

char* stpcpy_nonstring (char *d, NONSTRING const char *s)
{
  return stpcpy (d, s);  /* { dg-regexp "\[^\n\r\]+: warning: .stpcpy. argument 2 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "stpcpy" } */
}

char* stpncpy_nonstring (char *d)
{
  return stpncpy (d, ns5, sizeof ns5 + 1);  /* { dg-regexp "\[^\n\r\]+: warning: .stpncpy. argument 2 declared attribute .nonstring. \[^\n\r\]+ \\\[-Wstringop-overread\[^\n\r\]*" "stpncpy" } */
}

char* strchr_nonstring (NONSTRING const char *s, int c)
{
  return strchr (s, c);  /* { dg-regexp "\[^\n\r\]+: warning: .strchr. argument 1 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "strchr" } */
}

char* strrchr_nonstring (NONSTRING const char *s, int c)
{
  return strrchr (s, c);  /* { dg-regexp "\[^\n\r\]+: warning: .strrchr. argument 1 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "strrchr" } */
}

char* strcpy_nonstring (char *d, NONSTRING const char *s)
{
  return strcpy (d, s);  /* { dg-regexp "\[^\n\r\]+: warning: .strcpy. argument 2 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "strcpy" } */
}

char* strncpy_nonstring (char *d)
{
  return strncpy (d, ns5, sizeof ns5 + 1);  /* { dg-regexp "\[^\n\r\]+: warning: .strncpy. argument 2 declared attribute .nonstring. \[^\n\r\]+ \\\[-Wstringop-overread\[^\n\r\]*" "strncpy" } */
}

char* strstr_nonstring_1 (NONSTRING const char *a, const char *b)
{
  return strstr (a, b);  /* { dg-regexp "\[^\n\r\]+: warning: .strstr. argument 1 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "strstr" } */
}

char* strstr_nonstring_2 (const char *a, NONSTRING const char *b)
{
  return strstr (a, b);  /* { dg-regexp "\[^\n\r\]+: warning: .strstr. argument 2 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "strstr" } */
}

char* stdup_nonstring (NONSTRING const char *s)
{
  return strdup (s);  /* { dg-regexp "\[^\n\r\]+: warning: .strdup. argument 1 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "strdup" } */
}

size_t strlen_nonstring (NONSTRING const char *s)
{
  return strlen (s);  /* { dg-regexp "\[^\n\r\]+: warning: .strlen. argument 1 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "strlen" } */
}

int printf_nonstring (NONSTRING const char *s)
{
  return printf (s);  /* { dg-regexp "\[^\n\r\]+: warning: .printf. argument 1 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "printf" } */
}

int sprintf_nonstring_2 (char *d, NONSTRING const char *s)
{
  return sprintf (d, s);  /* { dg-regexp "\[^\n\r\]+: warning: .sprintf. argument 2 declared attribute .nonstring. \\\[-Wstringop-overread\[^\n\r\]*" "sprintf" } */
}

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
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     extern %[[VALUE_ns5:[0-9]+]] ns5: array<i8, 5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_puts:[0-9]+]] @puts(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_puts_unlocked:[0-9]+]] @puts_unlocked(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sprintf:[0-9]+]] @sprintf(%[[VALUE3:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_snprintf:[0-9]+]] @snprintf(%[[VALUE5:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE6:[0-9]+]] <unnamed>: u64, %[[VALUE7:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_vsprintf:[0-9]+]] @vsprintf(%[[VALUE8:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE9:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE10:[0-9]+]] <unnamed>: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_vsnprintf:[0-9]+]] @vsnprintf(%[[VALUE11:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE12:[0-9]+]] <unnamed>: u64, %[[VALUE13:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE14:[0-9]+]] <unnamed>: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE15:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE16:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncmp:[0-9]+]] @strncmp(%[[VALUE17:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE18:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE19:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_stpcpy:[0-9]+]] @stpcpy(%[[VALUE20:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE21:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_stpncpy:[0-9]+]] @stpncpy(%[[VALUE22:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE23:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE24:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcat:[0-9]+]] @strcat(%[[VALUE25:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE26:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncat:[0-9]+]] @strncat(%[[VALUE27:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE28:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE29:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcpy:[0-9]+]] @strcpy(%[[VALUE30:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE31:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncpy:[0-9]+]] @strncpy(%[[VALUE32:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE33:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE34:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strchr:[0-9]+]] @strchr(%[[VALUE35:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE36:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strrchr:[0-9]+]] @strrchr(%[[VALUE37:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE38:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strstr:[0-9]+]] @strstr(%[[VALUE39:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE40:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strdup:[0-9]+]] @strdup(%[[VALUE41:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE42:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strnlen:[0-9]+]] @strnlen(%[[VALUE43:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE44:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strndup:[0-9]+]] @strndup(%[[VALUE45:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE46:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp_nonstring_1:[0-9]+]] @strcmp_nonstring_1(%[[VALUE_a:[0-9]+]] a: ptr<const i8>, %[[VALUE_b:[0-9]+]] b: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_a]]), read<ptr<const i8>>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strcmp_nonstring_2:[0-9]+]] @strcmp_nonstring_2(%[[VALUE_a_2:[0-9]+]] a: ptr<const i8>, %[[VALUE_b_2:[0-9]+]] b: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_a_2]]), read<ptr<const i8>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strncmp_nonstring_1:[0-9]+]] @strncmp_nonstring_1(%[[VALUE_s:[0-9]+]] s: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE_strncmp]], read<ptr<const i8>>(%[[VALUE_s]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_ns5]])), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strncmp_nonstring_2:[0-9]+]] @strncmp_nonstring_2(%[[VALUE_s_2:[0-9]+]] s: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE_strncmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_ns5]])), read<ptr<const i8>>(%[[VALUE_s_2]]), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_stpcpy_nonstring:[0-9]+]] @stpcpy_nonstring(%[[VALUE_d:[0-9]+]] d: ptr<i8>, %[[VALUE_s_3:[0-9]+]] s: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_stpcpy]], read<ptr<i8>>(%[[VALUE_d]]), read<ptr<const i8>>(%[[VALUE_s_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_stpncpy_nonstring:[0-9]+]] @stpncpy_nonstring(%[[VALUE_d_2:[0-9]+]] d: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_ns5]])), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strchr_nonstring:[0-9]+]] @strchr_nonstring(%[[VALUE_s_4:[0-9]+]] s: ptr<const i8>, %[[VALUE_c:[0-9]+]] c: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], read<ptr<const i8>>(%[[VALUE_s_4]]), read<i32>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strrchr_nonstring:[0-9]+]] @strrchr_nonstring(%[[VALUE_s_5:[0-9]+]] s: ptr<const i8>, %[[VALUE_c_2:[0-9]+]] c: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strrchr]], read<ptr<const i8>>(%[[VALUE_s_5]]), read<i32>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strcpy_nonstring:[0-9]+]] @strcpy_nonstring(%[[VALUE_d_3:[0-9]+]] d: ptr<i8>, %[[VALUE_s_6:[0-9]+]] s: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], read<ptr<i8>>(%[[VALUE_d_3]]), read<ptr<const i8>>(%[[VALUE_s_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strncpy_nonstring:[0-9]+]] @strncpy_nonstring(%[[VALUE_d_4:[0-9]+]] d: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], read<ptr<i8>>(%[[VALUE_d_4]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_ns5]])), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strstr_nonstring_1:[0-9]+]] @strstr_nonstring_1(%[[VALUE_a_3:[0-9]+]] a: ptr<const i8>, %[[VALUE_b_3:[0-9]+]] b: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strstr]], read<ptr<const i8>>(%[[VALUE_a_3]]), read<ptr<const i8>>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strstr_nonstring_2:[0-9]+]] @strstr_nonstring_2(%[[VALUE_a_4:[0-9]+]] a: ptr<const i8>, %[[VALUE_b_4:[0-9]+]] b: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strstr]], read<ptr<const i8>>(%[[VALUE_a_4]]), read<ptr<const i8>>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_stdup_nonstring:[0-9]+]] @stdup_nonstring(%[[VALUE_s_7:[0-9]+]] s: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_strdup]], read<ptr<const i8>>(%[[VALUE_s_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strlen_nonstring:[0-9]+]] @strlen_nonstring(%[[VALUE_s_8:[0-9]+]] s: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE_s_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_printf_nonstring:[0-9]+]] @printf_nonstring(%[[VALUE_s_9:[0-9]+]] s: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], read<ptr<const i8>>(%[[VALUE_s_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sprintf_nonstring_2:[0-9]+]] @sprintf_nonstring_2(%[[VALUE_d_5:[0-9]+]] d: ptr<i8>, %[[VALUE_s_10:[0-9]+]] s: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sprintf]], read<ptr<i8>>(%[[VALUE_d_5]]), read<ptr<const i8>>(%[[VALUE_s_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
