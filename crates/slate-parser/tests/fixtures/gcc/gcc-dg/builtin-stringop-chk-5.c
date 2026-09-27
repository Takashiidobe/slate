/* Test exercising -Wstringop-overflow warnings.  */
/* { dg-do compile } */
/* { dg-options "-O2 -Wstringop-overflow=1 -Wno-array-bounds" } */

#define offsetof(type, mem)   __builtin_offsetof (type, mem)

/* Return the number of bytes from member MEM of TYPE to the end
   of object OBJ.  */
#define offsetfrom(type, obj, mem) (sizeof (obj) - offsetof (type, mem))


typedef __SIZE_TYPE__ size_t;
extern void* memcpy (void*, const void*, size_t);
extern void* memset (void*, int, __SIZE_TYPE__);


struct A { char a, b; };
struct B { struct A a; char c, d; };

/* Function to call to "escape" pointers from tests below to prevent
   GCC from assuming the values of the objects they point to stay
   the unchanged.  */
void escape (void*, ...);

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

/* Verify that writing past the end of a local array is diagnosed.  */

void test_memop_warn_local (const void *src)
{
  size_t n;

  n = range (8, 32);

  struct A a[2];

  memcpy (a, src, n);   /* { dg-warning "writing between 8 and 32 bytes into a region of size 4 overflows the destination" } */
  escape (a, src);

  /* At -Wstringop-overflow=1 the destination is considered to be
     the whole array and its size is therefore sizeof a.  */
  memcpy (&a[0], src, n);   /* { dg-warning "writing between 8 and 32 bytes into a region of size 4 overflows the destination" } */
  escape (a, src);

  /* Verify the same as above but by writing into the first mmeber
     of the first element of the array.  */
  memcpy (&a[0].a, src, n);   /* { dg-warning "writing between 8 and 32 bytes into a region of size 4 overflows the destination" } */
  escape (a, src);

  n = range (12, 32);

  struct B b[2];

  memcpy (&b[0], src, n);   /* { dg-warning "writing between 12 and 32 bytes into a region of size 8 overflows the destination" } */
  escape (b);

  /* The following idiom of clearing multiple members of a struct is
     used in a few places in the Linux kernel.  Verify that a warning
     is issued for it when it writes past the end of the array object.  */
  memset (&b[0].a.b, 0, offsetfrom (struct B, b, a.b) + 1);   /* { dg-warning "writing 8 bytes into a region of size 7" } */
  escape (b);

  memset (&b->a.b, 0, offsetfrom (struct B, b, a.b) + 1);   /* { dg-warning "writing 8 bytes into a region of size 7" } */
  escape (b);

  memset (&b[0].c, 0, offsetfrom (struct B, b, c) + 1);   /* { dg-warning "writing 7 bytes into a region of size 6" } */
  escape (b);

  memset (&b->c, 0, offsetfrom (struct B, b, c) + 1);   /* { dg-warning "writing 7 bytes into a region of size 6" } */
  escape (b);

  memset (&b[0].d, 0, offsetfrom (struct B, b, d) + 1);   /* { dg-warning "writing 6 bytes into a region of size 5" } */
  escape (b);

  memset (&b->d, 0, offsetfrom (struct B, b, d) + 1);   /* { dg-warning "writing 6 bytes into a region of size 5" } */
  escape (b);

  /* Same as above but clearing just members of the second element
     of the array.  */
  memset (&b[1].a.b, 0, offsetfrom (struct B, b[1], a.b) + 1);   /* { dg-warning "writing 4 bytes into a region of size 3" } */
  escape (b);

  memset (&b[1].c, 0, offsetfrom (struct B, b[1], c) + 1);   /* { dg-warning "writing 3 bytes into a region of size 2" } */
  escape (b);

  memset (&b[1].d, 0, offsetfrom (struct B, b[1], d) + 1);   /* { dg-warning "writing 2 bytes into a region of size 1" } */
  escape (b);
}

/* Verify that writing past the end of a dynamically allocated array
   of known size is diagnosed.  */

