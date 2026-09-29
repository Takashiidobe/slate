/* PR middle-end/88232 - Please implement -Winfinite-recursion
   Verify simple cases without optimization.
   { dg-do compile }
   { dg-options "-Wall -Winfinite-recursion" } */
/* { dg-require-effective-target nonlocal_goto } */

#define NORETURN __attribute__ ((noreturn))

typedef __SIZE_TYPE__ size_t;

extern void abort (void);
extern void exit (int);

extern int ei;
int (*pfi_v)(void);


/* Make sure the warning doesn't assume every call has a DECL.  */

int nowarn_pfi_v (void)
{
  return pfi_v ();
}


int warn_fi_v (void)                // { dg-warning "-Winfinite-recursion" }
{
  return warn_fi_v ();              // { dg-message "recursive call" }
}

/* Verify #pragma suppression works.  */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winfinite-recursion"

int suppress_warn_fi_v (void)
{
  return warn_fi_v ();
}

#pragma GCC diagnostic pop


int nowarn_fi_v (void)
{
  if (ei++ == 0)
    return nowarn_fi_v ();
  return 0;
}

int warn_if_i (int i)               // { dg-warning "-Winfinite-recursion" }
{
  if (i > 0)
    return warn_if_i (--i);         // { dg-message "recursive call" }
  else if (i < 0)
    return warn_if_i (-i);          // { dg-message "recursive call" }
  else
    return warn_if_i (7);           // { dg-message "recursive call" }
}


int nowarn_if_i (int i)
{
  if (i > 0)
    return nowarn_if_i (--i);
  else if (i < 0)
    return nowarn_if_i (-i);
  else
    return -1;
}

int nowarn_switch (int i, int a[])
{
  switch (i)
    {
    case 0: return nowarn_switch (a[3], a + 1);
    case 1: return nowarn_switch (a[5], a + 2);
    case 2: return nowarn_switch (a[7], a + 3);
    case 3: return nowarn_switch (a[9], a + 4);
    }
  return 77;
}

int warn_switch (int i, int a[])    // { dg-warning "-Winfinite-recursion" }
{
  switch (i)
    {
    case 0: return warn_switch (a[3], a + 1);
    case 1: return warn_switch (a[5], a + 2);
    case 2: return warn_switch (a[7], a + 3);
    case 3: return warn_switch (a[9], a + 4);
    default: return warn_switch (a[1], a + 5);
    }
}

NORETURN void fnoreturn (void);

/* Verify there's no warning for a function that doesn't return.  */
int nowarn_call_noret (void)
{
  fnoreturn ();
}

int warn_call_noret_r (void)        // { dg-warning "-Winfinite-recursion" }
{
  warn_call_noret_r ();             // { dg-message "recursive call" }
  fnoreturn ();
}

/* Verify a warning even though the abort() call would prevent the infinite
   recursion.  There's no good way to tell the two cases apart and letting
   a simple abort prevent the warning would make it ineffective in cases
   where it's the result of assert() expansion and not meant to actually
   prevent recursion.  */

int
warn_noret_call_abort_r (char *s, int n)  // { dg-warning "-Winfinite-recursion" }
{
  if (!s)
    abort ();

  if (n > 7)
    abort ();

  return n + warn_noret_call_abort_r (s, n - 1);  // { dg-message "recursive call" }
}

/* Verify that a warning is not issued for an apparently infinitely
   recursive function like the one above where the recursion would be
   prevented by a call to a noreturn function if the recursive function
   is itself declared noreturn.  */

NORETURN void nowarn_noret_call_abort_r (int n)
{
  if (n > 7)
    abort ();

  nowarn_noret_call_abort_r (n - 1);
}

int warn_call_abort_r (int n)       // { dg-warning "-Winfinite-recursion" }
{
  n += warn_call_abort_r (n - 1);   // { dg-message "recursive call" }
  if (n > 7)   // unreachable
    abort ();
  return n;
}


/* And again with exit() for good measure.  */

int warn_call_exit_r (int n)        // { dg-warning "-Winfinite-recursion" }
{
  n += warn_call_exit_r (n - 1);    // { dg-message "recursive call" }
  if (n > 7)
    exit (0);
  return n;
}

struct __jmp_buf_tag { };
typedef struct __jmp_buf_tag jmp_buf[1];

