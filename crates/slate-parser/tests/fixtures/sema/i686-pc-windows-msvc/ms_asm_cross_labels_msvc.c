// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-DEFINES DEFAULT

int asm_to_c(int value) {
  __asm {
    mov eax, value
    jmp done
  }
  return 0;
done:
  return value;
}

int c_to_asm(int value) {
  goto inside;
  __asm {
    mov eax, 1
  inside:
    mov eax, value
  }
  return value;
}

int mixed_labels(void) {
  __asm {
    jmp inner
  inner:
    nop
  }
  return 0;
}

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
// DEFAULT-NEXT:     fn %0 @asm_to_c(%2 value: i32) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%8)))] {
// DEFAULT-NEXT:         let %8: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile goto "mov eax, value\njmp done" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, " addr(%1) "\njmp " %l0;
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%8);
// DEFAULT-NEXT:             in 1 [value] mem<read> place<i32>(%2);
// DEFAULT-NEXT:             labels: %1;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:         label %1 done:
// DEFAULT-NEXT:             return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @c_to_asm(%5 value: i32) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%9)))] {
// DEFAULT-NEXT:         let %9: u32 [synthetic];
// DEFAULT-NEXT:         goto %4;
// DEFAULT-NEXT:         asm volatile "mov eax, 1\ninside:\nmov eax, value" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 1\n" entry_label(%4, inside) ":\nmov eax, " addr(%1);
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%9);
// DEFAULT-NEXT:             in 1 [value] mem<read> place<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @mixed_labels() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%10)))] {
// DEFAULT-NEXT:         let %10: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "jmp inner\ninner:\nnop" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "jmp " label(inner) "\n" entry_label(%7, inner) ":\nnop";
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%10);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
