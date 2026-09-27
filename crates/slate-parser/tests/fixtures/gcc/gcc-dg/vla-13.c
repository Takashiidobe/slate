/* Test for VLA size evaluation; see PR 35198.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do run } */
/* { dg-options "-std=c99" } */

extern void exit (int);
extern void abort (void);

int i;
void *p;

void
f1 (void *x, int j)
{
  p = (int (*)[++i])x;
  if (i != j)
    abort ();
}

void
f1c (void *x, int j)
{
  p = (int (*)[++i]){x};
  if (i != j)
    abort ();
}

void
f2 (void *x, int j)
{
  x = (void *)(int (*)[++i])p;
  if (i != j)
    abort ();
}

void
f2c (void *x, int j)
{
  x = (void *)(int (*)[++i]){p};
  if (i != j)
    abort ();
}

void
f3 (void *x, int j)
{
  (void)(int (*)[++i])p;
  if (i != j)
    abort ();
}

void
f3c (void *x, int j)
{
  (void)(int (*)[++i]){p};
  if (i != j)
    abort ();
}

void
f4 (void *x, int j)
{
  (int (*)[++i])p;
  (int (*)[++i])p;
  if (i != j)
    abort ();
}

void
f4c (void *x, int j)
{
  (int (*)[++i]){p};
  (int (*)[++i]){p};
  if (i != j)
    abort ();
}

void
f5c (void *x, int j, int k)
{
  (++i, f3c (x, j), (int (*)[++i]){p});
  if (i != k)
    abort ();
}

int
main (void)
{
  f1 (p, 1);
  f2 (p, 2);
  f3 (p, 3);
  f4 (p, 5);
  f1c (p, 6);
  f2c (p, 7);
  f3c (p, 8);
  f4c (p, 10);
  f5c (p, 12, 13);
  exit (0);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     global %2 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 p: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%33 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @f1(%5 x: ptr<void>, %6 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %51: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %52: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%51), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%52));
// DEFAULT-NEXT:         let %34: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%52)));
// DEFAULT-NEXT:         write<ptr<void>>(%3, pointer_cast<ptr<void>, reason=assign>(pointer_cast<ptr<vla<i32, %34>>, reason=explicit>(read<ptr<void>>(%5))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f1c(%8 x: ptr<void>, %9 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %53: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %54: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%53), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%54));
// DEFAULT-NEXT:         let %35: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%54)));
// DEFAULT-NEXT:         write<ptr<void>>(%3, pointer_cast<ptr<void>, reason=assign>(read<ptr<vla<i32, %35>>>(compound_literal %36 [storage=automatic] = pointer_cast<ptr<vla<i32, %35>>, reason=assign>(read<ptr<void>>(%8)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f2(%11 x: ptr<void>, %12 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %55: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %56: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%55), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%56));
// DEFAULT-NEXT:         let %37: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%56)));
// DEFAULT-NEXT:         write<ptr<void>>(%11, pointer_cast<ptr<void>, reason=explicit>(pointer_cast<ptr<vla<i32, %37>>, reason=explicit>(read<ptr<void>>(%3))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @f2c(%14 x: ptr<void>, %15 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %57: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %58: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%57), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%58));
// DEFAULT-NEXT:         let %38: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%58)));
// DEFAULT-NEXT:         write<ptr<void>>(%14, pointer_cast<ptr<void>, reason=explicit>(read<ptr<vla<i32, %38>>>(compound_literal %39 [storage=automatic] = pointer_cast<ptr<vla<i32, %38>>, reason=assign>(read<ptr<void>>(%3)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @f3(%17 x: ptr<void>, %18 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %59: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %60: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%59), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%60));
// DEFAULT-NEXT:         let %40: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%60)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%18))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @f3c(%20 x: ptr<void>, %21 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %61: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %62: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%61), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%62));
// DEFAULT-NEXT:         let %41: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%62)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%21))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @f4(%23 x: ptr<void>, %24 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %63: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %64: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%63), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%64));
// DEFAULT-NEXT:         let %43: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%64)));
// DEFAULT-NEXT:         let %65: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %66: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%65), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%66));
// DEFAULT-NEXT:         let %44: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%66)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%24))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @f4c(%26 x: ptr<void>, %27 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %67: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %68: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%67), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%68));
// DEFAULT-NEXT:         let %45: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%68)));
// DEFAULT-NEXT:         let %69: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %70: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%69), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%70));
// DEFAULT-NEXT:         let %47: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%70)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @f5c(%29 x: ptr<void>, %30 j: i32, %31 k: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %71: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %72: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%71), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%72));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%19, read<ptr<void>>(%29), read<i32>(%30));
// DEFAULT-NEXT:         let %73: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %74: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%73), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%74));
// DEFAULT-NEXT:         let %49: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%74)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), read<i32>(%31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%4, read<ptr<void>>(%3), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%10, read<ptr<void>>(%3), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%16, read<ptr<void>>(%3), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%22, read<ptr<void>>(%3), const<i32>(5));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%7, read<ptr<void>>(%3), const<i32>(6));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%13, read<ptr<void>>(%3), const<i32>(7));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%19, read<ptr<void>>(%3), const<i32>(8));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%25, read<ptr<void>>(%3), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32, i32) -> void>(%28, read<ptr<void>>(%3), const<i32>(12), const<i32>(13));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