extern jmp_buf jmpbuf;

/* A call to longjmp() breaks infinite recursion.  Verify it suppresses
   the warning.  */

int nowarn_call_longjmp_r (int n)
{
  if (n > 7)
    __builtin_longjmp (jmpbuf, 1);
  return n + nowarn_call_longjmp_r (n - 1);
}

int warn_call_longjmp_r (int n)     // { dg-warning "-Winfinite-recursion" }
{
  n += warn_call_longjmp_r (n - 1); // { dg-message "recursive call" }
  if (n > 7)
    __builtin_longjmp (jmpbuf, 1);
  return n;
}


struct __sigjmp_buf_tag { };
typedef struct __sigjmp_buf_tag sigjmp_buf[1];

extern sigjmp_buf sigjmpbuf;

/* GCC has no __builtin_siglongjmp().  */
extern void siglongjmp (sigjmp_buf, int);

/* A call to longjmp() breaks infinite recursion.  Verify it suppresses
   the warning.  */

int nowarn_call_siglongjmp_r (int n)
{
  if (n > 7)
    siglongjmp (sigjmpbuf, 1);
  return n + nowarn_call_siglongjmp_r (n - 1);
}


int nowarn_while_do_call_r (int n)
{
  int z = 0;
  while (n)
    z += nowarn_while_do_call_r (n--);
  return z;
}

int warn_do_while_call_r (int n)    // { dg-warning "-Winfinite-recursion" }
{
  int z = 0;
  do
    z += warn_do_while_call_r (n);  // { dg-message "recursive call" }
  while (--n);
  return z;
}

/* Verify warnings for a naive replacement of a built-in fucntion.  */

