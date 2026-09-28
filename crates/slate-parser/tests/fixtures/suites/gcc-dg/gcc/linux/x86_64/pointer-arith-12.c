/* { dg-options "-O2 -fdump-tree-optimized" } */

/* { dg-final { scan-tree-dump-not { & -16B?;} "optimized" } } */
/* { dg-final { scan-tree-dump-times { & 15;} 10 "optimized" } } */

typedef __UINTPTR_TYPE__ uintptr_t;

char *
f1 (char *x)
{
  char *y = x + 97;
  x += -((uintptr_t) y & 15);
  return x;
}

char *
f2 (char *x)
{
  char *y = x + 98;
  x += -((uintptr_t) y & 15);
  return x;
}

char *
f3 (char *x)
{
  char *y = x + 100;
  x += -((uintptr_t) y & 15);
  return x;
}

char *
f4 (char *x)
{
  char *y = x + 104;
  x += -((uintptr_t) y & 15);
  return x;
}

char *
f5 (char *x)
{
  x += 1 - ((uintptr_t) x & 15);
  return x;
}

char *
f6 (char *x)
{
  x += 2 - ((uintptr_t) x & 15);
  return x;
}

char *
f7 (char *x)
{
  x += 4 - ((uintptr_t) x & 15);
  return x;
}

char *
f8 (char *x)
{
  x += 8 - ((uintptr_t) x & 15);
  return x;
}

char *
f9 (char *x)
{
  char *y = x + 8;
  x += 16 - ((uintptr_t) y & 15);
  return x;
}

char *
f10 (char *x)
{
  char *y = x + 16;
  x += 8 - ((uintptr_t) y & 15);
  return x;
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
// DEFAULT-NEXT:     type @type0 uintptr_t = u64;
// DEFAULT-NEXT:     fn %1 @f1(%2 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%2), const<i32>(97));
// DEFAULT-NEXT:         let %27: ptr<i8> [synthetic] = read<ptr<i8>>(%2);
// DEFAULT-NEXT:         let %28: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%27), neg<u64, overflow=wrap>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%2, read<ptr<i8>>(%28));
// DEFAULT-NEXT:         return read<ptr<i8>>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f2(%5 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%5), const<i32>(98));
// DEFAULT-NEXT:         let %29: ptr<i8> [synthetic] = read<ptr<i8>>(%5);
// DEFAULT-NEXT:         let %30: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%29), neg<u64, overflow=wrap>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%6)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%5, read<ptr<i8>>(%30));
// DEFAULT-NEXT:         return read<ptr<i8>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f3(%8 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%8), const<i32>(100));
// DEFAULT-NEXT:         let %31: ptr<i8> [synthetic] = read<ptr<i8>>(%8);
// DEFAULT-NEXT:         let %32: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%31), neg<u64, overflow=wrap>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%9)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%8, read<ptr<i8>>(%32));
// DEFAULT-NEXT:         return read<ptr<i8>>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f4(%11 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%11), const<i32>(104));
// DEFAULT-NEXT:         let %33: ptr<i8> [synthetic] = read<ptr<i8>>(%11);
// DEFAULT-NEXT:         let %34: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%33), neg<u64, overflow=wrap>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%12)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%11, read<ptr<i8>>(%34));
// DEFAULT-NEXT:         return read<ptr<i8>>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @f5(%14 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35: ptr<i8> [synthetic] = read<ptr<i8>>(%14);
// DEFAULT-NEXT:         let %36: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%35), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%14)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%14, read<ptr<i8>>(%36));
// DEFAULT-NEXT:         return read<ptr<i8>>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @f6(%16 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %37: ptr<i8> [synthetic] = read<ptr<i8>>(%16);
// DEFAULT-NEXT:         let %38: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%37), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%16)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%16, read<ptr<i8>>(%38));
// DEFAULT-NEXT:         return read<ptr<i8>>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @f7(%18 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %39: ptr<i8> [synthetic] = read<ptr<i8>>(%18);
// DEFAULT-NEXT:         let %40: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%39), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%18)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%18, read<ptr<i8>>(%40));
// DEFAULT-NEXT:         return read<ptr<i8>>(%18);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @f8(%20 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %41: ptr<i8> [synthetic] = read<ptr<i8>>(%20);
// DEFAULT-NEXT:         let %42: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%41), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%20)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%20, read<ptr<i8>>(%42));
// DEFAULT-NEXT:         return read<ptr<i8>>(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @f9(%22 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%22), const<i32>(8));
// DEFAULT-NEXT:         let %43: ptr<i8> [synthetic] = read<ptr<i8>>(%22);
// DEFAULT-NEXT:         let %44: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%43), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%23)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%22, read<ptr<i8>>(%44));
// DEFAULT-NEXT:         return read<ptr<i8>>(%22);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @f10(%25 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %26 y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%25), const<i32>(16));
// DEFAULT-NEXT:         let %45: ptr<i8> [synthetic] = read<ptr<i8>>(%25);
// DEFAULT-NEXT:         let %46: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%45), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%26)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%25, read<ptr<i8>>(%46));
// DEFAULT-NEXT:         return read<ptr<i8>>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
