
// This test is verifying that the LLVM ABI library classifies argument types in
// the same way that Clang does without the library.

// The AArch64 support in the ABI library is a work in progress. New test cases
// will be added here as the types are implemented. Unimplemented cases will
// report a warning if the ABI library is used.

void arg_void(void) {
}

void arg_bool(_Bool b) {}

void arg_char(char c) {}

void arg_short(short s) {}

void arg_ushort(unsigned short us) {}

void arg_int(int i) {}

void arg_uint(unsigned int ui) {}

void arg_long(long int li) {}

void arg_float16(_Float16 f16) {}

void arg_fp16(__fp16 f16) {}

void arg_float(float f) {}

void arg_double(double d) {}

int gi;
void arg_int_ptr(int* pi) {}

void arg_void_ptr(void* pv) {}

typedef float fx2x2_t __attribute__((matrix_type(2, 2)));
void arg_matrix(fx2x2_t m) {}

// Transparent unions are passed as their first field.
typedef union {
  int i;
  float f;
} tu_int_t __attribute__((transparent_union));
void arg_transparent_union_int(tu_int_t tu) {}

typedef union {
  char c;
  signed char sc;
} tu_char_t __attribute__((transparent_union));
void arg_transparent_union_char(tu_char_t tu) {}

typedef union {
  void *p;
  int *ip;
} tu_ptr_t __attribute__((transparent_union));
void arg_transparent_union_ptr(tu_ptr_t tu) {}

void arg_bitint7(_BitInt(7) x) {}

void arg_ubitint7(unsigned _BitInt(7) x) {}

void arg_bitint65(_BitInt(65) x) {}

void arg_bitint128(_BitInt(128) x) {}

void arg_bitint129(_BitInt(129) x) {}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
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
// DEFAULT-NEXT:     type @type[[TYPE_fx2x2_t:[0-9]+]] fx2x2_t = f32;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_tu_int_t:[0-9]+]] tu_int_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 sc: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_tu_char_t:[0-9]+]] tu_char_t = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 p: ptr<void>;
// DEFAULT-NEXT:         field1 ip: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_tu_ptr_t:[0-9]+]] tu_ptr_t = @type[[TYPE2]];
// DEFAULT-NEXT:     global %[[VALUE_gi:[0-9]+]] gi: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_arg_void:[0-9]+]] @arg_void() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_bool:[0-9]+]] @arg_bool(%[[VALUE_b:[0-9]+]] b: bool) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_char:[0-9]+]] @arg_char(%[[VALUE_c:[0-9]+]] c: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_short:[0-9]+]] @arg_short(%[[VALUE_s:[0-9]+]] s: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_ushort:[0-9]+]] @arg_ushort(%[[VALUE_us:[0-9]+]] us: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_int:[0-9]+]] @arg_int(%[[VALUE_i:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_uint:[0-9]+]] @arg_uint(%[[VALUE_ui:[0-9]+]] ui: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_long:[0-9]+]] @arg_long(%[[VALUE_li:[0-9]+]] li: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_float16:[0-9]+]] @arg_float16(%[[VALUE_f16:[0-9]+]] f16: f16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_fp16:[0-9]+]] @arg_fp16(%[[VALUE_f16_2:[0-9]+]] f16: f16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_float:[0-9]+]] @arg_float(%[[VALUE_f:[0-9]+]] f: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_double:[0-9]+]] @arg_double(%[[VALUE_d:[0-9]+]] d: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_int_ptr:[0-9]+]] @arg_int_ptr(%[[VALUE_pi:[0-9]+]] pi: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_void_ptr:[0-9]+]] @arg_void_ptr(%[[VALUE_pv:[0-9]+]] pv: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_matrix:[0-9]+]] @arg_matrix(%[[VALUE_m:[0-9]+]] m: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_transparent_union_int:[0-9]+]] @arg_transparent_union_int(%[[VALUE_tu:[0-9]+]] tu: @type[[TYPE0]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_transparent_union_char:[0-9]+]] @arg_transparent_union_char(%[[VALUE_tu_2:[0-9]+]] tu: @type[[TYPE1]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_transparent_union_ptr:[0-9]+]] @arg_transparent_union_ptr(%[[VALUE_tu_3:[0-9]+]] tu: @type[[TYPE2]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_bitint7:[0-9]+]] @arg_bitint7(%[[VALUE_x:[0-9]+]] x: i7b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_ubitint7:[0-9]+]] @arg_ubitint7(%[[VALUE_x_2:[0-9]+]] x: u7b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_bitint65:[0-9]+]] @arg_bitint65(%[[VALUE_x_3:[0-9]+]] x: i65b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_bitint128:[0-9]+]] @arg_bitint128(%[[VALUE_x_4:[0-9]+]] x: i128b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_bitint129:[0-9]+]] @arg_bitint129(%[[VALUE_x_5:[0-9]+]] x: i129b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