void* malloc (size_t n)             // { dg-warning "-Winfinite-recursion" }
{
  size_t *p =
    (size_t*)__builtin_malloc (n + sizeof n);   // { dg-message "recursive call" }
  *p = n;
  return p + 1;
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 __jmp_buf_tag = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type2 jmp_buf = array<@type1, 1>;
// DEFAULT-NEXT:     type @type3 __sigjmp_buf_tag = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type4 sigjmp_buf = array<@type3, 1>;
// DEFAULT-NEXT:     extern %3 ei: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 pfi_v: ptr<fn() -> i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %33 jmpbuf: array<@type1, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %40 sigjmpbuf: array<@type3, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @exit(%53 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @nowarn_pfi_v() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(read<ptr<fn() -> i32>>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @warn_fi_v() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @suppress_warn_fi_v() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @nowarn_fi_v() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %65: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %66: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%65), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%66));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%65), const<i32>(0))
// DEFAULT-NEXT:             return call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @warn_if_i(%10 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:             let %67: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:             let %68: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%67), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%10, read<i32>(%68));
// DEFAULT-NEXT:             return call<i32, signature=fn(i32) -> i32>(%9, read<i32>(%68));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if lt<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:                 return call<i32, signature=fn(i32) -> i32>(%9, neg<i32, overflow=ub>(read<i32>(%10)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 return call<i32, signature=fn(i32) -> i32>(%9, const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @nowarn_if_i(%12 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%12), const<i32>(0))
// DEFAULT-NEXT:             let %69: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:             let %70: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%69), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%12, read<i32>(%70));
// DEFAULT-NEXT:             return call<i32, signature=fn(i32) -> i32>(%11, read<i32>(%70));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if lt<i32>(read<i32>(%12), const<i32>(0))
// DEFAULT-NEXT:                 return call<i32, signature=fn(i32) -> i32>(%11, neg<i32, overflow=ub>(read<i32>(%12)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @nowarn_switch(%14 i: i32, %15 a: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %54 read<i32>(%14)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %54 const<i32>(0):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%13, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(3)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(1)));
// DEFAULT-NEXT:                 case %54 const<i32>(1):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%13, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(5)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(2)));
// DEFAULT-NEXT:                 case %54 const<i32>(2):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%13, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(7)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(3)));
// DEFAULT-NEXT:                 case %54 const<i32>(3):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%13, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(9)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(4)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(77);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @warn_switch(%17 i: i32, %18 a: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %55 read<i32>(%17)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %55 const<i32>(0):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%16, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(3)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(1)));
// DEFAULT-NEXT:                 case %55 const<i32>(1):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%16, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(5)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(2)));
// DEFAULT-NEXT:                 case %55 const<i32>(2):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%16, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(7)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(3)));
// DEFAULT-NEXT:                 case %55 const<i32>(3):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%16, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(9)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(4)));
// DEFAULT-NEXT:                 default %55:
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%16, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(1)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(5)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @fnoreturn() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %20 @nowarn_call_noret() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @warn_call_noret_r() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @warn_noret_call_abort_r(%23 s: ptr<i8>, %24 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<i8>>(read<ptr<i8>>(%23), null<ptr<i8>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%24), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%24), call<i32, signature=fn(ptr<i8>, i32) -> i32>(%22, read<ptr<i8>>(%23), sub<i32, overflow=ub>(read<i32>(%24), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @nowarn_noret_call_abort_r(%26 n: i32) -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%26), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%25, sub<i32, overflow=ub>(read<i32>(%26), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @warn_call_abort_r(%28 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %71: i32 [synthetic] = read<i32>(%28);
// DEFAULT-NEXT:         let %72: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%71), call<i32, signature=fn(i32) -> i32>(%27, sub<i32, overflow=ub>(read<i32>(%28), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%28, read<i32>(%72));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%28), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return read<i32>(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @warn_call_exit_r(%30 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %73: i32 [synthetic] = read<i32>(%30);
// DEFAULT-NEXT:         let %74: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%73), call<i32, signature=fn(i32) -> i32>(%29, sub<i32, overflow=ub>(read<i32>(%30), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%30, read<i32>(%74));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%30), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%30);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @__builtin_longjmp(%56 <unnamed>: ptr<void>, %57 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %34 @nowarn_call_longjmp_r(%35 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%35), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, i32) -> void>(%58, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type1>, length=Some(1)>(%33)), const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%35), call<i32, signature=fn(i32) -> i32>(%34, sub<i32, overflow=ub>(read<i32>(%35), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @warn_call_longjmp_r(%37 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %75: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:         let %76: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%75), call<i32, signature=fn(i32) -> i32>(%36, sub<i32, overflow=ub>(read<i32>(%37), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%37, read<i32>(%76));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%37), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, i32) -> void>(%58, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type1>, length=Some(1)>(%33)), const<i32>(1));
// DEFAULT-NEXT:         return read<i32>(%37);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @siglongjmp(%59 <unnamed>: ptr<@type3> [array=1], %60 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %42 @nowarn_call_siglongjmp_r(%43 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%43), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type3>, i32) -> void>(%41, array_decay<ptr<@type3>, length=Some(1)>(%40), const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%43), call<i32, signature=fn(i32) -> i32>(%42, sub<i32, overflow=ub>(read<i32>(%43), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @nowarn_while_do_call_r(%45 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %46 z: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %61 ne<i32>(read<i32>(%45), const<i32>(0))
// DEFAULT-NEXT:             let %77: i32 [synthetic] = read<i32>(%46);
// DEFAULT-NEXT:             let %78: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:             let %79: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%78), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%45, read<i32>(%79));
// DEFAULT-NEXT:             let %80: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%77), call<i32, signature=fn(i32) -> i32>(%44, read<i32>(%78)));
// DEFAULT-NEXT:             write<i32>(%46, read<i32>(%80));
// DEFAULT-NEXT:         return read<i32>(%46);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @warn_do_while_call_r(%48 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %49 z: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         do %62
// DEFAULT-NEXT:             let %81: i32 [synthetic] = read<i32>(%49);
// DEFAULT-NEXT:             let %82: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%81), call<i32, signature=fn(i32) -> i32>(%47, read<i32>(%48)));
// DEFAULT-NEXT:             write<i32>(%49, read<i32>(%82));
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             let %83: i32 [synthetic] = read<i32>(%48);
// DEFAULT-NEXT:             let %84: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%83), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%48, read<i32>(%84));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%84), const<i32>(0));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:         return read<i32>(%49);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @__builtin_malloc(%63 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %50 @malloc(%51 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %52 p: ptr<u64> [storage=automatic] = pointer_cast<ptr<u64>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%64, add<u64, overflow=wrap>(read<u64>(%51), const<u64>(8))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%52)), read<u64>(%51));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%52), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
