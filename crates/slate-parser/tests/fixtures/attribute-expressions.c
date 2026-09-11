typedef int aligned_type __attribute__((__aligned__(8 + 8)));
typedef int vector_type __attribute__((__vector_size__(sizeof(int) * 4)));
__attribute__((__const__, __may_alias__)) int aliased;

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: typedef name=aligned_type type=int [attributes=aligned((8 + 8))]
// DEFAULT-NEXT: decl[1]: typedef name=vector_type type=int [attributes=vector_size((sizeof(int) * 4))]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=name=aliased [attributes=const,may_alias]
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: typedef name=aligned_type type=int [attributes=aligned((8 + 8))]
// DEFAULT-NEXT: decl[1]: typedef name=vector_type type=int [attributes=vector_size((sizeof(int) * 4))]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=name=aliased [attributes=const,may_alias]
// SLATE-FILECHECK-END DEFAULT
