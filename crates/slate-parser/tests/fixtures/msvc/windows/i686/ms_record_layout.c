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
struct C5 { char a : 3; int : 0; char b; };
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
struct __declspec(align(16)) C8 { char c; };
#pragma pack(pop)
#pragma pack(push, 16)
struct P8 { char c; double d; };
#pragma pack(pop)
struct __declspec(align(16)) A1 { char c; };
struct A2 { char c; __declspec(align(16)) int i; };
struct A6 { char c; struct A1 inner; };
#pragma pack(push, 1)
struct A7 { char c; struct A1 inner; };
struct A9 { char c; __declspec(align(8)) int i; };
struct __declspec(align(8)) A10 { char c; };
#pragma pack(pop)
#pragma pack(push, 2)
struct __declspec(align(4)) A11 { char c; };
struct Inner1 { char c; __declspec(align(8)) char d; };
struct C11 { char c; struct Inner1 i; };
#pragma pack(pop)
struct __declspec(align(4)) C9 { char c; };
struct C10 { char c; __declspec(align(4)) char d; };
struct F1 { int n; char d[]; };
struct F2 { char n; double d[]; };
struct F3 { char n; long long d[0]; };
struct R3 { char c; long double d; };
struct R4 { char c; struct B1 b[2]; };
struct R5 { int a : 3; struct N1 n; int b : 3; };
struct D1 { int a : 31; int b : 2; };
struct D2 { unsigned long long a : 40; unsigned long long b : 30; };
struct D3 { char a : 8, b : 8, c : 8, d : 8, e : 8; };
struct D4 { short a : 9; short b : 8; };
struct D5 { int a : 3; long b : 3; };
struct D6 { char a; short b : 3; char c : 3; short d : 3; };
struct G3 { struct C9 e[2]; char c; };
struct G5 { int a : 3; int : 0; };
struct G6 { int a : 3; char : 0; char b; };
struct G7 { char a : 3; int : 0; int b : 3; };
struct G8 { char c; int x[]; };

