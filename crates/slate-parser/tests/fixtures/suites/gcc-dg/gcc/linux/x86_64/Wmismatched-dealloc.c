/* PR middle-end/94527 - Add an attribute that marks a function as freeing
   an object
   Verify that attribute malloc with one or two arguments has the expected
   effect on diagnostics.
   { dg-options "-Wall -ftrack-macro-expansion=0" } */

#define A(...) __attribute__ ((malloc (__VA_ARGS__)))

typedef struct FILE   FILE;
typedef __SIZE_TYPE__ size_t;

void  free (void*);
void* malloc (size_t);
void* realloc (void*, size_t);

/* Declare functions with the minimum attributes malloc how they're
   likely going to be declared in <stdio.h>.  */
               int   fclose (FILE*);
A (fclose)     FILE* fdopen (int);
A (fclose)     FILE* fopen (const char*, const char*);
A (fclose)     FILE* fmemopen(void *, size_t, const char *);
A (fclose)     FILE* freopen (const char*, const char*, FILE*);
A (freopen, 3) FILE* freopen (const char*, const char*, FILE*);
A (fclose)     FILE* tmpfile (void);

A (fclose)     FILE* open_memstream (char**, size_t*);
A (fclose)     FILE* open_wmemstream (char**, size_t*);

               int   pclose (FILE*);
A (pclose)     FILE* popen (const char*, const char*);

               void  release (void*);
A (release)    FILE* acquire (void);

void sink (FILE*);


void nowarn_fdopen (void)
{
  {
    FILE *q = fdopen (0);
    if (!q)
      return;

    fclose (q);
  }

  {
    FILE *q = fdopen (0);
    if (!q)
      return;

    q = freopen ("1", "r", q);
    fclose (q);
  }

  {
    FILE *q = fdopen (0);
    if (!q)
      return;

    sink (q);
  }
}


void warn_fdopen (void)
{
  {
    FILE *q = fdopen (0);     // { dg-message "returned from 'fdopen'" "note" }
    sink (q);
    release (q);              // { dg-warning "'release' called on pointer returned from a mismatched allocation function" }
  }
  {
    FILE *q = fdopen (0);     // { dg-message "returned from 'fdopen'" "note" }
    sink (q);
    free (q);                 // { dg-warning "'free' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *q = fdopen (0);     // { dg-message "returned from 'fdopen'" "note" }
    sink (q);
    q = realloc (q, 7);       // { dg-warning "'realloc' called on pointer returned from a mismatched allocation function" }
    sink (q);
  }
}


void nowarn_fopen (void)
{
  {
    FILE *q = fopen ("1", "r");
    sink (q);
    fclose (q);
  }

  {
    FILE *q = fopen ("2", "r");
    sink (q);
    q = freopen ("3", "r", q);
    sink (q);
    fclose (q);
  }

  {
    FILE *q = fopen ("4", "r");
    sink (q);
  }
}


void warn_fopen (void)
{
  {
    FILE *q = fopen ("1", "r");
    sink (q);
    release (q);              // { dg-warning "'release' called on pointer returned from a mismatched allocation function" }
  }
  {
    FILE *q = fdopen (0);
    sink (q);
    free (q);                 // { dg-warning "'free' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *q = fdopen (0);
    sink (q);
    q = realloc (q, 7);       // { dg-warning "'realloc' called on pointer returned from a mismatched allocation function" }
    sink (q);
  }
}


void test_freopen (FILE *p[])
{
  {
    FILE *q = freopen ("1", "r", p[0]);
    sink (q);
    fclose (q);
  }
  {
    FILE *q = freopen ("2", "r", p[1]);
    sink (q);
    q = freopen ("3", "r", q);
    sink (q);
    fclose (q);
  }

  {
    FILE *q;
    q = freopen ("3", "r", p[2]); // { dg-message "returned from 'freopen'" }
    sink (q);
    q = realloc (q, 7);       // { dg-warning "'realloc' called on pointer returned from a mismatched allocation function" }
    sink (q);
  }
}


void test_tmpfile (void)
{
  {
    FILE *p = tmpfile ();
    sink (p);
    fclose (p);
  }

  {
    FILE *p = tmpfile ();
    sink (p);
    p = freopen ("1", "r", p);
    sink (p);
    fclose (p);
  }

  {
    FILE *p = tmpfile ();     // { dg-message "returned from 'tmpfile'" "note" }
    sink (p);
    pclose (p);               // { dg-warning "'pclose' called on pointer returned from a mismatched allocation function" }
  }
}


