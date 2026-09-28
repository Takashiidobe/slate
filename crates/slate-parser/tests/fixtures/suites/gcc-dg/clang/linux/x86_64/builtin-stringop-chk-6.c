/* Test exercising -Wrawmem-overflow and -Wstringop-overflow warnings.  */
/* { dg-do compile } */
/* { dg-options "-O2 -Wstringop-overflow=2" } */

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


void test_memop_warn_object (const void *src)
{
  unsigned n = range (17, 29);

  struct A a[2];

  /* At both -Wstringop-overflow=2, like at 1, the destination of functions
     that operate on raw memory is considered to be the whole array and its
     size is therefore sizeof a.  */
  memcpy (&a[0], src, n);   /* { dg-warning "writing between 17 and 29 bytes into a region of size 4 overflows the destination" } */
  escape (a);
}

void test_memop_warn_subobject (const void *src)
{
  unsigned n = range (17, 31);

  struct B b[2];

  /* At -Wrawmem-overflow=2 the destination is considered to be
     the member sobobject of the first array element and its size
     is therefore sizeof b[0].a.  */
  memcpy (&b[0].a, src, n);   /* { dg-warning "writing between 17 and 31 bytes into a region of size 8 overflows the destination" } */

  escape (b);
}

void test_memop_nowarn_subobject (void)
{
  struct B b[2];

  /* The following idiom of clearing multiple members of a struct
     has been seen in a few places in the Linux kernel.  Verify
     that a warning is not issued for it.  */
  memset (&b[0].c, 0, sizeof b[0] - offsetof (struct B, c));

  escape (b);
}

struct C { char a[3], b; };
struct D { struct C c; char d, e; };

extern char* strncpy (char*, const char*, __SIZE_TYPE__);

void test_stringop_warn_object (const char *str)
{
  unsigned n = range (2 * sizeof (struct D), 32);

  struct C c[2];

  /* Similarly, at -Wstringop-overflow=2 the destination is considered
     to be the array member of the first element of the array c and its
     size is therefore sizeof c[0].a.  */
  strncpy (c[0].a, "123", n);   /* { dg-warning "writing between 12 and 32 bytes into a region of size 3 overflows the destination" } */
  escape (c);

  strncpy (c[0].a, str, n);   /* { dg-warning "writing between 12 and 32 bytes into a region of size 3 overflows the destination" } */
  escape (c);
}

void test_stringop_warn_subobject (const char *src)
{
  unsigned n = range (2 * sizeof (struct D), 32);

  struct D d[2];

  /* Same as above.  */
  strncpy (d[0].c.a, "123", n);   /* { dg-warning "writing between 12 and 32 bytes into a region of size 3 overflows the destination" } */
  escape (d);

  strncpy (d[0].c.a, src, n);   /* { dg-warning "writing between 12 and 32 bytes into a region of size 3 overflows the destination" } */
  escape (d);
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
// DEFAULT-NEXT:     global %42 .str42: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @memcpy(%32 <unnamed>: ptr<void>, %33 <unnamed>: ptr<const void>, %34 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @memset(%35 <unnamed>: ptr<void>, %36 <unnamed>: i32, %37 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @escape(%38 <unnamed>: ptr<void>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @random_unsigned_value() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @range(%8 min: u64, %9 max: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 val: u64 [storage=automatic] [const] = call<u64, signature=fn() -> u64>(%6);
// DEFAULT-NEXT:         return conditional<u64>(logical_or<bool>(lt<u64>(read<u64>(%10), read<u64>(%8)), lt<u64>(read<u64>(%9), read<u64>(%10))), read<u64>(%8), read<u64>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test_memop_warn_object(%12 src: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(17))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(29)))));
// DEFAULT-NEXT:         let %14 a: array<@type1, 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type1>>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(2)>(%14), const<i32>(0))))), read<ptr<const void>>(%12), widen<u64, reason=arg>(read<u32>(%13)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type1>, length=Some(2)>(%14)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_memop_warn_subobject(%16 src: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %17 n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(17))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(31)))));
// DEFAULT-NEXT:         let %18 b: array<@type2, 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type1>>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%18), const<i32>(0)))))), read<ptr<const void>>(%16), widen<u64, reason=arg>(read<u32>(%17)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%18)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_memop_nowarn_subobject() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20 b: array<@type2, 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(2)>(%20), const<i32>(0)))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), const<u64>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%20)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @strncpy(%39 <unnamed>: ptr<i8>, %40 <unnamed>: ptr<const i8>, %41 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %24 @test_stringop_warn_object(%25 str: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %26 n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         let %27 c: array<@type3, 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%23, array_decay<ptr<i8>, length=Some(3)>(field0(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(2)>(%27), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%42)), widen<u64, reason=arg>(read<u32>(%26)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type3>, length=Some(2)>(%27)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%23, array_decay<ptr<i8>, length=Some(3)>(field0(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(2)>(%27), const<i32>(0))))), read<ptr<const i8>>(%25), widen<u64, reason=arg>(read<u32>(%26)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type3>, length=Some(2)>(%27)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @test_stringop_warn_subobject(%29 src: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %30 n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         let %31 d: array<@type4, 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%23, array_decay<ptr<i8>, length=Some(3)>(field0(field0(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(array_decay<ptr<@type4>, length=Some(2)>(%31), const<i32>(0)))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%43)), widen<u64, reason=arg>(read<u32>(%30)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type4>, length=Some(2)>(%31)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%23, array_decay<ptr<i8>, length=Some(3)>(field0(field0(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(array_decay<ptr<@type4>, length=Some(2)>(%31), const<i32>(0)))))), read<ptr<const i8>>(%29), widen<u64, reason=arg>(read<u32>(%30)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type4>, length=Some(2)>(%31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
