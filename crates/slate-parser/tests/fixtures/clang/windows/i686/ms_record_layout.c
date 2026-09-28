// SLATE-FILECHECK-DEFINES TARGET
// SLATE-FILECHECK-ARGS --dump-ir-types
struct B1 { char a : 4; int b : 4; };
struct B2 { char a : 3; char b : 5; char c : 1; };
struct B3 { int a : 20; int b : 20; };
struct B4 { char c; int a : 3; char d; };
struct B5 { long long a : 3; int b : 3; long long c : 60; };
struct B6 { _Bool a : 1; char b : 2; unsigned short c : 9; };
enum E { EA, EB };
struct B7 { enum E a : 2; int b : 3; };
struct Z1 { char a; int : 0; char b; };
struct Z2 { char a : 3; int : 0; char b; };
struct Z3 { char a : 3; char : 0; char b : 3; };
struct Z4 { char a : 3; long long : 0; char b; };
struct Z5 { int : 0; char a; };
struct Z6 { char a; long long : 0; };
struct Z7 { char a : 1; long long : 0; };
union U1 { int a : 3; char b; };
union U2 { char a : 3; long long b : 5; };
union U3 { char a; int : 0; };
union U4 { char a : 1; int : 0; };
union U5 { int a : 3; };
struct N1 { char c; double d; };
struct N2 { char c; long long d; short s; };
#pragma pack(push, 1)
struct P1 { char c; int i; double d; };
struct P2 { char a : 3; int b : 5; };
#pragma pack(pop)
#pragma pack(push, 2)
struct P3 { char c; int i; double d; };
struct P4 { char c; int b : 5; };
#pragma pack(pop)
#pragma pack(push, 4)
struct P5 { char c; double d; };
struct P6 { char c; };
#pragma pack(pop)
#pragma pack(push, 8)
struct P7 { char c; double d; };
#pragma pack(pop)
#pragma pack(push, 16)
struct P8 { char c; double d; };
#pragma pack(pop)
struct __attribute__((packed)) K1 { char c; int i; };
struct __attribute__((packed)) K2 { char a : 3; int b : 5; };
struct K3 { char c; int i __attribute__((packed)); };
struct __attribute__((aligned(16))) A1 { char c; };
struct A2 { char c; int i __attribute__((aligned(16))); };
struct A3 { char c; _Alignas(8) int i; };
typedef int AI __attribute__((aligned(16)));
struct A4 { char c; AI i; };
struct A5 { char c; int b : 3 __attribute__((aligned(8))); char d; };
struct A6 { char c; struct A1 inner; };
#pragma pack(push, 1)
struct A7 { char c; struct A1 inner; };
struct A8 { char c; AI i; };
struct A9 { char c; int i __attribute__((aligned(8))); };
struct __attribute__((aligned(8))) A10 { char c; };
#pragma pack(pop)
#pragma pack(push, 2)
struct __attribute__((aligned(4))) A11 { char c; };
struct A12 { char c; };
#pragma pack(pop)
struct __attribute__((packed, aligned(4))) A13 { char c; int i; };
struct E1 { };
struct E2 { int : 0; };
struct E3 { char a[0]; };
struct __attribute__((aligned(8))) E4 { };
struct __attribute__((aligned(2))) E5 { };
struct F1 { int n; char d[]; };
struct F2 { char n; double d[]; };
struct F3 { char n; long long d[0]; };
struct T1 { struct E1 e; char c; };
struct T2 { char c; struct E1 e; };
struct R1 { _Atomic(long long) a; char c; };
struct R2 { char c; _Atomic(struct B1) b; };
struct R3 { char c; long double d; };
struct R4 { char c; struct B1 b[2]; };
struct R5 { int a : 3; struct N1 n; int b : 3; };
typedef int AI8 __attribute__((aligned(8)));
typedef char AC2 __attribute__((aligned(2)));
struct C1 { char c; AI8 b : 3; char d; };
struct C2 { __attribute__((aligned(8))) char a[0]; };
struct C3 { char c; AC2 d; };
#pragma pack(push, 1)
struct C4 { char c; AC2 d; };
struct C5 { char a : 3; int : 0; char b; };
struct C6 { char c; AI8 b : 3; };
#pragma pack(pop)
#pragma pack(push, 8)
struct C7 { char c; long long l; };
struct __declspec(align(16)) C8 { char c; };
#pragma pack(pop)
struct __declspec(align(4)) C9 { char c; };
struct C10 { char c; __declspec(align(4)) char d; };
struct Inner1 { char c; __declspec(align(8)) char d; };
#pragma pack(push, 2)
struct C11 { char c; struct Inner1 i; };
struct Inner2 { char c; int i; };
struct C12 { char c; struct Inner2 i; };
#pragma pack(pop)
union __attribute__((packed)) V1 { int a; char b; };
union V2 { char a; __attribute__((aligned(8))) char b; };
union V3 { char a : 1; __attribute__((aligned(8))) int b : 3; };
struct D1 { int a : 31; int b : 2; };
struct D2 { unsigned long long a : 40; unsigned long long b : 30; };
struct D3 { char a : 8, b : 8, c : 8, d : 8, e : 8; };
struct D4 { short a : 9; short b : 8; };
struct D5 { int a : 3; long b : 3; };
struct D6 { char a; short b : 3; char c : 3; short d : 3; };
struct S3 { char a, b, c; };
struct At1 { char c; _Atomic(struct S3) s; };
struct At2 { char c; _Atomic(char) s; };
struct At3 { char c; _Atomic(long double) s; };
struct G1 { char c; long double d; };
struct G2 { char c; struct C2 e; };
struct G3 { struct C9 e[2]; char c; };
struct __attribute__((aligned(2))) G4 { int : 0; };
struct G5 { int a : 3; int : 0; };
struct G6 { int a : 3; char : 0; char b; };
struct G7 { char a : 3; int : 0; int b : 3; };
struct G8 { char c; int x[]; };

