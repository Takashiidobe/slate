/* PR middle-end/88232 - Please implement -Winfinite-recursion
   Exercise warning with optimization.  Same as -Winfinite-recursion.c
   plus mutually recursive calls that depend on inlining.
   { dg-do compile }
   { dg-options "-O2 -Wall -Winfinite-recursion" }
   { dg-require-effective-target nonlocal_goto } */

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


int nowarn_fact (int n)
{
  return n ? n * nowarn_fact (n - 1) : 1;
}


static int fi_v (void);

/* It would seem preferable to issue the warning for the extern function
   but as it happens it's the static function that's inlined into a recursive
   call to itself and warn_call_fi_v() expands to a call to it.  */

int warn_call_fi_v (void)     // { dg-warning "-Winfinite-recursion" "" { xfail *-*-* } }
{
  return fi_v ();             // { dg-message "recursive call" }
}

static int fi_v (void)        // { dg-warning "-Winfinite-recursion" }
{
  return warn_call_fi_v ();
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
// DEFAULT-NEXT:     type @type[[TYPE___jmp_buf_tag:[0-9]+]] __jmp_buf_tag = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_jmp_buf:[0-9]+]] jmp_buf = array<@type[[TYPE___jmp_buf_tag]], 1>;
// DEFAULT-NEXT:     type @type[[TYPE___sigjmp_buf_tag:[0-9]+]] __sigjmp_buf_tag = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_sigjmp_buf:[0-9]+]] sigjmp_buf = array<@type[[TYPE___sigjmp_buf_tag]], 1>;
// DEFAULT-NEXT:     extern %[[VALUE_ei:[0-9]+]] ei: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pfi_v:[0-9]+]] pfi_v: ptr<fn() -> i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_jmpbuf:[0-9]+]] jmpbuf: array<@type[[TYPE___jmp_buf_tag]], 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_sigjmpbuf:[0-9]+]] sigjmpbuf: array<@type[[TYPE___sigjmp_buf_tag]], 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_pfi_v:[0-9]+]] @nowarn_pfi_v() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(read<ptr<fn() -> i32>>(%[[VALUE_pfi_v]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_fi_v:[0-9]+]] @warn_fi_v() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(%[[VALUE_warn_fi_v]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_suppress_warn_fi_v:[0-9]+]] @suppress_warn_fi_v() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(%[[VALUE_warn_fi_v]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_fi_v:[0-9]+]] @nowarn_fi_v() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_ei]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_ei]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE1]]), const<i32>(0))
// DEFAULT-NEXT:             return call<i32, signature=fn() -> i32>(%[[VALUE_nowarn_fi_v]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_if_i:[0-9]+]] @warn_if_i(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:             return call<i32, signature=fn(i32) -> i32>(%[[VALUE_warn_if_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:                 return call<i32, signature=fn(i32) -> i32>(%[[VALUE_warn_if_i]], neg<i32, overflow=ub>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 return call<i32, signature=fn(i32) -> i32>(%[[VALUE_warn_if_i]], const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_if_i:[0-9]+]] @nowarn_if_i(%[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:             return call<i32, signature=fn(i32) -> i32>(%[[VALUE_nowarn_if_i]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0))
// DEFAULT-NEXT:                 return call<i32, signature=fn(i32) -> i32>(%[[VALUE_nowarn_if_i]], neg<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]])));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_switch:[0-9]+]] @nowarn_switch(%[[VALUE_i_3:[0-9]+]] i: i32, %[[VALUE_a:[0-9]+]] a: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE7:[0-9]+]] read<i32>(%[[VALUE_i_3]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE7]] const<i32>(0):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_nowarn_switch]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), const<i32>(3)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), const<i32>(1)));
// DEFAULT-NEXT:                 case %[[VALUE7]] const<i32>(1):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_nowarn_switch]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), const<i32>(5)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), const<i32>(2)));
// DEFAULT-NEXT:                 case %[[VALUE7]] const<i32>(2):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_nowarn_switch]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), const<i32>(7)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), const<i32>(3)));
// DEFAULT-NEXT:                 case %[[VALUE7]] const<i32>(3):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_nowarn_switch]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), const<i32>(9)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), const<i32>(4)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(77);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_switch:[0-9]+]] @warn_switch(%[[VALUE_i_4:[0-9]+]] i: i32, %[[VALUE_a_2:[0-9]+]] a: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE8:[0-9]+]] read<i32>(%[[VALUE_i_4]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE8]] const<i32>(0):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_warn_switch]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_2]]), const<i32>(3)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_2]]), const<i32>(1)));
// DEFAULT-NEXT:                 case %[[VALUE8]] const<i32>(1):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_warn_switch]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_2]]), const<i32>(5)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_2]]), const<i32>(2)));
// DEFAULT-NEXT:                 case %[[VALUE8]] const<i32>(2):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_warn_switch]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_2]]), const<i32>(7)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_2]]), const<i32>(3)));
// DEFAULT-NEXT:                 case %[[VALUE8]] const<i32>(3):
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_warn_switch]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_2]]), const<i32>(9)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_2]]), const<i32>(4)));
// DEFAULT-NEXT:                 default %[[VALUE8]]:
// DEFAULT-NEXT:                     return call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_warn_switch]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_2]]), const<i32>(1)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_2]]), const<i32>(5)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fnoreturn:[0-9]+]] @fnoreturn() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_call_noret:[0-9]+]] @nowarn_call_noret() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fnoreturn]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_call_noret_r:[0-9]+]] @warn_call_noret_r() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_warn_call_noret_r]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fnoreturn]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_noret_call_abort_r:[0-9]+]] @warn_noret_call_abort_r(%[[VALUE_s:[0-9]+]] s: ptr<i8>, %[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_s]]), null<ptr<i8>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_n]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_n]]), call<i32, signature=fn(ptr<i8>, i32) -> i32>(%[[VALUE_warn_noret_call_abort_r]], read<ptr<i8>>(%[[VALUE_s]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_n]]), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_noret_call_abort_r:[0-9]+]] @nowarn_noret_call_abort_r(%[[VALUE_n_2:[0-9]+]] n: i32) -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_n_2]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_nowarn_noret_call_abort_r]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_n_2]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_call_abort_r:[0-9]+]] @warn_call_abort_r(%[[VALUE_n_3:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_3]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_warn_call_abort_r]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_n_3]]), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n_3]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_n_3]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_n_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_call_exit_r:[0-9]+]] @warn_call_exit_r(%[[VALUE_n_4:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_4]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_warn_call_exit_r]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_n_4]]), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n_4]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_n_4]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_n_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_longjmp:[0-9]+]] @__builtin_longjmp(%[[VALUE13:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE14:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_call_longjmp_r:[0-9]+]] @nowarn_call_longjmp_r(%[[VALUE_n_5:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_n_5]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE___builtin_longjmp]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_jmpbuf]])), const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_n_5]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_nowarn_call_longjmp_r]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_n_5]]), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_call_longjmp_r:[0-9]+]] @warn_call_longjmp_r(%[[VALUE_n_6:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_6]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_warn_call_longjmp_r]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_n_6]]), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n_6]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_n_6]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE___builtin_longjmp]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_jmpbuf]])), const<i32>(1));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_n_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_siglongjmp:[0-9]+]] @siglongjmp(%[[VALUE17:[0-9]+]] <unnamed>: ptr<@type[[TYPE___sigjmp_buf_tag]]> [array=1], %[[VALUE18:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_call_siglongjmp_r:[0-9]+]] @nowarn_call_siglongjmp_r(%[[VALUE_n_7:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_n_7]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE___sigjmp_buf_tag]]>, i32) -> void>(%[[VALUE_siglongjmp]], array_decay<ptr<@type[[TYPE___sigjmp_buf_tag]]>, length=Some(1)>(%[[VALUE_sigjmpbuf]]), const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_n_7]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_nowarn_call_siglongjmp_r]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_n_7]]), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_while_do_call_r:[0-9]+]] @nowarn_while_do_call_r(%[[VALUE_n_8:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %[[VALUE19:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_n_8]]), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_z]]);
// DEFAULT-NEXT:             let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_8]]);
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_8]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_nowarn_while_do_call_r]], read<i32>(%[[VALUE21]])));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_z]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_z]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_do_while_call_r:[0-9]+]] @warn_do_while_call_r(%[[VALUE_n_9:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z_2:[0-9]+]] z: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:             let %[[VALUE25:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_z_2]]);
// DEFAULT-NEXT:             let %[[VALUE26:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE25]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_warn_do_while_call_r]], read<i32>(%[[VALUE_n_9]])));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_z_2]], read<i32>(%[[VALUE26]]));
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_9]]);
// DEFAULT-NEXT:             let %[[VALUE28:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE27]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_9]], read<i32>(%[[VALUE28]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE28]]), const<i32>(0));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_z_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE29:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE_n_10:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<u64> [storage=automatic] = pointer_cast<ptr<u64>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_n_10]]), const<u64>(8))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%[[VALUE_p]])), read<u64>(%[[VALUE_n_10]]));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_p]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn_fact:[0-9]+]] @nowarn_fact(%[[VALUE_n_11:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_n_11]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE30]], mul<i32, overflow=ub>(read<i32>(%[[VALUE_n_11]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_nowarn_fact]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_n_11]]), const<i32>(1)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE30]], const<i32>(1));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE30]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fi_v:[0-9]+]] @fi_v() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(%[[VALUE_warn_call_fi_v:[0-9]+]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn_call_fi_v]] @warn_call_fi_v() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(%[[VALUE_fi_v]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
