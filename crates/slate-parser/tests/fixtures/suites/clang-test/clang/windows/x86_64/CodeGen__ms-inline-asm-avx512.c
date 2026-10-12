
void t1(void) {
  __asm {
	  vaddpd zmm8, zmm27, zmm6
  }
}


void t2(void) {
  __asm {
	  vaddpd zmm8 {k1}, zmm27, zmm6
  }
}

void ignore_fe_size(void) {
  char c;
  __asm vaddps xmm1, xmm2, [c]{1to4}
  __asm vaddps xmm1, xmm2, [c]
  __asm mov eax, [c]
  __asm mov [c], rax
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
// DEFAULT-NEXT:     fn %[[VALUE_t1:[0-9]+]] @t1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "vaddpd zmm8, zmm27, zmm6" [dialect=intel] {
// DEFAULT-NEXT:             template: "vaddpd zmm8, zmm27, zmm6";
// DEFAULT-NEXT:             clobbers: "zmm8" as zmm8;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t2:[0-9]+]] @t2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "vaddpd zmm8{k1}, zmm27, zmm6" [dialect=intel] {
// DEFAULT-NEXT:             template: "vaddpd zmm8{k1}, zmm27, zmm6";
// DEFAULT-NEXT:             clobbers: "zmm8" as zmm8;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ignore_fe_size:[0-9]+]] @ignore_fe_size() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "vaddps xmm1, xmm2, [c]{1to4}\nvaddps xmm1, xmm2, [c]\nmov eax, [c]\nmov [c], rax" [dialect=intel] {
// DEFAULT-NEXT:             template: "vaddps xmm1, xmm2, " addr(%0) "{1to4}\nvaddps xmm1, xmm2, " addr(%0) "\nmov eax, " addr(%0) "\nmov " addr(%0) ", rax";
// DEFAULT-NEXT:             in 0 [c] mem<readwrite> place<i8>(%[[VALUE_c]]);
// DEFAULT-NEXT:             clobbers: "eax" as ax, "xmm1" as xmm1;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