void test_open_memstream (char **bufp, size_t *sizep)
{
  {
    FILE *p = open_memstream (bufp, sizep);
    sink (p);
    fclose (p);
  }

  {
    FILE *p = open_memstream (bufp, sizep);
    sink (p);
    p = freopen ("1", "r", p);
    sink (p);
    fclose (p);
  }

  {
    FILE *p;
    p = open_memstream (bufp, sizep);   // { dg-message "returned from 'open_memstream'" "note" }
    sink (p);
    pclose (p);               // { dg-warning "'pclose' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *p;
    p = open_memstream (bufp, sizep);   // { dg-message "returned from 'open_memstream'" "note" }
    sink (p);
    free (p);                 // { dg-warning "'free' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *p;
    p = open_memstream (bufp, sizep);   // { dg-message "returned from 'open_memstream'" "note" }
    sink (p);
    release (p);              // { dg-warning "'release' called on pointer returned from a mismatched allocation function" }
  }
}


void test_open_wmemstream (char **bufp, size_t *sizep)
{
  {
    FILE *p = open_wmemstream (bufp, sizep);
    sink (p);
    fclose (p);
  }

  {
    FILE *p = open_wmemstream (bufp, sizep);
    sink (p);
    p = freopen ("1", "r", p);
    sink (p);
    fclose (p);
  }

  {
    FILE *p;
    p = open_wmemstream (bufp, sizep);  // { dg-message "returned from 'open_wmemstream'" "note" }
    sink (p);
    pclose (p);               // { dg-warning "'pclose' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *p;
    p = open_wmemstream (bufp, sizep);  // { dg-message "returned from 'open_wmemstream'" "note" }
    sink (p);
    free (p);                 // { dg-warning "'free' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *p;
    p = open_wmemstream (bufp, sizep);  // { dg-message "returned from 'open_wmemstream'" "note" }
    sink (p);
    release (p);              // { dg-warning "'release' called on pointer returned from a mismatched allocation function" }
  }
}


void warn_malloc (void)
{
  {
    FILE *p = malloc (100);   // { dg-message "returned from 'malloc'" "note" }
    sink (p);
    fclose (p);               // { dg-warning "'fclose' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *p = malloc (100);   // { dg-message "returned from 'malloc'" "note" }
    sink (p);
    p = freopen ("1", "r", p);// { dg-warning "'freopen' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *p = malloc (100);   // { dg-message "returned from 'malloc'" "note" }
    sink (p);
    pclose (p);               // { dg-warning "'pclose' called on pointer returned from a mismatched allocation function" }
  }
}


