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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     extern %24 ns5: array<i8, 5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @printf(%66 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @puts(%67 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @puts_unlocked(%68 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @sprintf(%69 <unnamed>: ptr<i8>, %70 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @snprintf(%71 <unnamed>: ptr<i8>, %72 <unnamed>: u64, %73 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @vsprintf(%74 <unnamed>: ptr<i8>, %75 <unnamed>: ptr<const i8>, %76 <unnamed>: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @vsnprintf(%77 <unnamed>: ptr<i8>, %78 <unnamed>: u64, %79 <unnamed>: ptr<const i8>, %80 <unnamed>: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @strcmp(%81 <unnamed>: ptr<const i8>, %82 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %10 @strncmp(%83 <unnamed>: ptr<const i8>, %84 <unnamed>: ptr<const i8>, %85 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @stpcpy(%86 <unnamed>: ptr<i8>, %87 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %12 @stpncpy(%88 <unnamed>: ptr<i8>, %89 <unnamed>: ptr<const i8>, %90 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %13 @strcat(%91 <unnamed>: ptr<i8>, %92 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %14 @strncat(%93 <unnamed>: ptr<i8>, %94 <unnamed>: ptr<const i8>, %95 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %15 @strcpy(%96 <unnamed>: ptr<i8>, %97 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %16 @strncpy(%98 <unnamed>: ptr<i8>, %99 <unnamed>: ptr<const i8>, %100 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %17 @strchr(%101 <unnamed>: ptr<const i8>, %102 <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %18 @strrchr(%103 <unnamed>: ptr<const i8>, %104 <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %19 @strstr(%105 <unnamed>: ptr<const i8>, %106 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %20 @strdup(%107 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %21 @strlen(%108 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %22 @strnlen(%109 <unnamed>: ptr<const i8>, %110 <unnamed>: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %23 @strndup(%111 <unnamed>: ptr<const i8>, %112 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %25 @strcmp_nonstring_1(%26 a: ptr<const i8>, %27 b: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%9, read<ptr<const i8>>(%26), read<ptr<const i8>>(%27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @strcmp_nonstring_2(%29 a: ptr<const i8>, %30 b: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%9, read<ptr<const i8>>(%29), read<ptr<const i8>>(%30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @strncmp_nonstring_1(%32 s: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%10, read<ptr<const i8>>(%32), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%24)), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @strncmp_nonstring_2(%34 s: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%10, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%24)), read<ptr<const i8>>(%34), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @stpcpy_nonstring(%36 d: ptr<i8>, %37 s: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%11, read<ptr<i8>>(%36), read<ptr<const i8>>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @stpncpy_nonstring(%39 d: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%12, read<ptr<i8>>(%39), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%24)), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @strchr_nonstring(%41 s: ptr<const i8>, %42 c: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%17, read<ptr<const i8>>(%41), read<i32>(%42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @strrchr_nonstring(%44 s: ptr<const i8>, %45 c: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%18, read<ptr<const i8>>(%44), read<i32>(%45));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @strcpy_nonstring(%47 d: ptr<i8>, %48 s: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%15, read<ptr<i8>>(%47), read<ptr<const i8>>(%48));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @strncpy_nonstring(%50 d: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%16, read<ptr<i8>>(%50), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%24)), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @strstr_nonstring_1(%52 a: ptr<const i8>, %53 b: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%19, read<ptr<const i8>>(%52), read<ptr<const i8>>(%53));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @strstr_nonstring_2(%55 a: ptr<const i8>, %56 b: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%19, read<ptr<const i8>>(%55), read<ptr<const i8>>(%56));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @stdup_nonstring(%58 s: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%20, read<ptr<const i8>>(%58));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @strlen_nonstring(%60 s: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u64, signature=fn(ptr<const i8>) -> u64>(%21, read<ptr<const i8>>(%60));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %61 @printf_nonstring(%62 s: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, read<ptr<const i8>>(%62));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @sprintf_nonstring_2(%64 d: ptr<i8>, %65 s: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%5, read<ptr<i8>>(%64), read<ptr<const i8>>(%65));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
