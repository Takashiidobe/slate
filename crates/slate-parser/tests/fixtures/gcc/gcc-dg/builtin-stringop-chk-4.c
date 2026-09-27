/* Test exercising buffer overflow warnings emitted for raw memory and
   string manipulation builtins involving ranges of sizes and strings
   of varying lengths.  */
/* { dg-do compile } */
/* { dg-options "-O2 -ftrack-macro-expansion=0" } */

#define INT_MAX      __INT_MAX__
#define PTRDIFF_MAX  __PTRDIFF_MAX__
#define SIZE_MAX     __SIZE_MAX__

typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__    size_t;

static const size_t ssize_max = SIZE_MAX / 2;
static const size_t size_max = SIZE_MAX;

extern signed char    schar_val;
extern signed short   sshrt_val;
extern signed int     sint_val;
extern signed long    slong_val;
extern unsigned char  uchar_val;
extern unsigned short ushrt_val;
extern unsigned int   uint_val;
extern unsigned long  ulong_val;

#define memcpy(d, s, n) (memcpy ((d), (s), (n)), sink ((d)))
extern void* (memcpy)(void*, const void*, size_t);

#define mempcpy(d, s, n) (mempcpy ((d), (s), (n)), sink ((d)))
extern void* (mempcpy)(void*, const void*, size_t);

#define memset(d, c, n) (memset ((d), (c), (n)), sink ((d)))
extern void* (memset)(void*, int, size_t);

#define bzero(d, n) (bzero ((d), (n)), sink ((d)))
extern void (bzero)(void*, size_t);

#define strcat(d, s) (strcat ((d), (s)), sink ((d)))
extern char* (strcat)(char*, const char*);

#define strncat(d, s, n) (strncat ((d), (s), (n)), sink ((d)))
extern char* (strncat)(char*, const char*, size_t);

#define strcpy(d, s) (strcpy ((d), (s)), sink ((d)))
extern char* (strcpy)(char*, const char*);

#define strncpy(d, s, n) (strncpy ((d), (s), (n)), sink ((d)))
extern char* (strncpy)(char*, const char*, size_t);

void sink (void*);

/* Function to "generate" a random number each time it's called.  Declared
   (but not defined) and used to prevent GCC from making assumptions about
   their values based on the variables uses in the tested expressions.  */
size_t random_unsigned_value (void);
ptrdiff_t random_signed_value (void);

/* Return a random unsigned value between MIN and MAX.  */

static inline size_t
unsigned_range (size_t min, size_t max)
{
  const size_t val = random_unsigned_value ();
  return val < min || max < val ? min : val;
}

/* Return a random signed value between MIN and MAX.  */

static inline ptrdiff_t
signed_range (ptrdiff_t min, ptrdiff_t max)
{
  const ptrdiff_t val = random_signed_value ();
  return val < min || max < val ? min : val;
}

/* For brevity.  */
#define UR(min, max)   unsigned_range (min, max)
#define SR(min, max)   signed_range (min, max)

/* Return a pointer to constant string whose length is at least MINLEN
   and at most 10.  */
#define S(minlen)				\
  (minlen == random_unsigned_value ()		\
   ? "0123456789" + 10 - minlen : "0123456789")

/* Test memcpy with a number of bytes bounded by a known range.  */

