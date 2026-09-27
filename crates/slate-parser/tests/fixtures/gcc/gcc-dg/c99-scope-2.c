/* Test for new block scopes in C99.  Test for each new scope.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do run } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

extern void abort (void);
extern void exit (int);

int
main (void)
{
  struct foo { int i0; };
  int a, b, c, d;
  a = sizeof (struct foo);
  if (b = sizeof (struct foo { int i0; int i1; }))
    c = sizeof (struct foo { int i0; int i1; int i2; });
  if (!(a <= b && b <= c))
    abort ();
  if ((b = sizeof (struct foo { int i0; int i1; })), 0)
    c = sizeof (struct foo { int i0; int i1; int i2; });
  else
    d = sizeof (struct foo { int i0; int i1; int i2; int i3; });
  if (!(a <= b && b <= d))
    abort ();
  switch (b = sizeof (struct foo { int i0; int i1; }))
    default:
      c = sizeof (struct foo { int i0; int i1; int i2; });
  if (!(a <= b && b <= c))
    abort ();
  do
    c = sizeof (struct foo { int i0; int i1; int i2; });
  while ((b = sizeof (struct foo { int i0; int i1; })), 0);
  if (!(a <= b && b <= c))
    abort ();
  d = 1;
  while ((b = sizeof (struct foo { int i0; int i1; })), d)
    (c = sizeof (struct foo { int i0; int i1; int i2; })), d--;
  if (!(a <= b && b <= c))
    abort ();
  d = 1;
  for ((b = sizeof (struct foo { int i0; int i1; })); d; d--)
    c = sizeof (struct foo { int i0; int i1; int i2; });
  if (!(a <= b && b <= c))
    abort ();
  d = 1;
  for ((b = sizeof (struct foo { int i0; int i1; })); d; d--)
    c = sizeof (struct foo);
  if (!(a <= b && b == c))
    abort ();
  d = 1;
  for (; (b = sizeof (struct foo { int i0; int i1; })), d; d--)
    c = sizeof (struct foo { int i0; int i1; int i2; });
  if (!(a <= b && b <= c))
    abort ();
  d = 1;
  for (; (b = sizeof (struct foo { int i0; int i1; })), d; d--)
    c = sizeof (struct foo);
  if (!(a <= b && b == c))
    abort ();
  d = 1;
  for (; d; (b = sizeof (struct foo { int i0; int i1; })), d--)
    c = sizeof (struct foo { int i0; int i1; int i2; });
  if (!(a <= b && b <= c))
    abort ();
  d = 1;
  for (; d; (b = sizeof (struct foo { int i0; int i1; })), d--)
    c = sizeof (struct foo);
  if (!(a <= b && b == c))
    abort ();
  exit (0);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type3 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type4 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type5 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:         field3 i3: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     type @type6 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type7 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type8 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type9 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type10 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type11 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type12 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type13 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type14 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type15 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type16 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type17 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type18 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type19 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type20 foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%28 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 d: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%4, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), le<i32>(read<i32>(%5), read<i32>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%7, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(16))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), le<i32>(read<i32>(%5), read<i32>(%7))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         switch %29 reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8)))
// DEFAULT-NEXT:             default %29:
// DEFAULT-NEXT:                 write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), le<i32>(read<i32>(%5), read<i32>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         do %30
// DEFAULT-NEXT:             write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:             yield ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), le<i32>(read<i32>(%5), read<i32>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:         while %31 {
// DEFAULT-NEXT:             write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%7), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:             let %38: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:             let %39: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%7, read<i32>(%39));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), le<i32>(read<i32>(%5), read<i32>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:         for %32
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %40: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %41: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%41));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), le<i32>(read<i32>(%5), read<i32>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:         for %33
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %42: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%43));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), eq<i32>(read<i32>(%5), read<i32>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:         for %34
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:                 yield ne<i32>(read<i32>(%7), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %45: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%45));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), le<i32>(read<i32>(%5), read<i32>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:         for %35
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:                 yield ne<i32>(read<i32>(%7), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %46: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %47: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%47));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), eq<i32>(read<i32>(%5), read<i32>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:         for %36
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%49));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), le<i32>(read<i32>(%5), read<i32>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:         for %37
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %51: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%51));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%4), read<i32>(%5)), eq<i32>(read<i32>(%5), read<i32>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