// SLATE-FILECHECK-BEGIN TARGET
// TARGET: module {
// TARGET-NEXT:     target "i686-pc-windows-msvc" {
// TARGET-NEXT:         endian = little;
// TARGET-NEXT:         pointer [size=4, align=4];
// TARGET-NEXT:         stack_alignment = 4;
// TARGET-NEXT:         long_double = f64;
// TARGET-NEXT:         storage bool [size=1, align=1];
// TARGET-NEXT:         storage i8, u8 [size=1, align=1];
// TARGET-NEXT:         storage i16, u16 [size=2, align=2];
// TARGET-NEXT:         storage i32, u32 [size=4, align=4];
// TARGET-NEXT:         storage i64, u64 [size=8, align=8];
// TARGET-NEXT:         storage i128, u128 [size=16, align=16];
// TARGET-NEXT:         storage bf16 [size=2, align=2];
// TARGET-NEXT:         storage f16 [size=2, align=2];
// TARGET-NEXT:         storage f32 [size=4, align=4];
// TARGET-NEXT:         storage f64 [size=8, align=8];
// TARGET-NEXT:         storage f128 [size=16, align=16];
// TARGET-NEXT:         storage d32 [size=4, align=4];
// TARGET-NEXT:         storage d64 [size=8, align=8];
// TARGET-NEXT:         storage d128 [size=16, align=16];
// TARGET-NEXT:     }
// TARGET-NEXT:     type @type0 B1 = struct {
// TARGET-NEXT:         field0 a: i8 : 4;
// TARGET-NEXT:         field1 b: i32 : 4;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), Some(32)], bit_units=[(0, 1), (4, 4)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type1 B2 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 b: i8 : 5;
// TARGET-NEXT:         field2 c: i8 : 1;
// TARGET-NEXT:     } [size=2, align=1, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(3), Some(8)], bit_units=[(0, 1), (1, 1)], field_units=[Some(0), Some(0), Some(1)]];
// TARGET-NEXT:     type @type2 B3 = struct {
// TARGET-NEXT:         field0 a: i32 : 20;
// TARGET-NEXT:         field1 b: i32 : 20;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), Some(32)], bit_units=[(0, 4), (4, 4)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type3 B4 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 a: i32 : 3;
// TARGET-NEXT:         field2 d: i8;
// TARGET-NEXT:     } [size=12, align=4, offsets=[0, 4, 8], bit_offsets=[None, Some(32), None], bit_units=[(4, 4)], field_units=[None, Some(0), None]];
// TARGET-NEXT:     type @type4 B5 = struct {
// TARGET-NEXT:         field0 a: i64 : 3;
// TARGET-NEXT:         field1 b: i32 : 3;
// TARGET-NEXT:         field2 c: i64 : 60;
// TARGET-NEXT:     } [size=24, align=8, offsets=[0, 8, 16], bit_offsets=[Some(0), Some(64), Some(128)], bit_units=[(0, 8), (8, 4), (16, 8)], field_units=[Some(0), Some(1), Some(2)]];
// TARGET-NEXT:     type @type5 B6 = struct {
// TARGET-NEXT:         field0 a: bool : 1;
// TARGET-NEXT:         field1 b: i8 : 2;
// TARGET-NEXT:         field2 c: u16 : 9;
// TARGET-NEXT:     } [size=4, align=2, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(1), Some(16)], bit_units=[(0, 1), (2, 2)], field_units=[Some(0), Some(0), Some(1)]];
// TARGET-NEXT:     type @type6 E = enum : u32 {
// TARGET-NEXT:         %0 EA = const<i32>(0);
// TARGET-NEXT:         %1 EB = const<i32>(1);
// TARGET-NEXT:     } [size=4, align=4];
// TARGET-NEXT:     type @type7 B7 = struct {
// TARGET-NEXT:         field0 a: @type6 : 2;
// TARGET-NEXT:         field1 b: i32 : 3;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(2)], bit_units=[(0, 4)], field_units=[Some(0), Some(0)]];
// TARGET-NEXT:     type @type8 Z1 = struct {
// TARGET-NEXT:         field0 a: i8;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:         field2 b: i8;
// TARGET-NEXT:     } [size=2, align=1, offsets=[0, 1, 1], bit_offsets=[None, Some(8), None]];
// TARGET-NEXT:     type @type9 Z2 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:         field2 b: i8;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4, 4], bit_offsets=[Some(0), Some(32), None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// TARGET-NEXT:     type @type10 Z3 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 <anonymous>: i8 : 0;
// TARGET-NEXT:         field2 b: i8 : 3;
// TARGET-NEXT:     } [size=2, align=1, offsets=[0, 1, 1], bit_offsets=[Some(0), Some(8), Some(8)], bit_units=[(0, 1), (1, 1)], field_units=[Some(0), None, Some(1)]];
// TARGET-NEXT:     type @type11 Z4 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 <anonymous>: i64 : 0;
// TARGET-NEXT:         field2 b: i8;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8, 8], bit_offsets=[Some(0), Some(64), None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// TARGET-NEXT:     type @type12 Z5 = struct {
// TARGET-NEXT:         field0 <anonymous>: i32 : 0;
// TARGET-NEXT:         field1 a: i8;
// TARGET-NEXT:     } [size=1, align=1, offsets=[0, 0], bit_offsets=[Some(0), None]];
// TARGET-NEXT:     type @type13 Z6 = struct {
// TARGET-NEXT:         field0 a: i8;
// TARGET-NEXT:         field1 <anonymous>: i64 : 0;
// TARGET-NEXT:     } [size=1, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)]];
// TARGET-NEXT:     type @type14 Z7 = struct {
// TARGET-NEXT:         field0 a: i8 : 1;
// TARGET-NEXT:         field1 <anonymous>: i64 : 0;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0, 8], bit_offsets=[Some(0), Some(64)], bit_units=[(0, 1)], field_units=[Some(0), None]];
// TARGET-NEXT:     type @type15 U1 = union {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:         field1 b: i8;
// TARGET-NEXT:     } [size=4, align=1, offsets=[0, 0], bit_offsets=[Some(0), None], bit_units=[(0, 4)], field_units=[Some(0), None]];
// TARGET-NEXT:     type @type16 U2 = union {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 b: i64 : 5;
// TARGET-NEXT:     } [size=8, align=1, offsets=[0, 0], bit_offsets=[Some(0), Some(0)], bit_units=[(0, 1), (0, 8)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type17 U3 = union {
// TARGET-NEXT:         field0 a: i8;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:     } [size=1, align=1, offsets=[0, 0], bit_offsets=[None, Some(0)]];
// TARGET-NEXT:     type @type18 U4 = union {
// TARGET-NEXT:         field0 a: i8 : 1;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:     } [size=4, align=1, offsets=[0, 0], bit_offsets=[Some(0), Some(0)], bit_units=[(0, 1)], field_units=[Some(0), None]];
// TARGET-NEXT:     type @type19 U5 = union {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:     } [size=4, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 4)], field_units=[Some(0)]];
// TARGET-NEXT:     type @type20 N1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type21 N2 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: i64;
// TARGET-NEXT:         field2 s: i16;
// TARGET-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// TARGET-NEXT:     type @type22 P1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:         field2 d: f64;
// TARGET-NEXT:     } [size=13, align=1, offsets=[0, 1, 5]];
// TARGET-NEXT:     type @type23 P2 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 b: i32 : 5;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 1], bit_offsets=[Some(0), Some(8)], bit_units=[(0, 1), (1, 4)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type24 P3 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:         field2 d: f64;
// TARGET-NEXT:     } [size=14, align=2, offsets=[0, 2, 6]];
// TARGET-NEXT:     type @type25 P4 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 b: i32 : 5;
// TARGET-NEXT:     } [size=6, align=2, offsets=[0, 2], bit_offsets=[None, Some(16)], bit_units=[(2, 4)], field_units=[None, Some(0)]];
// TARGET-NEXT:     type @type26 P5 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// TARGET-NEXT:     type @type27 P6 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=1, align=1, offsets=[0]];
// TARGET-NEXT:     type @type28 P7 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type29 P8 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type30 K1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// TARGET-NEXT:     type @type31 K2 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 b: i32 : 5;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 1], bit_offsets=[Some(0), Some(8)], bit_units=[(0, 1), (1, 4)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type32 K3 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// TARGET-NEXT:     type @type33 A1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=16, align=16, offsets=[0]];
// TARGET-NEXT:     type @type34 A2 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// TARGET-NEXT:     type @type35 A3 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type36 AI = i32;
// TARGET-NEXT:     type @type37 A4 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// TARGET-NEXT:     type @type38 A5 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 b: i32 : 3;
// TARGET-NEXT:         field2 d: i8;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8, 12], bit_offsets=[None, Some(64), None], bit_units=[(8, 4)], field_units=[None, Some(0), None]];
// TARGET-NEXT:     type @type39 A6 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 inner: @type33;
// TARGET-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// TARGET-NEXT:     type @type40 A7 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 inner: @type33;
// TARGET-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// TARGET-NEXT:     type @type41 A8 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// TARGET-NEXT:     type @type42 A9 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type43 A10 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0]];
// TARGET-NEXT:     type @type44 A11 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0]];
// TARGET-NEXT:     type @type45 A12 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=1, align=1, offsets=[0]];
// TARGET-NEXT:     type @type46 A13 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 1]];
// TARGET-NEXT:     type @type47 E1 = struct {
// TARGET-NEXT:     } [size=4, align=1, offsets=[]];
// TARGET-NEXT:     type @type48 E2 = struct {
// TARGET-NEXT:         field0 <anonymous>: i32 : 0;
// TARGET-NEXT:     } [size=4, align=1, offsets=[0], bit_offsets=[Some(0)]];
// TARGET-NEXT:     type @type49 E3 = struct {
// TARGET-NEXT:         field0 a: array<i8, 0>;
// TARGET-NEXT:     } [size=4, align=1, offsets=[0]];
// TARGET-NEXT:     type @type50 E4 = struct {
// TARGET-NEXT:     } [size=8, align=8, offsets=[]];
// TARGET-NEXT:     type @type51 E5 = struct {
// TARGET-NEXT:     } [size=4, align=2, offsets=[]];
// TARGET-NEXT:     type @type52 F1 = struct {
// TARGET-NEXT:         field0 n: i32;
// TARGET-NEXT:         field1 d: array<i8, incomplete>;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// TARGET-NEXT:     type @type53 F2 = struct {
// TARGET-NEXT:         field0 n: i8;
// TARGET-NEXT:         field1 d: array<f64, incomplete>;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type54 F3 = struct {
// TARGET-NEXT:         field0 n: i8;
// TARGET-NEXT:         field1 d: array<i64, 0>;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type55 T1 = struct {
// TARGET-NEXT:         field0 e: @type47;
// TARGET-NEXT:         field1 c: i8;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 4]];
// TARGET-NEXT:     type @type56 T2 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 e: @type47;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// TARGET-NEXT:     type @type57 R1 = struct {
// TARGET-NEXT:         field0 a: atomic i64;
// TARGET-NEXT:         field1 c: i8;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type58 R2 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 b: atomic @type0;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type59 R3 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type60 R4 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 b: array<@type0, 2>;
// TARGET-NEXT:     } [size=20, align=4, offsets=[0, 4]];
// TARGET-NEXT:     type @type61 R5 = struct {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:         field1 n: @type20;
// TARGET-NEXT:         field2 b: i32 : 3;
// TARGET-NEXT:     } [size=32, align=8, offsets=[0, 8, 24], bit_offsets=[Some(0), None, Some(192)], bit_units=[(0, 4), (24, 4)], field_units=[Some(0), None, Some(1)]];
// TARGET-NEXT:     type @type62 AI8 = i32;
// TARGET-NEXT:     type @type63 AC2 = i8;
// TARGET-NEXT:     type @type64 C1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 b: i32 : 3;
// TARGET-NEXT:         field2 d: i8;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8, 12], bit_offsets=[None, Some(64), None], bit_units=[(8, 4)], field_units=[None, Some(0), None]];
// TARGET-NEXT:     type @type65 C2 = struct {
// TARGET-NEXT:         field0 a: array<i8, 0>;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0]];
// TARGET-NEXT:     type @type66 C3 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: i8;
// TARGET-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// TARGET-NEXT:     type @type67 C4 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: i8;
// TARGET-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// TARGET-NEXT:     type @type68 C5 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:         field2 b: i8;
// TARGET-NEXT:     } [size=2, align=1, offsets=[0, 1, 1], bit_offsets=[Some(0), Some(8), None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// TARGET-NEXT:     type @type69 C6 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 b: i32 : 3;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 4)], field_units=[None, Some(0)]];
// TARGET-NEXT:     type @type70 C7 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 l: i64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type71 C8 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=16, align=16, offsets=[0]];
// TARGET-NEXT:     type @type72 C9 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0]];
// TARGET-NEXT:     type @type73 C10 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: i8;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// TARGET-NEXT:     type @type74 Inner1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: i8;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type75 C11 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: @type74;
// TARGET-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type76 Inner2 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=6, align=2, offsets=[0, 2]];
// TARGET-NEXT:     type @type77 C12 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: @type76;
// TARGET-NEXT:     } [size=8, align=2, offsets=[0, 2]];
// TARGET-NEXT:     type @type78 V1 = union {
// TARGET-NEXT:         field0 a: i32;
// TARGET-NEXT:         field1 b: i8;
// TARGET-NEXT:     } [size=4, align=1, offsets=[0, 0]];
// TARGET-NEXT:     type @type79 V2 = union {
// TARGET-NEXT:         field0 a: i8;
// TARGET-NEXT:         field1 b: i8;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// TARGET-NEXT:     type @type80 V3 = union {
// TARGET-NEXT:         field0 a: i8 : 1;
// TARGET-NEXT:         field1 b: i32 : 3;
// TARGET-NEXT:     } [size=4, align=1, offsets=[0, 0], bit_offsets=[Some(0), Some(0)], bit_units=[(0, 1), (0, 4)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type81 D1 = struct {
// TARGET-NEXT:         field0 a: i32 : 31;
// TARGET-NEXT:         field1 b: i32 : 2;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), Some(32)], bit_units=[(0, 4), (4, 4)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type82 D2 = struct {
// TARGET-NEXT:         field0 a: u64 : 40;
// TARGET-NEXT:         field1 b: u64 : 30;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), Some(64)], bit_units=[(0, 8), (8, 8)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type83 D3 = struct {
// TARGET-NEXT:         field0 a: i8 : 8;
// TARGET-NEXT:         field1 b: i8 : 8;
// TARGET-NEXT:         field2 c: i8 : 8;
// TARGET-NEXT:         field3 d: i8 : 8;
// TARGET-NEXT:         field4 e: i8 : 8;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 1, 2, 3, 4], bit_offsets=[Some(0), Some(8), Some(16), Some(24), Some(32)], bit_units=[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1)], field_units=[Some(0), Some(1), Some(2), Some(3), Some(4)]];
// TARGET-NEXT:     type @type84 D4 = struct {
// TARGET-NEXT:         field0 a: i16 : 9;
// TARGET-NEXT:         field1 b: i16 : 8;
// TARGET-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[Some(0), Some(16)], bit_units=[(0, 2), (2, 2)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type85 D5 = struct {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:         field1 b: i32 : 3;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(3)], bit_units=[(0, 4)], field_units=[Some(0), Some(0)]];
// TARGET-NEXT:     type @type86 D6 = struct {
// TARGET-NEXT:         field0 a: i8;
// TARGET-NEXT:         field1 b: i16 : 3;
// TARGET-NEXT:         field2 c: i8 : 3;
// TARGET-NEXT:         field3 d: i16 : 3;
// TARGET-NEXT:     } [size=8, align=2, offsets=[0, 2, 4, 6], bit_offsets=[None, Some(16), Some(32), Some(48)], bit_units=[(2, 2), (4, 1), (6, 2)], field_units=[None, Some(0), Some(1), Some(2)]];
// TARGET-NEXT:     type @type87 S3 = struct {
// TARGET-NEXT:         field0 a: i8;
// TARGET-NEXT:         field1 b: i8;
// TARGET-NEXT:         field2 c: i8;
// TARGET-NEXT:     } [size=3, align=1, offsets=[0, 1, 2]];
// TARGET-NEXT:     type @type88 At1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 s: atomic @type87;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// TARGET-NEXT:     type @type89 At2 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 s: atomic i8;
// TARGET-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// TARGET-NEXT:     type @type90 At3 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 s: atomic f64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type91 G1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type92 G2 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 e: @type65;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type93 G3 = struct {
// TARGET-NEXT:         field0 e: array<@type72, 2>;
// TARGET-NEXT:         field1 c: i8;
// TARGET-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// TARGET-NEXT:     type @type94 G4 = struct {
// TARGET-NEXT:         field0 <anonymous>: i32 : 0;
// TARGET-NEXT:     } [size=4, align=2, offsets=[0], bit_offsets=[Some(0)]];
// TARGET-NEXT:     type @type95 G5 = struct {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 4], bit_offsets=[Some(0), Some(32)], bit_units=[(0, 4)], field_units=[Some(0), None]];
// TARGET-NEXT:     type @type96 G6 = struct {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:         field1 <anonymous>: i8 : 0;
// TARGET-NEXT:         field2 b: i8;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4, 4], bit_offsets=[Some(0), Some(32), None], bit_units=[(0, 4)], field_units=[Some(0), None, None]];
// TARGET-NEXT:     type @type97 G7 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:         field2 b: i32 : 3;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4, 4], bit_offsets=[Some(0), Some(32), Some(32)], bit_units=[(0, 1), (4, 4)], field_units=[Some(0), None, Some(1)]];
// TARGET-NEXT:     type @type98 G8 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 x: array<i32, incomplete>;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// TARGET-NEXT: }
// SLATE-FILECHECK-END TARGET