void test_memcpy_range (void *d, const void *s)
{
  char buf[5];

  memcpy (buf, s, UR (0, 5));
  memcpy (buf, s, UR (1, 5));
  memcpy (buf, s, UR (2, 5));
  memcpy (buf, s, UR (3, 5));
  memcpy (buf, s, UR (4, 5));

  memcpy (buf, s, UR (6, 7));  /* { dg-warning "writing between 6 and 7 bytes into a region of size 5 overflows the destination" } */

  memcpy (buf + 5, s, UR (1, 2));  /* { dg-warning "writing between 1 and 2 bytes into a region of size 0 overflows the destination" } */

  memcpy (buf + size_max, s, UR (1, 2));  /* { dg-warning "writing between 1 and 2 bytes into a region of size 0 overflows the destination" "excessive pointer offset" } */

  memcpy (buf, s, UR (ssize_max, size_max));   /* { dg-warning "writing \[0-9\]+ or more bytes into a region of size 5 overflows the destination" } */
  memcpy (buf, s, UR (ssize_max + 1, size_max));  /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */
  memcpy (buf, s, UR (size_max - 1, size_max));  /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */

  /* Exercise memcpy into a destination of unknown size with excessive
     number of bytes.  */
  memcpy (d, s, UR (ssize_max, size_max));
  memcpy (d, s, UR (ssize_max + 1, size_max));   /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */

  memcpy (buf, s, SR (-1, 1));
  memcpy (buf, s, SR (-3, 2));
  memcpy (buf, s, SR (-5, 3));
  memcpy (buf, s, SR (-7, 4));
  memcpy (buf, s, SR (-9, 5));
  memcpy (buf, s, SR (-11, 6));

  memcpy (d, s, SR (-1, 1));
  memcpy (d, s, SR (-3, 2));
  memcpy (d, s, SR (-5, 3));
  memcpy (d, s, SR (-7, 4));
  memcpy (d, s, SR (-9, 5));
  memcpy (d, s, SR (-11, 6));

  memcpy (buf, s, SR (-2, -1));   /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */
  memcpy (d, s, SR (-2, -1));   /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */

  /* Even though the following calls are bounded by the range of N's
     type they must not cause a warning for obvious reasons.  */
  memcpy (buf, s, schar_val);
  memcpy (buf, s, sshrt_val);
  memcpy (buf, s, sint_val);
  memcpy (buf, s, slong_val);

  memcpy (buf, s, uchar_val);
  memcpy (buf, s, ushrt_val);
  memcpy (buf, s, uint_val);
  memcpy (buf, s, ulong_val);

  memcpy (buf, s, schar_val + 1);
  memcpy (buf, s, sshrt_val + 2);
  memcpy (buf, s, sint_val + 3);
  memcpy (buf, s, slong_val + 4);

  memcpy (d, s, uchar_val + 5);
  memcpy (d, s, ushrt_val + 6);
  memcpy (d, s, uint_val + 7);
  memcpy (d, s, ulong_val + 8);

  memcpy (d, s, schar_val);
  memcpy (d, s, sshrt_val);
  memcpy (d, s, sint_val);
  memcpy (d, s, slong_val);

  memcpy (d, s, uchar_val);
  memcpy (d, s, ushrt_val);
  memcpy (d, s, uint_val);
  memcpy (d, s, ulong_val);

  memcpy (d, s, schar_val + 1);
  memcpy (d, s, sshrt_val + 2);
  memcpy (d, s, sint_val + 3);
  memcpy (d, s, slong_val + 4);

  memcpy (d, s, uchar_val + 5);
  memcpy (d, s, ushrt_val + 6);
  memcpy (d, s, uint_val + 7);
  memcpy (d, s, ulong_val + 8);
}

/* Test mempcpy with a number of bytes bounded by a known range.  */

void test_mempcpy_range (void *d, const void *s)
{
  char buf[5];

  mempcpy (buf, s, UR (0, 5));
  mempcpy (buf, s, UR (1, 5));
  mempcpy (buf, s, UR (2, 5));
  mempcpy (buf, s, UR (3, 5));
  mempcpy (buf, s, UR (4, 5));

  mempcpy (buf, s, UR (6, 7));  /* { dg-warning "writing between 6 and 7 bytes into a region of size 5 overflows the destination" } */

  mempcpy (buf, s, UR (6, 7));  /* { dg-warning "writing between 6 and 7 bytes into a region of size 5 overflows the destination" } */

  mempcpy (buf, s, UR (ssize_max, size_max));   /* { dg-warning "writing \[0-9\]+ or more bytes into a region of size 5 overflows the destination" } */
  mempcpy (buf, s, UR (ssize_max + 1, size_max));  /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */
  mempcpy (buf, s, UR (size_max - 1, size_max));  /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */

  /* Exercise mempcpy into a destination of unknown size with excessive
     number of bytes.  */
  mempcpy (d, s, UR (ssize_max, size_max));
  mempcpy (d, s, UR (ssize_max + 1, size_max));   /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */
}

/* Test memset with a number of bytes bounded by a known range.  */

void test_memset_range (void *d)
{
  char buf[5];

  memset (buf, 0, UR (0, 5));
  memset (buf, 0, UR (1, 5));
  memset (buf, 0, UR (2, 5));
  memset (buf, 0, UR (3, 5));
  memset (buf, 0, UR (4, 5));

  memset (buf, 0, UR (6, 7));  /* { dg-warning "writing between 6 and 7 bytes into a region of size 5 overflows the destination" } */

  memset (buf, 0, UR (6, 7));  /* { dg-warning "writing between 6 and 7 bytes into a region of size 5 overflows the destination" } */

  memset (buf, 0, UR (ssize_max, size_max));   /* { dg-warning "writing \[0-9\]+ or more bytes into a region of size 5 overflows the destination" } */
  memset (buf, 0, UR (ssize_max + 1, size_max));  /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */
  memset (buf, 0, UR (size_max - 1, size_max));  /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */

  /* Exercise memset into a destination of unknown size with excessive
     number of bytes.  */
  memset (d, 0, UR (ssize_max, size_max));
  memset (d, 0, UR (ssize_max + 1, size_max));   /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */
}

/* Test bzero with a number of bytes bounded by a known range.  */

