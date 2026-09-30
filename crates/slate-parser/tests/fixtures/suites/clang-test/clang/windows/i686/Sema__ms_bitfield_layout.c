
typedef struct A {
	char x;
	int a : 22;
	int : 0;
	int c : 10;
	char b : 3;
	char d: 4;
	short y;
} A;


typedef struct B {
	char x;
	int : 0;
	short a : 4;
	char y;
} B;


typedef struct C {
	char x;
	short a : 4;
	int : 0;
	char y;
} C;


typedef struct D {
	char x;
	short : 0;
	int : 0;
	char y;
} D;


typedef union E {
	char x;
	long long a : 3;
	int b : 3;
	long long : 0;
	short y;
} E;



typedef struct F {
	char x;
	char a : 3;
	char b : 3;
	char c : 3;
	short d : 6;
	short e : 6;
	short f : 6;
	short g : 11;
	short h : 11;
	short i : 11;
	short y;
} F;


typedef union G {
	char x;
	int a : 3;
	int : 0;
	long long : 0;
	short y;
} G;


typedef struct H {
	unsigned short a : 1;
	unsigned char : 0;
	unsigned long : 0;
	unsigned short c : 1;
} H;


typedef struct I {
	short : 8;
	__declspec(align(16)) short : 8;
} I;



#pragma pack(push, 1)

typedef struct A1 {
	char x;
	int a : 22;
	int : 0;
	int c : 10;
	char b : 3;
	char d: 4;
	short y;
} A1;


typedef struct B1 {
	char x;
	int : 0;
	short a : 4;
	char y;
} B1;


typedef struct C1 {
	char x;
	short a : 4;
	int : 0;
	char y;
} C1;


typedef struct D1 {
	char x;
	short : 0;
	int : 0;
	char y;
} D1;


typedef union E1 {
	char x;
	long long a : 3;
	int b : 3;
	long long : 0;
	short y;
} E1;


typedef struct F1 {
	char x;
	char a : 3;
	char b : 3;
	char c : 3;
	short d : 6;
	short e : 6;
	short f : 6;
	short g : 11;
	short h : 11;
	short i : 11;
	short y;
} F1;


typedef union G1 {
	char x;
	int a : 3;
	int : 0;
	long long : 0;
	short y;
} G1;


typedef struct H1 {
	unsigned long a : 1;
	unsigned char : 0;
	unsigned long : 0;
	unsigned long c : 1;
} H1;


typedef struct I1 {
	short : 8;
	__declspec(align(16)) short : 8;
} I1;


#pragma pack(pop)

