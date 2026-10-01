
struct s1 {
  int a;
  int b;
};
struct s1 f1_1(void) { while (1) {} }
void f1_2(struct s1 a0) {}

struct s2 {
  short a;
  short b;
};
struct s2 f2_1(void) { while (1) {} }

struct s3 {
  char a;
  char b;
};
struct s3 f3_1(void) { while (1) {} }

struct s4 {
  char a:4;
  char b:4;
};
struct s4 f4_1(void) { while (1) {} }

struct s5 {
  double a;
};
struct s5 f5_1(void) { while (1) {} }
void f5_2(struct s5 a0) {}

struct s6 {
  float a;
};
struct s6 f6_1(void) { while (1) {} }
void f6_2(struct s6 a0) {}


// MSVC passes up to three vectors in registers, and the rest indirectly.  We
// (arbitrarily) pass oversized vectors indirectly, since that is the safest way
// to do it.
typedef float __m128 __attribute__((__vector_size__(16), __aligned__(16)));
typedef float __m256 __attribute__((__vector_size__(32), __aligned__(32)));
typedef float __m512 __attribute__((__vector_size__(64), __aligned__(64)));
typedef float __m1024 __attribute__((__vector_size__(128), __aligned__(128)));

__m128 gv128;
__m256 gv256;
__m512 gv512;
__m1024 gv1024;

void receive_vec_128(__m128 x, __m128 y, __m128 z, __m128 w, __m128 q) {
  gv128 = x + y + z + w + q;
}
void receive_vec_256(__m256 x, __m256 y, __m256 z, __m256 w, __m256 q) {
  gv256 = x + y + z + w + q;
}
void receive_vec_512(__m512 x, __m512 y, __m512 z, __m512 w, __m512 q) {
  gv512 = x + y + z + w + q;
}
void receive_vec_1024(__m1024 x, __m1024 y, __m1024 z, __m1024 w, __m1024 q) {
  gv1024 = x + y + z + w + q;
}

void pass_vec_128(void) {
  __m128 z = {0};
  receive_vec_128(z, z, z, z, z);
}



void __fastcall fastcall_indirect_vec(__m128 x, __m128 y, __m128 z, __m128 w, int edx, __m128 q) {
  gv128 = x + y + z + w + q;
}

struct __declspec(align(1)) Align1 { unsigned long long x; };
struct __declspec(align(4)) Align4 { unsigned long long x; };
struct __declspec(align(8)) Align8 { unsigned long long x; };
void receive_align1(struct Align1 o);
void receive_align4(struct Align4 o);
void receive_align8(struct Align8 o);
void pass_underaligned_record() {
  struct Align1 a1;
  receive_align1(a1);
  struct Align4 a4;
  receive_align4(a4);
  struct Align8 a8;
  receive_align8(a8);
}

struct FieldAlign1 { unsigned long long __declspec(align(1)) x; };
struct FieldAlign4 { unsigned long long __declspec(align(4)) x; };
struct FieldAlign8 { unsigned long long __declspec(align(8)) x; };
void receive_falign1(struct FieldAlign1 o);
void receive_falign4(struct FieldAlign4 o);
void receive_falign8(struct FieldAlign8 o);
void pass_underaligned_record_field() {
  struct FieldAlign1 a1;
  receive_falign1(a1);
  struct FieldAlign4 a4;
  receive_falign4(a4);
  struct FieldAlign8 a8;
  receive_falign8(a8);
}

struct __declspec(align(8)) BigAligned {
  int big[5];
};

void receive_aligned_variadic(int f, ...);
void pass_aligned_variadic() {
  struct Align8 a8 = {42};
  struct FieldAlign8 f8 = {42};
  struct BigAligned big;
  receive_aligned_variadic(1, a8, f8, big);
}
// MSVC doesn't pass aligned objects to variadic functions indirectly.


