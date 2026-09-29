
// Yes, this is an assembly test from Clang, because we need to make it all the
// way through code generation to know if our call became a direct, pc-relative
// call or an indirect call through memory.

int k(int);
__declspec(dllimport) int kimport(int);
int (*kptr)(int);
int (*gptr(void))(int);

int foo(void) {
  int (*r)(int) = gptr();

  // Simple case: direct call.
  __asm call k;

  // Marginally harder: indirect calls, via dllimport or function pointer.
  __asm call r;
  __asm call kimport;

  // Call through a global function pointer.
  __asm call kptr;
}

int bar(void) {
  __asm {
    jmp k
    ja k
    JAE k
    LOOP k
    loope k
    loopne k
  };
}

int baz(void) {
  __asm mov eax, k;
  __asm mov eax, kptr;
}

// Test that this asm blob doesn't require more registers than available.  This
// has to be an LLVM code generation test.

void __declspec(naked) naked(void) {
  __asm pusha
  __asm call k
  __asm popa
  __asm ret
}

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
// DEFAULT-NEXT:     global %[[VALUE_kptr:[0-9]+]] kptr: ptr<fn(i32) -> i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_k:[0-9]+]] @k(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_kimport:[0-9]+]] @kimport(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [dllimport];
// DEFAULT-NEXT:     fn %[[VALUE_gptr:[0-9]+]] @gptr() -> ptr<fn(i32) -> i32> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE2:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE2]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: ptr<fn(i32) -> i32> [storage=automatic] = call<ptr<fn(i32) -> i32>, signature=fn() -> ptr<fn(i32) -> i32>>(%[[VALUE_gptr]]);
// DEFAULT-NEXT:         asm volatile "call k\ncall r\ncall kimport\ncall kptr" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "call " %1 "\ncall " addr<dword>(%2) "\ncall " %3 "\ncall " addr<dword>(%4);
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE2]]);
// DEFAULT-NEXT:             in 1 sym<offset=0>(%[[VALUE_k]]);
// DEFAULT-NEXT:             in 2 [r] mem<read> place<ptr<fn(i32) -> i32>>(%[[VALUE_r]]);
// DEFAULT-NEXT:             in 3 sym<offset=0>(%[[VALUE_kimport]]);
// DEFAULT-NEXT:             in 4 [kptr] mem<read> place<ptr<fn(i32) -> i32>>(%[[VALUE_kptr]]);
// DEFAULT-NEXT:             clobbers: "ecx" as cx, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE3:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE3]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "jmp k\nja k\nJAE k\nLOOP k\nloope k\nloopne k" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "jmp " %1 "\nja " %1 "\nJAE " %1 "\nLOOP " %1 "\nloope " %1 "\nloopne " %1;
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE3]]);
// DEFAULT-NEXT:             in 1 sym<offset=0>(%[[VALUE_k]]);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE4:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE4]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, k\nmov eax, kptr" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, " %1 "\nmov eax, " addr(%2);
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE4]]);
// DEFAULT-NEXT:             in 1 sym<offset=0>(%[[VALUE_k]]);
// DEFAULT-NEXT:             in 2 [kptr] mem<read> place<ptr<fn(i32) -> i32>>(%[[VALUE_kptr]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_naked:[0-9]+]] @naked() -> void [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "pusha\ncall k\npopa\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "pusha\ncall " %0 "\npopa\nret";
// DEFAULT-NEXT:             in 0 sym<offset=0>(%[[VALUE_k]]);
// DEFAULT-NEXT:             clobbers: "eax" as ax, "ebp" as bp, "ebx" as bx, "ecx" as cx, "edi" as di, "edx" as dx, "esi" as si;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
