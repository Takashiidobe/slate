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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_2:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_3:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_4:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_5:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_6:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:         field3 i3: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_7:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_8:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_9:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_10:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_11:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_12:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_13:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_14:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_15:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_16:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_17:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_18:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_19:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_20:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:         field2 i2: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_21:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 i0: i32;
// DEFAULT-NEXT:         field1 i1: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), le<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE_d]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(16))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), le<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_d]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8)))
// DEFAULT-NEXT:             default %[[VALUE1]]:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), le<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:             yield ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), le<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:         while %[[VALUE3:[0-9]+]] {
// DEFAULT-NEXT:             write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), le<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), le<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), eq<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:                 yield ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), le<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:         for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:                 yield ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), eq<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:         for %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(12))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), le<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:         for %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(le<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), eq<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
