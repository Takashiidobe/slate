/* { dg-options "-O2 -fdump-tree-optimized" } */

/* { dg-final { scan-tree-dump-times { & -16B?;} 4 "optimized" { target lp64 } } } */
/* { dg-final { scan-tree-dump-times { \+ 16;} 3 "optimized" } } */
/* { dg-final { scan-tree-dump-not { & 15;} "optimized" } } */
/* { dg-final { scan-tree-dump-not { \+ 96;} "optimized" } } */

typedef __UINTPTR_TYPE__ uintptr_t;

char *
f1 (char *x)
{
  char *y = x + 32;
  x += -((uintptr_t) y & 15);
  return x;
}

char *
f2 (char *x)
{
  x += 16 - ((uintptr_t) x & 15);
  return x;
}

char *
f3 (char *x)
{
  char *y = x + 32;
  x += 16 - ((uintptr_t) y & 15);
  return x;
}

char *
f4 (char *x)
{
  char *y = x + 16;
  x += 16 - ((uintptr_t) y & 15);
  return x;
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:         let %3 y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%2), const<i32>(32));
// DEFAULT-NEXT:         let %12: ptr<i8> [synthetic] = read<ptr<i8>>(%2);
// DEFAULT-NEXT:         let %13: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%12), neg<u64, overflow=wrap>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%2, read<ptr<i8>>(%13));
// DEFAULT-NEXT:         return read<ptr<i8>>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f2(%5 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14: ptr<i8> [synthetic] = read<ptr<i8>>(%5);
// DEFAULT-NEXT:         let %15: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%14), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%5, read<ptr<i8>>(%15));
// DEFAULT-NEXT:         return read<ptr<i8>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f3(%7 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%7), const<i32>(32));
// DEFAULT-NEXT:         let %16: ptr<i8> [synthetic] = read<ptr<i8>>(%7);
// DEFAULT-NEXT:         let %17: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%7, read<ptr<i8>>(%17));
// DEFAULT-NEXT:         return read<ptr<i8>>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f4(%10 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), const<i32>(16));
// DEFAULT-NEXT:         let %18: ptr<i8> [synthetic] = read<ptr<i8>>(%10);
// DEFAULT-NEXT:         let %19: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%18), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%11)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%10, read<ptr<i8>>(%19));
// DEFAULT-NEXT:         return read<ptr<i8>>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
