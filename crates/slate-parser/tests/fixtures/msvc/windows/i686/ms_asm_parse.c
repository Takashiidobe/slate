// SLATE-FILECHECK-DEFINES DEFAULT

#define READ_CPUID(func, a, b, c, d) \
  __asm {                             \
    __asm mov eax, func               \
    __asm xor ecx, ecx                \
    __asm cpuid                       \
    __asm mov a, eax                  \
    __asm mov d, edx                  \
  }

struct pair { int lo; int hi; };
int table[4];
void callee(void);

int syntax(int x, struct pair p) {
  int a, b, c, d;
  READ_CPUID(1, a, b, c, d);
  __asm mov eax, x
  __asm add eax, 1 ; a comment with a } and don't
  __asm { mov x, eax ; } does not close the block
  }
  __asm
  {
    mov ecx, p.hi
    mov eax, table[4]
    mov ecx, TYPE table
    mov edx, dword ptr [esp + 0Ch]
    mov eax, es:[edi]
    rep movsb
    lock xadd [ecx], eax
    fld st(1)
    _emit 0x90
    mov eax, 100h + 10b * 17o - 010
    call callee
    jmp short done
  done: int 3
  }
  if (x) __asm mov x, 2
  return x;
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
// DEFAULT-NEXT:     type @type0 pair = struct {
// DEFAULT-NEXT:         field0 lo: i32;
// DEFAULT-NEXT:         field1 hi: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %1 table: array<i32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @callee() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @syntax(%5 x: i32, %6 p: @type0) -> i32 [linkage=external] [abi=x86_win32(scalar, native_c) -> scalar] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%11)))] {
// DEFAULT-NEXT:         let %11: u32 [synthetic];
// DEFAULT-NEXT:         let %7 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 d: i32 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "mov eax, 1\nxor ecx, ecx\ncpuid\nmov a, eax\nmov d, edx" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 1\nxor ecx, ecx\ncpuid\nmov " addr(%1) ", eax\nmov " addr(%2) ", edx";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%11);
// DEFAULT-NEXT:             in 1 [a] mem<write> place<i32>(%7);
// DEFAULT-NEXT:             in 2 [d] mem<write> place<i32>(%10);
// DEFAULT-NEXT:             clobbers: "ebx" as bx, "ecx" as cx, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         asm volatile "mov eax, x\nadd eax, 1" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, " addr(%1) "\nadd eax, 1";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%11);
// DEFAULT-NEXT:             in 1 [x] mem<read> place<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "mov x, eax" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov " addr(%1) ", eax";
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%11);
// DEFAULT-NEXT:             in 1 [x] mem<write> place<i32>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "mov ecx, p.hi\nmov eax, table[4]\nmov ecx, type table\nmov edx, dword ptr [esp + 12]\nmov eax, es:[edi]\nrep movsb\nlock xadd [ecx], eax\nfld st(1)\n_emit 144\nmov eax, 256 + 2 * 15 - 8\ncall callee\njmp short done\ndone: int 3" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%1 + 4) "\nmov eax, " addr(%2 + 4) "\nmov ecx, 4\nmov edx, dword ptr [esp + 12]\nmov eax, es:[edi]\nrep movsb\nlock xadd [ecx], eax\nfld st(1)\n.byte 144\nmov eax, 278\ncall " %3 "\njmp short " label(done) "\n" entry_label(%4, done) ": int 3";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%11);
// DEFAULT-NEXT:             in 1 [p] mem<read> place<@type0>(%6);
// DEFAULT-NEXT:             in 2 [table] mem<read> place<array<i32, 4>>(%1);
// DEFAULT-NEXT:             in 3 sym<offset=0>(%2);
// DEFAULT-NEXT:             clobbers: "ecx" as cx, "edi" as di, "edx" as dx, "esi" as si, "st" as st, "st(1)" as st(1), "st(2)" as st(2), "st(3)" as st(3), "st(4)" as st(4), "st(5)" as st(5), "st(6)" as st(6), "st(7)" as st(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             asm volatile "mov x, 2" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:                 template: "mov " addr<dword>(%1) ", 2";
// DEFAULT-NEXT:                 lateout 0 "{eax}" [{ax}] width 32 place<u32>(%11);
// DEFAULT-NEXT:                 in 1 [x] mem<write> place<i32>(%5);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