int x[
sizeof(A ) +
sizeof(B ) +
sizeof(C ) +
sizeof(D ) +
sizeof(E ) +
sizeof(F ) +
sizeof(G ) +
sizeof(H ) +
sizeof(I ) +
sizeof(A1) +
sizeof(B1) +
sizeof(C1) +
sizeof(D1) +
sizeof(E1) +
sizeof(F1) +
sizeof(G1) +
sizeof(H1) +
sizeof(I1) +
0];

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i32 : 22;
// DEFAULT-NEXT:         field2 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field3 c: i32 : 10;
// DEFAULT-NEXT:         field4 b: i8 : 3;
// DEFAULT-NEXT:         field5 d: i8 : 4;
// DEFAULT-NEXT:         field6 y: i16;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 8, 12, 12, 14], bit_offsets=[None, Some(32), Some(64), Some(64), Some(96), Some(99), None], bit_units=[(4, 4), (8, 4), (12, 1)], field_units=[None, Some(0), None, Some(1), Some(2), Some(2), None]];
// DEFAULT-NEXT:     type @type[[TYPE_A_2:[0-9]+]] A = @type[[TYPE_A]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field2 a: i16 : 4;
// DEFAULT-NEXT:         field3 y: i8;
// DEFAULT-NEXT:     } [size=6, align=2, offsets=[0, 1, 2, 4], bit_offsets=[None, Some(8), Some(16), None], bit_units=[(2, 2)], field_units=[None, None, Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_B_2:[0-9]+]] B = @type[[TYPE_B]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i16 : 4;
// DEFAULT-NEXT:         field2 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field3 y: i8;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 2, 4, 4], bit_offsets=[None, Some(16), Some(32), None], bit_units=[(2, 2)], field_units=[None, Some(0), None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_C_2:[0-9]+]] C = @type[[TYPE_C]];
// DEFAULT-NEXT:     type @type[[TYPE_D:[0-9]+]] D = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i16 : 0;
// DEFAULT-NEXT:         field2 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field3 y: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1, 1, 1], bit_offsets=[None, Some(8), Some(8), None]];
// DEFAULT-NEXT:     type @type[[TYPE_D_2:[0-9]+]] D = @type[[TYPE_D]];
// DEFAULT-NEXT:     type @type[[TYPE_E:[0-9]+]] E = union {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i64 : 3;
// DEFAULT-NEXT:         field2 b: i32 : 3;
// DEFAULT-NEXT:         field3 <anonymous>: i64 : 0;
// DEFAULT-NEXT:         field4 y: i16;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0, 0, 0, 0, 0], bit_offsets=[None, Some(0), Some(0), Some(0), None], bit_units=[(0, 8), (0, 4)], field_units=[None, Some(0), Some(1), None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_E_2:[0-9]+]] E = @type[[TYPE_E]];
// DEFAULT-NEXT:     type @type[[TYPE_F:[0-9]+]] F = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i8 : 3;
// DEFAULT-NEXT:         field2 b: i8 : 3;
// DEFAULT-NEXT:         field3 c: i8 : 3;
// DEFAULT-NEXT:         field4 d: i16 : 6;
// DEFAULT-NEXT:         field5 e: i16 : 6;
// DEFAULT-NEXT:         field6 f: i16 : 6;
// DEFAULT-NEXT:         field7 g: i16 : 11;
// DEFAULT-NEXT:         field8 h: i16 : 11;
// DEFAULT-NEXT:         field9 i: i16 : 11;
// DEFAULT-NEXT:         field10 y: i16;
// DEFAULT-NEXT:     } [size=16, align=2, offsets=[0, 1, 1, 2, 4, 4, 6, 8, 10, 12, 14], bit_offsets=[None, Some(8), Some(11), Some(16), Some(32), Some(38), Some(48), Some(64), Some(80), Some(96), None], bit_units=[(1, 1), (2, 1), (4, 2), (6, 2), (8, 2), (10, 2), (12, 2)], field_units=[None, Some(0), Some(0), Some(1), Some(2), Some(2), Some(3), Some(4), Some(5), Some(6), None]];
// DEFAULT-NEXT:     type @type[[TYPE_F_2:[0-9]+]] F = @type[[TYPE_F]];
// DEFAULT-NEXT:     type @type[[TYPE_G:[0-9]+]] G = union {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i32 : 3;
// DEFAULT-NEXT:         field2 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field3 <anonymous>: i64 : 0;
// DEFAULT-NEXT:         field4 y: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 0, 0, 0, 0], bit_offsets=[None, Some(0), Some(0), Some(0), None], bit_units=[(0, 4)], field_units=[None, Some(0), None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_G_2:[0-9]+]] G = @type[[TYPE_G]];
// DEFAULT-NEXT:     type @type[[TYPE_H:[0-9]+]] H = struct {
// DEFAULT-NEXT:         field0 a: u16 : 1;
// DEFAULT-NEXT:         field1 <anonymous>: u8 : 0;
// DEFAULT-NEXT:         field2 <anonymous>: u32 : 0;
// DEFAULT-NEXT:         field3 c: u16 : 1;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2, 2, 2], bit_offsets=[Some(0), Some(16), Some(16), Some(16)], bit_units=[(0, 2), (2, 2)], field_units=[Some(0), None, None, Some(1)]];
// DEFAULT-NEXT:     type @type[[TYPE_H_2:[0-9]+]] H = @type[[TYPE_H]];
// DEFAULT-NEXT:     type @type[[TYPE_I:[0-9]+]] I = struct {
// DEFAULT-NEXT:         field0 <anonymous>: i16 : 8;
// DEFAULT-NEXT:         field1 <anonymous>: i16 : 8;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0, 1], bit_offsets=[Some(0), Some(8)], bit_units=[(0, 2)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_I_2:[0-9]+]] I = @type[[TYPE_I]];
// DEFAULT-NEXT:     type @type[[TYPE_A1:[0-9]+]] A1 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i32 : 22;
// DEFAULT-NEXT:         field2 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field3 c: i32 : 10;
// DEFAULT-NEXT:         field4 b: i8 : 3;
// DEFAULT-NEXT:         field5 d: i8 : 4;
// DEFAULT-NEXT:         field6 y: i16;
// DEFAULT-NEXT:     } [size=12, align=1, offsets=[0, 1, 5, 5, 9, 9, 10], bit_offsets=[None, Some(8), Some(40), Some(40), Some(72), Some(75), None], bit_units=[(1, 4), (5, 4), (9, 1)], field_units=[None, Some(0), None, Some(1), Some(2), Some(2), None]];
// DEFAULT-NEXT:     type @type[[TYPE_A1_2:[0-9]+]] A1 = @type[[TYPE_A1]];
// DEFAULT-NEXT:     type @type[[TYPE_B1:[0-9]+]] B1 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field2 a: i16 : 4;
// DEFAULT-NEXT:         field3 y: i8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 1, 1, 3], bit_offsets=[None, Some(8), Some(8), None], bit_units=[(1, 2)], field_units=[None, None, Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_B1_2:[0-9]+]] B1 = @type[[TYPE_B1]];
// DEFAULT-NEXT:     type @type[[TYPE_C1:[0-9]+]] C1 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i16 : 4;
// DEFAULT-NEXT:         field2 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field3 y: i8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 1, 3, 3], bit_offsets=[None, Some(8), Some(24), None], bit_units=[(1, 2)], field_units=[None, Some(0), None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_C1_2:[0-9]+]] C1 = @type[[TYPE_C1]];
// DEFAULT-NEXT:     type @type[[TYPE_D1:[0-9]+]] D1 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i16 : 0;
// DEFAULT-NEXT:         field2 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field3 y: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1, 1, 1], bit_offsets=[None, Some(8), Some(8), None]];
// DEFAULT-NEXT:     type @type[[TYPE_D1_2:[0-9]+]] D1 = @type[[TYPE_D1]];
// DEFAULT-NEXT:     type @type[[TYPE_E1:[0-9]+]] E1 = union {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i64 : 3;
// DEFAULT-NEXT:         field2 b: i32 : 3;
// DEFAULT-NEXT:         field3 <anonymous>: i64 : 0;
// DEFAULT-NEXT:         field4 y: i16;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0, 0, 0, 0, 0], bit_offsets=[None, Some(0), Some(0), Some(0), None], bit_units=[(0, 8), (0, 4)], field_units=[None, Some(0), Some(1), None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_E1_2:[0-9]+]] E1 = @type[[TYPE_E1]];
// DEFAULT-NEXT:     type @type[[TYPE_F1:[0-9]+]] F1 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i8 : 3;
// DEFAULT-NEXT:         field2 b: i8 : 3;
// DEFAULT-NEXT:         field3 c: i8 : 3;
// DEFAULT-NEXT:         field4 d: i16 : 6;
// DEFAULT-NEXT:         field5 e: i16 : 6;
// DEFAULT-NEXT:         field6 f: i16 : 6;
// DEFAULT-NEXT:         field7 g: i16 : 11;
// DEFAULT-NEXT:         field8 h: i16 : 11;
// DEFAULT-NEXT:         field9 i: i16 : 11;
// DEFAULT-NEXT:         field10 y: i16;
// DEFAULT-NEXT:     } [size=15, align=1, offsets=[0, 1, 1, 2, 3, 3, 5, 7, 9, 11, 13], bit_offsets=[None, Some(8), Some(11), Some(16), Some(24), Some(30), Some(40), Some(56), Some(72), Some(88), None], bit_units=[(1, 1), (2, 1), (3, 2), (5, 2), (7, 2), (9, 2), (11, 2)], field_units=[None, Some(0), Some(0), Some(1), Some(2), Some(2), Some(3), Some(4), Some(5), Some(6), None]];
// DEFAULT-NEXT:     type @type[[TYPE_F1_2:[0-9]+]] F1 = @type[[TYPE_F1]];
// DEFAULT-NEXT:     type @type[[TYPE_G1:[0-9]+]] G1 = union {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i32 : 3;
// DEFAULT-NEXT:         field2 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field3 <anonymous>: i64 : 0;
// DEFAULT-NEXT:         field4 y: i16;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 0, 0, 0, 0], bit_offsets=[None, Some(0), Some(0), Some(0), None], bit_units=[(0, 4)], field_units=[None, Some(0), None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_G1_2:[0-9]+]] G1 = @type[[TYPE_G1]];
// DEFAULT-NEXT:     type @type[[TYPE_H1:[0-9]+]] H1 = struct {
// DEFAULT-NEXT:         field0 a: u32 : 1;
// DEFAULT-NEXT:         field1 <anonymous>: u8 : 0;
// DEFAULT-NEXT:         field2 <anonymous>: u32 : 0;
// DEFAULT-NEXT:         field3 c: u32 : 1;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0, 4, 4, 4], bit_offsets=[Some(0), Some(32), Some(32), Some(32)], bit_units=[(0, 4), (4, 4)], field_units=[Some(0), None, None, Some(1)]];
// DEFAULT-NEXT:     type @type[[TYPE_H1_2:[0-9]+]] H1 = @type[[TYPE_H1]];
// DEFAULT-NEXT:     type @type[[TYPE_I1:[0-9]+]] I1 = struct {
// DEFAULT-NEXT:         field0 <anonymous>: i16 : 8;
// DEFAULT-NEXT:         field1 <anonymous>: i16 : 8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[Some(0), Some(8)], bit_units=[(0, 2)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_I1_2:[0-9]+]] I1 = @type[[TYPE_I1]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: array<i32, 125> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
