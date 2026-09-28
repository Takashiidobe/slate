/* PR middle-end/114073 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -Wno-psabi" } */
/* { dg-additional-options "-mavx512f" { target i?86-*-* x86_64-*-* } } */

typedef int V __attribute__((vector_size (sizeof (_BitInt(256)))));
typedef int W __attribute__((vector_size (sizeof (_BitInt(512)))));

#if __BITINT_MAXWIDTH__ >= 256 && defined (__SIZEOF_INT128__)
_Complex __int128
f1 (_BitInt(256) x)
{
  union U { _BitInt(256) x; _Complex __int128 y; } u;
  u.x = x;
  return u.y;
}

_Complex __int128
f2 (_BitInt(254) x)
{
  union U { _BitInt(254) x; _Complex __int128 y; } u;
  u.x = x;
  return u.y;
}

_BitInt(256)
f3 (_Complex __int128 x)
{
  union U { _BitInt(256) x; _Complex __int128 y; } u;
  u.y = x;
  return u.x;
}

_BitInt(252)
f4 (_Complex __int128 x)
{
  union U { _BitInt(252) x; _Complex __int128 y; } u;
  u.y = x;
  return u.x;
}

_Complex __int128
f5 (_BitInt(256) x)
{
  union U { _BitInt(256) x; _Complex __int128 y; } u;
  u.x = x + 1;
  return u.y;
}

_Complex __int128
f6 (_BitInt(254) x)
{
  union U { _BitInt(254) x; _Complex __int128 y; } u;
  u.x = x + 1;
  return u.y;
}

_Complex __int128
f7 (_BitInt(256) *x)
{
  union U { _BitInt(256) x; _Complex __int128 y; } u;
  u.x = *x + 1;
  return u.y;
}

_Complex __int128
f8 (_BitInt(254) *x)
{
  union U { _BitInt(254) x; _Complex __int128 y; } u;
  u.x = *x + 1;
  return u.y;
}

_BitInt(256)
f9 (_Complex __int128 x)
{
  union U { _BitInt(256) x; _Complex __int128 y; } u;
  u.y = x;
  return u.x + 1;
}

_BitInt(252)
f10 (_Complex __int128 x)
{
  union U { _BitInt(252) x; _Complex __int128 y; } u;
  u.y = x;
  return u.x + 1;
}
#endif

#if __BITINT_MAXWIDTH__ >= 256
V
f11 (_BitInt(256) x)
{
  union U { _BitInt(256) x; V y; } u;
  u.x = x;
  return u.y;
}

V
f12 (_BitInt(254) x)
{
  union U { _BitInt(254) x; V y; } u;
  u.x = x;
  return u.y;
}

_BitInt(256)
f13 (V x)
{
  union U { _BitInt(256) x; V y; } u;
  u.y = x;
  return u.x;
}

_BitInt(252)
f14 (V x)
{
  union U { _BitInt(252) x; V y; } u;
  u.y = x;
  return u.x;
}

V
f15 (_BitInt(256) x)
{
  union U { _BitInt(256) x; V y; } u;
  u.x = x + 1;
  return u.y;
}

V
f16 (_BitInt(254) x)
{
  union U { _BitInt(254) x; V y; } u;
  u.x = x + 1;
  return u.y;
}

V
f17 (_BitInt(256) *x)
{
  union U { _BitInt(256) x; V y; } u;
  u.x = *x + 1;
  return u.y;
}

V
f18 (_BitInt(254) *x)
{
  union U { _BitInt(254) x; V y; } u;
  u.x = *x + 1;
  return u.y;
}

_BitInt(256)
f19 (V x)
{
  union U { _BitInt(256) x; V y; } u;
  u.y = x;
  return u.x + 1;
}

_BitInt(252)
f20 (V x)
{
  union U { _BitInt(252) x; V y; } u;
  u.y = x;
  return u.x + 1;
}
#endif

#if __BITINT_MAXWIDTH__ >= 512
W
f21 (_BitInt(512) x)
{
  union U { _BitInt(512) x; W y; } u;
  u.x = x;
  return u.y;
}

W
f22 (_BitInt(509) x)
{
  union U { _BitInt(509) x; W y; } u;
  u.x = x;
  return u.y;
}

_BitInt(512)
f23 (W x)
{
  union U { _BitInt(512) x; W y; } u;
  u.y = x;
  return u.x;
}

_BitInt(506)
f24 (W x)
{
  union U { _BitInt(506) x; W y; } u;
  u.y = x;
  return u.x;
}

W
f25 (_BitInt(512) x)
{
  union U { _BitInt(512) x; W y; } u;
  u.x = x + 1;
  return u.y;
}

