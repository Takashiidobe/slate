/* Test exercising -Wstringop-overread warnings for reading past the end.  */
/* { dg-do compile } */
/* { dg-options "-O2 -Wstringop-overread -ftrack-macro-expansion=0" } */

#define PTRDIFF_MAX   __PTRDIFF_MAX__
#define SIZE_MAX      __SIZE_MAX__

#define offsetof(type, mem)   __builtin_offsetof (type, mem)

/* Return the number of bytes from member MEM of TYPE to the end
   of object OBJ.  */
#define offsetfrom(type, obj, mem) (sizeof (obj) - offsetof (type, mem))


typedef __SIZE_TYPE__ size_t;
extern void* memchr (const void*, int, size_t);
extern int memcmp (const void*, const void*, size_t);
extern void* memcpy (void*, const void*, size_t);
extern void* memmove (void*, const void*, size_t);
extern void* mempcpy (void*, const void*, size_t);

#define memchr(d, s, n) sink (memchr (d, s, n))
#define memcmp(d, s, n) sink (d, memcmp (d, s, n))
#define memcpy(d, s, n) sink (memcpy (d, s, n))
#define memmove(d, s, n) sink (memmove (d, s, n))
#define mempcpy(d, s, n) sink (mempcpy (d, s, n))

struct A { char a, b; };
struct B { struct A a; char c, d; };

/* Function to call to "escape" pointers from tests below to prevent
   GCC from assuming the values of the objects they point to stay
   the unchanged.  */
void sink (void*, ...);

/* Function to "generate" a random number each time it's called.  Declared
   (but not defined) and used to prevent GCC from making assumptions about
   their values based on the variables uses in the tested expressions.  */
size_t random_unsigned_value (void);

/* Return a random unsigned value between MIN and MAX.  */

static inline size_t
range (size_t min, size_t max)
{
  const size_t val = random_unsigned_value ();
  return val < min || max < val ? min : val;
}

#define R(min, max)   range (min, max)

/* Verify that reading beyond the end of a local array is diagnosed.  */

void test_memop_warn_local (void *p, const void *q)
{
  memcpy (p, "1234", R (6, 7));   /* { dg-warning "reading between 6 and 7 bytes from a region of size 5" } */

  struct A a[2];

  memcpy (p, a, R (7, 8));   /* { dg-warning "reading between 7 and 8 bytes from a region of size 4" } */

  /* At -Wstringop-overflow=1 the destination is considered to be
     the whole array and its size is therefore sizeof a.  */
  memcpy (p, &a[0], R (8, 9));   /* { dg-warning "reading between 8 and 9 bytes from a region of size 4" } */

  /* Verify the same as above but by reading from the first mmeber
     of the first element of the array.  */
  memcpy (p, &a[0].a, R (8, 9));   /* { dg-warning "reading between 8 and 9 bytes from a region of size 4" } */

  struct B b[2];

  memcpy (p, &b[0], R (12, 32));   /* { dg-warning "reading between 12 and 32 bytes from a region of size 8" } */

  /* Verify memchr/memcmp.  */
  int i = R (0, 255);
  memchr ("", i, 2);   /* { dg-warning "specified bound 2 exceeds source size 1" "memchr" } */
  memchr ("", i, 2);   /* { dg-warning "specified bound 2 exceeds source size 1" "memchr" } */
  memchr ("123", i, 5);   /* { dg-warning "specified bound 5 exceeds source size 4" "memchr" } */
  memchr (a, i, sizeof a + 1);   /* { dg-warning "specified bound 5 exceeds source size 4" "memchr" } */

  memcmp (p, "", 2);   /* { dg-warning "specified bound 2 exceeds source size 1" "memcmp" } */
  memcmp (p, "123", 5);   /* { dg-warning "specified bound 5 exceeds source size 4" "memcmp" } */
  memcmp (p, a, sizeof a + 1);   /* { dg-warning "specified bound 5 exceeds source size 4" "memcmp" } */

  size_t n = PTRDIFF_MAX + (size_t)1;
  memchr (p, 1, n);   /* { dg-warning "exceeds maximum object size" "memchr" } */
  memcmp (p, q, n);   /* { dg-warning "exceeds maximum object size" "memcmp" } */

  n = SIZE_MAX;
  memchr (p, 1, n);   /* { dg-warning "exceeds maximum object size" "memchr" } */
  memcmp (p, q, n);   /* { dg-warning "exceeds maximum object size" "memcmp" } */
}

