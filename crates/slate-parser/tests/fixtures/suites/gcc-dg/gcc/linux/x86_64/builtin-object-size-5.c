/* { dg-do compile { target i?86-*-linux* i?86-*-gnu* x86_64-*-linux* } } */
/* { dg-options "-O2" } */

#ifndef N
# define N 0x40000000
#endif

typedef __SIZE_TYPE__ size_t;
extern void abort (void);
extern char buf[N];

void
test1 (size_t x)
{
  char *p = &buf[8];
  size_t i;

  for (i = 0; i < x; ++i)
    p = p + 4;
#ifdef __builtin_object_size
  if (__builtin_object_size (p, 0) != sizeof (buf) - 8 - 4 * x)
#else
  if (__builtin_object_size (p, 0) != sizeof (buf) - 8)
#endif
    abort ();
}

void
test2 (size_t x)
{
  char *p = &buf[8];
  size_t i;

  for (i = 0; i < x; ++i)
    p = p + 4;
#ifdef __builtin_object_size
  if (__builtin_object_size (p, 1) != sizeof (buf) - 8 - 4 * x)
#else
  if (__builtin_object_size (p, 1) != sizeof (buf) - 8)
#endif
    abort ();
}

void
test3 (size_t x)
{
  char *p = &buf[8];
  size_t i;

  for (i = 0; i < x; ++i)
    p = p + 4;
#ifdef __builtin_object_size
  if (__builtin_object_size (p, 2) != sizeof (buf) - 8 - 4 * x)
#else
  if (__builtin_object_size (p, 2) != 0)
#endif
    abort ();
}

void
test4 (size_t x)
{
  char *p = &buf[8];
  size_t i;

  for (i = 0; i < x; ++i)
    p = p + 4;
#ifdef __builtin_object_size
  if (__builtin_object_size (p, 3) != sizeof (buf) - 8 - 4 * x)
#else
  if (__builtin_object_size (p, 3) != 0)
#endif
    abort ();
}

void
test5 (void)
{
  char *p = &buf[0x90000004];
  if (__builtin_object_size (p + 2, 0) != 0)
    abort ();
}

void
test6 (void)
{
  char *p = &buf[-4];
  if (__builtin_object_size (p + 2, 0) != 0)
    abort ();
}

#ifdef __builtin_object_size
void
test7 (void)
{
  char *buf2 = __builtin_malloc (8);
  char *p = &buf2[0x90000004];
  if (__builtin_object_size (p + 2, 0) != 0)
    abort ();
}
#endif

/* { dg-final { scan-assembler-not "abort" } } */

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
// DEFAULT-NEXT:     extern %2 buf: array<i8, 1073741824> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %26 @__builtin_object_size(%24 <unnamed>: ptr<const void>, %25 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @test1(%4 x: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 p: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1073741824)>(%2), const<i32>(8))));
// DEFAULT-NEXT:         let %6 i: u64 [storage=automatic];
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%6, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%6), read<u64>(%4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %30: u64 [synthetic] = read<u64>(%6);
// DEFAULT-NEXT:                 let %31: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%30), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%6, read<u64>(%31));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<i8>>(%5, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%5), const<i32>(4)));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%5)), const<i32>(0)), sub<u64, overflow=wrap>(const<u64>(1073741824), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test2(%8 x: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 p: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1073741824)>(%2), const<i32>(8))));
// DEFAULT-NEXT:         let %10 i: u64 [storage=automatic];
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%10), read<u64>(%8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %32: u64 [synthetic] = read<u64>(%10);
// DEFAULT-NEXT:                 let %33: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%32), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%10, read<u64>(%33));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<i8>>(%9, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(4)));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%9)), const<i32>(1)), sub<u64, overflow=wrap>(const<u64>(1073741824), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test3(%12 x: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 p: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1073741824)>(%2), const<i32>(8))));
// DEFAULT-NEXT:         let %14 i: u64 [storage=automatic];
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%14, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%14), read<u64>(%12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %34: u64 [synthetic] = read<u64>(%14);
// DEFAULT-NEXT:                 let %35: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%34), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%14, read<u64>(%35));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<i8>>(%13, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%13), const<i32>(4)));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%13)), const<i32>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test4(%16 x: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %17 p: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1073741824)>(%2), const<i32>(8))));
// DEFAULT-NEXT:         let %18 i: u64 [storage=automatic];
// DEFAULT-NEXT:         for %29
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%18, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%18), read<u64>(%16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %36: u64 [synthetic] = read<u64>(%18);
// DEFAULT-NEXT:                 let %37: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%36), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%18, read<u64>(%37));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<i8>>(%17, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(4)));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%17)), const<i32>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20 p: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1073741824)>(%2), const<u32>(2415919108))));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%20), const<i32>(2))), const<i32>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %22 p: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1073741824)>(%2), neg<i32, overflow=ub>(const<i32>(4)))));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%22), const<i32>(2))), const<i32>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
