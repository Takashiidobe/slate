// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

struct s { int a; };
typedef struct s S_t;
int f(int x) __attribute__((overloadable)) { return x; }
int f(double x) __attribute__((overloadable)) { return 2; }
int g(struct s *a, const struct s *b, struct s *c, S_t *d, struct s **e) __attribute__((overloadable)) { return 0; }
int g(void (*cb)(struct s *), void (*cb2)(struct s *), int *(*cb3)(int *)) __attribute__((overloadable)) { return 1; }
int g(volatile int *a, const volatile int *b, int *restrict c, char **const d) __attribute__((overloadable)) { return 2; }
int g(const char *const *a, const char *const *b, int m[3][4]) __attribute__((overloadable)) { return 3; }
int g(float _Complex a, _Bool b, unsigned __int128 c, __int128 d, _Float16 h, long double ld) __attribute__((overloadable)) { return 4; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_S_t:[0-9]+]] S_t = @type[[TYPE_s]];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [asm_name="_Z1fi"] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_f_2:[0-9]+]] @f(%[[VALUE_x_2:[0-9]+]] x: f64) -> i32 [linkage=external] [asm_name="_Z1fd"] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(2);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_s]]>, %[[VALUE_b:[0-9]+]] b: ptr<const @type[[TYPE_s]]>, %[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_s]]>, %[[VALUE_d:[0-9]+]] d: ptr<@type[[TYPE_s]]> [c="S_t *"], %[[VALUE_e:[0-9]+]] e: ptr<ptr<@type[[TYPE_s]]>>) -> i32 [linkage=external] [asm_name="_Z1gP1sPKS_S0_S0_PS0_"] [overloadable] [fallthrough=ub_if_used] [c="int(struct s *, const struct s *, struct s *, S_t *, struct s **)"] {
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_g_2:[0-9]+]] @g(%[[VALUE_cb:[0-9]+]] cb: ptr<fn(ptr<@type[[TYPE_s]]>) -> void>, %[[VALUE_cb2:[0-9]+]] cb2: ptr<fn(ptr<@type[[TYPE_s]]>) -> void>, %[[VALUE_cb3:[0-9]+]] cb3: ptr<fn(ptr<i32>) -> ptr<i32>>) -> i32 [linkage=external] [asm_name="_Z1gPFvP1sES2_PFPiS3_E"] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_g_3:[0-9]+]] @g(%[[VALUE_a_2:[0-9]+]] a: ptr<volatile i32>, %[[VALUE_b_2:[0-9]+]] b: ptr<const volatile i32>, %[[VALUE_c_2:[0-9]+]] c: ptr<i32> [restrict], %[[VALUE_d_2:[0-9]+]] d: ptr<ptr<i8>> [const]) -> i32 [linkage=external] [asm_name="_Z1gPViPVKiPiPPc"] [overloadable] [fallthrough=ub_if_used] [c="int(volatile int *, const volatile int *, int *restrict, char **const)"] {
// IR-NEXT:         return const<i32>(2);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_g_4:[0-9]+]] @g(%[[VALUE_a_3:[0-9]+]] a: ptr<const ptr<const i8>>, %[[VALUE_b_3:[0-9]+]] b: ptr<const ptr<const i8>>, %[[VALUE_m:[0-9]+]] m: ptr<array<i32, 4>> [array=3]) -> i32 [linkage=external] [asm_name="_Z1gPKPKcS2_PA4_i"] [overloadable] [fallthrough=ub_if_used] [c="int(const char *const *, const char *const *, int[3][4])"] {
// IR-NEXT:         return const<i32>(3);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_g_5:[0-9]+]] @g(%[[VALUE_a_4:[0-9]+]] a: complex<f32>, %[[VALUE_b_4:[0-9]+]] b: bool, %[[VALUE_c_3:[0-9]+]] c: u128, %[[VALUE_d_3:[0-9]+]] d: i128, %[[VALUE_h:[0-9]+]] h: f16, %[[VALUE_ld:[0-9]+]] ld: f80) -> i32 [linkage=external] [asm_name="_Z1gCfbonDF16_e"] [overloadable] [abi=sysv64(native_c, scalar, scalar, scalar, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(4);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
