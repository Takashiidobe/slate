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
// DEFAULT-NEXT:     type @type[[TYPE_uintptr_t:[0-9]+]] uintptr_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x]]), const<i32>(97));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE0]]), neg<u64, overflow=wrap>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_y]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x]], read<ptr<i8>>(%[[VALUE1]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_2:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_2]]), const<i32>(98));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE2]]), neg<u64, overflow=wrap>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_y_2]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_2]], read<ptr<i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_x_3:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_3]]), const<i32>(100));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE4]]), neg<u64, overflow=wrap>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_y_3]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_3]], read<ptr<i8>>(%[[VALUE5]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_x_4:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_4:[0-9]+]] y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_4]]), const<i32>(104));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE6]]), neg<u64, overflow=wrap>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_y_4]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_4]], read<ptr<i8>>(%[[VALUE7]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_x_5:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_5]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE8]]), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_x_5]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_5]], read<ptr<i8>>(%[[VALUE9]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_x_6:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_6]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE10]]), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_x_6]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_6]], read<ptr<i8>>(%[[VALUE11]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_x_7:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_7]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE12]]), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_x_7]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_7]], read<ptr<i8>>(%[[VALUE13]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_x_8:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_8]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE14]]), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_x_8]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_8]], read<ptr<i8>>(%[[VALUE15]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE_x_9:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_5:[0-9]+]] y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_9]]), const<i32>(8));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_9]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE16]]), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_y_5]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_9]], read<ptr<i8>>(%[[VALUE17]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_x_10:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_6:[0-9]+]] y: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_10]]), const<i32>(16));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x_10]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE18]]), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_y_6]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x_10]], read<ptr<i8>>(%[[VALUE19]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_x_10]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
