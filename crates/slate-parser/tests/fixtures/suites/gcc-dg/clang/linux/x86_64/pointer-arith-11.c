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
// DEFAULT-NEXT:     type @type[[TYPE_uintptr_t:[0-9]+]] uintptr_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x]]), const<i32>(32));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE0]]), neg<u64, overflow=wrap>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_y]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x]], read<ptr<i8>>(%[[VALUE1]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_2:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE2]]), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_x_2]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_2]], read<ptr<i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_x_3:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_3]]), const<i32>(32));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE4]]), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_y_2]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_3]], read<ptr<i8>>(%[[VALUE5]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_x_4:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_4]]), const<i32>(16));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE6]]), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_y_3]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_4]], read<ptr<i8>>(%[[VALUE7]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
