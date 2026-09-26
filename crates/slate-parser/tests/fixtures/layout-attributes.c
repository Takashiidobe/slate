struct __attribute__((packed, aligned(4))) PackedAligned {
  char a;
  int b __attribute__((aligned(8)));
};

typedef int aligned_t __attribute__((aligned(8)));
typedef int vector_t __attribute__((vector_size(16)));
typedef int mode_t __attribute__((mode(QI)));

struct Trailing {
  int value;
} __attribute__((packed));

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: machine mode attribute
// SLATE-FILECHECK-END DEFAULT
