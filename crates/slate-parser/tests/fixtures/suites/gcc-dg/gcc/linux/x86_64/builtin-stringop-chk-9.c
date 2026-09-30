/* PR middle-end/82646 - bogus -Wstringop-overflow with -D_FORTIFY_SOURCE=2
   on strncpy with range to a member array
   { dg-do compile }
   { dg-options "-O2 -Wstringop-overflow -ftrack-macro-expansion=0" } */

#define bos(p)   __builtin_object_size (p, 1)

struct S {
  char a[5];
  void (*pf)(void);
};

/* Verify that none of the string function calls below triggers a warning.  */

char* test_stpncpy_const_nowarn (struct S *p)
{
  int n = sizeof p->a;

  return __builtin_stpncpy (p->a, "123456", n);
}

char* test_strncpy_const_nowarn (struct S *p)
{
  int n = sizeof p->a;

  return __builtin_strncpy (p->a, "1234567", n);
}

char* test_stpncpy_chk_const_nowarn (struct S *p)
{
  int n = sizeof p->a;

  return __builtin___stpncpy_chk (p->a, "12345678", n, bos (p->a));
}

char* test_strncpy_chk_const_nowarn (struct S *p)
{
  int n = sizeof p->a;

  return __builtin___strncpy_chk (p->a, "123456789", n, bos (p->a));
}


char* test_stpncpy_range_nowarn (struct S *p, int n)
{
  if (n < sizeof p->a)
    n = sizeof p->a;

  return __builtin_stpncpy (p->a, "123456", n);
}

char* test_strncpy_range_nowarn (struct S *p, int n)
{
  if (n < sizeof p->a)
    n = sizeof p->a;

  return __builtin_strncpy (p->a, "1234567", n);
}

char* test_stpncpy_chk_range_nowarn (struct S *p, int n)
{
  if (n < sizeof p->a)
    n = sizeof p->a;

  return __builtin___stpncpy_chk (p->a, "12345678", n, bos (p->a));   /* { dg-bogus "\\\[-Wstringop-overflow=]" } */
}

char* test_strncpy_chk_range_nowarn (struct S *p, int n)
{
  if (n < sizeof p->a)
    n = sizeof p->a;

  return __builtin___strncpy_chk (p->a, "123456789", n, bos (p->a));  /* { dg-bogus "\\\[-Wstringop-overflow=]" } */
}


/* Verify that all of the string function calls below trigger a warning.  */

char* test_stpncpy_const_warn (struct S *p)
{
  int n = sizeof p->a;

  ++n;

  return __builtin_stpncpy (p->a, "123456", n);                       /* { dg-warning "\\\[-Wstringop-overflow=]" } */
}

char* test_strncpy_const_warn (struct S *p)
{
  int n = sizeof p->a;

  /* A call to strncpy() with a known string and small bound is folded
     into memcpy() which defeats the warning in this case since memcpy
     uses Object Size Type 0, i.e., the largest object that p->a may
     be a part of.  Use a larger bound to get around this here.  */
  n += 11;

  return __builtin_strncpy (p->a, "1234567", n);                      /* { dg-warning "\\\[-Wstringop-overflow=]" } */
}

char* test_stpncpy_chk_const_warn (struct S *p)
{
  int n = sizeof p->a;

  ++n;

  return __builtin___stpncpy_chk (p->a, "12345678", n, bos (p->a));   /* { dg-warning "\\\[-Wstringop-overflow=]" } */
}

char* test_strncpy_chk_const_warn (struct S *p)
{
  int n = sizeof p->a;

  ++n;

  return __builtin___strncpy_chk (p->a, "123456789", n, bos (p->a));  /* { dg-warning "\\\[-Wstringop-overflow=]" } */
}


char* test_stpncpy_range_warn (struct S *p, int n)
{
  if (n < sizeof p->a + 1)
    n = sizeof p->a + 1;

  return __builtin_stpncpy (p->a, "123456", n);                       /* { dg-warning "\\\[-Wstringop-overflow=]" } */
}

char* test_strncpy_range_warn (struct S *p, int n)
{
  if (n < sizeof p->a + 1)
    n = sizeof p->a + 1;

  return __builtin_strncpy (p->a, "1234567", n);                      /* { dg-warning "\\\[-Wstringop-overflow=]" } */
}

char* test_stpncpy_chk_range_warn (struct S *p, int n)
{
  if (n < sizeof p->a + 1)
    n = sizeof p->a + 1;

  return __builtin___stpncpy_chk (p->a, "12345678", n, bos (p->a));   /* { dg-warning "\\\[-Wstringop-overflow=]" } */
}