void test_acquire (void)
{
  {
    FILE *p = acquire ();
    release (p);
  }

  {
    FILE *p = acquire ();
    sink (p);
    release (p);
  }

  {
    FILE *p = acquire ();     // { dg-message "returned from 'acquire'" "note" }
    sink (p);
    fclose (p);               // { dg-warning "'fclose' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *p = acquire ();     // { dg-message "returned from 'acquire'" "note" }
    sink (p);
    pclose (p);               // { dg-warning "'pclose' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *p = acquire ();     // { dg-message "returned from 'acquire'" "note" }
    sink (p);
    p = freopen ("1", "r", p);  // { dg-warning "'freopen' called on pointer returned from a mismatched allocation function" }
    sink (p);
  }

  {
    FILE *p = acquire ();     // { dg-message "returned from 'acquire'" "note" }
    sink (p);
    free (p);               // { dg-warning "'free' called on pointer returned from a mismatched allocation function" }
  }

  {
    FILE *p = acquire ();     // { dg-message "returned from 'acquire'" "note" }
    sink (p);
    p = realloc (p, 123);     // { dg-warning "'realloc' called on pointer returned from a mismatched allocation function" }
    sink (p);
  }
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
// DEFAULT-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_FILE_2:[0-9]+]] FILE = @type[[TYPE_FILE]];
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE1:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_realloc:[0-9]+]] @realloc(%[[VALUE2:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fclose:[0-9]+]] @fclose(%[[VALUE4:[0-9]+]] <unnamed>: ptr<@type[[TYPE_FILE]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fdopen:[0-9]+]] @fdopen(%[[VALUE5:[0-9]+]] <unnamed>: i32) -> ptr<@type[[TYPE_FILE]]> [linkage=external] [deallocator=%[[VALUE_fclose]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_fopen:[0-9]+]] @fopen(%[[VALUE6:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE7:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<@type[[TYPE_FILE]]> [linkage=external] [deallocator=%[[VALUE_fclose]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_fmemopen:[0-9]+]] @fmemopen(%[[VALUE8:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE9:[0-9]+]] <unnamed>: u64, %[[VALUE10:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<@type[[TYPE_FILE]]> [linkage=external] [deallocator=%[[VALUE_fclose]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_freopen:[0-9]+]] @freopen(%[[VALUE11:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE12:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE13:[0-9]+]] <unnamed>: ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]> [linkage=external] [deallocator=%[[VALUE_fclose]], argument=0] [deallocator=%[[VALUE_freopen]], argument=2];
// DEFAULT-NEXT:     fn %[[VALUE_tmpfile:[0-9]+]] @tmpfile() -> ptr<@type[[TYPE_FILE]]> [linkage=external] [deallocator=%[[VALUE_fclose]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_open_memstream:[0-9]+]] @open_memstream(%[[VALUE14:[0-9]+]] <unnamed>: ptr<ptr<i8>>, %[[VALUE15:[0-9]+]] <unnamed>: ptr<u64>) -> ptr<@type[[TYPE_FILE]]> [linkage=external] [deallocator=%[[VALUE_fclose]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_open_wmemstream:[0-9]+]] @open_wmemstream(%[[VALUE16:[0-9]+]] <unnamed>: ptr<ptr<i8>>, %[[VALUE17:[0-9]+]] <unnamed>: ptr<u64>) -> ptr<@type[[TYPE_FILE]]> [linkage=external] [deallocator=%[[VALUE_fclose]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_pclose:[0-9]+]] @pclose(%[[VALUE18:[0-9]+]] <unnamed>: ptr<@type[[TYPE_FILE]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_popen:[0-9]+]] @popen(%[[VALUE19:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE20:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<@type[[TYPE_FILE]]> [linkage=external] [deallocator=%[[VALUE_pclose]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_release:[0-9]+]] @release(%[[VALUE21:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acquire:[0-9]+]] @acquire() -> ptr<@type[[TYPE_FILE]]> [linkage=external] [deallocator=%[[VALUE_release]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE22:[0-9]+]] <unnamed>: ptr<@type[[TYPE_FILE]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_fdopen:[0-9]+]] @nowarn_fdopen() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(i32) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fdopen]], const<i32>(0));
// DEFAULT-NEXT:             if not<bool>(ne<ptr<@type[[TYPE_FILE]]>>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q]]), null<ptr<@type[[TYPE_FILE]]>>))
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_2:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(i32) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fdopen]], const<i32>(0));
// DEFAULT-NEXT:             if not<bool>(ne<ptr<@type[[TYPE_FILE]]>>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_2]]), null<ptr<@type[[TYPE_FILE]]>>))
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_2]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]])), read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_2]])));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_3:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(i32) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fdopen]], const<i32>(0));
// DEFAULT-NEXT:             if not<bool>(ne<ptr<@type[[TYPE_FILE]]>>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_3]]), null<ptr<@type[[TYPE_FILE]]>>))
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_3]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_fdopen:[0-9]+]] @warn_fdopen() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_4:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(i32) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fdopen]], const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_4]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_release]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_4]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_5:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(i32) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fdopen]], const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_5]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_5]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_6:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(i32) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fdopen]], const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_6]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_6]], pointer_cast<ptr<@type[[TYPE_FILE]]>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_6]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_6]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_fopen:[0-9]+]] @nowarn_fopen() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_7:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_3]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_7]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_7]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_8:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_5]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_6]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_8]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_8]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_7]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_8]])), read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_8]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_8]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_8]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_9:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_9]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_10]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_9]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_fopen:[0-9]+]] @warn_fopen() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_10:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_11]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_12]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_10]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_release]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_10]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_11:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(i32) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fdopen]], const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_11]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_11]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_12:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(i32) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_fdopen]], const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_12]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_12]], pointer_cast<ptr<@type[[TYPE_FILE]]>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_12]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_12]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_freopen:[0-9]+]] @test_freopen(%[[VALUE_p:[0-9]+]] p: ptr<ptr<@type[[TYPE_FILE]]>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_13:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_13]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_14]])), read<ptr<@type[[TYPE_FILE]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_FILE]]>>, subtract=false, element=ptr<@type[[TYPE_FILE]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_FILE]]>>>(%[[VALUE_p]]), const<i32>(0)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_13]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_13]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_14:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_15]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_16]])), read<ptr<@type[[TYPE_FILE]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_FILE]]>>, subtract=false, element=ptr<@type[[TYPE_FILE]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_FILE]]>>>(%[[VALUE_p]]), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_14]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_14]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_17]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_18]])), read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_14]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_14]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_14]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_15:[0-9]+]] q: ptr<@type[[TYPE_FILE]]> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_15]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_19]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_20]])), read<ptr<@type[[TYPE_FILE]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_FILE]]>>, subtract=false, element=ptr<@type[[TYPE_FILE]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_FILE]]>>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_15]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_15]], pointer_cast<ptr<@type[[TYPE_FILE]]>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_15]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_q_15]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_tmpfile:[0-9]+]] @test_tmpfile() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn() -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_tmpfile]]);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn() -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_tmpfile]]);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_3]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_21]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_22]])), read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_3]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_4:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn() -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_tmpfile]]);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_4]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_pclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_4]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_open_memstream:[0-9]+]] @test_open_memstream(%[[VALUE_bufp:[0-9]+]] bufp: ptr<ptr<i8>>, %[[VALUE_sizep:[0-9]+]] sizep: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_5:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_open_memstream]], read<ptr<ptr<i8>>>(%[[VALUE_bufp]]), read<ptr<u64>>(%[[VALUE_sizep]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_5]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_6:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_open_memstream]], read<ptr<ptr<i8>>>(%[[VALUE_bufp]]), read<ptr<u64>>(%[[VALUE_sizep]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_6]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_6]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_23]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_24]])), read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_6]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_6]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_6]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_7:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_7]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_open_memstream]], read<ptr<ptr<i8>>>(%[[VALUE_bufp]]), read<ptr<u64>>(%[[VALUE_sizep]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_7]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_pclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_7]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_8:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_8]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_open_memstream]], read<ptr<ptr<i8>>>(%[[VALUE_bufp]]), read<ptr<u64>>(%[[VALUE_sizep]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_8]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_8]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_9:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_9]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_open_memstream]], read<ptr<ptr<i8>>>(%[[VALUE_bufp]]), read<ptr<u64>>(%[[VALUE_sizep]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_9]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_release]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_9]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_open_wmemstream:[0-9]+]] @test_open_wmemstream(%[[VALUE_bufp_2:[0-9]+]] bufp: ptr<ptr<i8>>, %[[VALUE_sizep_2:[0-9]+]] sizep: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_10:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_open_wmemstream]], read<ptr<ptr<i8>>>(%[[VALUE_bufp_2]]), read<ptr<u64>>(%[[VALUE_sizep_2]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_10]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_10]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_11:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_open_wmemstream]], read<ptr<ptr<i8>>>(%[[VALUE_bufp_2]]), read<ptr<u64>>(%[[VALUE_sizep_2]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_11]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_11]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_25]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_26]])), read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_11]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_11]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_11]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_12:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_12]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_open_wmemstream]], read<ptr<ptr<i8>>>(%[[VALUE_bufp_2]]), read<ptr<u64>>(%[[VALUE_sizep_2]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_12]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_pclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_12]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_13:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_13]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_open_wmemstream]], read<ptr<ptr<i8>>>(%[[VALUE_bufp_2]]), read<ptr<u64>>(%[[VALUE_sizep_2]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_13]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_13]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_14:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_14]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_open_wmemstream]], read<ptr<ptr<i8>>>(%[[VALUE_bufp_2]]), read<ptr<u64>>(%[[VALUE_sizep_2]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_14]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_release]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_14]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_malloc:[0-9]+]] @warn_malloc() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_15:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_FILE]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(100)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_15]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_15]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_16:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_FILE]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(100)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_16]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_16]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_27]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_28]])), read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_16]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_17:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_FILE]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(100)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_17]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_pclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_17]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_acquire:[0-9]+]] @test_acquire() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_18:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn() -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_acquire]]);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_release]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_18]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_19:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn() -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_acquire]]);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_19]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_release]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_19]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_20:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn() -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_acquire]]);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_20]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_20]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_21:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn() -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_acquire]]);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_21]]));
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<@type[[TYPE_FILE]]>) -> i32>(%[[VALUE_pclose]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_21]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_22:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn() -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_acquire]]);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_22]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_22]], call<ptr<@type[[TYPE_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>, ptr<@type[[TYPE_FILE]]>) -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_freopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_29]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_30]])), read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_22]])));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_22]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_23:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn() -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_acquire]]);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_23]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_23]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_24:[0-9]+]] p: ptr<@type[[TYPE_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE_FILE]]>, signature=fn() -> ptr<@type[[TYPE_FILE]]>>(%[[VALUE_acquire]]);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_24]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_24]], pointer_cast<ptr<@type[[TYPE_FILE]]>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_24]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(123))))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_FILE]]>) -> void>(%[[VALUE_sink]], read<ptr<@type[[TYPE_FILE]]>>(%[[VALUE_p_24]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
