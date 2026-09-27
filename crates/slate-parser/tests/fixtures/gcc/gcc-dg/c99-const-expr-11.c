/* Test for constant expressions: cases involving VLAs.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

/* It appears address constants may contain casts to variably modified
   types.  Whether they should be permitted was discussed in
   <http://groups.google.com/group/comp.std.c/msg/923eee5ab690fd98>
   <LV7g2Vy3ARF$Ew9Q@romana.davros.org>; since static pointers to VLAs
   are definitely permitted within functions and may be initialized
   and such initialization involves implicit conversion to a variably
   modified type, allowing explicit casts seems appropriate.  Thus,
   GCC allows them as long as the "evaluated" size expressions do not
   contain the various operators not permitted to be evaluated in a
   constant expression, and as long as the result is genuinely
   constant (meaning that pointer arithmetic using the size of the VLA
   is generally not permitted).  */

static int sa[100];

volatile int nv;

int
f (int m, int n)
{
  static int (*a1)[n] = &sa;
  static int (*a2)[n] = (int (*)[n])sa;
  static int (*a3)[n] = (int (*)[(int){n}])sa;
  static int (*a4)[n] = (int (*)[(int){m++}])sa; /* { dg-error "constant" } */
  static int (*a5)[n] = (int (*)[(int){++m}])sa; /* { dg-error "constant" } */
  static int (*a6)[n] = (int (*)[(int){m--}])sa; /* { dg-error "constant" } */
  static int (*a7)[n] = (int (*)[(int){--m}])sa; /* { dg-error "constant" } */
  static int (*a8)[n] = (int (*)[(m=n)])sa; /* { dg-error "constant" } */
  static int (*a9)[n] = (int (*)[(m+=n)])sa; /* { dg-error "constant" } */
  static int (*a10)[n] = (int (*)[f(m,n)])sa; /* { dg-error "constant" } */
  static int (*a11)[n] = (int (*)[(m,n)])sa; /* { dg-error "constant" } */
  static int (*a12)[n] = (int (*)[sizeof(int[n])])sa;
  static int (*a13)[n] = (int (*)[sizeof(int[m++])])sa; /* { dg-error "constant" } */
  static int (*a14)[n] = (int (*)[sizeof(*a1)])sa;
  static int (*a15)[n] = (int (*)[sizeof(*(int (*)[n])sa)])sa;
  static int (*a16)[n] = (int (*)[sizeof(*(int (*)[m++])sa)])sa; /* { dg-error "constant" } */
  static int (*a17)[n] = (int (*)[nv])sa;
  typedef int (*vmt)[m++];
  static int (*a18)[n] = (vmt)sa;
  return n;
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
// DEFAULT-NEXT:     type @type0 vmt = ptr<vla<i32, %66>>;
// DEFAULT-NEXT:     global %0 sa: array<i32, 100> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %1 nv: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 a1: ptr<vla<i32, %24>> [storage=static] = pointer_cast<ptr<vla<i32, %24>>, reason=assign>(addr_of<ptr<array<i32, 100>>>(%0)) [linkage=internal];
// DEFAULT-NEXT:     global %6 a2: ptr<vla<i32, %25>> [storage=static] = pointer_cast<ptr<vla<i32, %25>>, reason=assign>(capture<%26>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4))), pointer_cast<ptr<vla<i32, %26>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %7 a3: ptr<vla<i32, %27>> [storage=static] = pointer_cast<ptr<vla<i32, %27>>, reason=assign>(capture<%29>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(compound_literal %28 [storage=automatic] = read<i32>(%4)))), pointer_cast<ptr<vla<i32, %29>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %8 a4: ptr<vla<i32, %30>> [storage=static] = pointer_cast<ptr<vla<i32, %30>>, reason=assign>(capture<%32>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(compound_literal %31 [storage=automatic] = update<i32, result=old>(%3, add<i32, overflow=ub>(old<i32>, const<i32>(1)))))), pointer_cast<ptr<vla<i32, %32>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %9 a5: ptr<vla<i32, %33>> [storage=static] = pointer_cast<ptr<vla<i32, %33>>, reason=assign>(capture<%35>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(compound_literal %34 [storage=automatic] = update<i32, result=new>(%3, add<i32, overflow=ub>(old<i32>, const<i32>(1)))))), pointer_cast<ptr<vla<i32, %35>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %10 a6: ptr<vla<i32, %36>> [storage=static] = pointer_cast<ptr<vla<i32, %36>>, reason=assign>(capture<%38>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(compound_literal %37 [storage=automatic] = update<i32, result=old>(%3, sub<i32, overflow=ub>(old<i32>, const<i32>(1)))))), pointer_cast<ptr<vla<i32, %38>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %11 a7: ptr<vla<i32, %39>> [storage=static] = pointer_cast<ptr<vla<i32, %39>>, reason=assign>(capture<%41>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(compound_literal %40 [storage=automatic] = update<i32, result=new>(%3, sub<i32, overflow=ub>(old<i32>, const<i32>(1)))))), pointer_cast<ptr<vla<i32, %41>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %12 a8: ptr<vla<i32, %42>> [storage=static] = pointer_cast<ptr<vla<i32, %42>>, reason=assign>(capture<%43>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(store<i32>(%3, read<i32>(%4)))), pointer_cast<ptr<vla<i32, %43>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %13 a9: ptr<vla<i32, %44>> [storage=static] = pointer_cast<ptr<vla<i32, %44>>, reason=assign>(capture<%45>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(update<i32, result=new>(%3, add<i32, overflow=ub>(old<i32>, read<i32>(%4))))), pointer_cast<ptr<vla<i32, %45>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %14 a10: ptr<vla<i32, %46>> [storage=static] = pointer_cast<ptr<vla<i32, %46>>, reason=assign>(capture<%47>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn(i32, i32) -> i32>(%2, read<i32>(%3), read<i32>(%4)))), pointer_cast<ptr<vla<i32, %47>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %15 a11: ptr<vla<i32, %48>> [storage=static] = pointer_cast<ptr<vla<i32, %48>>, reason=assign>(capture<%49>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sequence<i32>(read<i32>(%3), read<i32>(%4)))), pointer_cast<ptr<vla<i32, %49>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %16 a12: ptr<vla<i32, %50>> [storage=static] = pointer_cast<ptr<vla<i32, %50>>, reason=assign>(capture<%52>(capture<%51>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4))), mul<u64, overflow=wrap>(read<u64>(%51), const<u64>(4))), pointer_cast<ptr<vla<i32, %52>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %17 a13: ptr<vla<i32, %53>> [storage=static] = pointer_cast<ptr<vla<i32, %53>>, reason=assign>(capture<%55>(capture<%54>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(update<i32, result=old>(%3, add<i32, overflow=ub>(old<i32>, const<i32>(1))))), mul<u64, overflow=wrap>(read<u64>(%54), const<u64>(4))), pointer_cast<ptr<vla<i32, %55>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %18 a14: ptr<vla<i32, %56>> [storage=static] = pointer_cast<ptr<vla<i32, %56>>, reason=assign>(capture<%57>(mul<u64, overflow=wrap>(read<u64>(%24), const<u64>(4)), pointer_cast<ptr<vla<i32, %57>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %19 a15: ptr<vla<i32, %58>> [storage=static] = pointer_cast<ptr<vla<i32, %58>>, reason=assign>(capture<%60>(sequence<u64>(addr_of<ptr<vla<i32, %59>>>(deref(capture<%59>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4))), pointer_cast<ptr<vla<i32, %59>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0))))), mul<u64, overflow=wrap>(read<u64>(%59), const<u64>(4))), pointer_cast<ptr<vla<i32, %60>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %20 a16: ptr<vla<i32, %61>> [storage=static] = pointer_cast<ptr<vla<i32, %61>>, reason=assign>(capture<%63>(sequence<u64>(addr_of<ptr<vla<i32, %62>>>(deref(capture<%62>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(update<i32, result=old>(%3, add<i32, overflow=ub>(old<i32>, const<i32>(1))))), pointer_cast<ptr<vla<i32, %62>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0))))), mul<u64, overflow=wrap>(read<u64>(%62), const<u64>(4))), pointer_cast<ptr<vla<i32, %63>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %21 a17: ptr<vla<i32, %64>> [storage=static] = pointer_cast<ptr<vla<i32, %64>>, reason=assign>(capture<%65>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32, volatile>(%1))), pointer_cast<ptr<vla<i32, %65>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0)))) [linkage=internal];
// DEFAULT-NEXT:     global %23 a18: ptr<vla<i32, %67>> [storage=static] = pointer_cast<ptr<vla<i32, %67>>, reason=assign>(pointer_cast<ptr<vla<i32, %66>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%0))) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @f(%3 m: i32, %4 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %25: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %27: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %30: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %33: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %36: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %39: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %42: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %44: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %46: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %48: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %50: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %53: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %56: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %58: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %61: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %64: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %68: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%69));
// DEFAULT-NEXT:         let %66: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%68)));
// DEFAULT-NEXT:         let %67: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
