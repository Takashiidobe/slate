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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 5>;
// DEFAULT-NEXT:         field1 pf: ptr<fn() -> void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([49, 50, 51, 52, 53, 54, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %64 .str64: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([49, 50, 51, 52, 53, 54, 55, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %73 .str73: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %74 .str74: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %75 .str75: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([49, 50, 51, 52, 53, 54, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %76 .str76: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([49, 50, 51, 52, 53, 54, 55, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 .str77: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %78 .str78: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([49, 50, 51, 52, 53, 54, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %80 .str80: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([49, 50, 51, 52, 53, 54, 55, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %81 .str81: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %82 .str82: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %83 .str83: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([49, 50, 51, 52, 53, 54, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %84 .str84: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([49, 50, 51, 52, 53, 54, 55, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %85 .str85: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %52 @__builtin_stpncpy(%49 <unnamed>: ptr<i8>, %50 <unnamed>: ptr<const i8>, %51 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %1 @test_stpncpy_const_nowarn(%2 p: ptr<@type0>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%52, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%2)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%53)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @__builtin_strncpy(%54 <unnamed>: ptr<i8>, %55 <unnamed>: ptr<const i8>, %56 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %4 @test_strncpy_const_nowarn(%5 p: ptr<@type0>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%57, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%5)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%58)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @__builtin___stpncpy_chk(%59 <unnamed>: ptr<i8>, %60 <unnamed>: ptr<const i8>, %61 <unnamed>: u64, %62 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %67 @__builtin_object_size(%65 <unnamed>: ptr<const void>, %66 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @test_stpncpy_chk_const_nowarn(%8 p: ptr<@type0>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%63, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%8)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%64)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%9))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%67, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%8))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @__builtin___strncpy_chk(%68 <unnamed>: ptr<i8>, %69 <unnamed>: ptr<const i8>, %70 <unnamed>: u64, %71 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %10 @test_strncpy_chk_const_nowarn(%11 p: ptr<@type0>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%72, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%11)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%73)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%12))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%67, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%11))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test_stpncpy_range_nowarn(%14 p: ptr<@type0>, %15 n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))), const<u64>(5))
// DEFAULT-NEXT:             write<i32>(%15, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%52, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%14)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%74)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test_strncpy_range_nowarn(%17 p: ptr<@type0>, %18 n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%18))), const<u64>(5))
// DEFAULT-NEXT:             write<i32>(%18, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%57, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%17)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%75)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%18))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_stpncpy_chk_range_nowarn(%20 p: ptr<@type0>, %21 n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%21))), const<u64>(5))
// DEFAULT-NEXT:             write<i32>(%21, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%63, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%20)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%76)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%21))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%67, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%20))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test_strncpy_chk_range_nowarn(%23 p: ptr<@type0>, %24 n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%24))), const<u64>(5))
// DEFAULT-NEXT:             write<i32>(%24, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%72, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%23)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%77)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%24))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%67, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%23))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test_stpncpy_const_warn(%26 p: ptr<@type0>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         let %86: i32 [synthetic] = read<i32>(%27);
// DEFAULT-NEXT:         let %87: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%86), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%27, read<i32>(%87));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%52, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%26)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%78)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%27))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @test_strncpy_const_warn(%29 p: ptr<@type0>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %30 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         let %88: i32 [synthetic] = read<i32>(%30);
// DEFAULT-NEXT:         let %89: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%88), const<i32>(11));
// DEFAULT-NEXT:         write<i32>(%30, read<i32>(%89));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%57, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%29)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%79)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%30))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @test_stpncpy_chk_const_warn(%32 p: ptr<@type0>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         let %90: i32 [synthetic] = read<i32>(%33);
// DEFAULT-NEXT:         let %91: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%90), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%33, read<i32>(%91));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%63, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%32)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%80)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%33))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%67, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%32))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @test_strncpy_chk_const_warn(%35 p: ptr<@type0>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %36 n: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(5)));
// DEFAULT-NEXT:         let %92: i32 [synthetic] = read<i32>(%36);
// DEFAULT-NEXT:         let %93: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%92), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%36, read<i32>(%93));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%72, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%35)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%81)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%36))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%67, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%35))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @test_stpncpy_range_warn(%38 p: ptr<@type0>, %39 n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%39))), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i32>(%39, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%52, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%38)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%82)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%39))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @test_strncpy_range_warn(%41 p: ptr<@type0>, %42 n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%42))), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i32>(%42, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%57, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%41)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%83)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%42))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @test_stpncpy_chk_range_warn(%44 p: ptr<@type0>, %45 n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%45))), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i32>(%45, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%63, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%44)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%84)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%45))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%67, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%44))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @test_strncpy_chk_range_warn(%47 p: ptr<@type0>, %48 n: i32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%48))), add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             write<i32>(%48, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64, u64) -> ptr<i8>>(%72, array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%47)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%85)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%48))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%67, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field0(deref(read<ptr<@type0>>(%47))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
