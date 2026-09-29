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
// DEFAULT-NEXT:     type @type[[TYPE_vmt:[0-9]+]] vmt = ptr<vla<i32, %[[VALUE0:[0-9]+]]>>;
// DEFAULT-NEXT:     global %[[VALUE_sa:[0-9]+]] sa: array<i32, 100> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_nv:[0-9]+]] nv: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a1:[0-9]+]] a1: ptr<vla<i32, %[[VALUE1:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE1]]>>, reason=assign>(addr_of<ptr<array<i32, 100>>>(%[[VALUE_sa]])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a2:[0-9]+]] a2: ptr<vla<i32, %[[VALUE2:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE2]]>>, reason=assign>(capture<%[[VALUE3:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n:[0-9]+]]))), pointer_cast<ptr<vla<i32, %[[VALUE3]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a3:[0-9]+]] a3: ptr<vla<i32, %[[VALUE4:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE4]]>>, reason=assign>(capture<%[[VALUE5:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(compound_literal %[[VALUE6:[0-9]+]] [storage=automatic] = read<i32>(%[[VALUE_n]])))), pointer_cast<ptr<vla<i32, %[[VALUE5]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a4:[0-9]+]] a4: ptr<vla<i32, %[[VALUE7:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE7]]>>, reason=assign>(capture<%[[VALUE8:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(compound_literal %[[VALUE9:[0-9]+]] [storage=automatic] = update<i32, result=old>(%[[VALUE_m:[0-9]+]], add<i32, overflow=ub>(old<i32>, const<i32>(1)))))), pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a5:[0-9]+]] a5: ptr<vla<i32, %[[VALUE10:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE10]]>>, reason=assign>(capture<%[[VALUE11:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(compound_literal %[[VALUE12:[0-9]+]] [storage=automatic] = update<i32, result=new>(%[[VALUE_m]], add<i32, overflow=ub>(old<i32>, const<i32>(1)))))), pointer_cast<ptr<vla<i32, %[[VALUE11]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a6:[0-9]+]] a6: ptr<vla<i32, %[[VALUE13:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE13]]>>, reason=assign>(capture<%[[VALUE14:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(compound_literal %[[VALUE15:[0-9]+]] [storage=automatic] = update<i32, result=old>(%[[VALUE_m]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)))))), pointer_cast<ptr<vla<i32, %[[VALUE14]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a7:[0-9]+]] a7: ptr<vla<i32, %[[VALUE16:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE16]]>>, reason=assign>(capture<%[[VALUE17:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(compound_literal %[[VALUE18:[0-9]+]] [storage=automatic] = update<i32, result=new>(%[[VALUE_m]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)))))), pointer_cast<ptr<vla<i32, %[[VALUE17]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a8:[0-9]+]] a8: ptr<vla<i32, %[[VALUE19:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE19]]>>, reason=assign>(capture<%[[VALUE20:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(store<i32>(%[[VALUE_m]], read<i32>(%[[VALUE_n]])))), pointer_cast<ptr<vla<i32, %[[VALUE20]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a9:[0-9]+]] a9: ptr<vla<i32, %[[VALUE21:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE21]]>>, reason=assign>(capture<%[[VALUE22:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(update<i32, result=new>(%[[VALUE_m]], add<i32, overflow=ub>(old<i32>, read<i32>(%[[VALUE_n]]))))), pointer_cast<ptr<vla<i32, %[[VALUE22]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a10:[0-9]+]] a10: ptr<vla<i32, %[[VALUE23:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE23]]>>, reason=assign>(capture<%[[VALUE24:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f:[0-9]+]], read<i32>(%[[VALUE_m]]), read<i32>(%[[VALUE_n]])))), pointer_cast<ptr<vla<i32, %[[VALUE24]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a11:[0-9]+]] a11: ptr<vla<i32, %[[VALUE25:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE25]]>>, reason=assign>(capture<%[[VALUE26:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(sequence<i32>(read<i32>(%[[VALUE_m]]), read<i32>(%[[VALUE_n]])))), pointer_cast<ptr<vla<i32, %[[VALUE26]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a12:[0-9]+]] a12: ptr<vla<i32, %[[VALUE27:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE27]]>>, reason=assign>(capture<%[[VALUE28:[0-9]+]]>(capture<%[[VALUE29:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]]))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE29]]), const<u64>(4))), pointer_cast<ptr<vla<i32, %[[VALUE28]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a13:[0-9]+]] a13: ptr<vla<i32, %[[VALUE30:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE30]]>>, reason=assign>(capture<%[[VALUE31:[0-9]+]]>(capture<%[[VALUE32:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(update<i32, result=old>(%[[VALUE_m]], add<i32, overflow=ub>(old<i32>, const<i32>(1))))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE32]]), const<u64>(4))), pointer_cast<ptr<vla<i32, %[[VALUE31]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a14:[0-9]+]] a14: ptr<vla<i32, %[[VALUE33:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE33]]>>, reason=assign>(capture<%[[VALUE34:[0-9]+]]>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), const<u64>(4)), pointer_cast<ptr<vla<i32, %[[VALUE34]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a15:[0-9]+]] a15: ptr<vla<i32, %[[VALUE35:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE35]]>>, reason=assign>(capture<%[[VALUE36:[0-9]+]]>(sequence<u64>(addr_of<ptr<vla<i32, %[[VALUE37:[0-9]+]]>>>(deref(capture<%[[VALUE37]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]]))), pointer_cast<ptr<vla<i32, %[[VALUE37]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]]))))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE37]]), const<u64>(4))), pointer_cast<ptr<vla<i32, %[[VALUE36]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a16:[0-9]+]] a16: ptr<vla<i32, %[[VALUE38:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE38]]>>, reason=assign>(capture<%[[VALUE39:[0-9]+]]>(sequence<u64>(addr_of<ptr<vla<i32, %[[VALUE40:[0-9]+]]>>>(deref(capture<%[[VALUE40]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(update<i32, result=old>(%[[VALUE_m]], add<i32, overflow=ub>(old<i32>, const<i32>(1))))), pointer_cast<ptr<vla<i32, %[[VALUE40]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]]))))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE40]]), const<u64>(4))), pointer_cast<ptr<vla<i32, %[[VALUE39]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a17:[0-9]+]] a17: ptr<vla<i32, %[[VALUE41:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE41]]>>, reason=assign>(capture<%[[VALUE42:[0-9]+]]>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32, volatile>(%[[VALUE_nv]]))), pointer_cast<ptr<vla<i32, %[[VALUE42]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a18:[0-9]+]] a18: ptr<vla<i32, %[[VALUE43:[0-9]+]]>> [storage=static] = pointer_cast<ptr<vla<i32, %[[VALUE43]]>>, reason=assign>(pointer_cast<ptr<vla<i32, %[[VALUE0]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_sa]]))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_f]] @f(%[[VALUE_m]] m: i32, %[[VALUE_n]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE2]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE4]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE7]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE10]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE13]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE16]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE19]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE21]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE23]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE25]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE27]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE30]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE33]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE35]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE38]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE41]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_m]]);
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE44]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_m]], read<i32>(%[[VALUE45]]));
// DEFAULT-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE44]])));
// DEFAULT-NEXT:         let %[[VALUE43]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
