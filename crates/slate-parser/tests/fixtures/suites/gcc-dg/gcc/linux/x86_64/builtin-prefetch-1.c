/* Test that __builtin_prefetch does no harm.

   Prefetch using some invalid rw and locality values.  These must be
   compile-time constants.  */

/* { dg-do run } */

extern void exit (int);

enum locality { none, low, moderate, high, bogus };
enum rws { read, write, read_shared };

int arr[10];

void
good (int *p)
{
  __builtin_prefetch (p, 0, 0);
  __builtin_prefetch (p, 0, 1);
  __builtin_prefetch (p, 0, 2);
  __builtin_prefetch (p, 0, 3);
  __builtin_prefetch (p, 1, 0);
  __builtin_prefetch (p, 1, 1);
  __builtin_prefetch (p, 1, 2);
  __builtin_prefetch (p, 1, 3);
  __builtin_prefetch (p, 2, 0);
  __builtin_prefetch (p, 2, 1);
  __builtin_prefetch (p, 2, 2);
  __builtin_prefetch (p, 2, 3);
}

void
bad (int *p)
{
  __builtin_prefetch (p, -1, 0);  /* { dg-warning "invalid second argument to '__builtin_prefetch'; using zero" } */
  __builtin_prefetch (p, 3, 0);   /* { dg-warning "invalid second argument to '__builtin_prefetch'; using zero" } */
  __builtin_prefetch (p, bogus, 0);   /* { dg-warning "invalid second argument to '__builtin_prefetch'; using zero" } */
  __builtin_prefetch (p, 0, -1);  /* { dg-warning "invalid third argument to '__builtin_prefetch'; using zero" } */
  __builtin_prefetch (p, 0, 4);   /* { dg-warning "invalid third argument to '__builtin_prefetch'; using zero" } */
  __builtin_prefetch (p, 0, bogus);   /* { dg-warning "invalid third argument to '__builtin_prefetch'; using zero" } */
}

int
main ()
{
  good (arr);
  bad (arr);
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type0 locality = enum : u32 {
// DEFAULT-NEXT:         %0 none = const<i32>(0);
// DEFAULT-NEXT:         %1 low = const<i32>(1);
// DEFAULT-NEXT:         %2 moderate = const<i32>(2);
// DEFAULT-NEXT:         %3 high = const<i32>(3);
// DEFAULT-NEXT:         %4 bogus = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 rws = enum : u32 {
// DEFAULT-NEXT:         %0 read = const<i32>(0);
// DEFAULT-NEXT:         %1 write = const<i32>(1);
// DEFAULT-NEXT:         %2 read_shared = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %11 arr: array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%17 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %19 @__builtin_prefetch(%18 <unnamed>: ptr<const void>, ...) -> void [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %12 @good(%13 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(0), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(0), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(1), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(1), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(2), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(2), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%13)), const<i32>(2), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @bad(%15 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%15)), neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%15)), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%15)), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%15)), const<i32>(0), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%15)), const<i32>(0), const<i32>(4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%19, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%15)), const<i32>(0), const<i32>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%12, array_decay<ptr<i32>, length=Some(10)>(%11));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%14, array_decay<ptr<i32>, length=Some(10)>(%11));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
