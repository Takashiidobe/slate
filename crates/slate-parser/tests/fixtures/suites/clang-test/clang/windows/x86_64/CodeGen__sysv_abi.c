
#define SYSV_CC __attribute__((sysv_abi))

// Make sure we coerce structs according to the SysV rules instead of passing
// them indirectly as we would for Win64.
struct StringRef {
  char *Str;
  __SIZE_TYPE__ Size;
};
extern volatile char gc;
void SYSV_CC take_stringref(struct StringRef s);
void callit(void) {
  struct StringRef s = {"asdf", 4};
  take_stringref(s);
}

// Check that we pass vectors directly if the target feature is enabled, and
// not otherwise.
typedef __attribute__((vector_size(32))) float my_m256;
typedef __attribute__((vector_size(64))) float my_m512;

my_m256 SYSV_CC get_m256(void);
void SYSV_CC take_m256(my_m256);
my_m512 SYSV_CC get_m512(void);
void SYSV_CC take_m512(my_m512);

void use_vectors(void) {
  my_m256 v1 = get_m256();
  take_m256(v1);
  my_m512 v2 = get_m512();
  take_m512(v2);
}


// Added test to explicitly cover the case when __attribute__((target("avx"))) is used 
// with __attribute__((sysv_abi))

__attribute__((target("avx"))) my_m256 SYSV_CC get_avx_m256(void);
__attribute__((target("avx"))) void SYSV_CC take_avx_m256(my_m256);

__attribute__((target("avx")))
void use_target_attr_vectors(void) {
  my_m256 v = get_avx_m256();
  take_avx_m256(v);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
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
// DEFAULT-NEXT:     type @type0 StringRef = struct {
// DEFAULT-NEXT:         field0 Str: ptr<i8>;
// DEFAULT-NEXT:         field1 Size: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 my_m256 = vector<f32, 8>;
// DEFAULT-NEXT:     type @type2 my_m512 = vector<f32, 16>;
// DEFAULT-NEXT:     extern %1 gc: volatile i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 115, 100, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @take_stringref(%18 s: @type0) -> void [linkage=external] [abi=win64(native_c) -> void];
// DEFAULT-NEXT:     fn %3 @callit() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 s: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = array_decay<ptr<i8>, length=Some(5)>(%19), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void, abi=win64(native_c) -> void>(%2, copy<@type0, reason=arg>(read<@type0>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @get_m256() -> vector<f32, 8> [linkage=external] [abi=win64() -> direct];
// DEFAULT-NEXT:     fn %8 @take_m256(%20 <unnamed>: vector<f32, 8>) -> void [linkage=external] [abi=win64(direct) -> void];
// DEFAULT-NEXT:     fn %9 @get_m512() -> vector<f32, 16> [linkage=external] [abi=win64() -> direct];
// DEFAULT-NEXT:     fn %10 @take_m512(%21 <unnamed>: vector<f32, 16>) -> void [linkage=external] [abi=win64(direct) -> void];
// DEFAULT-NEXT:     fn %11 @use_vectors() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 v1: vector<f32, 8> [storage=automatic] = call<vector<f32, 8>, signature=fn() -> vector<f32, 8>, abi=win64() -> direct>(%7);
// DEFAULT-NEXT:         call<void, signature=fn(vector<f32, 8>) -> void, abi=win64(direct) -> void>(%8, read<vector<f32, 8>>(%12));
// DEFAULT-NEXT:         let %13 v2: vector<f32, 16> [storage=automatic] = call<vector<f32, 16>, signature=fn() -> vector<f32, 16>, abi=win64() -> direct>(%9);
// DEFAULT-NEXT:         call<void, signature=fn(vector<f32, 16>) -> void, abi=win64(direct) -> void>(%10, read<vector<f32, 16>>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @get_avx_m256() -> vector<f32, 8> [linkage=external] [abi=win64() -> direct];
// DEFAULT-NEXT:     fn %15 @take_avx_m256(%22 <unnamed>: vector<f32, 8>) -> void [linkage=external] [abi=win64(direct) -> void];
// DEFAULT-NEXT:     fn %16 @use_target_attr_vectors() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %17 v: vector<f32, 8> [storage=automatic] = call<vector<f32, 8>, signature=fn() -> vector<f32, 8>, abi=win64() -> direct>(%14);
// DEFAULT-NEXT:         call<void, signature=fn(vector<f32, 8>) -> void, abi=win64(direct) -> void>(%15, read<vector<f32, 8>>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