void test_bzero_range (void *d)
{
  char buf[5];

  bzero (buf, UR (0, 5));
  bzero (buf, UR (1, 5));
  bzero (buf, UR (2, 5));
  bzero (buf, UR (3, 5));
  bzero (buf, UR (4, 5));

  bzero (buf, UR (6, 7));  /* { dg-warning "writing between 6 and 7 bytes into a region of size 5 overflows the destination" } */

  bzero (buf, UR (6, 7));  /* { dg-warning "writing between 6 and 7 bytes into a region of size 5 overflows the destination" } */

  bzero (buf, UR (ssize_max, size_max));   /* { dg-warning "writing \[0-9\]+ or more bytes into a region of size 5 overflows the destination" } */
  bzero (buf, UR (ssize_max + 1, size_max));  /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */
  bzero (buf, UR (size_max - 1, size_max));  /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */

  /* Exercise bzero into a destination of unknown size with excessive
     number of bytes.  */
  bzero (d, UR (ssize_max, size_max));
  bzero (d, UR (ssize_max + 1, size_max));   /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */
}

/* Test strcat with an argument referencing a non-constant string of
   lengths in a known range.  */

void test_strcat_range (void)
{
  char buf[5] = "";

  strcat (buf, S (0));
  strcat (buf, S (1));
  strcat (buf, S (2));
  strcat (buf, S (3));
  strcat (buf, S (4));
  strcat (buf, S (5));   /* { dg-warning "writing between 6 and 11 bytes into a region of size 5 " } */

  {
    /* The implementation of the warning isn't smart enough to determine
       the length of the string in the buffer so it assumes it's empty
       and issues the warning basically for the same cases as strcat.  */
    char buf2[5] = "12";
    strcat (buf2, S (4));   /* { dg-warning "writing 5 bytes into a region of size 3" "strcat to a non-empty string" { xfail *-*-* } } */
  }
}

/* Verify that strcpy with an unknown source string doesn't cause
   warnings unless the destination has zero size.  */

void test_strcpy (const char *src)
{
  struct A { char a[2]; char b[3]; } a;

  strcpy (a.a, src);
  strcpy (a.a + 1, src);

  /* There must be enough room in the destination for the terminating
     nul, otherwise verify that a warning is issued.
     The following works as expected with __builtin___strcpy_chk and
     __builtin_object_size because they see that the offset is from
     the a.a array.  When optimization is enabled, it isn't detected
     by __bultin_strcpy (when __builtin_object_size isn't called
     explicitly) because by the time it's seen the offset has been
     transformed to one from the beginning of the whole object, i.e.,
     as if it had been written as (char*)&a + 2 .  Then the destination
     size is taken to be the rest of the whole object.  It is detected
     by __builtin_strcpy when optimization is not enabled because then
     the &a.a + 2 expression is preserved.  But without optimization
     an ordinary call to strcpy isn't transformed to __builtin_strcpy
     and so it can't be detected here (since the rest of the test
     relies on optimization).  */
  strcpy (a.a + 2, src);    /* { dg-warning "writing at least 1 byte into a region of size 0 " "strcpy into empty substring" { xfail *-*-* } } */

  /* This does work.  */
  strcpy (a.a + 5, src);    /* { dg-warning "writing 1 or more bytes into a region of size 0 " } */

  /* As does this.  */
  strcpy (a.a + 17, src);    /* { dg-warning "writing 1 or more bytes into a region of size 0 " } */
}

/* Test strcpy with a non-constant source string of length in a known
   range.  */

void test_strcpy_range (void)
{
  char buf[5];

  strcpy (buf, S (0));
  strcpy (buf, S (1));
  strcpy (buf, S (2));
  strcpy (buf, S (4));
  strcpy (buf, S (5));   /* { dg-warning "writing between 6 and 11 bytes into a region of size 5 " } */
  strcpy (buf, S (6));   /* { dg-warning "writing between 7 and 11 bytes" } */
  strcpy (buf, S (7));   /* { dg-warning "writing between 8 and 11 bytes" } */
  strcpy (buf, S (8));   /* { dg-warning "writing between 9 and 11 bytes" } */
  strcpy (buf, S (9));   /* { dg-warning "writing between 10 and 11 bytes" } */
  strcpy (buf, S (10));   /* { dg-warning "writing 11 bytes" } */

  strcpy (buf + 5, S (0));   /* { dg-warning "writing between 1 and 11 bytes" } */

  strcpy (buf + 17, S (0));   /* { dg-warning "writing between 1 and 11 bytes " } */
}

/* Test strncat with an argument referencing a non-constant string of
   lengths in a known range.  */

