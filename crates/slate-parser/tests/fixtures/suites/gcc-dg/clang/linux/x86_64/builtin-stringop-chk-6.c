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
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 3>;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 3]];
// DEFAULT-NEXT:     type @type[[TYPE_D:[0-9]+]] D = struct {
// DEFAULT-NEXT:         field0 c: @type[[TYPE_C]];
// DEFAULT-NEXT:         field1 d: i8;
// DEFAULT-NEXT:         field2 e: i8;
// DEFAULT-NEXT:     } [size=6, align=1, offsets=[0, 4, 5]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_escape:[0-9]+]] @escape(%[[VALUE6:[0-9]+]] <unnamed>: ptr<void>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_random_unsigned_value:[0-9]+]] @random_unsigned_value() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_range:[0-9]+]] @range(%[[VALUE_min:[0-9]+]] min: u64, %[[VALUE_max:[0-9]+]] max: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val:[0-9]+]] val: u64 [storage=automatic] [const] = call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]]);
// DEFAULT-NEXT:         return conditional<u64>(logical_or<bool>(lt<u64>(read<u64>(%[[VALUE_val]]), read<u64>(%[[VALUE_min]])), lt<u64>(read<u64>(%[[VALUE_max]]), read<u64>(%[[VALUE_val]]))), read<u64>(%[[VALUE_min]]), read<u64>(%[[VALUE_val]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_memop_warn_object:[0-9]+]] @test_memop_warn_object(%[[VALUE_src:[0-9]+]] src: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(17))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(29)))));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<@type[[TYPE_A]], 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_A]]>>(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0))))), read<ptr<const void>>(%[[VALUE_src]]), widen<u64, reason=arg>(read<u32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_escape]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(2)>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_memop_warn_subobject:[0-9]+]] @test_memop_warn_subobject(%[[VALUE_src_2:[0-9]+]] src: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(17))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(31)))));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<@type[[TYPE_B]], 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_A]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b]]), const<i32>(0)))))), read<ptr<const void>>(%[[VALUE_src_2]]), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_escape]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_memop_nowarn_subobject:[0-9]+]] @test_memop_nowarn_subobject() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: array<@type[[TYPE_B]], 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(field1(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_2]]), const<i32>(0)))))), const<i32>(0), sub<u64, overflow=wrap>(const<u64>(4), const<u64>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_escape]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(2)>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strncpy:[0-9]+]] @strncpy(%[[VALUE7:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE8:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE9:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_stringop_warn_object:[0-9]+]] @test_stringop_warn_object(%[[VALUE_str_3:[0-9]+]] str: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n_3:[0-9]+]] n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: array<@type[[TYPE_C]], 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(3)>(field0(deref(ptr_offset<ptr<@type[[TYPE_C]]>, subtract=false, element=@type[[TYPE_C]], overflow=ub>(array_decay<ptr<@type[[TYPE_C]]>, length=Some(2)>(%[[VALUE_c]]), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_escape]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE_C]]>, length=Some(2)>(%[[VALUE_c]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(3)>(field0(deref(ptr_offset<ptr<@type[[TYPE_C]]>, subtract=false, element=@type[[TYPE_C]], overflow=ub>(array_decay<ptr<@type[[TYPE_C]]>, length=Some(2)>(%[[VALUE_c]]), const<i32>(0))))), read<ptr<const i8>>(%[[VALUE_str_3]]), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_escape]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE_C]]>, length=Some(2)>(%[[VALUE_c]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_stringop_warn_subobject:[0-9]+]] @test_stringop_warn_subobject(%[[VALUE_src_3:[0-9]+]] src: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n_4:[0-9]+]] n: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32)))));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: array<@type[[TYPE_D]], 2> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(3)>(field0(field0(deref(ptr_offset<ptr<@type[[TYPE_D]]>, subtract=false, element=@type[[TYPE_D]], overflow=ub>(array_decay<ptr<@type[[TYPE_D]]>, length=Some(2)>(%[[VALUE_d]]), const<i32>(0)))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_escape]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE_D]]>, length=Some(2)>(%[[VALUE_d]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(3)>(field0(field0(deref(ptr_offset<ptr<@type[[TYPE_D]]>, subtract=false, element=@type[[TYPE_D]], overflow=ub>(array_decay<ptr<@type[[TYPE_D]]>, length=Some(2)>(%[[VALUE_d]]), const<i32>(0)))))), read<ptr<const i8>>(%[[VALUE_src_3]]), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ...) -> void>(%[[VALUE_escape]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE_D]]>, length=Some(2)>(%[[VALUE_d]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