/* Verify that reading beyond the end of a dynamically allocated array
   of known size is diagnosed.  */

void test_memop_warn_alloc (void *p)
{
  size_t n;

  n = range (8, 32);

  struct A *a = __builtin_malloc (sizeof *a * 2);

  memcpy (p, a, n);   /* { dg-warning "reading between 8 and 32 bytes from a region of size 4" "memcpy from allocated" } */

  memcpy (p, &a[0], n);   /* { dg-warning "reading between 8 and 32 bytes from a region of size 4" "memcpy from allocated" } */

  memcpy (p, &a[0].a, n);   /* { dg-warning "reading between 8 and 32 bytes from a region of size " "memcpy from allocated" } */

  n = range (12, 32);

  struct B *b = __builtin_malloc (sizeof *b * 2);

  memcpy (p, &b[0], n);   /* { dg-warning "reading between 12 and 32 bytes from a region of size 8" "memcpy from allocated" } */

  /* Verify memchr/memcmp.  */
  n = sizeof *b * 2 + 1;

  memchr (b, 1, n);   /* { dg-warning "specified bound 9 exceeds source size 8" "memchr from allocated" } */
  memcmp (p, b, n);   /* { dg-warning "specified bound 9 exceeds source size 8" "memcmp from allocated" } */
}


void test_memop_nowarn (void *p)
{
  struct B b[2];

  size_t n = range (sizeof b, 32);

  /* Verify that copying the whole array is not diagnosed regardless
     of whether the expression pointing to its beginning is obtained
     from the array itself or its first member(s).  */
  memcpy (p, b, n);

  memcpy (p, &b[0], n);

  memcpy (p, &b[0].a, n);

  memcpy (p, &b[0].a.a, n);

  /* Verify that memchr/memcmp doesn't cause a warning.  */
  memchr (p, 1, n);
  memchr (b, 2, n);
  memchr (&b[0], 3, n);
  memchr (&b[0].a, 4, n);
  memchr (&b[0].a.a, 5, n);
  memchr ("01234567", R (0, 255), n);

  memcmp (p, p, n);
  memcmp (p, b, n);
  memcmp (p, &b[0], n);
  memcmp (p, &b[0].a, n);
  memcmp (p, &b[0].a.a, n);
  memcmp (p, "01234567", n);
}


/* The following function could specify in its API that it takes
   an array of exactly two elements, as shown below (or simply be
   called with such an array).  Verify that reading from both
   elements is not diagnosed.  */
void test_memop_nowarn_arg (void*, const struct A[2]);

