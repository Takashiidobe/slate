/* { dg-do compile } */
/* { dg-options "-Wcalloc-transposed-args" } */

typedef __SIZE_TYPE__ size_t;
void free (void *);
void *calloc (size_t, size_t);
void *myfree (void *, int, int);
void *mycalloc (int, int, size_t, size_t) __attribute__((malloc, malloc (myfree), alloc_size (3, 4)));

void
foo (int n)
{
  void *p;
  p = __builtin_calloc (1, sizeof (int));
  __builtin_free (p);
  p = __builtin_calloc (n, sizeof (int));
  __builtin_free (p);
  p = __builtin_calloc (sizeof (int), 1);		/* { dg-warning "'__builtin_calloc' sizes specified with 'sizeof' in the earlier argument and not in the later argument" } */
  __builtin_free (p);					/* { dg-message "earlier argument should specify number of elements, later size of each element" "" { target *-*-* } .-1 } */
  p = __builtin_calloc (sizeof (int), n);		/* { dg-warning "'__builtin_calloc' sizes specified with 'sizeof' in the earlier argument and not in the later argument" } */
  __builtin_free (p);					/* { dg-message "earlier argument should specify number of elements, later size of each element" "" { target *-*-* } .-1 } */
  p = __builtin_calloc ((sizeof (int)), 1);		/* { dg-warning "'__builtin_calloc' sizes specified with 'sizeof' in the earlier argument and not in the later argument" } */
  __builtin_free (p);					/* { dg-message "earlier argument should specify number of elements, later size of each element" "" { target *-*-* } .-1 } */
  p = __builtin_calloc (sizeof (int) + 0, 1);
  __builtin_free (p);
  p = __builtin_calloc (sizeof (int), sizeof (char));
  __builtin_free (p);
  p = __builtin_calloc (1 * sizeof (int), 1);
  __builtin_free (p);
  p = calloc (1, sizeof (int));
  free (p);
  p = calloc (n, sizeof (int));
  free (p);
  p = calloc (sizeof (int), 1);				/* { dg-warning "'calloc' sizes specified with 'sizeof' in the earlier argument and not in the later argument" } */
  free (p);						/* { dg-message "earlier argument should specify number of elements, later size of each element" "" { target *-*-* } .-1 } */
  p = calloc (sizeof (int), n);				/* { dg-warning "'calloc' sizes specified with 'sizeof' in the earlier argument and not in the later argument" } */
  free (p);						/* { dg-message "earlier argument should specify number of elements, later size of each element" "" { target *-*-* } .-1 } */
  p = calloc (sizeof (int), sizeof (char));
  free (p);
  p = calloc (1 * sizeof (int), 1);
  free (p);
  p = mycalloc (42, 42, 1, sizeof (int));
  myfree (p, 42, 42);
  p = mycalloc (42, 42, n, sizeof (int));
  myfree (p, 42, 42);
  p = mycalloc (42, 42, sizeof (int), 1);		/* { dg-warning "'mycalloc' sizes specified with 'sizeof' in the earlier argument and not in the later argument" } */
  myfree (p, 42, 42);					/* { dg-message "earlier argument should specify number of elements, later size of each element" "" { target *-*-* } .-1 } */
  p = mycalloc (42, 42, sizeof (int), n);		/* { dg-warning "'mycalloc' sizes specified with 'sizeof' in the earlier argument and not in the later argument" } */
  myfree (p, 42, 42);					/* { dg-message "earlier argument should specify number of elements, later size of each element" "" { target *-*-* } .-1 } */
  p = mycalloc (42, 42, sizeof (int), sizeof (char));
  myfree (p, 42, 42);
  p = mycalloc (42, 42, 1 * sizeof (int), 1);
  myfree (p, 42, 42);
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
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_calloc:[0-9]+]] @calloc(%[[VALUE1:[0-9]+]] <unnamed>: u64, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_myfree:[0-9]+]] @myfree(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE5:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mycalloc:[0-9]+]] @mycalloc(%[[VALUE6:[0-9]+]] <unnamed>: i32, %[[VALUE7:[0-9]+]] <unnamed>: i32, %[[VALUE8:[0-9]+]] <unnamed>: u64, %[[VALUE9:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [deallocator=%[[VALUE_myfree]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_calloc:[0-9]+]] @__builtin_calloc(%[[VALUE10:[0-9]+]] <unnamed>: u64, %[[VALUE11:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_free:[0-9]+]] @__builtin_free(%[[VALUE12:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(4)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]]))), const<u64>(4)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]]))), const<u64>(4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]])))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], const<u64>(4), const<u64>(1)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], const<u64>(4), const<u64>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(4)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]]))), const<u64>(4)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]]))), const<u64>(4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]])))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], const<u64>(4), const<u64>(1)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], const<u64>(4), const<u64>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(4)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(4));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]]))), const<u64>(4)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]]))), const<u64>(4));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]])))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), const<u64>(4), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]]))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), const<u64>(4), const<u64>(1)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), const<u64>(4), const<u64>(1));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