void test_strncat_range (void)
{
  char buf[5] = "";

  strncat (buf, S (0), 0);
  strncat (buf, S (0), 1);
  strncat (buf, S (0), 2);
  strncat (buf, S (0), 3);
  strncat (buf, S (0), 4);

  strncat (buf + 5, S (0), 0);

  strncat (buf + 5, S (0), 1);   /* { dg-warning "specified \(bound|size\) 1 exceeds destination size 0" } */
  strncat (buf + 5, S (1), 1);   /* { dg-warning "specified \(bound|size\) 1 exceeds destination size 0" } */

  /* Strncat always appends a terminating null after copying the N
     characters so the following triggers a warning pointing out
     that specifying sizeof(buf) as the upper bound may cause
     the nul to overflow the destination.  */
  strncat (buf, S (0), 5);   /* { dg-warning "specified \(bound|size\) 5 equals destination size" } */
  strncat (buf, S (0), 6);   /* { dg-warning "specified \(bound|size\) 6 exceeds destination size 5" } */

  strncat (buf, S (1), 0);
  strncat (buf, S (1), 1);
  strncat (buf, S (1), 2);
  strncat (buf, S (1), 3);
  strncat (buf, S (1), 4);
  strncat (buf, S (1), 5);   /* { dg-warning "specified \(bound|size\) 5 equals destination size" } */
  strncat (buf, S (1), 6);   /* { dg-warning "specified \(bound|size\) 6 exceeds destination size 5" } */
  strncat (buf, S (2), 6);   /* { dg-warning "specified \(bound|size\) 6 exceeds destination size 5" } */

  /* The following could just as well say "writing 6 bytes into a region
     of size 5.  Either would be correct and probably equally as clear
     in this case.  But when the length of the source string is not known
     at all then the bound warning seems clearer.  */
  strncat (buf, S (5), 6);   /* { dg-warning "specified \(bound|size\) 6 exceeds destination size 5" } */
  strncat (buf, S (7), 6);   /* { dg-warning "specified \(bound|size\) 6 exceeds destination size 5" } */

  {
    /* The implementation of the warning isn't smart enough to determine
       the length of the string in the buffer so it assumes it's empty
       and issues the warning basically for the same cases as strncpy.  */
    char buf2[5] = "12";
    strncat (buf2, S (4), 4);   /* { dg-warning "writing 5 bytes into a region of size 3" "strncat to a non-empty string" { xfail *-*-* } } */
  }
}

/* Test strncat_chk with an argument referencing a non-constant string
   of lengths in a known range.  */

void test_strncat_chk_range (char *d)
{
  char buf[5] = "";

#define strncat_chk(d, s, n) \
  __builtin___strncat_chk ((d), (s), (n), __builtin_object_size (d, 1));

  strncat_chk (buf, S (0), 1);
  strncat_chk (buf, S (0), 2);
  strncat_chk (buf, S (0), 3);
  strncat_chk (buf, S (0), 4);
  strncat_chk (buf, S (0), 5);   /* { dg-warning "specified \(bound|size\) 5 equals destination size" } */

  strncat_chk (buf, S (5), 1);
  strncat_chk (buf, S (5), 2);
  strncat_chk (buf, S (5), 3);
  strncat_chk (buf, S (5), 4);
  strncat_chk (buf, S (5), 5);   /* { dg-warning "specified \(bound|size\) 5 equals destination size" } */

  strncat_chk (buf, S (5), 10);   /* { dg-warning "specified \(bound|size\) \[0-9\]+ exceeds destination size 5" } */

  strncat_chk (d, S (5), size_max);   /* { dg-warning "specified \(bound|size\) \[0-9\]+ exceeds maximum object size " } */
}

/* Test strncpy with a non-constant source string of length in a known
   range and a constant number of bytes.  */

void test_strncpy_string_range (char *d)
{
  char buf[5];

  strncpy (buf, S (0), 0);
  strncpy (buf, S (0), 1);
  strncpy (buf, S (0), 2);
  strncpy (buf, S (0), 3);
  strncpy (buf, S (0), 4);
  strncpy (buf, S (0), 5);
  strncpy (buf, S (0), 6);   /* { dg-warning "writing 6 bytes into a region of size 5 " } */

  strncpy (buf, S (6), 4);
  strncpy (buf, S (7), 5);
  strncpy (buf, S (8), 6);   /* { dg-warning "writing 6 bytes into a region of size 5 " } */

  strncpy (buf, S (1), ssize_max - 1);   /* { dg-warning "writing \[0-9\]+ bytes into a region of size 5" } */
  strncpy (buf, S (2), ssize_max);   /* { dg-warning "writing \[0-9\]+ bytes into a region of size 5" } */
  strncpy (buf, S (3), ssize_max + 1);   /* { dg-warning "specified \(bound|size\) \[0-9\]+ exceeds maximum object size" } */
  strncpy (buf, S (4), size_max);   /* { dg-warning "specified \(bound|size\) \[0-9\]+ exceeds maximum object size" } */

  /* Exercise strncpy into a destination of unknown size with a valid
     and invalid constant number of bytes.  */
  strncpy (d, S (1), ssize_max - 1);
  strncpy (d, S (2), ssize_max);
  strncpy (d, S (3), ssize_max + 1);   /* { dg-warning "specified \(bound|size\) \[0-9\]+ exceeds maximum object size" } */
  strncpy (d, S (4), size_max);   /* { dg-warning "specified \(bound|size\) \[0-9\]+ exceeds maximum object size" } */
}

/* Test strncpy with a non-constant source string of length in a known
   range and a non-constant number of bytes also in a known range.  */

