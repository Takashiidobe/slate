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
// DEFAULT-NEXT:     type @type[[TYPE_ptrdiff_t:[0-9]+]] ptrdiff_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 2>;
// DEFAULT-NEXT:         field1 b: array<i8, 3>;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 2]];
// DEFAULT-NEXT:     global %[[VALUE_ssize_max:[0-9]+]] ssize_max: u64 [storage=static] [const] = div<u64, by_zero=ub>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_size_max:[0-9]+]] size_max: u64 [storage=static] [const] = const<u64>(18446744073709551615) [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE_schar_val:[0-9]+]] schar_val: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_sshrt_val:[0-9]+]] sshrt_val: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_sint_val:[0-9]+]] sint_val: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_slong_val:[0-9]+]] slong_val: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_uchar_val:[0-9]+]] uchar_val: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_ushrt_val:[0-9]+]] ushrt_val: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_uint_val:[0-9]+]] uint_val: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_ulong_val:[0-9]+]] ulong_val: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_35:[0-9]+]] .str[[VALUE_str_35]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_36:[0-9]+]] .str[[VALUE_str_36]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_37:[0-9]+]] .str[[VALUE_str_37]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_38:[0-9]+]] .str[[VALUE_str_38]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_39:[0-9]+]] .str[[VALUE_str_39]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_40:[0-9]+]] .str[[VALUE_str_40]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_41:[0-9]+]] .str[[VALUE_str_41]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_42:[0-9]+]] .str[[VALUE_str_42]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_43:[0-9]+]] .str[[VALUE_str_43]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_44:[0-9]+]] .str[[VALUE_str_44]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_45:[0-9]+]] .str[[VALUE_str_45]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_46:[0-9]+]] .str[[VALUE_str_46]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_47:[0-9]+]] .str[[VALUE_str_47]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_48:[0-9]+]] .str[[VALUE_str_48]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_49:[0-9]+]] .str[[VALUE_str_49]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_50:[0-9]+]] .str[[VALUE_str_50]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_51:[0-9]+]] .str[[VALUE_str_51]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_52:[0-9]+]] .str[[VALUE_str_52]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_53:[0-9]+]] .str[[VALUE_str_53]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_54:[0-9]+]] .str[[VALUE_str_54]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_55:[0-9]+]] .str[[VALUE_str_55]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_56:[0-9]+]] .str[[VALUE_str_56]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_57:[0-9]+]] .str[[VALUE_str_57]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_58:[0-9]+]] .str[[VALUE_str_58]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_59:[0-9]+]] .str[[VALUE_str_59]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_60:[0-9]+]] .str[[VALUE_str_60]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_61:[0-9]+]] .str[[VALUE_str_61]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_62:[0-9]+]] .str[[VALUE_str_62]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_63:[0-9]+]] .str[[VALUE_str_63]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_64:[0-9]+]] .str[[VALUE_str_64]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_65:[0-9]+]] .str[[VALUE_str_65]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_66:[0-9]+]] .str[[VALUE_str_66]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_67:[0-9]+]] .str[[VALUE_str_67]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_68:[0-9]+]] .str[[VALUE_str_68]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_69:[0-9]+]] .str[[VALUE_str_69]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_70:[0-9]+]] .str[[VALUE_str_70]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_71:[0-9]+]] .str[[VALUE_str_71]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_72:[0-9]+]] .str[[VALUE_str_72]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_73:[0-9]+]] .str[[VALUE_str_73]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_74:[0-9]+]] .str[[VALUE_str_74]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_75:[0-9]+]] .str[[VALUE_str_75]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_76:[0-9]+]] .str[[VALUE_str_76]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_77:[0-9]+]] .str[[VALUE_str_77]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_78:[0-9]+]] .str[[VALUE_str_78]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_79:[0-9]+]] .str[[VALUE_str_79]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_80:[0-9]+]] .str[[VALUE_str_80]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_81:[0-9]+]] .str[[VALUE_str_81]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_82:[0-9]+]] .str[[VALUE_str_82]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_83:[0-9]+]] .str[[VALUE_str_83]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_84:[0-9]+]] .str[[VALUE_str_84]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_85:[0-9]+]] .str[[VALUE_str_85]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_86:[0-9]+]] .str[[VALUE_str_86]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_87:[0-9]+]] .str[[VALUE_str_87]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_88:[0-9]+]] .str[[VALUE_str_88]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_89:[0-9]+]] .str[[VALUE_str_89]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_90:[0-9]+]] .str[[VALUE_str_90]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_91:[0-9]+]] .str[[VALUE_str_91]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_92:[0-9]+]] .str[[VALUE_str_92]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_93:[0-9]+]] .str[[VALUE_str_93]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_94:[0-9]+]] .str[[VALUE_str_94]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_95:[0-9]+]] .str[[VALUE_str_95]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_96:[0-9]+]] .str[[VALUE_str_96]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_97:[0-9]+]] .str[[VALUE_str_97]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_98:[0-9]+]] .str[[VALUE_str_98]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_99:[0-9]+]] .str[[VALUE_str_99]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_100:[0-9]+]] .str[[VALUE_str_100]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_101:[0-9]+]] .str[[VALUE_str_101]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_102:[0-9]+]] .str[[VALUE_str_102]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_103:[0-9]+]] .str[[VALUE_str_103]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_104:[0-9]+]] .str[[VALUE_str_104]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_105:[0-9]+]] .str[[VALUE_str_105]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_106:[0-9]+]] .str[[VALUE_str_106]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_107:[0-9]+]] .str[[VALUE_str_107]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_108:[0-9]+]] .str[[VALUE_str_108]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_109:[0-9]+]] .str[[VALUE_str_109]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_110:[0-9]+]] .str[[VALUE_str_110]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_111:[0-9]+]] .str[[VALUE_str_111]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_112:[0-9]+]] .str[[VALUE_str_112]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_113:[0-9]+]] .str[[VALUE_str_113]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_114:[0-9]+]] .str[[VALUE_str_114]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_115:[0-9]+]] .str[[VALUE_str_115]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_116:[0-9]+]] .str[[VALUE_str_116]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_117:[0-9]+]] .str[[VALUE_str_117]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_118:[0-9]+]] .str[[VALUE_str_118]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_119:[0-9]+]] .str[[VALUE_str_119]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_120:[0-9]+]] .str[[VALUE_str_120]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_121:[0-9]+]] .str[[VALUE_str_121]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_122:[0-9]+]] .str[[VALUE_str_122]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_123:[0-9]+]] .str[[VALUE_str_123]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_124:[0-9]+]] .str[[VALUE_str_124]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_125:[0-9]+]] .str[[VALUE_str_125]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_126:[0-9]+]] .str[[VALUE_str_126]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_127:[0-9]+]] .str[[VALUE_str_127]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_128:[0-9]+]] .str[[VALUE_str_128]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_129:[0-9]+]] .str[[VALUE_str_129]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_130:[0-9]+]] .str[[VALUE_str_130]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_131:[0-9]+]] .str[[VALUE_str_131]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_132:[0-9]+]] .str[[VALUE_str_132]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_133:[0-9]+]] .str[[VALUE_str_133]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_134:[0-9]+]] .str[[VALUE_str_134]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_135:[0-9]+]] .str[[VALUE_str_135]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_136:[0-9]+]] .str[[VALUE_str_136]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_137:[0-9]+]] .str[[VALUE_str_137]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_138:[0-9]+]] .str[[VALUE_str_138]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_139:[0-9]+]] .str[[VALUE_str_139]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_140:[0-9]+]] .str[[VALUE_str_140]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_141:[0-9]+]] .str[[VALUE_str_141]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_142:[0-9]+]] .str[[VALUE_str_142]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_143:[0-9]+]] .str[[VALUE_str_143]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_144:[0-9]+]] .str[[VALUE_str_144]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_145:[0-9]+]] .str[[VALUE_str_145]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_146:[0-9]+]] .str[[VALUE_str_146]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_147:[0-9]+]] .str[[VALUE_str_147]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_148:[0-9]+]] .str[[VALUE_str_148]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_149:[0-9]+]] .str[[VALUE_str_149]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_150:[0-9]+]] .str[[VALUE_str_150]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_151:[0-9]+]] .str[[VALUE_str_151]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_152:[0-9]+]] .str[[VALUE_str_152]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_153:[0-9]+]] .str[[VALUE_str_153]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_154:[0-9]+]] .str[[VALUE_str_154]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_155:[0-9]+]] .str[[VALUE_str_155]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_156:[0-9]+]] .str[[VALUE_str_156]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_157:[0-9]+]] .str[[VALUE_str_157]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_158:[0-9]+]] .str[[VALUE_str_158]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_159:[0-9]+]] .str[[VALUE_str_159]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_160:[0-9]+]] .str[[VALUE_str_160]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_161:[0-9]+]] .str[[VALUE_str_161]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_162:[0-9]+]] .str[[VALUE_str_162]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_163:[0-9]+]] .str[[VALUE_str_163]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_164:[0-9]+]] .str[[VALUE_str_164]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_165:[0-9]+]] .str[[VALUE_str_165]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_166:[0-9]+]] .str[[VALUE_str_166]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_167:[0-9]+]] .str[[VALUE_str_167]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_168:[0-9]+]] .str[[VALUE_str_168]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_169:[0-9]+]] .str[[VALUE_str_169]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_170:[0-9]+]] .str[[VALUE_str_170]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_171:[0-9]+]] .str[[VALUE_str_171]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_172:[0-9]+]] .str[[VALUE_str_172]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_173:[0-9]+]] .str[[VALUE_str_173]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_174:[0-9]+]] .str[[VALUE_str_174]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_175:[0-9]+]] .str[[VALUE_str_175]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_176:[0-9]+]] .str[[VALUE_str_176]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_177:[0-9]+]] .str[[VALUE_str_177]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_178:[0-9]+]] .str[[VALUE_str_178]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_179:[0-9]+]] .str[[VALUE_str_179]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_180:[0-9]+]] .str[[VALUE_str_180]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_181:[0-9]+]] .str[[VALUE_str_181]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_182:[0-9]+]] .str[[VALUE_str_182]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_183:[0-9]+]] .str[[VALUE_str_183]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_184:[0-9]+]] .str[[VALUE_str_184]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_185:[0-9]+]] .str[[VALUE_str_185]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_186:[0-9]+]] .str[[VALUE_str_186]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_187:[0-9]+]] .str[[VALUE_str_187]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_188:[0-9]+]] .str[[VALUE_str_188]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_189:[0-9]+]] .str[[VALUE_str_189]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_190:[0-9]+]] .str[[VALUE_str_190]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_191:[0-9]+]] .str[[VALUE_str_191]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_192:[0-9]+]] .str[[VALUE_str_192]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_193:[0-9]+]] .str[[VALUE_str_193]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_194:[0-9]+]] .str[[VALUE_str_194]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_195:[0-9]+]] .str[[VALUE_str_195]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_196:[0-9]+]] .str[[VALUE_str_196]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_197:[0-9]+]] .str[[VALUE_str_197]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_198:[0-9]+]] .str[[VALUE_str_198]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_199:[0-9]+]] .str[[VALUE_str_199]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_200:[0-9]+]] .str[[VALUE_str_200]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_201:[0-9]+]] .str[[VALUE_str_201]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_202:[0-9]+]] .str[[VALUE_str_202]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_203:[0-9]+]] .str[[VALUE_str_203]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_204:[0-9]+]] .str[[VALUE_str_204]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_205:[0-9]+]] .str[[VALUE_str_205]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_206:[0-9]+]] .str[[VALUE_str_206]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_207:[0-9]+]] .str[[VALUE_str_207]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_208:[0-9]+]] .str[[VALUE_str_208]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_209:[0-9]+]] .str[[VALUE_str_209]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_210:[0-9]+]] .str[[VALUE_str_210]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mempcpy:[0-9]+]] @mempcpy(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE6:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE7:[0-9]+]] <unnamed>: i32, %[[VALUE8:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bzero:[0-9]+]] @bzero(%[[VALUE9:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE10:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcat:[0-9]+]] @strcat(%[[VALUE11:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE12:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncat:[0-9]+]] @strncat(%[[VALUE13:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE14:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE15:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcpy:[0-9]+]] @strcpy(%[[VALUE16:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE17:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncpy:[0-9]+]] @strncpy(%[[VALUE18:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE19:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE20:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE21:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_random_unsigned_value:[0-9]+]] @random_unsigned_value() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_random_signed_value:[0-9]+]] @random_signed_value() -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_unsigned_range:[0-9]+]] @unsigned_range(%[[VALUE_min:[0-9]+]] min: u64, %[[VALUE_max:[0-9]+]] max: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val:[0-9]+]] val: u64 [storage=automatic] [const] = call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]]);
// DEFAULT-NEXT:         return conditional<u64>(logical_or<bool>(lt<u64>(read<u64>(%[[VALUE_val]]), read<u64>(%[[VALUE_min]])), lt<u64>(read<u64>(%[[VALUE_max]]), read<u64>(%[[VALUE_val]]))), read<u64>(%[[VALUE_min]]), read<u64>(%[[VALUE_val]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_signed_range:[0-9]+]] @signed_range(%[[VALUE_min_2:[0-9]+]] min: i64, %[[VALUE_max_2:[0-9]+]] max: i64) -> i64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val_2:[0-9]+]] val: i64 [storage=automatic] [const] = call<i64, signature=fn() -> i64>(%[[VALUE_random_signed_value]]);
// DEFAULT-NEXT:         return conditional<i64>(logical_or<bool>(lt<i64>(read<i64>(%[[VALUE_val_2]]), read<i64>(%[[VALUE_min_2]])), lt<i64>(read<i64>(%[[VALUE_max_2]]), read<i64>(%[[VALUE_val_2]]))), read<i64>(%[[VALUE_min_2]]), read<i64>(%[[VALUE_val_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_memcpy_range:[0-9]+]] @test_memcpy_range(%[[VALUE_d:[0-9]+]] d: ptr<void>, %[[VALUE_s:[0-9]+]] s: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]]), const<i32>(5))), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]]), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]]), read<u64>(%[[VALUE_size_max]]))), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]]), read<u64>(%[[VALUE_size_max]]))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], read<u64>(%[[VALUE_ssize_max]]), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_size_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], read<u64>(%[[VALUE_ssize_max]]), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(3))), widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(5))), widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(7))), widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(9))), widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(11))), widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(3))), widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(5))), widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(7))), widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(9))), widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(11))), widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(2))), widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(2))), widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i8>(%[[VALUE_schar_val]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i16>(%[[VALUE_sshrt_val]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_sint_val]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_slong_val]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), widen<u64, reason=arg>(read<u8>(%[[VALUE_uchar_val]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), widen<u64, reason=arg>(read<u16>(%[[VALUE_ushrt_val]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), widen<u64, reason=arg>(read<u32>(%[[VALUE_uint_val]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), read<u64>(%[[VALUE_ulong_val]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_schar_val]])), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_sshrt_val]])), const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%[[VALUE_sint_val]]), const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(add<i64, overflow=ub>(read<i64>(%[[VALUE_slong_val]]), widen<i64, reason=usual_arith>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_uchar_val]]))), const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_ushrt_val]]))), const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), widen<u64, reason=arg>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_uint_val]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), add<u64, overflow=wrap>(read<u64>(%[[VALUE_ulong_val]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i8>(%[[VALUE_schar_val]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i16>(%[[VALUE_sshrt_val]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_sint_val]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_slong_val]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), widen<u64, reason=arg>(read<u8>(%[[VALUE_uchar_val]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), widen<u64, reason=arg>(read<u16>(%[[VALUE_ushrt_val]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), widen<u64, reason=arg>(read<u32>(%[[VALUE_uint_val]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), read<u64>(%[[VALUE_ulong_val]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_schar_val]])), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_sshrt_val]])), const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%[[VALUE_sint_val]]), const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(add<i64, overflow=ub>(read<i64>(%[[VALUE_slong_val]]), widen<i64, reason=usual_arith>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_uchar_val]]))), const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_ushrt_val]]))), const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), widen<u64, reason=arg>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_uint_val]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s]]), add<u64, overflow=wrap>(read<u64>(%[[VALUE_ulong_val]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_mempcpy_range:[0-9]+]] @test_mempcpy_range(%[[VALUE_d_2:[0-9]+]] d: ptr<void>, %[[VALUE_s_2:[0-9]+]] s: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_2:[0-9]+]] buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], read<u64>(%[[VALUE_ssize_max]]), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_size_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], read<ptr<void>>(%[[VALUE_d_2]]), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], read<u64>(%[[VALUE_ssize_max]]), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], read<ptr<void>>(%[[VALUE_d_2]]), read<ptr<const void>>(%[[VALUE_s_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_memset_range:[0-9]+]] @test_memset_range(%[[VALUE_d_3:[0-9]+]] d: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_3:[0-9]+]] buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], read<u64>(%[[VALUE_ssize_max]]), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_size_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_3]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], read<ptr<void>>(%[[VALUE_d_3]]), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], read<u64>(%[[VALUE_ssize_max]]), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], read<ptr<void>>(%[[VALUE_d_3]]), const<i32>(0), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_bzero_range:[0-9]+]] @test_bzero_range(%[[VALUE_d_4:[0-9]+]] d: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_4:[0-9]+]] buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], read<u64>(%[[VALUE_ssize_max]]), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_size_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], read<ptr<void>>(%[[VALUE_d_4]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], read<u64>(%[[VALUE_ssize_max]]), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d_4]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_bzero]], read<ptr<void>>(%[[VALUE_d_4]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_size_max]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_d_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strcat_range:[0-9]+]] @test_strcat_range() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_5:[0-9]+]] buf: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_2]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_3]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_4]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_5]]), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_6]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_7]]), const<i32>(10)), const<i32>(3)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_8]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_9]]), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_10]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_11]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_12]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_5]])));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_buf2:[0-9]+]] buf2: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([49, 50, 0, 0, 0]);
// DEFAULT-NEXT:             call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf2]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_13]]), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_14]]))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf2]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strcpy:[0-9]+]] @test_strcpy(%[[VALUE_src:[0-9]+]] src: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a]])), read<ptr<const i8>>(%[[VALUE_src]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a]]))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a]])), const<i32>(1)), read<ptr<const i8>>(%[[VALUE_src]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a]])), const<i32>(1))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a]])), const<i32>(2)), read<ptr<const i8>>(%[[VALUE_src]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a]])), const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a]])), const<i32>(5)), read<ptr<const i8>>(%[[VALUE_src]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a]])), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a]])), const<i32>(17)), read<ptr<const i8>>(%[[VALUE_src]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a]])), const<i32>(17))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strcpy_range:[0-9]+]] @test_strcpy_range() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_6:[0-9]+]] buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_15]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_16]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_17]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_18]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_19]]), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_20]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_21]]), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_22]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_23]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_24]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_25]]), const<i32>(10)), const<i32>(6)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_26]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_27]]), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_28]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_29]]), const<i32>(10)), const<i32>(8)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_30]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_31]]), const<i32>(10)), const<i32>(9)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_32]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_33]]), const<i32>(10)), const<i32>(10)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_34]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_35]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_36]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), const<i32>(17)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_37]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_38]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_6]]), const<i32>(17))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncat_range:[0-9]+]] @test_strncat_range() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_7:[0-9]+]] buf: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_39]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_40]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_41]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_42]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_43]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_44]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_45]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_46]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_47]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_48]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_49]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_50]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_51]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_52]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_53]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_54]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_55]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_56]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_57]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_58]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_59]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_60]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_61]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_62]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_63]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_64]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_65]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_66]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_67]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_68]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_69]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_70]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_71]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_72]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_73]]), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_74]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_75]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_76]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_77]]), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_78]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_7]])));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_buf2_2:[0-9]+]] buf2: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([49, 50, 0, 0, 0]);
// DEFAULT-NEXT:             call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf2_2]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_79]]), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_80]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf2_2]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin___strncat_chk:[0-9]+]] @__builtin___strncat_chk(%[[VALUE22:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE23:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE24:[0-9]+]] <unnamed>: u64, %[[VALUE25:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_object_size:[0-9]+]] @__builtin_object_size(%[[VALUE26:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE27:[0-9]+]] <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_strncat_chk_range:[0-9]+]] @test_strncat_chk_range(%[[VALUE_d_5:[0-9]+]] d: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_8:[0-9]+]] buf: array<i8, 5> [storage=automatic] = code_units<array<i8, 5>>([0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_81]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_82]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_83]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_84]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_85]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_86]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_87]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_88]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_89]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_90]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_91]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_92]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_93]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_94]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_95]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_96]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_97]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_98]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_99]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_100]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_101]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_102]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_8]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncat_chk]], read<ptr<i8>>(%[[VALUE_d_5]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_103]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_104]]))), read<u64>(%[[VALUE_size_max]]), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_d_5]])), const<i32>(1)));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_string_range:[0-9]+]] @test_strncpy_string_range(%[[VALUE_d_6:[0-9]+]] d: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_9:[0-9]+]] buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_105]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_106]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_107]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_108]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_109]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_110]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_111]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_112]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_113]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_114]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_115]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_116]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_117]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_118]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_119]]), const<i32>(10)), const<i32>(6)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_120]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_121]]), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_122]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_123]]), const<i32>(10)), const<i32>(8)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_124]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_125]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_126]]))), sub<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_127]]), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_128]]))), read<u64>(%[[VALUE_ssize_max]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_129]]), const<i32>(10)), const<i32>(3)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_130]]))), add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_131]]), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_132]]))), read<u64>(%[[VALUE_size_max]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_9]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], read<ptr<i8>>(%[[VALUE_d_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_133]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_134]]))), sub<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_d_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], read<ptr<i8>>(%[[VALUE_d_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_135]]), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_136]]))), read<u64>(%[[VALUE_ssize_max]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_d_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], read<ptr<i8>>(%[[VALUE_d_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_137]]), const<i32>(10)), const<i32>(3)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_138]]))), add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_d_6]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], read<ptr<i8>>(%[[VALUE_d_6]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_139]]), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_140]]))), read<u64>(%[[VALUE_size_max]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_d_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_string_count_range:[0-9]+]] @test_strncpy_string_count_range(%[[VALUE_dst:[0-9]+]] dst: ptr<i8>, %[[VALUE_src_2:[0-9]+]] src: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_10:[0-9]+]] buf: array<i8, 5> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_141]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_142]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_143]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_144]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_145]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_146]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_147]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_148]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_149]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_150]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_151]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_152]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_153]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_154]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_155]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_156]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_157]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_158]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_159]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_160]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_161]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_162]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_163]]), const<i32>(10)), const<i32>(9)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_164]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_165]]), const<i32>(10)), const<i32>(8)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_166]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_167]]), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_168]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_169]]), const<i32>(10)), const<i32>(6)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_170]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_171]]), const<i32>(10)), const<i32>(8)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_172]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_173]]), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_174]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_175]]), const<i32>(10)), const<i32>(6)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_176]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_177]]), const<i32>(10)), const<i32>(5)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_178]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_179]]), const<i32>(10)), const<i32>(9)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_180]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_181]]), const<i32>(10)), const<i32>(8)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_182]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_183]]), const<i32>(10)), const<i32>(7)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_184]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_185]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_186]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_187]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_188]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_189]]), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_190]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], read<u64>(%[[VALUE_ssize_max]]), add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(5)>(%[[VALUE_buf_10]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith,
// DEFAULT-SAME: fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>,
// DEFAULT-SAME: subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_191]]),
// DEFAULT-SAME: const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_192]]))), call<u64, signature=fn(u64, u64) ->
// DEFAULT-SAME: u64>(%[[VALUE_unsigned_range]], add<u64,
// DEFAULT-SAME: overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64,
// DEFAULT-SAME: overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_193]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_194]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_195]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_196]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_197]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_198]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), const<i32>(5)), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_199]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_200]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]]), read<ptr<const i8>>(%[[VALUE_src_2]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_buf_10]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], read<ptr<i8>>(%[[VALUE_dst]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_201]]), const<i32>(10)), const<i32>(0)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_202]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_dst]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], read<ptr<i8>>(%[[VALUE_dst]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_203]]), const<i32>(10)), const<i32>(1)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_204]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_dst]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], read<ptr<i8>>(%[[VALUE_dst]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_205]]), const<i32>(10)), const<i32>(2)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_206]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_dst]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], read<ptr<i8>>(%[[VALUE_dst]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_207]]), const<i32>(10)), const<i32>(3)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_208]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], read<u64>(%[[VALUE_ssize_max]]), add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_dst]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], read<ptr<i8>>(%[[VALUE_dst]]), pointer_cast<ptr<const i8>, reason=arg>(conditional<ptr<i8>>(eq<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]])), ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_209]]), const<i32>(10)), const<i32>(4)), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_210]]))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64, overflow=wrap>(read<u64>(%[[VALUE_ssize_max]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_dst]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