void test_memop_warn_alloc (const void *src)
{
  size_t n;

  n = range (8, 32);

  struct A *a = __builtin_malloc (sizeof *a * 2);

  memcpy (a, src, n);   /* { dg-warning "writing between 8 and 32 bytes into a region of size 4 " "memcpy into allocated" } */
  escape (a, src);

  /* At -Wstringop-overflow=1 the destination is considered to be
     the whole array and its size is therefore sizeof a.  */
  memcpy (&a[0], src, n);   /* { dg-warning "writing between 8 and 32 bytes into a region of size 4 overflows the destination" "memcpy into allocated" } */
  escape (a, src);

  /* Verify the same as above but by writing into the first mmeber
     of the first element of the array.  */
  memcpy (&a[0].a, src, n);   /* { dg-warning "writing between 8 and 32 bytes into a region of size " "memcpy into allocated" } */
  escape (a, src);

  n = range (12, 32);

  struct B *b = __builtin_malloc (sizeof (struct B[2]));

  memcpy (&b[0], src, n);   /* { dg-warning "writing between 12 and 32 bytes into a region of size 8 " "memcpy into allocated" } */
  escape (b);

  /* The following idiom of clearing multiple members of a struct is
     used in a few places in the Linux kernel.  Verify that a warning
     is issued for it when it writes past the end of the array object.  */
  memset (&b[0].a.b, 0, offsetfrom (struct B, struct B[2], a.b) + 1);   /* { dg-warning "writing 8 bytes into a region of size " "memcpy into allocated" } */
  escape (b);

  memset (&b->a.b, 0, offsetfrom (struct B, struct B[2], a.b) + 1);   /* { dg-warning "writing 8 bytes into a region of size " "memcpy into allocated" } */
  escape (b);

  memset (&b[0].c, 0, offsetfrom (struct B, struct B[2], c) + 1);   /* { dg-warning "writing 7 bytes into a region of size " "memcpy into allocated" } */
  escape (b);

  memset (&b->c, 0, offsetfrom (struct B, struct B[2], c) + 1);   /* { dg-warning "writing 7 bytes into a region of size " "memcpy into allocated" } */
  escape (b);

  memset (&b[0].d, 0, offsetfrom (struct B, struct B[2], d) + 1);   /* { dg-warning "writing 6 bytes into a region of size " "memcpy into allocated" } */
  escape (b);

  memset (&b->d, 0, offsetfrom (struct B, struct B[2], d) + 1);   /* { dg-warning "writing 6 bytes into a region of size " "memcpy into allocated" } */
  escape (b);

  /* Same as above but clearing just elements of the second element
     of the array.  */
  memset (&b[1].a.b, 0, offsetfrom (struct B, b[1], a.b) + 1);   /* { dg-warning "writing 4 bytes into a region of size " "memcpy into allocated" } */
  escape (b);

  memset (&b[1].c, 0, offsetfrom (struct B, b[1], c) + 1);   /* { dg-warning "writing 3 bytes into a region of size " "memcpy into allocated" } */
  escape (b);

  memset (&b[1].d, 0, offsetfrom (struct B, b[1], d) + 1);   /* { dg-warning "writing 2 bytes into a region of size 1" "memcpy into allocated" } */
  escape (b);
}


void test_memop_nowarn (const void *src)
{
  struct B b[2];

  size_t n = range (sizeof b, 32);

  /* Verify that clearing the whole array is not diagnosed regardless
     of whether the expression pointing to its beginning is obtained
     from the array itself or its first member(s).  */
  memcpy (b, src, n);
  escape (b);

  memcpy (&b[0], src, n);
  escape (b);

  memcpy (&b[0].a, src, n);
  escape (b, src);

  memcpy (&b[0].a.a, src, n);
  escape (b, src);

  /* Clearing multiple elements of an array of structs.  */
  memset (&b[0].a.b, 0, sizeof b - offsetof (struct B, a.b));
  escape (b);

  memset (&b->a.b, 0, sizeof b - offsetof (struct B, a.b));
  escape (b);

  memset (&b[0].c, 0, sizeof b - offsetof (struct B, c));
  escape (b);

  memset (&b->c, 0, sizeof b - offsetof (struct B, c));
  escape (b);

  memset (&b[0].d, 0, sizeof b - offsetof (struct B, d));
  escape (b);

  memset (&b->d, 0, sizeof b - offsetof (struct B, d));
  escape (b);

  /* Same as above but clearing just elements of the second element
     of the array.  */
  memset (&b[1].a.b, 0, sizeof b[1] - offsetof (struct B, a.b));
  escape (b);

  memset (&b[1].c, 0, sizeof b[1] - offsetof (struct B, c));
  escape (b);

  memset (&b[1].d, 0, sizeof b[1] - offsetof (struct B, d));
  escape (b);
}


