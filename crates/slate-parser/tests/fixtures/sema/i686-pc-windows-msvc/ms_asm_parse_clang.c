// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

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
    mov eax, 100h + 10b * 17o - 010
    call callee
    jmp short done
  done: int 3
  }
  if (x) __asm mov x, 2
  return x;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: MSVC `__asm` statement
// SLATE-FILECHECK-END DEFAULT
