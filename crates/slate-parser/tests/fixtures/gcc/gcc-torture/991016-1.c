/* Two of these types will, on current gcc targets, have the same
   mode but have different alias sets.  DOIT tries to get gcse to
   invalidly hoist one of the values out of the loop.  */

void abort(void);
void exit(int);

typedef int       T0;
typedef long      T1;
typedef long long T2;

int doit(int sel, int n, void *p) {
  T0 *const p0 = p;
  T1 *const p1 = p;
  T2 *const p2 = p;

  switch (sel) {
  case 0:
    do
      *p0 += *p0;
    while (--n);
    return *p0 == 0;

  case 1:
    do
      *p1 += *p1;
    while (--n);
    return *p1 == 0;

  case 2:
    do
      *p2 += *p2;
    while (--n);
    return *p2 == 0;

  default:
    abort();
  }
}

int main() {
  T0 v0;
  T1 v1;
  T2 v2;

  v0 = 1;
  doit(0, 5, &v0);
  v1 = 1;
  doit(1, 5, &v1);
  v2 = 1;
  doit(2, 5, &v2);

  if (v0 != 32)
    abort();
  if (v1 != 32)
    abort();
  if (v2 != 32)
    abort();

  exit(0);
}


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
// DEFAULT-NEXT:     type @type0 T0 = i32;
// DEFAULT-NEXT:     type @type1 T1 = i64;
// DEFAULT-NEXT:     type @type2 T2 = i64;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%16 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @doit(%6 sel: i32, %7 n: i32, %8 p: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 p0: ptr<i32> [storage=automatic] [const] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<void>>(%8));
// DEFAULT-NEXT:         let %10 p1: ptr<i64> [storage=automatic] [const] = pointer_cast<ptr<i64>, reason=assign>(read<ptr<void>>(%8));
// DEFAULT-NEXT:         let %11 p2: ptr<i64> [storage=automatic] [const] = pointer_cast<ptr<i64>, reason=assign>(read<ptr<void>>(%8));
// DEFAULT-NEXT:         switch %17 read<i32>(%6)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %17 const<i32>(0):
// DEFAULT-NEXT:                     do %18
// DEFAULT-NEXT:                         let %21: ptr<i32> [synthetic] = read<ptr<i32>>(%9);
// DEFAULT-NEXT:                         let %22: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%21)));
// DEFAULT-NEXT:                         let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), read<i32>(deref(read<ptr<i32>>(%9))));
// DEFAULT-NEXT:                         write<i32>(deref(read<ptr<i32>>(%21)), read<i32>(%23));
// DEFAULT-NEXT:                     while {
// DEFAULT-NEXT:                         let %24: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                         let %25: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%7, read<i32>(%25));
// DEFAULT-NEXT:                         yield ne<i32>(read<i32>(%25), const<i32>(0));
// DEFAULT-NEXT:                     };
// DEFAULT-NEXT:                 return from_bool<i32, reason=return>(eq<i32>(read<i32>(deref(read<ptr<i32>>(%9))), const<i32>(0)));
// DEFAULT-NEXT:                 case %17 const<i32>(1):
// DEFAULT-NEXT:                     do %19
// DEFAULT-NEXT:                         let %26: ptr<i64> [synthetic] = read<ptr<i64>>(%10);
// DEFAULT-NEXT:                         let %27: i64 [synthetic] = read<i64>(deref(read<ptr<i64>>(%26)));
// DEFAULT-NEXT:                         let %28: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%27), read<i64>(deref(read<ptr<i64>>(%10))));
// DEFAULT-NEXT:                         write<i64>(deref(read<ptr<i64>>(%26)), read<i64>(%28));
// DEFAULT-NEXT:                     while {
// DEFAULT-NEXT:                         let %29: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                         let %30: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%7, read<i32>(%30));
// DEFAULT-NEXT:                         yield ne<i32>(read<i32>(%30), const<i32>(0));
// DEFAULT-NEXT:                     };
// DEFAULT-NEXT:                 return from_bool<i32, reason=return>(eq<i64>(read<i64>(deref(read<ptr<i64>>(%10))), widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:                 case %17 const<i32>(2):
// DEFAULT-NEXT:                     do %20
// DEFAULT-NEXT:                         let %31: ptr<i64> [synthetic] = read<ptr<i64>>(%11);
// DEFAULT-NEXT:                         let %32: i64 [synthetic] = read<i64>(deref(read<ptr<i64>>(%31)));
// DEFAULT-NEXT:                         let %33: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%32), read<i64>(deref(read<ptr<i64>>(%11))));
// DEFAULT-NEXT:                         write<i64>(deref(read<ptr<i64>>(%31)), read<i64>(%33));
// DEFAULT-NEXT:                     while {
// DEFAULT-NEXT:                         let %34: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                         let %35: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%7, read<i32>(%35));
// DEFAULT-NEXT:                         yield ne<i32>(read<i32>(%35), const<i32>(0));
// DEFAULT-NEXT:                     };
// DEFAULT-NEXT:                 return from_bool<i32, reason=return>(eq<i64>(read<i64>(deref(read<ptr<i64>>(%11))), widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:                 default %17:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 v0: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 v1: i64 [storage=automatic];
// DEFAULT-NEXT:         let %15 v2: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%13, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, ptr<void>) -> i32>(%5, const<i32>(0), const<i32>(5), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%13)));
// DEFAULT-NEXT:         write<i64>(%14, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, ptr<void>) -> i32>(%5, const<i32>(1), const<i32>(5), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i64>>(%14)));
// DEFAULT-NEXT:         write<i64>(%15, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, ptr<void>) -> i32>(%5, const<i32>(2), const<i32>(5), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i64>>(%15)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%13), const<i32>(32))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%14), widen<i64, reason=usual_arith>(const<i32>(32)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%15), widen<i64, reason=usual_arith>(const<i32>(32)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