/* The foollowing function could specify in its API that it takes
   an array of exactly two elements, as shown below.  Verify that
   writing into both elements is not diagnosed.  */
void test_memop_nowarn_arg (struct A[2], const void*);

void test_memop_nowarn_arg (struct A *a, const void *src)
{
  memcpy (a, src, 2 * sizeof *a);
  escape (a, src);

  memcpy (a, src, range (2 * sizeof *a, 123));
  escape (a, src);
}


struct C { char a[3], b; };
struct D { struct C c; char d, e; };

extern char* strncpy (char*, const char*, __SIZE_TYPE__);

void test_stringop_warn (void)
{
  size_t n = range (2 * sizeof (struct D) + 1, 33);

  struct C c[2];

  /* Similarly, at -Wstringop-overflow=1 the destination is considered
     to be the whole array and its size is therefore sizeof c.  */
  strncpy (c[0].a, "123", n);   /* { dg-warning "writing between 13 and 33 bytes into a region of size 8 overflows the destination" } */

  escape (c);
}


void test_stringop_nowarn (void)
{
  struct D d[2];

  strncpy (d[0].c.a, "123", range (sizeof d, 32));
  escape (d);
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
// DEFAULT-NEXT:     type @type1 A = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type2 B = struct {
// DEFAULT-NEXT:         field0 a: @type1;
// DEFAULT-NEXT:         field1 c: i8;
// DEFAULT-NEXT:         field2 d: i8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 2, 3]];
// DEFAULT-NEXT:     type @type3 C = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 3>;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 3]];
// DEFAULT-NEXT:     type @type4 D = struct {
// DEFAULT-NEXT:         field0 c: @type3;
// DEFAULT-NEXT:         field1 d: i8;
// DEFAULT-NEXT:         field2 e: i8;
// DEFAULT-NEXT:     } [size=6, align=1, offsets=[0, 4, 5]];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %51 .str51: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @memcpy(%36 <unnamed>: ptr<void>, %37 <unnamed>: ptr<const void>, %38 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @memset(%39 <unnamed>: ptr<void>, %40 <unnamed>: i32, %41 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @escape(%42 <unnamed>: ptr<void>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @random_unsigned_value() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @range(%8 min: u64, %9 max: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 val: u64 [storage=automatic] [const] = call<u64, signature=fn() -> u64>(%6);
// DEFAULT-NEXT:         return conditional<u64>(logical_or<bool>(lt<u64>(read<u64>(%10), read<u64>(%8)), lt<u64>(read<u64>(%9), read<u64>(%10))), read<u64>(%8), read<u64>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test_memop_warn_local(%12 src: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 n: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%13, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         let %14 a: array<@type1, 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type1>, length=Some(2)>(%14)), read<ptr<const void>>(%12), read<u64>(%13));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type1>, length=Some(2)>(%14)), read<ptr<const void>>(%12));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type1>>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(2)>(%14), const<i32>(0))))), read<ptr<const void>>(%12), read<u64>(%13));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type1>, length=Some(2)>(%14)), read<ptr<const void>>(%12));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(2)>(%14), const<i32>(0)))))), read<ptr<const void>>(%12), read<u64>(%13));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type1>, length=Some(2)>(%14)), read<ptr<const void>>(%12));
// DEFAULT-NEXT:         write<u64>(%13, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         let %15 b: array<@type2, 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%15), const<i32>(0))))), read<ptr<const void>>(%12), read<u64>(%13));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%15), const<i32>(0))))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(field0(deref(array_decay<ptr<@type2>, length=Some(2)>(%15)))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%15), const<i32>(0)))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(array_decay<ptr<@type2>, length=Some(2)>(%15))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%15), const<i32>(0)))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field2(deref(array_decay<ptr<@type2>, length=Some(2)>(%15))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%15), const<i32>(1))))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(4), const<u64>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%15), const<i32>(1)))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(4), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%15), const<i32>(1)))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(4), const<u64>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @__builtin_malloc(%43 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %16 @test_memop_warn_alloc(%17 src: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %18 n: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%18, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         let %19 a: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%44, mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%19)), read<ptr<const void>>(%17), read<u64>(%18));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%19)), read<ptr<const void>>(%17));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type1>>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(read<ptr<@type1>>(%19), const<i32>(0))))), read<ptr<const void>>(%17), read<u64>(%18));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%19)), read<ptr<const void>>(%17));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(read<ptr<@type1>>(%19), const<i32>(0)))))), read<ptr<const void>>(%17), read<u64>(%18));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%19)), read<ptr<const void>>(%17));
// DEFAULT-NEXT:         write<u64>(%18, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         let %20 b: ptr<@type2> [storage=automatic] = pointer_cast<ptr<@type2>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%44, const<u64>(8)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(0))))), read<ptr<const void>>(%17), read<u64>(%18));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%20)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(0))))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%20)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(field0(deref(read<ptr<@type2>>(%20)))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%20)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(0)))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%20)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(read<ptr<@type2>>(%20))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%20)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(0)))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%20)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field2(deref(read<ptr<@type2>>(%20))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(8), const<u64>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%20)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(1))))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(4), const<u64>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%20)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(1)))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(4), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%20)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(1)))))), const<i32>(0), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(const<u64>(4), const<u64>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%20)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test_memop_nowarn(%22 src: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %23 b: array<@type2, 2> [storage=automatic];
// DEFAULT-NEXT:         let %24 n: u64 [storage=automatic] = call<u64, signature=fn(u64, u64) -> u64>(%7, const<u64>(8), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)), read<ptr<const void>>(%22), read<u64>(%24));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%23), const<i32>(0))))), read<ptr<const void>>(%22), read<u64>(%24));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type1>>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%23), const<i32>(0)))))), read<ptr<const void>>(%22), read<u64>(%24));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)), read<ptr<const void>>(%22));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field0(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%23), const<i32>(0))))))), read<ptr<const void>>(%22), read<u64>(%24));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)), read<ptr<const void>>(%22));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%23), const<i32>(0))))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(field0(deref(array_decay<ptr<@type2>, length=Some(2)>(%23)))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%23), const<i32>(0)))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(array_decay<ptr<@type2>, length=Some(2)>(%23))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%23), const<i32>(0)))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field2(deref(array_decay<ptr<@type2>, length=Some(2)>(%23))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%23), const<i32>(1))))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), const<u64>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%23), const<i32>(1)))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), const<u64>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%23), const<i32>(1)))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), const<u64>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%23)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test_memop_nowarn_arg(%26 a: ptr<@type1>, %27 src: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%26)), read<ptr<const void>>(%27), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%26)), read<ptr<const void>>(%27));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%26)), read<ptr<const void>>(%27), call<u64, signature=fn(u64, u64) -> u64>(%7, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(2)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(123)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%26)), read<ptr<const void>>(%27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @strncpy(%47 <unnamed>: ptr<i8>, %48 <unnamed>: ptr<const i8>, %49 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %31 @test_stringop_warn() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %32 n: u64 [storage=automatic] = call<u64, signature=fn(u64, u64) -> u64>(%7, add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(6)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(33))));
// DEFAULT-NEXT:         let %33 c: array<@type3, 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%30, array_decay<ptr<i8>, length=Some(3)>(field0(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(2)>(%33), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%50)), read<u64>(%32));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type3>, length=Some(2)>(%33)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @test_stringop_nowarn() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %35 d: array<@type4, 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%30, array_decay<ptr<i8>, length=Some(3)>(field0(field0(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(array_decay<ptr<@type4>, length=Some(2)>(%35), const<i32>(0)))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%51)), call<u64, signature=fn(u64, u64) -> u64>(%7, const<u64>(12), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type4>, length=Some(2)>(%35)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
