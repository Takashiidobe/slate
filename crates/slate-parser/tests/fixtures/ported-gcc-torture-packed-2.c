typedef struct s {
  unsigned short a;
  unsigned long  b __attribute__((packed));
} s;

s t;

int main() {
  t.b = 0;
  return 0;
}

// SLATE-FILECHECK-ERROR GCC

// SLATE-FILECHECK-BEGIN GCC
// GCC: Error:   × expected declarator
// GCC: ╰─▶ expected declarator
// GCC: ╭─[tests/fixtures/ported-gcc-torture-packed-2.c:1:18]
// GCC: 1 │ typedef struct s {
// GCC: ·                  ─
// GCC: 2 │   unsigned short a;
// GCC: ╰────
// SLATE-FILECHECK-END GCC
