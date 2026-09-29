
__attribute__ ((target("branch-protection=none")))
void none() {}

  __attribute__ ((target("branch-protection=standard")))
void std() {}

__attribute__ ((target("branch-protection=bti")))
void btionly() {}

__attribute__ ((target("branch-protection=pac-ret")))
void paconly() {}

__attribute__ ((target("branch-protection=pac-ret+bti")))
void pacbti0() {}

__attribute__ ((target("branch-protection=bti+pac-ret")))
void pacbti1() {}

__attribute__ ((target("branch-protection=pac-ret+leaf")))
void leaf() {}

__attribute__ ((target("branch-protection=pac-ret+b-key")))
void bkey() {}

__attribute__ ((target("branch-protection=pac-ret+b-key+leaf")))
void bkeyleaf0() {}

__attribute__ ((target("branch-protection=pac-ret+leaf+b-key")))
void bkeyleaf1() {}

__attribute__ ((target("branch-protection=pac-ret+leaf+bti")))
void btileaf() {}


__attribute__ ((target("branch-protection=pac-ret+pc")))
void pauthlr() {}

__attribute__ ((target("branch-protection=pac-ret+pc+b-key")))
void pauthlr_bkey() {}

__attribute__ ((target("branch-protection=pac-ret+pc+leaf")))
void pauthlr_leaf() {}

__attribute__ ((target("branch-protection=pac-ret+pc+bti")))
void pauthlr_bti() {}

__attribute__ ((target("branch-protection=gcs")))
void gcs() {}

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
// DEFAULT-NEXT:     fn %[[VALUE_none:[0-9]+]] @none(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_std:[0-9]+]] @std(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_btionly:[0-9]+]] @btionly(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_paconly:[0-9]+]] @paconly(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pacbti0:[0-9]+]] @pacbti0(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pacbti1:[0-9]+]] @pacbti1(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_leaf:[0-9]+]] @leaf(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bkey:[0-9]+]] @bkey(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bkeyleaf0:[0-9]+]] @bkeyleaf0(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bkeyleaf1:[0-9]+]] @bkeyleaf1(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_btileaf:[0-9]+]] @btileaf(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pauthlr:[0-9]+]] @pauthlr(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pauthlr_bkey:[0-9]+]] @pauthlr_bkey(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pauthlr_leaf:[0-9]+]] @pauthlr_leaf(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pauthlr_bti:[0-9]+]] @pauthlr_bti(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gcs:[0-9]+]] @gcs(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
