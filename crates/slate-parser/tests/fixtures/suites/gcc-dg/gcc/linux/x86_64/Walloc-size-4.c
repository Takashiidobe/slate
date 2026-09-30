/* Tests the warnings for insufficient allocation size.  */
/* { dg-do compile } */
/* { dg-options "-Walloc-size -Wno-calloc-transposed-args" } */

struct S { int x[10]; };
void bar (struct S *);
typedef __SIZE_TYPE__ size_t;
void *myfree (void *, int, int);
void *mymalloc (int, int, size_t) __attribute__((malloc, malloc (myfree), alloc_size (3)));
void *mycalloc (int, int, size_t, size_t) __attribute__((malloc, malloc (myfree), alloc_size (3, 4)));

void
foo (void)
{
  struct S *p = (struct S *) __builtin_malloc (sizeof *p);
  __builtin_free (p);
  p = (struct S *) __builtin_malloc (sizeof p);		/* { dg-warning "allocation of insufficient size" } */
  __builtin_free (p);
  p = (struct S *) __builtin_alloca (sizeof p);		/* { dg-warning "allocation of insufficient size" } */
  bar (p);
  p = (struct S *) __builtin_calloc (1, sizeof p);	/* { dg-warning "allocation of insufficient size" } */
  __builtin_free (p);
  bar ((struct S *) __builtin_malloc (4));		/* { dg-warning "allocation of insufficient size" } */
  __builtin_free (p);
  p = (struct S *) __builtin_calloc (sizeof *p, 1);
  __builtin_free (p);
  p = __builtin_calloc (sizeof *p, 1);
  __builtin_free (p);
}

void
baz (void)
{
  struct S *p = (struct S *) mymalloc (42, 42, sizeof *p);
  myfree (p, 42, 42);
  p = (struct S *) mymalloc (42, 42, sizeof p);		/* { dg-warning "allocation of insufficient size" } */
  myfree (p, 42, 42);
  p = (struct S *) mycalloc (42, 42, 1, sizeof p);	/* { dg-warning "allocation of insufficient size" } */
  myfree (p, 42, 42);
  bar ((struct S *) mymalloc (42, 42, 4));		/* { dg-warning "allocation of insufficient size" } */
  myfree (p, 42, 42);
  p = (struct S *) mycalloc (42, 42, sizeof *p, 1);
  myfree (p, 42, 42);
  p = mycalloc (42, 42, sizeof *p, 1);
  myfree (p, 42, 42);
  p = mymalloc (42, 42, sizeof *p);
  myfree (p, 42, 42);
  p = mymalloc (42, 42, sizeof p);			/* { dg-warning "allocation of insufficient size" } */
  myfree (p, 42, 42);
  p = mycalloc (42, 42, 1, sizeof p);			/* { dg-warning "allocation of insufficient size" } */
  myfree (p, 42, 42);
  bar (mymalloc (42, 42, 4));				/* { dg-warning "allocation of insufficient size" } */
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 x: array<i32, 10>;
// DEFAULT-NEXT:     } [size=40, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_S]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_myfree:[0-9]+]] @myfree(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mymalloc:[0-9]+]] @mymalloc(%[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE5:[0-9]+]] <unnamed>: i32, %[[VALUE6:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [deallocator=%[[VALUE_myfree]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_mycalloc:[0-9]+]] @mycalloc(%[[VALUE7:[0-9]+]] <unnamed>: i32, %[[VALUE8:[0-9]+]] <unnamed>: i32, %[[VALUE9:[0-9]+]] <unnamed>: u64, %[[VALUE10:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [deallocator=%[[VALUE_myfree]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE11:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_free:[0-9]+]] @__builtin_free(%[[VALUE12:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE13:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_calloc:[0-9]+]] @__builtin_calloc(%[[VALUE14:[0-9]+]] <unnamed>: u64, %[[VALUE15:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], const<u64>(40)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], const<u64>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], const<u64>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_bar]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_bar]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], const<u64>(40), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=assign>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], const<u64>(40), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_S]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(i32, i32, u64) -> ptr<void>>(%[[VALUE_mymalloc]], const<i32>(42), const<i32>(42), const<u64>(40)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(i32, i32, u64) -> ptr<void>>(%[[VALUE_mymalloc]], const<i32>(42), const<i32>(42), const<u64>(8))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_bar]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(i32, i32, u64) -> ptr<void>>(%[[VALUE_mymalloc]], const<i32>(42), const<i32>(42), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), const<u64>(40), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=assign>(call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), const<u64>(40), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=assign>(call<ptr<void>, signature=fn(i32, i32, u64) -> ptr<void>>(%[[VALUE_mymalloc]], const<i32>(42), const<i32>(42), const<u64>(40))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=assign>(call<ptr<void>, signature=fn(i32, i32, u64) -> ptr<void>>(%[[VALUE_mymalloc]], const<i32>(42), const<i32>(42), const<u64>(8))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=assign>(call<ptr<void>, signature=fn(i32, i32, u64, u64) -> ptr<void>>(%[[VALUE_mycalloc]], const<i32>(42), const<i32>(42), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(8))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_bar]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=arg>(call<ptr<void>, signature=fn(i32, i32, u64) -> ptr<void>>(%[[VALUE_mymalloc]], const<i32>(42), const<i32>(42), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, i32) -> ptr<void>>(%[[VALUE_myfree]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]])), const<i32>(42), const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