void test_memop_nowarn_arg (void *p, const struct A *a)
{
  memcpy (p, a, 2 * sizeof *a);

  memcpy (p, a, range (2 * sizeof *a, 123));

  memchr (p, 1, 1234);
  memcmp (p, a, 1234);
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 a: @type[[TYPE_A]];
// DEFAULT-NEXT:         field1 c: i8;
// DEFAULT-NEXT:         field2 d: i8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 2, 3]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([49, 50, 51, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([48, 49, 50, 51, 52, 53, 54, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([48, 49, 50, 51, 52, 53, 54, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_memchr:[0-9]+]] @memchr(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE6:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE7:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE8:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memmove:[0-9]+]] @memmove(%[[VALUE9:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE10:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE11:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mempcpy:[0-9]+]] @mempcpy(%[[VALUE12:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE13:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE14:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE15:[0-9]+]] <unnamed>: ptr<void>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_random_unsigned_value:[0-9]+]] @random_unsigned_value() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_range:[0-9]+]] @range(%[[VALUE_min:[0-9]+]] min: u64, %[[VALUE_max:[0-9]+]] max: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val:[0-9]+]] val: u64 [storage=automatic] [const] = call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]]);
// DEFAULT-NEXT:         return conditional<u64>(logical_or<bool>(lt<u64>(read<u64>(%[[VALUE_val]]), read<u64>(%[[VALUE_min]])), lt<u64>(read<u64>(%[[VALUE_max]]), read<u64>(%[[VALUE_val]]))), read<u64>(%[[VALUE_min]]), read<u64>(%[[VALUE_val]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_memop_warn_local:[0-9]+]] @test_memop_warn_local(%[[VALUE_p:[0-9]+]] p: ptr<void>, %[[VALUE_q:[0-9]+]] q: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p]]), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))))));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<@type[[TYPE_A]], 2> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p]]), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(2)>(%[[VALUE_a]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A]]>>(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0))))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field0(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))))));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<@type[[TYPE_B]], 2> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_B]]>>(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0))))), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))))));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(255))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_i]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]])), read<i32>(%[[VALUE_i]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])), read<i32>(%[[VALUE_i]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(2)>(%[[VALUE_a]])), read<i32>(%[[VALUE_i]]), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_5]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_6]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(2)>(%[[VALUE_a]])), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: u64 [storage=automatic] = add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), const<i32>(1), read<u64>(%[[VALUE_n]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), read<ptr<const void>>(%[[VALUE_q]]), read<u64>(%[[VALUE_n]])));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_n]], const<u64>(18446744073709551615));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), const<i32>(1), read<u64>(%[[VALUE_n]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), read<ptr<const void>>(%[[VALUE_q]]), read<u64>(%[[VALUE_n]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE16:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_memop_warn_alloc:[0-9]+]] @test_memop_warn_alloc(%[[VALUE_p_2:[0-9]+]] p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%[[VALUE_n_2]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: ptr<@type[[TYPE_A]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_A]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p_2]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a_2]])), read<u64>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p_2]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A]]>>(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a_2]]), const<i32>(0))))), read<u64>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p_2]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field0(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a_2]]), const<i32>(0)))))), read<u64>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_n_2]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: ptr<@type[[TYPE_B]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_B]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p_2]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_B]]>>(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_b_2]]), const<i32>(0))))), read<u64>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_n_2]], add<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_b_2]])), const<i32>(1), read<u64>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_2]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_2]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_b_2]])), read<u64>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_memop_nowarn:[0-9]+]] @test_memop_nowarn(%[[VALUE_p_3:[0-9]+]] p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_b_3:[0-9]+]] b: array<@type[[TYPE_B]], 2> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_3:[0-9]+]] n: u64 [storage=automatic] = call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], const<u64>(8), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p_3]]), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]])), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p_3]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_B]]>>(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]]), const<i32>(0))))), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p_3]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]]), const<i32>(0)))))), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p_3]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field0(field0(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]]), const<i32>(0))))))), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_3]])), const<i32>(1), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]])), const<i32>(2), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_B]]>>(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]]), const<i32>(0))))), const<i32>(3), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]]), const<i32>(0)))))), const<i32>(4), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field0(field0(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]]), const<i32>(0))))))), const<i32>(5), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_7]])), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(255)))))), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_3]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_3]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_3]])), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_3]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_3]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]])), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_3]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_B]]>>(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]]), const<i32>(0))))), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_3]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]]), const<i32>(0)))))), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_3]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field0(field0(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_3]]), const<i32>(0))))))), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_3]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_3]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_8]])), read<u64>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_memop_nowarn_arg:[0-9]+]] @test_memop_nowarn_arg(%[[VALUE_p_4:[0-9]+]] p: ptr<void>, %[[VALUE_a_3:[0-9]+]] a: ptr<const @type[[TYPE_A]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p_4]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_a_3]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p_4]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_a_3]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(2)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(123))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_4]])), const<i32>(1), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_4]]), call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_4]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_a_3]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
