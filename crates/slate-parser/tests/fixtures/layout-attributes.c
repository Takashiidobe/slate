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

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: struct name=PackedAligned [attributes=packed,aligned(4)]
// DEFAULT-NEXT:   field: type=char declarator=name=a
// DEFAULT-NEXT:   field: type=int declarator=name=b [attributes=aligned(8)]
// DEFAULT-NEXT: decl[1]: typedef name=aligned_t type=int [attributes=aligned(8)]
// DEFAULT-NEXT: decl[2]: typedef name=vector_t type=int [attributes=vector_size(16)]
// DEFAULT-NEXT: decl[3]: typedef name=mode_t type=int [attributes=mode(QI)]
// DEFAULT-NEXT: decl[4]: struct name=Trailing [attributes=packed]
// DEFAULT-NEXT:   field: type=int declarator=name=value
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: struct name=PackedAligned
// DEFAULT-NEXT: decl[1]: typedef name=aligned_t type=int [attributes=aligned(8)]
// DEFAULT-NEXT: decl[2]: typedef name=vector_t type=int [attributes=vector_size(16)]
// DEFAULT-NEXT: decl[3]: typedef name=mode_t type=int [attributes=mode(QI)]
// DEFAULT-NEXT: decl[4]: struct name=Trailing
// SLATE-FILECHECK-END DEFAULT