char* test_strncpy_chk_range_warn (struct S *p, int n)
{
  if (n < sizeof p->a + 1)
    n = sizeof p->a + 1;

  return __builtin___strncpy_chk (p->a, "123456789", n, bos (p->a));  /* { dg-warning "\\\[-Wstringop-overflow=]" } */
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 5>;
// DEFAULT-NEXT:         field1 pf: ptr<fn() -> void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([49, 50, 51, 52, 53, 54, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([49, 50, 51, 52, 53, 54, 55, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([49, 50, 51, 52, 53, 54, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([49, 50, 51, 52, 53, 54, 55, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([49, 50, 51, 52, 53, 54, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([49, 50, 51, 52, 53, 54, 55, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([49, 50, 51, 52, 53, 54, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([49, 50, 51, 52, 53, 54, 55, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stpncpy:[0-9]+]] @__builtin_stpncpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_stpncpy_const_nowarn:[0-9]+]] @test_stpncpy_const_nowarn(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strncpy:[0-9]+]] @__builtin_strncpy(%[[VALUE3:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_const_nowarn:[0-9]+]] @test_strncpy_const_nowarn(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strncpy]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin___stpncpy_chk:[0-9]+]] @__builtin___stpncpy_chk(%[[VALUE6:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE7:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE8:[0-9]+]] <unnamed>: u64, %[[VALUE9:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_object_size:[0-9]+]] @__builtin_object_size(%[[VALUE10:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE11:[0-9]+]] <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_stpncpy_chk_const_nowarn:[0-9]+]] @test_stpncpy_chk_const_nowarn(%[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_3:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___stpncpy_chk]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_3]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_3]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_3]]))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_3]]))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin___strncpy_chk:[0-9]+]] @__builtin___strncpy_chk(%[[VALUE12:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE13:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE14:[0-9]+]] <unnamed>: u64, %[[VALUE15:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_chk_const_nowarn:[0-9]+]] @test_strncpy_chk_const_nowarn(%[[VALUE_p_4:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_4:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncpy_chk]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_4]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_4]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_4]]))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_4]]))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_stpncpy_range_nowarn:[0-9]+]] @test_stpncpy_range_nowarn(%[[VALUE_p_5:[0-9]+]] p: ptr<@type[[TYPE_S]]>, %[[VALUE_n_5:[0-9]+]] n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_5]]))), const<u64>(5))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_5]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_5]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_5]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_5]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_range_nowarn:[0-9]+]] @test_strncpy_range_nowarn(%[[VALUE_p_6:[0-9]+]] p: ptr<@type[[TYPE_S]]>, %[[VALUE_n_6:[0-9]+]] n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_6]]))), const<u64>(5))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_6]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strncpy]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_6]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_6]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_6]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_stpncpy_chk_range_nowarn:[0-9]+]] @test_stpncpy_chk_range_nowarn(%[[VALUE_p_7:[0-9]+]] p: ptr<@type[[TYPE_S]]>, %[[VALUE_n_7:[0-9]+]] n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_7]]))), const<u64>(5))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_7]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___stpncpy_chk]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_7]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_7]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_7]]))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_7]]))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_chk_range_nowarn:[0-9]+]] @test_strncpy_chk_range_nowarn(%[[VALUE_p_8:[0-9]+]] p: ptr<@type[[TYPE_S]]>, %[[VALUE_n_8:[0-9]+]] n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_8]]))), const<u64>(5))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_8]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncpy_chk]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_8]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_8]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_8]]))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_8]]))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_stpncpy_const_warn:[0-9]+]] @test_stpncpy_const_warn(%[[VALUE_p_9:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_9:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_9]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n_9]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_9]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_9]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_9]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_const_warn:[0-9]+]] @test_strncpy_const_warn(%[[VALUE_p_10:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_10:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_10]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(11));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n_10]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strncpy]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_10]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_10]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_10]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_stpncpy_chk_const_warn:[0-9]+]] @test_stpncpy_chk_const_warn(%[[VALUE_p_11:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_11:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_11]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n_11]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___stpncpy_chk]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_11]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_11]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_11]]))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_11]]))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_chk_const_warn:[0-9]+]] @test_strncpy_chk_const_warn(%[[VALUE_p_12:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_12:[0-9]+]] n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_12]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n_12]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncpy_chk]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_12]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_12]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_12]]))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_12]]))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_stpncpy_range_warn:[0-9]+]] @test_stpncpy_range_warn(%[[VALUE_p_13:[0-9]+]] p: ptr<@type[[TYPE_S]]>, %[[VALUE_n_13:[0-9]+]] n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_13]]))), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_13]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_13]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_13]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_13]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_range_warn:[0-9]+]] @test_strncpy_range_warn(%[[VALUE_p_14:[0-9]+]] p: ptr<@type[[TYPE_S]]>, %[[VALUE_n_14:[0-9]+]] n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_14]]))), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_14]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strncpy]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_14]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_14]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_14]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_stpncpy_chk_range_warn:[0-9]+]] @test_stpncpy_chk_range_warn(%[[VALUE_p_15:[0-9]+]] p: ptr<@type[[TYPE_S]]>, %[[VALUE_n_15:[0-9]+]] n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_15]]))), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_15]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___stpncpy_chk]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_15]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_15]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_15]]))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_15]]))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_chk_range_warn:[0-9]+]] @test_strncpy_chk_range_warn(%[[VALUE_p_16:[0-9]+]] p: ptr<@type[[TYPE_S]]>, %[[VALUE_n_16:[0-9]+]] n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_16]]))), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_16]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%[[VALUE___builtin___strncpy_chk]], array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_16]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_16]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n_16]]))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_16]]))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