void test_strncpy_string_count_range (char *dst, const char *src)
{
  char buf[5];

  strncpy (buf, S (0), UR (0, 1));
  strncpy (buf, S (0), UR (0, 2));
  strncpy (buf, S (0), UR (0, 3));
  strncpy (buf, S (0), UR (0, 4));
  strncpy (buf, S (0), UR (0, 5));
  strncpy (buf, S (0), UR (0, 6));
  strncpy (buf, S (0), UR (1, 6));
  strncpy (buf, S (0), UR (2, 6));
  strncpy (buf, S (0), UR (3, 6));
  strncpy (buf, S (0), UR (4, 6));
  strncpy (buf, S (0), UR (5, 6));

  strncpy (buf, S (9), UR (0, 1));
  strncpy (buf, S (8), UR (0, 2));
  strncpy (buf, S (7), UR (0, 3));
  strncpy (buf, S (6), UR (0, 4));
  strncpy (buf, S (8), UR (0, 5));
  strncpy (buf, S (7), UR (0, 6));
  strncpy (buf, S (6), UR (1, 6));
  strncpy (buf, S (5), UR (2, 6));
  strncpy (buf, S (9), UR (3, 6));
  strncpy (buf, S (8), UR (4, 6));
  strncpy (buf, S (7), UR (5, 6));

  strncpy (buf, S (0), UR (6, 7));   /* { dg-warning "writing between 6 and 7 bytes into a region of size 5 " } */
  strncpy (buf, S (1), UR (7, 8));   /* { dg-warning "writing between 7 and 8 bytes into a region of size 5 " } */
  strncpy (buf, S (2), UR (ssize_max, ssize_max + 1));   /* { dg-warning "writing \[0-9\]+ or more bytes into a region of size 5 " } */

  strncpy (buf, S (2), UR (ssize_max + 1, ssize_max + 2));   /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */

  strncpy (buf + 5, S (0), UR (0, 1));
  strncpy (buf + 5, S (1), UR (0, 1));
  strncpy (buf + 5, S (0), UR (1, 2));   /* { dg-warning "writing between 1 and 2 bytes into a region of size 0 " } */
  strncpy (buf + 5, S (1), UR (1, 2));   /* { dg-warning "writing between 1 and 2 bytes into a region of size 0 " } */

  strncpy (buf, src, UR (0, 1));
  strncpy (buf, src, UR (0, 2));
  strncpy (buf, src, UR (0, 3));
  strncpy (buf, src, UR (0, 4));
  strncpy (buf, src, UR (0, 5));
  strncpy (buf, src, UR (0, 6));
  strncpy (buf, src, UR (1, 6));
  strncpy (buf, src, UR (2, 6));
  strncpy (buf, src, UR (3, 6));
  strncpy (buf, src, UR (4, 6));
  strncpy (buf, src, UR (5, 6));
  strncpy (buf, src, UR (6, 7));   /* { dg-warning "writing between 6 and 7 bytes into a region of size 5 " } */

  /* Exercise strncpy into a destination of unknown size  with a valid
     and invalid constant number of bytes.  */
  strncpy (dst, S (0), UR (5, 6));
  strncpy (dst, S (1), UR (6, 7));
  strncpy (dst, S (2), UR (7, 8));

  strncpy (dst, S (3), UR (ssize_max, ssize_max + 1));

  strncpy (dst, S (4), UR (ssize_max + 1, ssize_max + 2));   /* { dg-warning "specified \(bound|size\) between \[0-9\]+ and \[0-9\]+ exceeds maximum object size" } */
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
// DEFAULT-NEXT:     type @type0 ptrdiff_t = i64;
// DEFAULT-NEXT:     type @type1 size_t = u64;
// DEFAULT-NEXT:     type @type2 A = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 2>;
// DEFAULT-NEXT:         field1 b: array<i8, 3>;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 2]];
// DEFAULT-NEXT:     global %2 ssize_max: u64 [storage=static] [const] = div<u64, by_zero=ub>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %3 size_max: u64 [storage=static] [const] = const<u64>(18446744073709551615) [linkage=internal];
// DEFAULT-NEXT:     extern %4 schar_val: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %5 sshrt_val: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %6 sint_val: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %7 slong_val: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %8 uchar_val: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %9 ushrt_val: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %10 uint_val: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %11 ulong_val: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %89 .str89: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %90 .str90: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %91 .str91: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %92 .str92: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %93 .str93: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %94 .str94: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %95 .str95: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %96 .str96: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %97 .str97: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %98 .str98: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %99 .str99: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %129 .str129: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %134 .str134: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %135 .str135: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %136 .str136: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %137 .str137: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %138 .str138: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %139 .str139: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %140 .str140: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %141 .str141: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %142 .str142: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %143 .str143: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %144 .str144: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %145 .str145: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %146 .str146: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %147 .str147: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %148 .str148: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %149 .str149: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %150 .str150: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %151 .str151: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %152 .str152: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %153 .str153: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %154 .str154: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %155 .str155: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %156 .str156: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %157 .str157: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %158 .str158: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %159 .str159: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %160 .str160: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %161 .str161: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %162 .str162: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %163 .str163: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %164 .str164: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %165 .str165: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %166 .str166: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %167 .str167: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %168 .str168: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %174 .str174: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %175 .str175: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %179 .str179: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %180 .str180: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %181 .str181: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %182 .str182: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %183 .str183: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %184 .str184: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %185 .str185: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %186 .str186: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %187 .str187: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %188 .str188: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %189 .str189: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %190 .str190: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %191 .str191: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %192 .str192: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %193 .str193: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %194 .str194: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %195 .str195: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %196 .str196: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %197 .str197: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %198 .str198: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %199 .str199: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %200 .str200: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %201 .str201: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %202 .str202: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %203 .str203: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %204 .str204: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %205 .str205: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %206 .str206: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %207 .str207: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %208 .str208: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %209 .str209: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %210 .str210: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %211 .str211: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %212 .str212: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %213 .str213: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %214 .str214: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %215 .str215: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %216 .str216: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %217 .str217: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %218 .str218: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %219 .str219: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %220 .str220: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %221 .str221: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %222 .str222: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %223 .str223: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %224 .str224: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %225 .str225: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %226 .str226: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %227 .str227: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %228 .str228: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %229 .str229: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %230 .str230: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %231 .str231: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %232 .str232: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %233 .str233: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %234 .str234: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %235 .str235: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %236 .str236: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %237 .str237: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %238 .str238: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %239 .str239: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %240 .str240: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %241 .str241: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %242 .str242: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %243 .str243: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %244 .str244: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %245 .str245: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %246 .str246: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %247 .str247: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %248 .str248: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %249 .str249: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %250 .str250: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %251 .str251: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %252 .str252: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %253 .str253: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %254 .str254: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %255 .str255: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %256 .str256: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %257 .str257: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %258 .str258: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %259 .str259: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %260 .str260: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %261 .str261: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %262 .str262: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %263 .str263: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %264 .str264: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %265 .str265: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %266 .str266: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %267 .str267: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %268 .str268: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %269 .str269: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %270 .str270: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %271 .str271: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %272 .str272: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %273 .str273: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %274 .str274: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %275 .str275: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %276 .str276: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %277 .str277: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %278 .str278: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %279 .str279: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %280 .str280: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %281 .str281: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %282 .str282: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %283 .str283: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %284 .str284: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %285 .str285: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %286 .str286: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %287 .str287: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %288 .str288: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %289 .str289: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %290 .str290: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %291 .str291: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %292 .str292: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %293 .str293: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %294 .str294: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %295 .str295: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %296 .str296: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %297 .str297: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %298 .str298: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %299 .str299: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %300 .str300: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %301 .str301: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %302 .str302: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %303 .str303: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %304 .str304: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %305 .str305: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %306 .str306: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %12 @memcpy(%67 <unnamed>: ptr<void>, %68 <unnamed>: ptr<const void>, %69 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %13 @mempcpy(%70 <unnamed>: ptr<void>, %71 <unnamed>: ptr<const void>, %72 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %14 @memset(%73 <unnamed>: ptr<void>, %74 <unnamed>: i32, %75 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %15 @bzero(%76 <unnamed>: ptr<void>, %77 <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %16 @strcat(%78 <unnamed>: ptr<i8>, %79 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %17 @strncat(%80 <unnamed>: ptr<i8>, %81 <unnamed>: ptr<const i8>, %82 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %18 @strcpy(%83 <unnamed>: ptr<i8>, %84 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %19 @strncpy(%85 <unnamed>: ptr<i8>, %86 <unnamed>: ptr<const i8>, %87 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %20 @sink(%88 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %21 @random_unsigned_value() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %22 @random_signed_value() -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %23 @unsigned_range(%24 min: u64, %25 max: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %26 val: u64 [storage=automatic] [const] = call<u64, signature=fn() -> u64>(%21);
// DEFAULT-NEXT:         return conditional<u64>(logical_or<bool>(lt<u64>(read<u64>(%26), read<u64>(%24)), lt<u64>(read<u64>(%25), read<u64>(%26))), read<u64>(%24), read<u64>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @signed_range(%28 min: i64, %29 max: i64) -> i64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %30 val: i64 [storage=automatic] [const] = call<i64, signature=fn() -> i64>(%22);
// DEFAULT-NEXT:         return conditional<i64>(logical_or<bool>(lt<i64>(read<i64>(%30), read<i64>(%28)), lt<i64>(read<i64>(%29), read<i64>(%30))), read<i64>(%28), read<i64>(%30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @test_memcpy_range(%32 d: ptr<void>, %33 s: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %34 buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%34), const<i32>(5))), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%34), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%34), read<u64>(%3))), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%34), read<u64>(%3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, read<u64>(%2), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, sub<u64, overflow=wrap>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, read<u64>(%2), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), call<u64, signature=fn(u64, u64) -> u64>(%23, add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(3))), widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(5))), widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(7))), widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(9))), widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(11))), widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(3))), widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(5))), widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(7))), widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(9))), widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(11))), widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(2))), widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%27, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(2))), widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i8>(%4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i16>(%5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%7)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), widen<u64, reason=arg>(read<u8>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), widen<u64, reason=arg>(read<u16>(%9)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), widen<u64, reason=arg>(read<u32>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), read<u64>(%11));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%4)), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%5)), const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%6), const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(add<i64, overflow=ub>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%34)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8))), const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%9))), const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), widen<u64, reason=arg>(add<u32, overflow=wrap>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), add<u64, overflow=wrap>(read<u64>(%11), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i8>(%4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i16>(%5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%7)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), widen<u64, reason=arg>(read<u8>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), widen<u64, reason=arg>(read<u16>(%9)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), widen<u64, reason=arg>(read<u32>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), read<u64>(%11));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%4)), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%5)), const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%6), const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(add<i64, overflow=ub>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8))), const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%9))), const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), widen<u64, reason=arg>(add<u32, overflow=wrap>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, read<ptr<void>>(%32), read<ptr<const void>>(%33), add<u64, overflow=wrap>(read<u64>(%11), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @test_mempcpy_range(%36 d: ptr<void>, %37 s: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %38 buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, read<u64>(%2), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, sub<u64, overflow=wrap>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%38)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, read<ptr<void>>(%36), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, read<u64>(%2), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%36));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%13, read<ptr<void>>(%36), read<ptr<const void>>(%37), call<u64, signature=fn(u64, u64) -> u64>(%23, add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%36));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @test_memset_range(%40 d: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %41 buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, read<u64>(%2), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, sub<u64, overflow=wrap>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%41)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, read<ptr<void>>(%40), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, read<u64>(%2), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%40));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, read<ptr<void>>(%40), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%23, add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @test_bzero_range(%43 d: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %44 buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)), call<u64, signature=fn(u64, u64) -> u64>(%23, read<u64>(%2), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)), call<u64, signature=fn(u64, u64) -> u64>(%23, add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)), call<u64, signature=fn(u64, u64) -> u64>(%23, sub<u64, overflow=wrap>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, read<ptr<void>>(%43), call<u64, signature=fn(u64, u64) -> u64>(%23, read<u64>(%2), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%43));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%15, read<ptr<void>>(%43), call<u64, signature=fn(u64, u64) -> u64>(%23, add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, read<ptr<void>>(%43));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @test_strcat_range() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %46 buf: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%16, array_decay<ptr<i8>, length=Some(5)>(%46), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%89), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%90))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%46)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%16, array_decay<ptr<i8>, length=Some(5)>(%46), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%91), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%92))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%46)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%16, array_decay<ptr<i8>, length=Some(5)>(%46), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%93), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%94))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%46)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%16, array_decay<ptr<i8>, length=Some(5)>(%46), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%95), const<i32>(10)), const<i32>(3)), array_decay<ptr<i8>, length=Some(11)>(%96))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%46)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%16, array_decay<ptr<i8>, length=Some(5)>(%46), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%97), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%98))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%46)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%16, array_decay<ptr<i8>, length=Some(5)>(%46), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%99), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%100))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%46)));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %47 buf2: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([49, 50, 0, 0, 0]);
// DEFAULT-NEXT:             call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%16, array_decay<ptr<i8>, length=Some(5)>(%47), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%101), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%102))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%47)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @test_strcpy(%49 src: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %51 a: @type2 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(2)>(field0(%51)), read<ptr<const i8>>(%49));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(field0(%51))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%51)), const<i32>(1)), read<ptr<const i8>>(%49));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%51)), const<i32>(1))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%51)), const<i32>(2)), read<ptr<const i8>>(%49));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%51)), const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%51)), const<i32>(5)), read<ptr<const i8>>(%49));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%51)), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%51)), const<i32>(17)), read<ptr<const i8>>(%49));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%51)), const<i32>(17))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @test_strcpy_range() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %53 buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(5)>(%53), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%103), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%104))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%53)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(5)>(%53), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%105), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%106))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%53)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(5)>(%53), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%107), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%108))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%53)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(5)>(%53), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%109), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%110))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%53)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(5)>(%53), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%111), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%112))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%53)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(5)>(%53), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%113), const<i32>(10)), const<i32>(6)), array_decay<ptr<i8>, length=Some(11)>(%114))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%53)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(5)>(%53), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%115), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%116))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%53)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(5)>(%53), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%117), const<i32>(10)), const<i32>(8)), array_decay<ptr<i8>, length=Some(11)>(%118))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%53)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(5)>(%53), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%119), const<i32>(10)), const<i32>(9)), array_decay<ptr<i8>, length=Some(11)>(%120))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%53)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, array_decay<ptr<i8>, length=Some(5)>(%53), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%121), const<i32>(10)), const<i32>(10)), array_decay<ptr<i8>, length=Some(11)>(%122))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%53)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%53), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%123), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%124))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%53), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%18, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%53), const<i32>(17)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%125), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%126))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%53), const<i32>(17))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @test_strncat_range() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %55 buf: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%127), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%128))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%129), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%130))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%131), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%132))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%133), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%134))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%135), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%136))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%55), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%137), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%138))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%55), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%55), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%139), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%140))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%55), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%55), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%141), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%142))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%55), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%143), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%144))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%145), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%146))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%147), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%148))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%149), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%150))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%151), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%152))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%153), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%154))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%155), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%156))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%157), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%158))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%159), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%160))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%161), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%162))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%163), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%164))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%55), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%165), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%166))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%55)));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %56 buf2: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([49, 50, 0, 0, 0]);
// DEFAULT-NEXT:             call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%17, array_decay<ptr<i8>, length=Some(5)>(%56), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%167), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%168))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%56)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %173 @__builtin___strncat_chk(%169 <unnamed>: ptr<i8>, %170 <unnamed>: ptr<const i8>, %171 <unnamed>: u64, %172 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %178 @__builtin_object_size(%176 <unnamed>: ptr<const void>, %177 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %57 @test_strncat_chk_range(%58 d: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %59 buf: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%174), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%175))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%179), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%180))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%181), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%182))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%183), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%184))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%185), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%186))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%187), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%188))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%189), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%190))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%191), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%192))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%193), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%194))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%195), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%196))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, array_decay<ptr<i8>, length=Some(5)>(%59), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%197), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%198))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%59)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%173, read<ptr<i8>>(%58), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%199), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%200))), read<u64>(%3), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%178, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%58)), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @test_strncpy_string_range(%61 d: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %62 buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%201), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%202))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%203), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%204))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%205), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%206))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%207), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%208))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%209), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%210))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%211), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%212))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%213), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%214))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%215), const<i32>(10)), const<i32>(6)), array_decay<ptr<i8>, length=Some(11)>(%216))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%217), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%218))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%219), const<i32>(10)), const<i32>(8)), array_decay<ptr<i8>, length=Some(11)>(%220))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%221), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%222))), sub<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%223), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%224))), read<u64>(%2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%225), const<i32>(10)), const<i32>(3)), array_decay<ptr<i8>, length=Some(11)>(%226))), add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%62), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%227), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%228))), read<u64>(%3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%62)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, read<ptr<i8>>(%61), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%229), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%230))), sub<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%61)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, read<ptr<i8>>(%61), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%231), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%232))), read<u64>(%2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%61)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, read<ptr<i8>>(%61), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%233), const<i32>(10)), const<i32>(3)), array_decay<ptr<i8>, length=Some(11)>(%234))), add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%61)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, read<ptr<i8>>(%61), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%235), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%236))), read<u64>(%3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%61)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @test_strncpy_string_count_range(%64 dst: ptr<i8>, %65 src: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %66 buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%237), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%238))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%239), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%240))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%241), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%242))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%243), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%244))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%245), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%246))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%247), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%248))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%249), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%250))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%251), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%252))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%253), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%254))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%255), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%256))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%257), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%258))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%259), const<i32>(10)), const<i32>(9)), array_decay<ptr<i8>, length=Some(11)>(%260))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%261), const<i32>(10)), const<i32>(8)), array_decay<ptr<i8>, length=Some(11)>(%262))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%263), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%264))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%265), const<i32>(10)), const<i32>(6)), array_decay<ptr<i8>, length=Some(11)>(%266))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%267), const<i32>(10)), const<i32>(8)), array_decay<ptr<i8>, length=Some(11)>(%268))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%269), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%270))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%271), const<i32>(10)), const<i32>(6)), array_decay<ptr<i8>, length=Some(11)>(%272))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%273), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%274))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%275), const<i32>(10)), const<i32>(9)), array_decay<ptr<i8>, length=Some(11)>(%276))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%277), const<i32>(10)), const<i32>(8)), array_decay<ptr<i8>, length=Some(11)>(%278))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%279), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%280))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%281), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%282))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%283), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%284))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%285), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%286))), call<u64, signature=fn(u64, u64) -> u64>(%23, read<u64>(%2), add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%287), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%288))), call<u64, signature=fn(u64, u64) -> u64>(%23, add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%66), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%289), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%290))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%66), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%66), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%291), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%292))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%66), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%66), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%293), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%294))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%66), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%66), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%295), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%296))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%66), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, array_decay<ptr<i8>, length=Some(5)>(%66), read<ptr<const i8>>(%65), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%66)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, read<ptr<i8>>(%64), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%297), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%298))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%64)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, read<ptr<i8>>(%64), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%299), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%300))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%64)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, read<ptr<i8>>(%64), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%301), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%302))), call<u64, signature=fn(u64, u64) -> u64>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%64)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, read<ptr<i8>>(%64), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%303), const<i32>(10)), const<i32>(3)), array_decay<ptr<i8>, length=Some(11)>(%304))), call<u64, signature=fn(u64, u64) -> u64>(%23, read<u64>(%2), add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%64)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%19, read<ptr<i8>>(%64), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%21)), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%305), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%306))), call<u64, signature=fn(u64, u64) -> u64>(%23, add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%20, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%64)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