void receive_fixed_align_variadic(struct BigAligned big, ...);
void pass_fixed_align_variadic() {
  struct BigAligned big;
  receive_fixed_align_variadic(big, 42);
}
// MSVC emits error C2719 and C3916 when receiving and passing arguments with
// required alignment greater than 4 to the fixed part of a variadic function
// prototype, but it's actually easier to just implement this functionality
// correctly in Clang than it is to be bug for bug compatible, so we pass such
// arguments indirectly.

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
// DEFAULT-NEXT:     type @type[[TYPE_s1:[0-9]+]] s1 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 a: i16;
// DEFAULT-NEXT:         field1 b: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_s3:[0-9]+]] s3 = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_s4:[0-9]+]] s4 = struct {
// DEFAULT-NEXT:         field0 a: i8 : 4;
// DEFAULT-NEXT:         field1 b: i8 : 4;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 0], bit_offsets=[Some(0), Some(4)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_s5:[0-9]+]] s5 = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_s6:[0-9]+]] s6 = struct {
// DEFAULT-NEXT:         field0 a: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE___m128:[0-9]+]] __m128 = vector<f32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE___m256:[0-9]+]] __m256 = vector<f32, 8>;
// DEFAULT-NEXT:     type @type[[TYPE___m512:[0-9]+]] __m512 = vector<f32, 16>;
// DEFAULT-NEXT:     type @type[[TYPE___m1024:[0-9]+]] __m1024 = vector<f32, 32>;
// DEFAULT-NEXT:     type @type[[TYPE_Align1:[0-9]+]] Align1 = struct {
// DEFAULT-NEXT:         field0 x: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Align4:[0-9]+]] Align4 = struct {
// DEFAULT-NEXT:         field0 x: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Align8:[0-9]+]] Align8 = struct {
// DEFAULT-NEXT:         field0 x: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_FieldAlign1:[0-9]+]] FieldAlign1 = struct {
// DEFAULT-NEXT:         field0 x: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_FieldAlign4:[0-9]+]] FieldAlign4 = struct {
// DEFAULT-NEXT:         field0 x: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_FieldAlign8:[0-9]+]] FieldAlign8 = struct {
// DEFAULT-NEXT:         field0 x: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_BigAligned:[0-9]+]] BigAligned = struct {
// DEFAULT-NEXT:         field0 big: array<i32, 5>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_gv128:[0-9]+]] gv128: vector<f32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_gv256:[0-9]+]] gv256: vector<f32, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_gv512:[0-9]+]] gv512: vector<f32, 16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_gv1024:[0-9]+]] gv1024: vector<f32, 32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1_1:[0-9]+]] @f1_1() -> @type[[TYPE_s1]] [linkage=external] [abi=x86_win32() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1_2:[0-9]+]] @f1_2(%[[VALUE_a0:[0-9]+]] a0: @type[[TYPE_s1]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2_1:[0-9]+]] @f2_1() -> @type[[TYPE_s2]] [linkage=external] [abi=x86_win32() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3_1:[0-9]+]] @f3_1() -> @type[[TYPE_s3]] [linkage=external] [abi=x86_win32() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE2:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4_1:[0-9]+]] @f4_1() -> @type[[TYPE_s4]] [linkage=external] [abi=x86_win32() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE3:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5_1:[0-9]+]] @f5_1() -> @type[[TYPE_s5]] [linkage=external] [abi=x86_win32() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE4:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5_2:[0-9]+]] @f5_2(%[[VALUE_a0_2:[0-9]+]] a0: @type[[TYPE_s5]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6_1:[0-9]+]] @f6_1() -> @type[[TYPE_s6]] [linkage=external] [abi=x86_win32() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE5:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6_2:[0-9]+]] @f6_2(%[[VALUE_a0_3:[0-9]+]] a0: @type[[TYPE_s6]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_receive_vec_128:[0-9]+]] @receive_vec_128(%[[VALUE_x:[0-9]+]] x: vector<f32, 4>, %[[VALUE_y:[0-9]+]] y: vector<f32, 4>, %[[VALUE_z:[0-9]+]] z: vector<f32, 4>, %[[VALUE_w:[0-9]+]] w: vector<f32, 4>, %[[VALUE_q:[0-9]+]] q: vector<f32, 4>) -> void [linkage=external] [abi=x86_win32(direct, direct, direct, byref<align=16>, byref<align=16>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_gv128]], add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_x]]), read<vector<f32, 4>>(%[[VALUE_y]])), read<vector<f32, 4>>(%[[VALUE_z]])), read<vector<f32, 4>>(%[[VALUE_w]])), read<vector<f32, 4>>(%[[VALUE_q]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_receive_vec_256:[0-9]+]] @receive_vec_256(%[[VALUE_x_2:[0-9]+]] x: vector<f32, 8>, %[[VALUE_y_2:[0-9]+]] y: vector<f32, 8>, %[[VALUE_z_2:[0-9]+]] z: vector<f32, 8>, %[[VALUE_w_2:[0-9]+]] w: vector<f32, 8>, %[[VALUE_q_2:[0-9]+]] q: vector<f32, 8>) -> void [linkage=external] [abi=x86_win32(direct, direct, direct, byref<align=32>, byref<align=32>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<vector<f32, 8>>(%[[VALUE_gv256]], add<vector<f32, 8>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 8>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 8>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 8>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 8>>(%[[VALUE_x_2]]), read<vector<f32, 8>>(%[[VALUE_y_2]])), read<vector<f32, 8>>(%[[VALUE_z_2]])), read<vector<f32, 8>>(%[[VALUE_w_2]])), read<vector<f32, 8>>(%[[VALUE_q_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_receive_vec_512:[0-9]+]] @receive_vec_512(%[[VALUE_x_3:[0-9]+]] x: vector<f32, 16>, %[[VALUE_y_3:[0-9]+]] y: vector<f32, 16>, %[[VALUE_z_3:[0-9]+]] z: vector<f32, 16>, %[[VALUE_w_3:[0-9]+]] w: vector<f32, 16>, %[[VALUE_q_3:[0-9]+]] q: vector<f32, 16>) -> void [linkage=external] [abi=x86_win32(direct, direct, direct, byref<align=64>, byref<align=64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<vector<f32, 16>>(%[[VALUE_gv512]], add<vector<f32, 16>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 16>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 16>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 16>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 16>>(%[[VALUE_x_3]]), read<vector<f32, 16>>(%[[VALUE_y_3]])), read<vector<f32, 16>>(%[[VALUE_z_3]])), read<vector<f32, 16>>(%[[VALUE_w_3]])), read<vector<f32, 16>>(%[[VALUE_q_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_receive_vec_1024:[0-9]+]] @receive_vec_1024(%[[VALUE_x_4:[0-9]+]] x: vector<f32, 32>, %[[VALUE_y_4:[0-9]+]] y: vector<f32, 32>, %[[VALUE_z_4:[0-9]+]] z: vector<f32, 32>, %[[VALUE_w_4:[0-9]+]] w: vector<f32, 32>, %[[VALUE_q_4:[0-9]+]] q: vector<f32, 32>) -> void [linkage=external] [abi=x86_win32(byref<align=128>, byref<align=128>, byref<align=128>, byref<align=128>, byref<align=128>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<vector<f32, 32>>(%[[VALUE_gv1024]], add<vector<f32, 32>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 32>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 32>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 32>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 32>>(%[[VALUE_x_4]]), read<vector<f32, 32>>(%[[VALUE_y_4]])), read<vector<f32, 32>>(%[[VALUE_z_4]])), read<vector<f32, 32>>(%[[VALUE_w_4]])), read<vector<f32, 32>>(%[[VALUE_q_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pass_vec_128:[0-9]+]] @pass_vec_128() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_z_5:[0-9]+]] z: vector<f32, 4> [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=true>(index0 = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(vector<f32, 4>, vector<f32, 4>, vector<f32, 4>, vector<f32, 4>, vector<f32, 4>) -> void, abi=x86_win32(direct, direct, direct, byref<align=16>, byref<align=16>) -> void>(%[[VALUE_receive_vec_128]], read<vector<f32, 4>>(%[[VALUE_z_5]]), read<vector<f32, 4>>(%[[VALUE_z_5]]), read<vector<f32, 4>>(%[[VALUE_z_5]]), read<vector<f32, 4>>(%[[VALUE_z_5]]), read<vector<f32, 4>>(%[[VALUE_z_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fastcall_indirect_vec:[0-9]+]] @fastcall_indirect_vec(%[[VALUE_x_5:[0-9]+]] x: vector<f32, 4>, %[[VALUE_y_5:[0-9]+]] y: vector<f32, 4>, %[[VALUE_z_6:[0-9]+]] z: vector<f32, 4>, %[[VALUE_w_5:[0-9]+]] w: vector<f32, 4>, %[[VALUE_edx:[0-9]+]] edx: i32, %[[VALUE_q_5:[0-9]+]] q: vector<f32, 4>) -> void [linkage=external] [abi=x86_win32 fastcall(direct, direct, direct, byref<align=16>, scalar, byref<align=16>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_gv128]], add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_x_5]]), read<vector<f32, 4>>(%[[VALUE_y_5]])), read<vector<f32, 4>>(%[[VALUE_z_6]])), read<vector<f32, 4>>(%[[VALUE_w_5]])), read<vector<f32, 4>>(%[[VALUE_q_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_receive_align1:[0-9]+]] @receive_align1(%[[VALUE_o:[0-9]+]] o: @type[[TYPE_Align1]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_receive_align4:[0-9]+]] @receive_align4(%[[VALUE_o_2:[0-9]+]] o: @type[[TYPE_Align4]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_receive_align8:[0-9]+]] @receive_align8(%[[VALUE_o_3:[0-9]+]] o: @type[[TYPE_Align8]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_pass_underaligned_record:[0-9]+]] @pass_underaligned_record(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a1:[0-9]+]] a1: @type[[TYPE_Align1]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_Align1]]) -> void, abi=x86_win32(native_c) -> void>(%[[VALUE_receive_align1]], copy<@type[[TYPE_Align1]], reason=arg>(read<@type[[TYPE_Align1]]>(%[[VALUE_a1]])));
// DEFAULT-NEXT:         let %[[VALUE_a4:[0-9]+]] a4: @type[[TYPE_Align4]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_Align4]]) -> void, abi=x86_win32(native_c) -> void>(%[[VALUE_receive_align4]], copy<@type[[TYPE_Align4]], reason=arg>(read<@type[[TYPE_Align4]]>(%[[VALUE_a4]])));
// DEFAULT-NEXT:         let %[[VALUE_a8:[0-9]+]] a8: @type[[TYPE_Align8]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_Align8]]) -> void, abi=x86_win32(native_c) -> void>(%[[VALUE_receive_align8]], copy<@type[[TYPE_Align8]], reason=arg>(read<@type[[TYPE_Align8]]>(%[[VALUE_a8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_receive_falign1:[0-9]+]] @receive_falign1(%[[VALUE_o_4:[0-9]+]] o: @type[[TYPE_FieldAlign1]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_receive_falign4:[0-9]+]] @receive_falign4(%[[VALUE_o_5:[0-9]+]] o: @type[[TYPE_FieldAlign4]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_receive_falign8:[0-9]+]] @receive_falign8(%[[VALUE_o_6:[0-9]+]] o: @type[[TYPE_FieldAlign8]]) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_pass_underaligned_record_field:[0-9]+]] @pass_underaligned_record_field(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a1_2:[0-9]+]] a1: @type[[TYPE_FieldAlign1]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_FieldAlign1]]) -> void, abi=x86_win32(native_c) -> void>(%[[VALUE_receive_falign1]], copy<@type[[TYPE_FieldAlign1]], reason=arg>(read<@type[[TYPE_FieldAlign1]]>(%[[VALUE_a1_2]])));
// DEFAULT-NEXT:         let %[[VALUE_a4_2:[0-9]+]] a4: @type[[TYPE_FieldAlign4]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_FieldAlign4]]) -> void, abi=x86_win32(native_c) -> void>(%[[VALUE_receive_falign4]], copy<@type[[TYPE_FieldAlign4]], reason=arg>(read<@type[[TYPE_FieldAlign4]]>(%[[VALUE_a4_2]])));
// DEFAULT-NEXT:         let %[[VALUE_a8_2:[0-9]+]] a8: @type[[TYPE_FieldAlign8]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_FieldAlign8]]) -> void, abi=x86_win32(native_c) -> void>(%[[VALUE_receive_falign8]], copy<@type[[TYPE_FieldAlign8]], reason=arg>(read<@type[[TYPE_FieldAlign8]]>(%[[VALUE_a8_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_receive_aligned_variadic:[0-9]+]] @receive_aligned_variadic(%[[VALUE_f:[0-9]+]] f: i32, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pass_aligned_variadic:[0-9]+]] @pass_aligned_variadic(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a8_3:[0-9]+]] a8: @type[[TYPE_Align8]] [storage=automatic] = aggregate<@type[[TYPE_Align8]], zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(42))));
// DEFAULT-NEXT:         let %[[VALUE_f8:[0-9]+]] f8: @type[[TYPE_FieldAlign8]] [storage=automatic] = aggregate<@type[[TYPE_FieldAlign8]], zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(42))));
// DEFAULT-NEXT:         let %[[VALUE_big:[0-9]+]] big: @type[[TYPE_BigAligned]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=x86_win32(scalar, native_c, native_c, native_c) -> void>(%[[VALUE_receive_aligned_variadic]], const<i32>(1), copy<@type[[TYPE_Align8]], reason=vararg>(read<@type[[TYPE_Align8]]>(%[[VALUE_a8_3]])), copy<@type[[TYPE_FieldAlign8]], reason=vararg>(read<@type[[TYPE_FieldAlign8]]>(%[[VALUE_f8]])), copy<@type[[TYPE_BigAligned]], reason=vararg>(read<@type[[TYPE_BigAligned]]>(%[[VALUE_big]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_receive_fixed_align_variadic:[0-9]+]] @receive_fixed_align_variadic(%[[VALUE_big_2:[0-9]+]] big: @type[[TYPE_BigAligned]], ...) -> void [linkage=external] [abi=x86_win32(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_pass_fixed_align_variadic:[0-9]+]] @pass_fixed_align_variadic(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_big_3:[0-9]+]] big: @type[[TYPE_BigAligned]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_BigAligned]], ...) -> void, abi=x86_win32(native_c, scalar) -> void>(%[[VALUE_receive_fixed_align_variadic]], copy<@type[[TYPE_BigAligned]], reason=arg>(read<@type[[TYPE_BigAligned]]>(%[[VALUE_big_3]])), const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