W
f26 (_BitInt(509) x)
{
  union U { _BitInt(509) x; W y; } u;
  u.x = x + 1;
  return u.y;
}

W
f27 (_BitInt(512) *x)
{
  union U { _BitInt(512) x; W y; } u;
  u.x = *x + 1;
  return u.y;
}

W
f28 (_BitInt(509) *x)
{
  union U { _BitInt(509) x; W y; } u;
  u.x = *x + 1;
  return u.y;
}

_BitInt(512)
f29 (W x)
{
  union U { _BitInt(512) x; W y; } u;
  u.y = x;
  return u.x + 1;
}

_BitInt(506)
f30 (W x)
{
  union U { _BitInt(506) x; W y; } u;
  u.y = x;
  return u.x + 1;
}
#endif

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
// DEFAULT-NEXT:     type @type0 V = vector<i32, 8>;
// DEFAULT-NEXT:     type @type1 W = vector<i32, 16>;
// DEFAULT-NEXT:     type @type2 U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type3 U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type4 U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type5 U = union {
// DEFAULT-NEXT:         field0 x: i252b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type6 U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type7 U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type8 U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type9 U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type10 U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type11 U = union {
// DEFAULT-NEXT:         field0 x: i252b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type12 U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type13 U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type14 U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type15 U = union {
// DEFAULT-NEXT:         field0 x: i252b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type16 U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type17 U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type18 U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type19 U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type20 U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type21 U = union {
// DEFAULT-NEXT:         field0 x: i252b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type22 U = union {
// DEFAULT-NEXT:         field0 x: i512b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type23 U = union {
// DEFAULT-NEXT:         field0 x: i509b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type24 U = union {
// DEFAULT-NEXT:         field0 x: i512b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type25 U = union {
// DEFAULT-NEXT:         field0 x: i506b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type26 U = union {
// DEFAULT-NEXT:         field0 x: i512b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type27 U = union {
// DEFAULT-NEXT:         field0 x: i509b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type28 U = union {
// DEFAULT-NEXT:         field0 x: i512b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type29 U = union {
// DEFAULT-NEXT:         field0 x: i509b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type30 U = union {
// DEFAULT-NEXT:         field0 x: i512b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type31 U = union {
// DEFAULT-NEXT:         field0 x: i506b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %2 @f1(%3 x: i256b) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 u: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%5), read<i256b>(%3));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f2(%7 x: i254b) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 u: @type3 [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%9), read<i254b>(%7));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f3(%11 x: complex<i128>) -> i256b [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 u: @type4 [storage=automatic];
// DEFAULT-NEXT:         write<complex<i128>>(field1(%13), read<complex<i128>>(%11));
// DEFAULT-NEXT:         return read<i256b>(field0(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f4(%15 x: complex<i128>) -> i252b [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 u: @type5 [storage=automatic];
// DEFAULT-NEXT:         write<complex<i128>>(field1(%17), read<complex<i128>>(%15));
// DEFAULT-NEXT:         return read<i252b>(field0(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f5(%19 x: i256b) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %21 u: @type6 [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%21), add<i256b, overflow=ub>(read<i256b>(%19), widen<i256b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @f6(%23 x: i254b) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %25 u: @type7 [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%25), add<i254b, overflow=ub>(read<i254b>(%23), widen<i254b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @f7(%27 x: ptr<i256b>) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %29 u: @type8 [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%29), add<i256b, overflow=ub>(read<i256b>(deref(read<ptr<i256b>>(%27))), widen<i256b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @f8(%31 x: ptr<i254b>) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33 u: @type9 [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%33), add<i254b, overflow=ub>(read<i254b>(deref(read<ptr<i254b>>(%31))), widen<i254b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%33));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @f9(%35 x: complex<i128>) -> i256b [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %37 u: @type10 [storage=automatic];
// DEFAULT-NEXT:         write<complex<i128>>(field1(%37), read<complex<i128>>(%35));
// DEFAULT-NEXT:         return add<i256b, overflow=ub>(read<i256b>(field0(%37)), widen<i256b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @f10(%39 x: complex<i128>) -> i252b [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %41 u: @type11 [storage=automatic];
// DEFAULT-NEXT:         write<complex<i128>>(field1(%41), read<complex<i128>>(%39));
// DEFAULT-NEXT:         return add<i252b, overflow=ub>(read<i252b>(field0(%41)), widen<i252b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @f11(%43 x: i256b) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %45 u: @type12 [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%45), read<i256b>(%43));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%45));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @f12(%47 x: i254b) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %49 u: @type13 [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%49), read<i254b>(%47));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%49));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @f13(%51 x: vector<i32, 8>) -> i256b [linkage=external] [abi=sysv64(byval<align=32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %53 u: @type14 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 8>>(field1(%53), read<vector<i32, 8>>(%51));
// DEFAULT-NEXT:         return read<i256b>(field0(%53));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @f14(%55 x: vector<i32, 8>) -> i252b [linkage=external] [abi=sysv64(byval<align=32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %57 u: @type15 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 8>>(field1(%57), read<vector<i32, 8>>(%55));
// DEFAULT-NEXT:         return read<i252b>(field0(%57));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @f15(%59 x: i256b) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %61 u: @type16 [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%61), add<i256b, overflow=ub>(read<i256b>(%59), widen<i256b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%61));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @f16(%63 x: i254b) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %65 u: @type17 [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%65), add<i254b, overflow=ub>(read<i254b>(%63), widen<i254b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%65));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @f17(%67 x: ptr<i256b>) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %69 u: @type18 [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%69), add<i256b, overflow=ub>(read<i256b>(deref(read<ptr<i256b>>(%67))), widen<i256b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%69));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @f18(%71 x: ptr<i254b>) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %73 u: @type19 [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%73), add<i254b, overflow=ub>(read<i254b>(deref(read<ptr<i254b>>(%71))), widen<i254b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%73));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @f19(%75 x: vector<i32, 8>) -> i256b [linkage=external] [abi=sysv64(byval<align=32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %77 u: @type20 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 8>>(field1(%77), read<vector<i32, 8>>(%75));
// DEFAULT-NEXT:         return add<i256b, overflow=ub>(read<i256b>(field0(%77)), widen<i256b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @f20(%79 x: vector<i32, 8>) -> i252b [linkage=external] [abi=sysv64(byval<align=32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %81 u: @type21 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 8>>(field1(%81), read<vector<i32, 8>>(%79));
// DEFAULT-NEXT:         return add<i252b, overflow=ub>(read<i252b>(field0(%81)), widen<i252b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %82 @f21(%83 x: i512b) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %85 u: @type22 [storage=automatic];
// DEFAULT-NEXT:         write<i512b>(field0(%85), read<i512b>(%83));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%85));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %86 @f22(%87 x: i509b) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %89 u: @type23 [storage=automatic];
// DEFAULT-NEXT:         write<i509b>(field0(%89), read<i509b>(%87));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%89));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @f23(%91 x: vector<i32, 16>) -> i512b [linkage=external] [abi=sysv64(byval<align=64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %93 u: @type24 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 16>>(field1(%93), read<vector<i32, 16>>(%91));
// DEFAULT-NEXT:         return read<i512b>(field0(%93));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %94 @f24(%95 x: vector<i32, 16>) -> i506b [linkage=external] [abi=sysv64(byval<align=64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %97 u: @type25 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 16>>(field1(%97), read<vector<i32, 16>>(%95));
// DEFAULT-NEXT:         return read<i506b>(field0(%97));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %98 @f25(%99 x: i512b) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %101 u: @type26 [storage=automatic];
// DEFAULT-NEXT:         write<i512b>(field0(%101), add<i512b, overflow=ub>(read<i512b>(%99), widen<i512b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%101));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %102 @f26(%103 x: i509b) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %105 u: @type27 [storage=automatic];
// DEFAULT-NEXT:         write<i509b>(field0(%105), add<i509b, overflow=ub>(read<i509b>(%103), widen<i509b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%105));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @f27(%107 x: ptr<i512b>) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %109 u: @type28 [storage=automatic];
// DEFAULT-NEXT:         write<i512b>(field0(%109), add<i512b, overflow=ub>(read<i512b>(deref(read<ptr<i512b>>(%107))), widen<i512b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%109));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %110 @f28(%111 x: ptr<i509b>) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %113 u: @type29 [storage=automatic];
// DEFAULT-NEXT:         write<i509b>(field0(%113), add<i509b, overflow=ub>(read<i509b>(deref(read<ptr<i509b>>(%111))), widen<i509b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%113));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %114 @f29(%115 x: vector<i32, 16>) -> i512b [linkage=external] [abi=sysv64(byval<align=64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %117 u: @type30 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 16>>(field1(%117), read<vector<i32, 16>>(%115));
// DEFAULT-NEXT:         return add<i512b, overflow=ub>(read<i512b>(field0(%117)), widen<i512b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %118 @f30(%119 x: vector<i32, 16>) -> i506b [linkage=external] [abi=sysv64(byval<align=64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %121 u: @type31 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 16>>(field1(%121), read<vector<i32, 16>>(%119));
// DEFAULT-NEXT:         return add<i506b, overflow=ub>(read<i506b>(field0(%121)), widen<i506b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
