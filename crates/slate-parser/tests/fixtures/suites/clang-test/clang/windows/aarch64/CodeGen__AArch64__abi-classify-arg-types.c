
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
// DEFAULT-NEXT:     type @type0 fx2x2_t = f32;
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 tu_int_t = @type1;
// DEFAULT-NEXT:     type @type3 = union {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 sc: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type4 tu_char_t = @type3;
// DEFAULT-NEXT:     type @type5 = union {
// DEFAULT-NEXT:         field0 p: ptr<void>;
// DEFAULT-NEXT:         field1 ip: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type6 tu_ptr_t = @type5;
// DEFAULT-NEXT:     global %23 gi: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @arg_void() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @arg_bool(%2 b: bool) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @arg_char(%4 c: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @arg_short(%6 s: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @arg_ushort(%8 us: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @arg_int(%10 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @arg_uint(%12 ui: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @arg_long(%14 li: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @arg_float16(%16 f16: f16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @arg_fp16(%18 f16: f16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @arg_float(%20 f: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @arg_double(%22 d: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @arg_int_ptr(%25 pi: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @arg_void_ptr(%27 pv: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @arg_matrix(%30 m: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @arg_transparent_union_int(%34 tu: @type1) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @arg_transparent_union_char(%38 tu: @type3) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @arg_transparent_union_ptr(%42 tu: @type5) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @arg_bitint7(%44 x: i7b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @arg_ubitint7(%46 x: u7b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @arg_bitint65(%48 x: i65b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @arg_bitint128(%50 x: i128b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @arg_bitint129(%52 x: i129b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