#include <stddef.h>
_Static_assert(sizeof(struct B1) == 8, "B1 size");
_Static_assert(_Alignof(struct B1) == 4, "B1 align");
_Static_assert(sizeof(struct B2) == 2, "B2 size");
_Static_assert(_Alignof(struct B2) == 1, "B2 align");
_Static_assert(sizeof(struct B3) == 8, "B3 size");
_Static_assert(_Alignof(struct B3) == 4, "B3 align");
_Static_assert(sizeof(struct B4) == 12, "B4 size");
_Static_assert(_Alignof(struct B4) == 4, "B4 align");
_Static_assert(sizeof(struct B5) == 24, "B5 size");
_Static_assert(_Alignof(struct B5) == 8, "B5 align");
_Static_assert(sizeof(struct B6) == 4, "B6 size");
_Static_assert(_Alignof(struct B6) == 2, "B6 align");
_Static_assert(sizeof(struct B7) == 4, "B7 size");
_Static_assert(_Alignof(struct B7) == 4, "B7 align");
_Static_assert(sizeof(struct Z1) == 2, "Z1 size");
_Static_assert(_Alignof(struct Z1) == 1, "Z1 align");
_Static_assert(sizeof(struct Z2) == 8, "Z2 size");
_Static_assert(_Alignof(struct Z2) == 4, "Z2 align");
_Static_assert(sizeof(struct Z3) == 2, "Z3 size");
_Static_assert(_Alignof(struct Z3) == 1, "Z3 align");
_Static_assert(sizeof(struct Z4) == 16, "Z4 size");
_Static_assert(_Alignof(struct Z4) == 8, "Z4 align");
_Static_assert(sizeof(struct Z5) == 1, "Z5 size");
_Static_assert(_Alignof(struct Z5) == 1, "Z5 align");
_Static_assert(sizeof(struct Z6) == 1, "Z6 size");
_Static_assert(_Alignof(struct Z6) == 1, "Z6 align");
_Static_assert(sizeof(struct Z7) == 8, "Z7 size");
_Static_assert(_Alignof(struct Z7) == 8, "Z7 align");
_Static_assert(sizeof(union U1) == 4, "U1 size");
_Static_assert(_Alignof(union U1) == 1, "U1 align");
_Static_assert(sizeof(union U2) == 8, "U2 size");
_Static_assert(_Alignof(union U2) == 1, "U2 align");
_Static_assert(sizeof(union U3) == 1, "U3 size");
_Static_assert(_Alignof(union U3) == 1, "U3 align");
_Static_assert(sizeof(union U4) == 4, "U4 size");
_Static_assert(_Alignof(union U4) == 1, "U4 align");
_Static_assert(sizeof(union U5) == 4, "U5 size");
_Static_assert(_Alignof(union U5) == 1, "U5 align");
_Static_assert(sizeof(struct N1) == 16, "N1 size");
_Static_assert(_Alignof(struct N1) == 8, "N1 align");
_Static_assert(sizeof(struct N2) == 24, "N2 size");
_Static_assert(_Alignof(struct N2) == 8, "N2 align");
_Static_assert(sizeof(struct P1) == 13, "P1 size");
_Static_assert(_Alignof(struct P1) == 1, "P1 align");
_Static_assert(sizeof(struct P2) == 5, "P2 size");
_Static_assert(_Alignof(struct P2) == 1, "P2 align");
_Static_assert(sizeof(struct C5) == 2, "C5 size");
_Static_assert(_Alignof(struct C5) == 1, "C5 align");
_Static_assert(sizeof(struct P3) == 14, "P3 size");
_Static_assert(_Alignof(struct P3) == 2, "P3 align");
_Static_assert(sizeof(struct P4) == 6, "P4 size");
_Static_assert(_Alignof(struct P4) == 2, "P4 align");
_Static_assert(sizeof(struct P5) == 12, "P5 size");
_Static_assert(_Alignof(struct P5) == 4, "P5 align");
_Static_assert(sizeof(struct P6) == 1, "P6 size");
_Static_assert(_Alignof(struct P6) == 1, "P6 align");
_Static_assert(sizeof(struct P7) == 16, "P7 size");
_Static_assert(_Alignof(struct P7) == 8, "P7 align");
_Static_assert(sizeof(struct C8) == 16, "C8 size");
_Static_assert(_Alignof(struct C8) == 16, "C8 align");
_Static_assert(sizeof(struct P8) == 16, "P8 size");
_Static_assert(_Alignof(struct P8) == 8, "P8 align");
_Static_assert(sizeof(struct A1) == 16, "A1 size");
_Static_assert(_Alignof(struct A1) == 16, "A1 align");
_Static_assert(sizeof(struct A2) == 32, "A2 size");
_Static_assert(_Alignof(struct A2) == 16, "A2 align");
_Static_assert(sizeof(struct A6) == 32, "A6 size");
_Static_assert(_Alignof(struct A6) == 16, "A6 align");
_Static_assert(sizeof(struct A7) == 32, "A7 size");
_Static_assert(_Alignof(struct A7) == 16, "A7 align");
_Static_assert(sizeof(struct A9) == 16, "A9 size");
_Static_assert(_Alignof(struct A9) == 8, "A9 align");
_Static_assert(sizeof(struct A10) == 8, "A10 size");
_Static_assert(_Alignof(struct A10) == 8, "A10 align");
_Static_assert(sizeof(struct A11) == 4, "A11 size");
_Static_assert(_Alignof(struct A11) == 4, "A11 align");
_Static_assert(sizeof(struct Inner1) == 16, "Inner1 size");
_Static_assert(_Alignof(struct Inner1) == 8, "Inner1 align");
_Static_assert(sizeof(struct C11) == 24, "C11 size");
_Static_assert(_Alignof(struct C11) == 8, "C11 align");
_Static_assert(sizeof(struct C9) == 4, "C9 size");
_Static_assert(_Alignof(struct C9) == 4, "C9 align");
_Static_assert(sizeof(struct C10) == 8, "C10 size");
_Static_assert(_Alignof(struct C10) == 4, "C10 align");
_Static_assert(sizeof(struct F1) == 4, "F1 size");
_Static_assert(_Alignof(struct F1) == 4, "F1 align");
_Static_assert(sizeof(struct F2) == 8, "F2 size");
_Static_assert(_Alignof(struct F2) == 8, "F2 align");
_Static_assert(sizeof(struct F3) == 8, "F3 size");
_Static_assert(_Alignof(struct F3) == 8, "F3 align");
_Static_assert(sizeof(struct R3) == 16, "R3 size");
_Static_assert(_Alignof(struct R3) == 8, "R3 align");
_Static_assert(sizeof(struct R4) == 20, "R4 size");
_Static_assert(_Alignof(struct R4) == 4, "R4 align");
_Static_assert(sizeof(struct R5) == 32, "R5 size");
_Static_assert(_Alignof(struct R5) == 8, "R5 align");
_Static_assert(sizeof(struct D1) == 8, "D1 size");
_Static_assert(_Alignof(struct D1) == 4, "D1 align");
_Static_assert(sizeof(struct D2) == 16, "D2 size");
_Static_assert(_Alignof(struct D2) == 8, "D2 align");
_Static_assert(sizeof(struct D3) == 5, "D3 size");
_Static_assert(_Alignof(struct D3) == 1, "D3 align");
_Static_assert(sizeof(struct D4) == 4, "D4 size");
_Static_assert(_Alignof(struct D4) == 2, "D4 align");
_Static_assert(sizeof(struct D5) == 4, "D5 size");
_Static_assert(_Alignof(struct D5) == 4, "D5 align");
_Static_assert(sizeof(struct D6) == 8, "D6 size");
_Static_assert(_Alignof(struct D6) == 2, "D6 align");
_Static_assert(sizeof(struct G3) == 12, "G3 size");
_Static_assert(_Alignof(struct G3) == 4, "G3 align");
_Static_assert(sizeof(struct G5) == 4, "G5 size");
_Static_assert(_Alignof(struct G5) == 4, "G5 align");
_Static_assert(sizeof(struct G6) == 8, "G6 size");
_Static_assert(_Alignof(struct G6) == 4, "G6 align");
_Static_assert(sizeof(struct G7) == 8, "G7 size");
_Static_assert(_Alignof(struct G7) == 4, "G7 align");
_Static_assert(sizeof(struct G8) == 4, "G8 size");
_Static_assert(_Alignof(struct G8) == 4, "G8 align");

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
// TARGET-NEXT:     type @type[[TYPE_B1:[0-9]+]] B1 = struct {
// TARGET-NEXT:         field0 a: i8 : 4;
// TARGET-NEXT:         field1 b: i32 : 4;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), Some(32)], bit_units=[(0, 1), (4, 4)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_B2:[0-9]+]] B2 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 b: i8 : 5;
// TARGET-NEXT:         field2 c: i8 : 1;
// TARGET-NEXT:     } [size=2, align=1, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(3), Some(8)], bit_units=[(0, 1), (1, 1)], field_units=[Some(0), Some(0), Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_B3:[0-9]+]] B3 = struct {
// TARGET-NEXT:         field0 a: i32 : 20;
// TARGET-NEXT:         field1 b: i32 : 20;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), Some(32)], bit_units=[(0, 4), (4, 4)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_B4:[0-9]+]] B4 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 a: i32 : 3;
// TARGET-NEXT:         field2 d: i8;
// TARGET-NEXT:     } [size=12, align=4, offsets=[0, 4, 8], bit_offsets=[None, Some(32), None], bit_units=[(4, 4)], field_units=[None, Some(0), None]];
// TARGET-NEXT:     type @type[[TYPE_B5:[0-9]+]] B5 = struct {
// TARGET-NEXT:         field0 a: i64 : 3;
// TARGET-NEXT:         field1 b: i32 : 3;
// TARGET-NEXT:         field2 c: i64 : 60;
// TARGET-NEXT:     } [size=24, align=8, offsets=[0, 8, 16], bit_offsets=[Some(0), Some(64), Some(128)], bit_units=[(0, 8), (8, 4), (16, 8)], field_units=[Some(0), Some(1), Some(2)]];
// TARGET-NEXT:     type @type[[TYPE_B6:[0-9]+]] B6 = struct {
// TARGET-NEXT:         field0 a: bool : 1;
// TARGET-NEXT:         field1 b: i8 : 2;
// TARGET-NEXT:         field2 c: u16 : 9;
// TARGET-NEXT:     } [size=4, align=2, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(1), Some(16)], bit_units=[(0, 1), (2, 2)], field_units=[Some(0), Some(0), Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : u32 {
// TARGET-NEXT:         %[[VALUE_EA:[0-9]+]] EA = const<i32>(0);
// TARGET-NEXT:         %[[VALUE_EB:[0-9]+]] EB = const<i32>(1);
// TARGET-NEXT:     } [size=4, align=4];
// TARGET-NEXT:     type @type[[TYPE_B7:[0-9]+]] B7 = struct {
// TARGET-NEXT:         field0 a: @type[[TYPE_E]] : 2;
// TARGET-NEXT:         field1 b: i32 : 3;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(2)], bit_units=[(0, 4)], field_units=[Some(0), Some(0)]];
// TARGET-NEXT:     type @type[[TYPE_Z1:[0-9]+]] Z1 = struct {
// TARGET-NEXT:         field0 a: i8;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:         field2 b: i8;
// TARGET-NEXT:     } [size=2, align=1, offsets=[0, 1, 1], bit_offsets=[None, Some(8), None]];
// TARGET-NEXT:     type @type[[TYPE_Z2:[0-9]+]] Z2 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:         field2 b: i8;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4, 4], bit_offsets=[Some(0), Some(32), None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// TARGET-NEXT:     type @type[[TYPE_Z3:[0-9]+]] Z3 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 <anonymous>: i8 : 0;
// TARGET-NEXT:         field2 b: i8 : 3;
// TARGET-NEXT:     } [size=2, align=1, offsets=[0, 1, 1], bit_offsets=[Some(0), Some(8), Some(8)], bit_units=[(0, 1), (1, 1)], field_units=[Some(0), None, Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_Z4:[0-9]+]] Z4 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 <anonymous>: i64 : 0;
// TARGET-NEXT:         field2 b: i8;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8, 8], bit_offsets=[Some(0), Some(64), None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// TARGET-NEXT:     type @type[[TYPE_Z5:[0-9]+]] Z5 = struct {
// TARGET-NEXT:         field0 <anonymous>: i32 : 0;
// TARGET-NEXT:         field1 a: i8;
// TARGET-NEXT:     } [size=1, align=1, offsets=[0, 0], bit_offsets=[Some(0), None]];
// TARGET-NEXT:     type @type[[TYPE_Z6:[0-9]+]] Z6 = struct {
// TARGET-NEXT:         field0 a: i8;
// TARGET-NEXT:         field1 <anonymous>: i64 : 0;
// TARGET-NEXT:     } [size=1, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)]];
// TARGET-NEXT:     type @type[[TYPE_Z7:[0-9]+]] Z7 = struct {
// TARGET-NEXT:         field0 a: i8 : 1;
// TARGET-NEXT:         field1 <anonymous>: i64 : 0;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0, 8], bit_offsets=[Some(0), Some(64)], bit_units=[(0, 1)], field_units=[Some(0), None]];
// TARGET-NEXT:     type @type[[TYPE_U1:[0-9]+]] U1 = union {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:         field1 b: i8;
// TARGET-NEXT:     } [size=4, align=1, offsets=[0, 0], bit_offsets=[Some(0), None], bit_units=[(0, 4)], field_units=[Some(0), None]];
// TARGET-NEXT:     type @type[[TYPE_U2:[0-9]+]] U2 = union {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 b: i64 : 5;
// TARGET-NEXT:     } [size=8, align=1, offsets=[0, 0], bit_offsets=[Some(0), Some(0)], bit_units=[(0, 1), (0, 8)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_U3:[0-9]+]] U3 = union {
// TARGET-NEXT:         field0 a: i8;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:     } [size=1, align=1, offsets=[0, 0], bit_offsets=[None, Some(0)]];
// TARGET-NEXT:     type @type[[TYPE_U4:[0-9]+]] U4 = union {
// TARGET-NEXT:         field0 a: i8 : 1;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:     } [size=4, align=1, offsets=[0, 0], bit_offsets=[Some(0), Some(0)], bit_units=[(0, 1)], field_units=[Some(0), None]];
// TARGET-NEXT:     type @type[[TYPE_U5:[0-9]+]] U5 = union {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:     } [size=4, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 4)], field_units=[Some(0)]];
// TARGET-NEXT:     type @type[[TYPE_N1:[0-9]+]] N1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type[[TYPE_N2:[0-9]+]] N2 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: i64;
// TARGET-NEXT:         field2 s: i16;
// TARGET-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// TARGET-NEXT:     type @type[[TYPE_P1:[0-9]+]] P1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:         field2 d: f64;
// TARGET-NEXT:     } [size=13, align=1, offsets=[0, 1, 5]];
// TARGET-NEXT:     type @type[[TYPE_P2:[0-9]+]] P2 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 b: i32 : 5;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 1], bit_offsets=[Some(0), Some(8)], bit_units=[(0, 1), (1, 4)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_C5:[0-9]+]] C5 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:         field2 b: i8;
// TARGET-NEXT:     } [size=2, align=1, offsets=[0, 1, 1], bit_offsets=[Some(0), Some(8), None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// TARGET-NEXT:     type @type[[TYPE_P3:[0-9]+]] P3 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:         field2 d: f64;
// TARGET-NEXT:     } [size=14, align=2, offsets=[0, 2, 6]];
// TARGET-NEXT:     type @type[[TYPE_P4:[0-9]+]] P4 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 b: i32 : 5;
// TARGET-NEXT:     } [size=6, align=2, offsets=[0, 2], bit_offsets=[None, Some(16)], bit_units=[(2, 4)], field_units=[None, Some(0)]];
// TARGET-NEXT:     type @type[[TYPE_P5:[0-9]+]] P5 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// TARGET-NEXT:     type @type[[TYPE_P6:[0-9]+]] P6 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=1, align=1, offsets=[0]];
// TARGET-NEXT:     type @type[[TYPE_P7:[0-9]+]] P7 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type[[TYPE_C8:[0-9]+]] C8 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=16, align=16, offsets=[0]];
// TARGET-NEXT:     type @type[[TYPE_P8:[0-9]+]] P8 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type[[TYPE_A1:[0-9]+]] A1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=16, align=16, offsets=[0]];
// TARGET-NEXT:     type @type[[TYPE_A2:[0-9]+]] A2 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// TARGET-NEXT:     type @type[[TYPE_A6:[0-9]+]] A6 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 inner: @type[[TYPE_A1]];
// TARGET-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// TARGET-NEXT:     type @type[[TYPE_A7:[0-9]+]] A7 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 inner: @type[[TYPE_A1]];
// TARGET-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// TARGET-NEXT:     type @type[[TYPE_A9:[0-9]+]] A9 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: i32;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type[[TYPE_A10:[0-9]+]] A10 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0]];
// TARGET-NEXT:     type @type[[TYPE_A11:[0-9]+]] A11 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0]];
// TARGET-NEXT:     type @type[[TYPE_Inner1:[0-9]+]] Inner1 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: i8;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type[[TYPE_C11:[0-9]+]] C11 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 i: @type[[TYPE_Inner1]];
// TARGET-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type[[TYPE_C9:[0-9]+]] C9 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0]];
// TARGET-NEXT:     type @type[[TYPE_C10:[0-9]+]] C10 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: i8;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// TARGET-NEXT:     type @type[[TYPE_F1:[0-9]+]] F1 = struct {
// TARGET-NEXT:         field0 n: i32;
// TARGET-NEXT:         field1 d: array<i8, incomplete>;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// TARGET-NEXT:     type @type[[TYPE_F2:[0-9]+]] F2 = struct {
// TARGET-NEXT:         field0 n: i8;
// TARGET-NEXT:         field1 d: array<f64, incomplete>;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type[[TYPE_F3:[0-9]+]] F3 = struct {
// TARGET-NEXT:         field0 n: i8;
// TARGET-NEXT:         field1 d: array<i64, 0>;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type[[TYPE_R3:[0-9]+]] R3 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 d: f64;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT:     type @type[[TYPE_R4:[0-9]+]] R4 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 b: array<@type[[TYPE_B1]], 2>;
// TARGET-NEXT:     } [size=20, align=4, offsets=[0, 4]];
// TARGET-NEXT:     type @type[[TYPE_R5:[0-9]+]] R5 = struct {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:         field1 n: @type[[TYPE_N1]];
// TARGET-NEXT:         field2 b: i32 : 3;
// TARGET-NEXT:     } [size=32, align=8, offsets=[0, 8, 24], bit_offsets=[Some(0), None, Some(192)], bit_units=[(0, 4), (24, 4)], field_units=[Some(0), None, Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_D1:[0-9]+]] D1 = struct {
// TARGET-NEXT:         field0 a: i32 : 31;
// TARGET-NEXT:         field1 b: i32 : 2;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), Some(32)], bit_units=[(0, 4), (4, 4)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_D2:[0-9]+]] D2 = struct {
// TARGET-NEXT:         field0 a: u64 : 40;
// TARGET-NEXT:         field1 b: u64 : 30;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), Some(64)], bit_units=[(0, 8), (8, 8)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_D3:[0-9]+]] D3 = struct {
// TARGET-NEXT:         field0 a: i8 : 8;
// TARGET-NEXT:         field1 b: i8 : 8;
// TARGET-NEXT:         field2 c: i8 : 8;
// TARGET-NEXT:         field3 d: i8 : 8;
// TARGET-NEXT:         field4 e: i8 : 8;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 1, 2, 3, 4], bit_offsets=[Some(0), Some(8), Some(16), Some(24), Some(32)], bit_units=[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1)], field_units=[Some(0), Some(1), Some(2), Some(3), Some(4)]];
// TARGET-NEXT:     type @type[[TYPE_D4:[0-9]+]] D4 = struct {
// TARGET-NEXT:         field0 a: i16 : 9;
// TARGET-NEXT:         field1 b: i16 : 8;
// TARGET-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[Some(0), Some(16)], bit_units=[(0, 2), (2, 2)], field_units=[Some(0), Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_D5:[0-9]+]] D5 = struct {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:         field1 b: i32 : 3;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(3)], bit_units=[(0, 4)], field_units=[Some(0), Some(0)]];
// TARGET-NEXT:     type @type[[TYPE_D6:[0-9]+]] D6 = struct {
// TARGET-NEXT:         field0 a: i8;
// TARGET-NEXT:         field1 b: i16 : 3;
// TARGET-NEXT:         field2 c: i8 : 3;
// TARGET-NEXT:         field3 d: i16 : 3;
// TARGET-NEXT:     } [size=8, align=2, offsets=[0, 2, 4, 6], bit_offsets=[None, Some(16), Some(32), Some(48)], bit_units=[(2, 2), (4, 1), (6, 2)], field_units=[None, Some(0), Some(1), Some(2)]];
// TARGET-NEXT:     type @type[[TYPE_G3:[0-9]+]] G3 = struct {
// TARGET-NEXT:         field0 e: array<@type[[TYPE_C9]], 2>;
// TARGET-NEXT:         field1 c: i8;
// TARGET-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// TARGET-NEXT:     type @type[[TYPE_G5:[0-9]+]] G5 = struct {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 4], bit_offsets=[Some(0), Some(32)], bit_units=[(0, 4)], field_units=[Some(0), None]];
// TARGET-NEXT:     type @type[[TYPE_G6:[0-9]+]] G6 = struct {
// TARGET-NEXT:         field0 a: i32 : 3;
// TARGET-NEXT:         field1 <anonymous>: i8 : 0;
// TARGET-NEXT:         field2 b: i8;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4, 4], bit_offsets=[Some(0), Some(32), None], bit_units=[(0, 4)], field_units=[Some(0), None, None]];
// TARGET-NEXT:     type @type[[TYPE_G7:[0-9]+]] G7 = struct {
// TARGET-NEXT:         field0 a: i8 : 3;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:         field2 b: i32 : 3;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 4, 4], bit_offsets=[Some(0), Some(32), Some(32)], bit_units=[(0, 1), (4, 4)], field_units=[Some(0), None, Some(1)]];
// TARGET-NEXT:     type @type[[TYPE_G8:[0-9]+]] G8 = struct {
// TARGET-NEXT:         field0 c: i8;
// TARGET-NEXT:         field1 x: array<i32, incomplete>;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// TARGET-NEXT: }
// SLATE-FILECHECK-END TARGET
